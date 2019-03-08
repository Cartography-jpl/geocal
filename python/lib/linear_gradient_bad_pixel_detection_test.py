from geocal.linear_gradient_bad_pixel_detection import *
from geocal_swig import IterativeMorphologicalDilation
import matplotlib.pyplot as plt
from test_support import *
import math

def test_linear_gradient_bad_pixel_detection():
    '''Test detection of bad/outlier pixels'''
    sz = 2048
    original_data = np.empty((sz,sz))
    fill_data = np.linspace(0.0,10.0, sz)
    for i in range(original_data.shape[0]):
        original_data[i, :] = fill_data + fill_data[i]
    if(False):
        plt.imshow(original_data, cmap=plt.cm.gray, vmin=0.0,
                   vmax=20.0)
        plt.show()

    # Add a couple bad pixels
    original_data[100,200] = 40.0
    original_data[300,400] = -40.0
    original_data[0,100] = -80
    original_data[-1,-1] = 80

    bpixdetect = LinearGradientBadPixelDetection()
    is_bad = bpixdetect.bad_pixel_detection(original_data)
    assert (np.argwhere(is_bad == True) == [[0,100], [100, 200], [300, 400],
                                            [2047,2047]]).all()
    
def test_bad_pixel_fill():
    '''Test bad pixel fill with IterativeMorphologicalDilation. 

    Note that this isn't actually in
    linear_gradient_bad_pixel_detection, instead this is C++ code. But
    it is closely related and it is useful to have a python level unit test. 
    So we stick it in here for testing.'''

    sz = 100
    original_data = np.empty((sz,sz))
    fill_data = np.linspace(0.0,10.0, sz)
    for i in range(original_data.shape[0]):
        original_data[i, :] = fill_data + fill_data[i]
    data_with_bad_pixel = original_data.copy()
    mask = np.empty((sz,sz))
    mask[:,:] = False
    data_with_bad_pixel[10:16,20:31] = -9999
    mask[10:16,20:31] = True
    kernel = np.empty((5,5))
    krhs = 2
    kchs = 2
    sigma = 1.0
    for i in range(-krhs, krhs+1):
        for j in range(-kchs, kchs+1):
            kernel[i+krhs,j+kchs] = math.exp(-(i*i + j*j) / (2*sigma*sigma))
    if(False):
        plt.imshow(data_with_bad_pixel, cmap=plt.cm.gray, vmin=0.0,
                   vmax=20.0)
        plt.show()
    m = IterativeMorphologicalDilation(data_with_bad_pixel, mask)
    m.fill_missing_data()
    if(False):
        print(m.filled_image[10:16,20:31])
        print(original_data[10:16,20:31])
    # Bit hard to know what a "good" result is here. We check that most of
    # the data is pretty close the original - the pixels are spaced 0.1 in value
    assert np.median(np.abs(original_data[10:16,20:31] - m.filled_image[10:16,20:31])) < 0.2
    if(False):
        plt.imshow(m.filled_image, cmap=plt.cm.gray, vmin=0.0,
                   vmax=20.0)
        plt.show()
    