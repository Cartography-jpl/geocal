import ogr
import os.path
import collections

class ShapeFile(collections.Mapping):
    '''library OGR. You can see supported formats at 
    http://gdal.org/ogr/ogr_formats.html. This only supports a subset of the
    available functions, but it gives a simpler interface. You can also just
    directly use data_source and layers["blah"].layer which are 
    ogr.DataSource and ogr.Layer'''

    # Need to add support for using as a "with" file
    def __init__(self, fname, mode = "r", driver_name = "ESRI Shapefile"):
        '''Open the given file, with the given mode. If you are writing, 
        you can supply the driver name to use.'''
        # Shape files don't like being overwritten, so we remove
        if(mode == "w" and driver_name == "ESRI Shapefile"and
           os.path.exists(fname)):
            os.remove(fname)
        self.layers = {}
        if(mode == "r"):
            self.data_source = ogr.Open(fname)
            if(self.data_source is None):
                raise RuntimeError("Error opening file %s" % fname)
            for i in range(self.data_source.GetLayerCount()):
                lay = ShapeLayer(self, i)
                self.layers[lay.name] = lay
        elif(mode =="w"):
            self.data_source = ogr.GetDriverByName(driver_name).CreateDataSource(fname)
        else:
            raise RuntimeError("Invalid mode %s" % mode)

    @property
    def file_name(self):
        return self.data_source.GetName()

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.close()

    def __len__(self):
        return self.layers.__len__()

    def __iter__(self):
        return self.layers.__iter__()
    
    def __getitem__(self, key):
        return self.layers.__getitem__(key)

    def close(self):
        '''Close the file and write everything to disk'''
        self.data_source.SyncToDisk()
        self.data_source = None

class ShapeLayer(object):
    '''This class handles access to a single layers in a Shapefile.'''
    def __init__(self, shape_file, index):
        '''Create ShapeLayer for given ShapeFile and index. This isn't 
        normally called directly, instead go through ShapeFile'''
        self.shape_file = shape_file
        self.layer = self.shape_file.data_source.GetLayer(index)
        self.field_list = [self.layer.GetLayerDefn().GetFieldDefn(i).GetName() for i in range(self.layer.GetLayerDefn().GetFieldCount())]

    @property
    def name(self):
        return self.layer.GetName()
