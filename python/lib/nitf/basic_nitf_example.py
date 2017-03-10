#======================================================================
# Example of creating a simple nitf file
#======================================================================

# Code is in geocal.nitf (not geocal)
from geocal.nitf import *
import copy

# Create the file. We don't supply a name yet, that comes when we actually
# write

f = NitfFile()

# Create a NitfImage source. The particular one we have here just puts all
# the data into a numpy array, which is nice for testing. We'll probably
# need to write other image sources (although most things can go to a
# numpy array, so maybe not).
img = NitfImageFromNumpy(nrow=10, ncol=10)
for i in range(10):
    for j in range(10):
        img.data[i,j] = i + j

# We just directly add this to the NitfFile. We need to wrap this as a
# NitfImageSegment (which adds the subheader). f.image_segment here is
# just a normal python array.
f.image_segment.append(NitfImageSegment(img))

# Can add TRES to either the file or image segment level. This automatically
# handles TRE overflow, you just put the tre in and the writing figures out
# where it should go.
t = TreUSE00A()
t.angle_to_north = 270
t.mean_gsd = 105.2
t.dynamic_range = 2047
t.obl_ang = 34.12
t.roll_ang = -21.15
t.n_ref = 0
t.rev_num = 3317
t.n_seg = 1
t.max_lp_seg = 6287
t.sun_el = 68.5
t.sun_az = 131.3
t2 = copy.deepcopy(t)
t2.angle_to_north = 290
f.tre_list.append(t)
f.image_segment[0].tre_list.append(t2)

# Now we write out to a nitf file
f.write("basic_nitf.ntf")

# We can also read this back in
f2 = NitfFile("basic_nitf.ntf")

# We then print out a description of the file
print(f2)

# And the actual data (not normally done since most images are too large to
# print the values
print("Data:")
print(f2.image_segment[0].data.data)


