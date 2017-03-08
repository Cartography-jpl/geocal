#======================================================================
# Example of creating a simple nitf file
#======================================================================

# Code is in geocal.nitf (not geocal)
from geocal.nitf import *

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


