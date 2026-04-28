from __future__ import annotations
import os
import struct
import numpy as np
from geocal_swig import Time

# For reference, here are the C structures associated with this data. This comes
# from the old SDP toolkit header files.
"""
typedef struct
{
    char          spacecraftID[24];     /* Spacecraft Name */
    char          asciiTimeRange[48];   /* start/stop times to nearest hour */
    char          source[32];           /* Source of the data */
    char          version[8];           /* Version number (default = 1) */
    PGSt_double   startTime;            /* Ephemeris dataset start time,
                                           secTAI93 */
    PGSt_double   endTime;              /* Ephemeris dataset end time,
                                           secTAI93 */
    PGSt_real     interval;             /* Standard interval between records */
    PGSt_uinteger nURs;
    PGSt_uinteger nRecords;             /* Number of ephemeris records */
    PGSt_uinteger nOrbits;              /* Number of orbits spanned */
    PGSt_uinteger orbitNumberStart;     /* Number of 1st orbit or part orbit */
    PGSt_uinteger orbitNumberEnd;       /* Number of last orbit or part orb. */
    char          keplerRefFrame[8];    /* Reference Frame: "TOD" or "J2000" */
    PGSt_double   keplerElements[6];    /* Osculating Keplerian elements at
                                           epoch */
    PGSt_double   keplerEpochTAI;       /* TAI93 Epoch of Keplerian Elements */
    PGSt_real     qaParameters[16];     /* Ephemeris data quality processing
                                           parameters */
    PGSt_real     qaStatistics[4];      /* Quality assurance statistics */
    char          spare[216];           /* Pad to 512 bytes */
} PGSt_ephemHeader;

typedef struct
{
    PGSt_double   secTAI93;
    PGSt_double   position[3];
    PGSt_double   velocity[3];
    PGSt_uinteger qualityFlag;
    char          spare[4];
} PGSt_ephemRecord;

typedef struct
{
    PGSt_uinteger orbitNumber;           /* Orbit number, from mission start */
    char          spare[4];
    PGSt_double   orbitAscendTime;       /* Time of upward true equator
					    crossing, secTAI93 */
    PGSt_double   orbitDescendTime;      /* Time of downward true equator
					    crossing, secTAI93 */
    PGSt_double   orbitDescendLongitude; /* Orbit down-crossing terrestrial
					    longitude, radians */
} PGSt_ephemMetadata;
"""


class DprepEphemerisFile:
    """DPREP is an old binary format used by the SDP toolkit, for such instruments
    as MISR. This reads the ephemeris file. A separate class puts this together
    into a DprepOrbit, this is the low level file reading."""

    def __init__(self, filename: str | os.PathLike[str]) -> None:
        # Various structures used for reading. We generate these directly from
        # the header file data listed above from the old SDP toolkit code.
        # For some historical reason, the data is actually in big-endian for double etc.
        header_struct_fmt = ">24s48s32s8sddfIIIII8s6dd16f4f216s"
        hstruct = struct.Struct(header_struct_fmt)
        # Sanity check that we have the right structure format
        assert hstruct.size == 512
        ur_struct_fmt = "256s"
        urstruct = struct.Struct(ur_struct_fmt)
        assert urstruct.size == 256
        rec_struct_fmt = ">d3d3dI4s"
        recstruct = struct.Struct(rec_struct_fmt)
        met_struct_fmt = ">I4sddd"
        metstruct = struct.Struct(met_struct_fmt)

        # Use the structures to read and unpack the data
        with open(filename, "rb") as fh:
            # Read and unpack the header
            data = fh.read(hstruct.size)
            hs = hstruct.unpack(data)
            (
                self.spacraft_id,
                self.time_range,
                self.source,
                self.version,
                start_time_f,
                end_time_f,
                self.interval,
                n_urs,
                n_record,
                n_orbit,
                self.orbit_start_number,
                self.orbit_end_number,
                self.kepler_reference_frame,
            ) = hs[:13]
            self.start_time = Time.time_pgs(start_time_f)
            self.end_time = Time.time_pgs(end_time_f)
            self.kepler_element = np.empty((6,), dtype=np.float64)
            self.kepler_element[:] = hs[13 : 13 + 6]
            self.kepler_epoch = Time.time_pgs(hs[19])
            self.qa_parm = np.empty((16,), dtype=np.float32)
            self.qa_parm[:] = hs[20 : 20 + 16]
            self.qa_stat = np.empty((4,), dtype=np.float32)
            self.qa_stat[:] = hs[36 : 36 + 4]
            # Read the URs. We don't do anything with this, so just need to read the data
            _ = fh.read(urstruct.size * n_urs)
            data = fh.read(recstruct.size * n_record)
            self.tm = np.empty((n_record,), dtype=np.float64)
            self.pos = np.empty((n_record, 3), dtype=np.float64)
            self.vel = np.empty((n_record, 3), dtype=np.float64)
            self.qa_flag = np.empty((n_record,), dtype=np.uint32)
            for i, d in enumerate(recstruct.iter_unpack(data)):
                self.tm[i] = d[0]
                self.pos[i, :] = d[1 : 1 + 3]
                self.vel[i, :] = d[4 : 4 + 3]
                self.qa_flag[i] = d[7]
            data = fh.read(metstruct.size * n_orbit)
            self.onum = np.empty((n_orbit,), dtype=np.uint32)
            self.oasc_time = np.empty((n_orbit,), dtype=object)
            self.odesc_time = np.empty((n_orbit,), dtype=object)
            self.odesc_lon = np.empty((n_orbit,), dtype=np.float64)
            for i, d in enumerate(metstruct.iter_unpack(data)):
                (
                    self.onum[i],
                    _,
                    oasc_time_v,
                    odesc_time_v,
                    self.odesc_lon[i],
                ) = d
                self.oasc_time[i] = Time.time_pgs(oasc_time_v)
                self.odesc_time[i] = Time.time_pgs(odesc_time_v)

class DprepAttitudeFile:
    """DPREP is an old binary format used by the SDP toolkit, for such instruments
    as MISR. This reads the attitude file. A separate class puts this together
    into a DprepOrbit, this is the low level file reading."""

    def __init__(self, filename: str | os.PathLike[str]) -> None:
        pass


__all__ = ["DprepEphemerisFile", "DprepAttitudeFile"]
