from geocal_swig import *
import math
import numpy as np

def _rotated_map_gc(self, gc1, gc2):
    '''Rotate a MapInfo so it follows the path between gc1 and gc2.
    This is something like the minimum gore for passing between the two
    points. 

    We preserve the coordinate system, and map resolution.

    This function doesn't actually cover the original MapInfo or gc1/gc2.
    We adjust this in _rotated_map
    
    The new MapInfo is returned.
    '''
    x1, y1 = self.coordinate(gc1)
    x2, y2 = self.coordinate(gc2)
    a = math.atan2(x1 - x2, y1 - y2)
    rot = np.array([[math.cos(a), -math.sin(a)],[math.sin(a),math.cos(a)]])
    p = self.transform
    pm = np.array([[p[1], p[2]],[p[4], p[5]]])
    pm2 = np.matmul(rot,pm)
    mi = MapInfo(self.coordinate_converter,
                 np.array([p[0],pm2[0,0],pm2[0,1],p[3],pm2[1,0],pm2[1,1]]),
                 self.number_x_pixel, self.number_y_pixel, self.is_point)
    # In general, mi will cover invalid lat/lon. Just pull in to a reasonable
    # area, we handling the actual cover later
    mi = mi.cover([Geodetic(10,10),Geodetic(20,20)])
    s = self.resolution_meter / mi.resolution_meter
    mi = mi.scale(s, s)
    return mi

def _rotated_map(self, igc):
    '''Rotate a MapInfo so it follows the ground path of the center of 
    the igc. This is something like the minimum gore for passing between 
    the two points of the center top and bottom edge of of the igc.

    We preserve the coordinate system, and map resolution, and cover the
    igc. In general, although this is the same resolution as the starting
    MapInfo it won't be the same size.

    The new MapInfo is returned.
    '''
    gc1 = igc.ground_coordinate(ImageCoordinate(0, igc.number_sample / 2))
    gc2 = igc.ground_coordinate(ImageCoordinate(igc.number_line - 1,
                                                igc.number_sample / 2))
    
    return igc.cover(_rotated_map_gc(self, gc1, gc2))

MapInfo.rotated_map = _rotated_map


