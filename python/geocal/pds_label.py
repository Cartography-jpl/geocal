import re
import bisect
from geocal_swig import *
import subprocess


def pds_label_text(fname):
    """While GDAL can read PDS format, it doesn't read most of the metadata.
    So this routine extracts out the front PDS metadata, which we could either
    process further, or store as a  "LBL" file (which is a detached header).

    This is PDS3 format, we don't yet handle PDS4.
    """
    res = b""
    with open(fname, "rb") as fh:
        while True:
            line = fh.readline()
            # Strip off /r. Shows up in python 2, but not 3. Not sure why
            # the difference, but in any case we don't want them
            res += line.replace(b"\r", b"")
            if line.strip().upper() == b"END" or line == b"":
                break
    return res


def pds_label(fname):
    """Read PDS data in and convert to a dict. Not sure how robust this is,
    we'll have to try this on a number of datasets and see what breaks."""
    txt = pds_label_text(fname)
    res = dict()
    for ln in txt.split(b"\n"):
        m = re.match(rb"\s*(\^?\w+)\s*=(.+)*", ln)
        if m:
            k = m.group(1)
            v = m.group(2).strip()
            m = re.match(b'"(.*)"', v)
            if m:
                v = m.group(1)
            res[k.decode("utf-8")] = str(v.decode("utf-8"))
    return res


class SpiceKernelByTime(object):
    """In some cases, we have a range of SPICE kernels with one for a range
    of times. These kernels have PDS labels associated with them that give
    the time ranges. So for example the mro spk kernels.

    This class handles parsing all the PDS labels, setting up the mapping,
    and then loading the right kernel for a particular time.

    This class can be saved (e.g., use json or pickling pickling through
    write_shelve) so we don't need to parse the directory every time.

    The class use to just use the PDS labels, but the SPICE data directory
    seems to have dropped creating the ".lbl" files. Not sure why, but
    we now accept ".lbl", ".bc" or ".bsp" files using the appropriate code
    to support each files. Note that the ".bc" and ".bsp" require a additional
    kernel file to convert space craft time to utc, so for example
    mro_create_json.ker
    """

    def __init__(self, lbl_or_binary_file_list, kernel_file=None):
        """Parse each PDS label file. Note that we assume the spice kernel is
        in the same directory as each PDS label.

        We read the PDS labels START_TIME, STOP_TIME and PRODUCT_ID.

        Often the label list can be created by a glob, e.g.
        glob.glob("/raid26/mars_kernels/mro_kernels/spk/mro_*_ssd_mro95a.lbl")

        We also support directly reading .bc or .bsp files, since SPICE seems
        to have dropped creating the .lbl files.
        """
        self.data = []
        for f in lbl_or_binary_file_list:
            ext = os.path.splitext(f)[1]
            if ext == ".lbl":
                self.process_lbl(f)
            elif ext == ".bsp":
                self.process_bsp(f, kernel_file)
            elif ext == ".bc":
                self.process_bc(f, kernel_file)
            else:
                raise RuntimeError("Unrecognized file extension %s" % ext)

        # Sort by start time
        self.data.sort(key=lambda v: v[0])
        self.tstart_et = [v[0] for v in self.data]
        self.tend_et = [v[1] for v in self.data]
        self.kernel_file = [v[2] for v in self.data]

    def process_lbl(self, f):
        d = pds_label(f)
        self.data.append(
            (
                Time.parse_time(d["START_TIME"]).et,
                Time.parse_time(d["STOP_TIME"]).et,
                os.path.abspath(os.path.dirname(f) + "/" + d["PRODUCT_ID"]),
            )
        )

    def process_bsp(self, f, kernel_file):
        res = subprocess.run(["brief", f, kernel_file], check=True, capture_output=True)
        # From looking at the output, this is the string with start and
        # end time"
        tm_line = res.stdout.split(b"\n")[-3]
        # There is a good deal of separation between the two times, character
        # 40 is in the middle of this
        tstart = Time.parse_time(tm_line[:40].decode("utf-8"))
        tend = Time.parse_time(tm_line[40:].decode("utf-8"))
        self.data.append((tstart.et, tend.et, os.path.abspath(f)))

    def process_bc(self, f, kernel_file):
        res = subprocess.run(
            ["ckbrief", f, kernel_file], check=True, capture_output=True
        )
        # Note this parsing may be fragile to changes in spice, so if this
        # starts fails check this.
        tm_line = res.stdout.split(b"\n")[-3]
        tstart = Time.parse_time(
            b" ".join(tm_line.lstrip().split(b" ")[0:2]).decode("utf-8")
        )
        tend = Time.parse_time(
            b" ".join(tm_line.lstrip().split(b" ")[2:4]).decode("utf-8")
        )
        self.data.append((tstart.et, tend.et, os.path.abspath(f)))

    def __getstate__(self):
        return {"data": self.data}

    def __setstate__(self, d):
        self.data = d["data"]
        self.tstart_et = [v[0] for v in self.data]
        self.tend_et = [v[1] for v in self.data]
        self.kernel_file = [v[2] for v in self.data]

    def kernel(self, tm):
        """Return the kernel covering the given time. If nothing
        matches, we raise a ValueError."""
        # Find largest value less than or equal to tm
        tm_et = tm.et
        i = bisect.bisect_left(self.tstart_et, tm_et)
        if not i:
            raise ValueError
        # Check that this matches the upper range (since there may be
        # gaps in the data)
        if tm_et > self.tend_et[i - 1]:
            raise ValueError(
                "Kernel file %s time %f" % (self.kernel_file[i - 1], tm_et)
            )
        return self.kernel_file[i - 1]


__all__ = ["pds_label_text", "pds_label", "SpiceKernelByTime"]
