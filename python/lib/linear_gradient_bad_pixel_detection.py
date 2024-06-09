import numpy as np
import geocal
import line_profiler

class LinearGradientBadPixelDetection(object):
    '''This class is used to detect bad/outlier pixels in an image. This class
    looks at the gradient in 4 directions for each pixel (up/down/left/right)
    and looks for pixels with larger than normal gradients in a threshold 
    number of directions. A good pixel might have a large gradient in 
    1 direction (e.g, be the edge of a building) or 2 directions (eg., the
    corner of a building), but if it has a large gradient in 3 or 4 directions
    this algorithm assumes they are bad (e.g., a pixel with a abnormally large
    value relative to its neighbors).

    This method does not find large areas of bad pixels (e.g. a defect
    in the CCD) - since in a group of bad pixels they may all be
    similar and "look" ok. So you would likely want to first use a set
    of known bad pixels (e.g., static bad pixels found during the
    calibration of the instrument), but then use this algorithm to
    find dynamic bad pixels (e.g., a high energy particle hit of a CCD
    element).  

    If you have a set of known bad pixels, you should first fill the
    bad pixels in before using this algorithm (e.g., use
    IterativeMorphologicalDilation) - so the known bad pixels don't
    make neighboring pixels look bad because they are different than 
    the neighbors.

    We use the following algorithm:

    1. Calculate the gradient in each of 4 directions (left,right,up,down).
       We do the next steps for each of the 4 directions independently
    2. Calculate the local median for a fixed window in the perpendicular
       direction (e.g., left and right look at a window up and down).
    3. Subtract the local median from each gradient and take abs value
    4. Calculate a given percentile (e.g., 90 percentile). Multiply by 
       a factor (e.g., 2) to give a threshold
    5. A pixel "fails" the test for a particular direction if the 
       abs(gradient - local median) > threshold
    6. Count the number of failures out of the 4 directions. Compare to
       a threshold percentage (e.g., 75.0), and if we are >= nfail_thresh 
       the pixel is marked as bad. Note for edges and corners we only have 
       3 and 2 directions, so the percentage is calculated using the number
       of directions.

    '''
    def __init__(self, window_size = 7, percentile = 90.0, thresh_fact = 2,
                 nfail_thresh_percentage = 75.0,
                 edge_handle = geocal.ARRAY_LOCAL_MEDIAN_TRUNCATE):
        self.window_size = window_size
        self.percentile = percentile
        self.thresh_fact = thresh_fact
        self.nfail_thresh_percentage = nfail_thresh_percentage
        self.edge_handle = edge_handle

    @line_profiler.profile
    def bad_pixel_detection(self, img):
        '''Take the given image as a 2d numpy array. Return a boolean array
        with a True value for bad pixels.

        Note that if you have a known static bad pixel set, you should first
        fill the image in with data over the bad pixel (e.g. use 
        IterativeMorphologicalDilation).
        '''

        # Difference right, left, up, down. This trims the edges, we'll add
        # that back in shortly. Note that other than a sign up and down are the
        # same, as is left and right. We take an abs value in next step, so we only
        # need to calculate one of these.
        nl, ns = img.shape
        down_diff = img[0:nl-1, :] - img[1:nl, :]
        #up_diff = img[1:nl, :] - img[0:(nl-1), :]
        left_diff = img[:, 1:ns] - img[:, 0:(ns-1)]
        #right_diff = img[:, 0:ns-1] - img[:, 1:ns]

        # Local median
        # Note, this is the python version. It is slow
        # down_diff_local_med = \
        #    scipy.ndimage.generic_filter(down_diff, np.nanmedian,
        #       (1, window_size), mode='constant', cval=np.nan)
        down_diff_local_med = np.abs(down_diff -
                     geocal.array_local_median(down_diff, 1, self.window_size,
                                               self.edge_handle))
        left_diff_local_med = np.abs(left_diff -
                     geocal.array_local_median(left_diff, self.window_size, 1,
                                               self.edge_handle))

        # Calculate thresholds
        down_thresh = np.percentile(down_diff_local_med,
                                    self.percentile) * self.thresh_fact
        left_thresh = np.percentile(left_diff_local_med,
                                    self.percentile) * self.thresh_fact
        # Count failures
        nfail_down = (down_diff_local_med > down_thresh).astype(int)
        nfail_left = (left_diff_local_med > left_thresh).astype(int)
        # Put back the edges we trimmed while calculating and calculate total results
        nfail = np.zeros(img.shape, dtype=int)
        nfail[1:nl,:] += nfail_down
        nfail[0:(nl-1),:] += nfail_down
        nfail[:,1:ns] += nfail_left
        nfail[:,0:(ns-1)] += nfail_left
        
        # Convert to percentage
        npix = np.empty(nfail.shape)
        npix[:,:]=4
        npix[0,:]=3
        npix[-1,:]=3
        npix[:,0]=3
        npix[:,-1]=3
        npix[0,0]=2
        npix[0,-1]=2
        npix[-1,0]=2
        npix[-1,-1]=2
        nfail = nfail / npix * 100.0

        # Compare to threshold and return results as bad pixel mask
        is_bad = nfail >= self.nfail_thresh_percentage
        return is_bad

__all__ = ["LinearGradientBadPixelDetection",]
    
