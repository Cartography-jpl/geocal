try:
    # Depending of the build options, this might be missing. Just skip 
    # ShapeFile if we don't have this.
    import ogr
    import osr
    have_shape_file = True
except ImportError:
    have_shape_file = False

import os.path
import collections
import weakref

if(have_shape_file):
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
        ext = os.path.splitext(fname)
        if(mode == "w" and driver_name == "ESRI Shapefile" and
           ext == ".shp" and os.path.exists(fname)):
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
        if(self.data_source is not None):
            self.data_source.SyncToDisk()
        self.data_source = None

    def __del__(self):
        self.close()

    def add_layer(self, name, geometry_type, fields, spatial_reference = osr.SpatialReference(osr.GetUserInputAsWKT("WGS84"))):
        '''Add a layer of the given geometry type with the given fields.

        For example:
            t.add_layer("out", ogr.wkbPolygon,
                [["File", ogr.OFTString, 100],
                ["Row", ogr.OFTInteger],])

        Each field has a name, type, optionally a width, and optionally a
        precision.

        You can specify the spatial reference, but the default is WGS84'''
        data_layer = self.data_source.CreateLayer(name, spatial_reference,
                                                  geometry_type)
        for v in fields:
            f = ogr.FieldDefn(v[0], v[1])
            if(len(v) >= 3):
                f.SetWidth(v[2])
            if(len(v) >= 4):
                f.SetPrecision(v[3])
            data_layer.CreateField(f)
        lay= ShapeLayer(self, self.data_source.GetLayerCount() - 1)
        self.layers[lay.name] = lay
        return lay

  class ShapeLayer(collections.Sequence):
    '''This class handles access to a single layers in a Shapefile.'''
    def __init__(self, shape_file, index):
        '''Create ShapeLayer for given ShapeFile and index. This isn't 
        normally called directly, instead go through ShapeFile'''
        self.shape_file = weakref.proxy(shape_file)
        self.layer = self.shape_file.data_source.GetLayer(index)
        self.field_list = [self.layer.GetLayerDefn().GetFieldDefn(i).GetName() for i in range(self.layer.GetLayerDefn().GetFieldCount())]

    @property
    def name(self):
        return self.layer.GetName()

    @staticmethod
    def linear_ring_2d(v):
        '''Create a linear  ring using 2d points. We pass in an array,
        with each entry in the array being an array of 2 values.'''
        g = ogr.Geometry(ogr.wkbLinearRing)
        for i in v:
            g.AddPoint_2D(*i)
        return g

    @staticmethod
    def polygon_2d(*v):
        '''Create a polygon using 2D points. A polygon has one exterior
        ring and zero or more interior rings. This gets passed as one or
        more arrays, with each array having arrays of 2 values.
        
        We close any rings passed in, so you don't have to give the closing
        point if you don't want to (although it doesn't hurt to do so).'''
        g = ogr.Geometry(ogr.wkbPolygon)
        for vf in v:
            gf = ShapeLayer.linear_ring_2d(vf)
            gf.CloseRings()
            g.AddGeometry(gf)
        return g

    @staticmethod
    def point_2d(x, y):
        '''Create a 2D point'''
        g = ogr.Geometry(ogr.wkbPoint)
        g.AddPoint_2D(x, y)
        return g

    def add_feature(self, d):
        '''Add a feature. We pass in a dictionary of keyword/value pairs. The 
        keyword "Geometry" is used to set the geometry. You can optionally
        include a "Style" to pass in a style string.

        As a convenience, this returns this object, so you can add multiple 
        features in a row'''
        f = ogr.Feature(self.layer.GetLayerDefn())
        for name, value in d.items():
            if(name =="Geometry"):
                f.SetGeometry(value)
            elif(name =="Style"):
                f.SetStyleString(value)
            else:
                f.SetField(name, value)
        self.layer.CreateFeature(f)
        return self
        

    def set_filter_rect(self, x1, y1, x2, y2):
        '''Filter the features so only those that intersect the rectangle
        are returned. The x and y are given in whatever the spatial
        reference is for the layer, e.g., longitude and latitude
        (note the order, X is longitude)'''
        self.layer.SetSpatialFilterRect(x1, y1, x2, y2)

    def set_spatial_filter(self, ogr_geometry):
        '''Filter the features based on a spatial filter. The spatial filter 
        should be an Ogr::Geometry, and we filter based on intersecting 
        that geometry. Note that the special case of a rectangle filter is
        handled by set_filter_rect, which has an easier interface (although
        there is nothing wrong with creating a rectangle as an Ogr::Geometry
        and calling this function if desired.
        
        Note that you can additionally filter by attributes using 
        set_attribute_filter.'''
        self.layer.SetSpatialFilter(ogr_geometry)

    def set_attribute_filter(self, where_clause):
        '''Filter the features based on an SQL where clause on the attributes.
        Note that this is in addition to any spatial filters, you can have
        both a spatial and an attribute filter at the same time.'''
        self.layer.SetAttributeFilter(where_clause)

    def clear_filter(self):
        '''Clear whatever filters have been set.'''
        self.layer.SetSpatialFilter(None)
        self.layer.SetAttributeFilter(None)

    def geometry_collection(self):
        '''Return the geometry union of all the features. This is useful for 
        example if this layer is POI list and we are finding the intersection
        with a footprint layer.'''
        gcol = ogr.Geometry(ogr.wkbGeometryCollection)
        for ft in self:
            gcol.AddGeometry(ft["Geometry"])
        return gcol

    def __getitem__(self, index):
        if(index < 0 or index >= len(self)):
            raise IndexError
        self.layer.SetNextByIndex(index)
        return ShapeFeature(self, self.layer.GetNextFeature())

    def __len__(self):
        return self.layer.GetFeatureCount()
    

class ShapeFeature(collections.Mapping):
    '''This handles a Feature in a Layer.'''
    def __init__(self, shape_layer, ogr_feature):
        self.layer = weakref.proxy(shape_layer)
        self.feature = ogr_feature

    def keys():
        res = ["Geometry"]
        res.extend(self.layer.field_list)
        return res

    def __iter__(self):
        for k in self.keys():
            yield k
        return

    def __len__(self, key):
        return 1 + len(self.layer.field_list)

    def __getitem__(self, key):
        if(key == "Geometry"):
            return self.feature.GetGeometryRef()
        else:
            ind = self.feature.GetFieldIndex(key)
            if(ind < 0):
                raise KeyError(key)
            return self.feature.GetField(ind)

    
