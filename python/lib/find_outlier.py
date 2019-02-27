import pytest
import numpy as np
import scipy
import scipy.ndimage
import matplotlib.pyplot as plt
import geocal
import math

# Fill in missing/bad pixels

# Take difference with neighbor (each direction, up/down/left/right). For edges,
# just drop. So one less row or column. Keep sign

# Window median (cross the direction, so right/left have window up/down). For
# near edges, just use data that is present (so actual edge only has 1/2 the
# window). Think this keeps the sign

# Subtract median, take abs value

# For whole scene, order data and get x% point (e.g., 90%). Think scipy for
# this. Multiple by a factor, which gives a threshold

# Find points outside of the threshold. Do each direction independent. Points
# that are larger than an amount (e.g., 3) are then marked as bad.

# Create test data, have a few points missing and a few points bad. Can
# fill in the data with a simple gradient pattern or something like that

def test_outlier():
    sz = 2048
    window_size = 7
    percentile = 90.0
    thresh_fact = 2.0
    nfail_thresh = 3
    original_data = np.empty((sz,sz))
    fill_data = np.linspace(0.0,10.0, sz)
    for i in range(original_data.shape[0]):
        original_data[i, :] = fill_data + fill_data[i]
    if(False):
        plt.imshow(original_data, cmap=plt.cm.gray, vmin=0.0,
                   vmax=20.0)
        plt.show()
    # Skip static bad pixels, want to think about how to do this.
    # Add a couple bad pixels
    original_data[100,200] = 40.0
    original_data[300,400] = -40.0

    d = original_data
    # Difference right, left, up, down
    nl, ns = d.shape
    down_diff = d[0:nl-1, :] - d[1:nl, :]
    up_diff = d[1:nl, :] - d[0:(nl-1), :]
    left_diff = d[:, 0:ns-1] - d[:, 1:ns]
    right_diff = d[:, 1:ns] - d[:, 0:(ns-1)]
    
    # Local median
    # Temp, use array with different values
    #d[10,10] = 100.0
    #down_diff = d
    # This is slow
    #down_diff_local_med = scipy.ndimage.generic_filter(down_diff, np.nanmedian,
    #                                                   (1, window_size), mode='constant',
    #                                                   cval=np.nan)
    # Much faster
    down_diff_local_med = np.abs(down_diff - geocal.array_local_median(down_diff, 1, window_size))
    up_diff_local_med = np.abs(up_diff - geocal.array_local_median(up_diff, 1, window_size))
    left_diff_local_med = np.abs(left_diff - geocal.array_local_median(left_diff, 1, window_size))
    right_diff_local_med = np.abs(right_diff - geocal.array_local_median(right_diff, 1, window_size))
    down_thresh = np.percentile(down_diff_local_med, percentile) * thresh_fact
    up_thresh = np.percentile(up_diff_local_med, percentile) * thresh_fact
    right_thresh = np.percentile(right_diff_local_med, percentile) * thresh_fact
    left_thresh = np.percentile(left_diff_local_med, percentile) * thresh_fact

    # Put back the edges we trimmed while calculating
    z = np.zeros((1,d.shape[1]))
    down_diff_local_med = np.concatenate((down_diff_local_med,z), axis=0)
    up_diff_local_med = np.concatenate((z,up_diff_local_med), axis=0)
    z = np.zeros((d.shape[0], 1))
    left_diff_local_med = np.concatenate((left_diff_local_med,z), axis=1)
    right_diff_local_med = np.concatenate((z,right_diff_local_med), axis=1)
    nfail = ((down_diff_local_med > down_thresh).astype(int) +
             (up_diff_local_med > up_thresh).astype(int) +
             (right_diff_local_med > right_thresh).astype(int) +
             (left_diff_local_med > left_thresh).astype(int))
    is_bad = nfail >= nfail_thresh
    print(np.argwhere(nfail >= nfail_thresh))

def test_fill():
    '''Fill in data after we detect bad pixels'''
    sz = 100
    original_data = np.empty((sz,sz))
    fill_data = np.linspace(0.0,10.0, sz)
    for i in range(original_data.shape[0]):
        original_data[i, :] = fill_data + fill_data[i]
    mask = np.empty((sz,sz))
    mask[:,:] = False
    original_data[10:16,20:31] = -9999
    mask[10:16,20:31] = True
    kernel = np.empty((5,5))
    krhs = 2
    kchs = 2
    sigma = 1.0
    for i in range(-krhs, krhs+1):
        for j in range(-kchs, kchs+1):
            kernel[i+krhs,j+kchs] = math.exp(-(i*i + j*j) / (2*sigma*sigma))
    if(False):
        plt.imshow(original_data, cmap=plt.cm.gray, vmin=0.0,
                   vmax=20.0)
        plt.show()
    m = geocal.IterativeMorphologicalDilation(original_data, mask, kernel)
    m.fill_missing_data()
    print(m.filled_image[10:16,20:31])
    if(False):
        plt.imshow(m.filled_image, cmap=plt.cm.gray, vmin=0.0,
                   vmax=20.0)
        plt.show()
    

