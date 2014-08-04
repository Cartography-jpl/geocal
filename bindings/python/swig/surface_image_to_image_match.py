# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _surface_image_to_image_match.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_surface_image_to_image_match', [dirname(__file__)])
        except ImportError:
            import _surface_image_to_image_match
            return _surface_image_to_image_match
        if fp is not None:
            try:
                _mod = imp.load_module('_surface_image_to_image_match', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _surface_image_to_image_match = swig_import_helper()
    del swig_import_helper
else:
    import _surface_image_to_image_match
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except:
    weakref_proxy = lambda x: x


SHARED_PTR_DISOWN = _surface_image_to_image_match.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

def _new_vector(cls, version, lst):
    '''Create a vector from a list.'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    for i in lst:
       inst.append(i)
    return inst

def _new_from_set(cls, version, *args):
    '''For use with pickle, covers common case where we use a set function 
    to assign the value'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    inst.set(*args)
    return inst

import geocal_swig.image_to_image_match
import geocal_swig.generic_object
import geocal_swig.geocal_exception
import geocal_swig.look_vector
class SurfaceImageToImageMatch(geocal_swig.image_to_image_match.ImageToImageMatch):
    """
    This is an ImageToImageMatch that takes two image ground connections
    along with a MapInfo to project to.

    The data is matched at the surface, although the results are reported
    in image coordinates.

    The MapInfo is used to get the resolution and map projection, the
    actual coverage isn't important (e.g., the ulc and lrc). We make sure
    that the coverage works over the full image.

    Depending on your application, we can do the map projection only where
    we need an image patch for matching, or you can supply the full
    surface image.

    If you are collecting tiepoints over a large image, you usually want
    to do the map projection on the fly as needed, since we typically only
    map project a small part of the image.

    In some cases, you may be doing lots of points or simply already have
    the data map projected. In those cases, you can pass the RasterImage
    in. These should be map projected, with a MapInfo (e.g., geotiff label
    or whatever in the data).

    C++ includes: surface_image_to_image_match.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        SurfaceImageToImageMatch::SurfaceImageToImageMatch(const boost::shared_ptr< ImageGroundConnection > &Igc1, const
        boost::shared_ptr< RasterImage > &Surface_image1, const
        boost::shared_ptr< ImageGroundConnection > &Igc2, const
        boost::shared_ptr< RasterImage > &Surface_image2, const
        boost::shared_ptr< ImageMatcher > &Matcher)
        Constructor, where we supply the already map projected data. 
        """
        _surface_image_to_image_match.SurfaceImageToImageMatch_swiginit(self,_surface_image_to_image_match.new_SurfaceImageToImageMatch(*args))
    def match_surf(self, *args):
        """
        void SurfaceImageToImageMatch::match_surf(const GroundCoordinate &Gc, ImageCoordinate &Ic1, ImageCoordinate
        &Ic2, double &Line_sigma, double &Sample_sigma, bool &Success, int
        *Diagnostic=0) const
        Variation of match where we supply the ground location to start with.

        This can be faster than match if the particular ImageGroundConnection
        has a ground_coordinate that is expensive (typically ground_coordinate
        takes more time than image_coordinate).

        Note that you don't need to worry about including height in Gc, only
        the latitude and longitude is important (however the height doesn't
        hurt either). 
        """
        return _surface_image_to_image_match.SurfaceImageToImageMatch_match_surf(self, *args)

    def _v_image_ground_connection1(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::SurfaceImageToImageMatch::image_ground_connection1() const
        ImageGroundConnection for image 1. 
        """
        return _surface_image_to_image_match.SurfaceImageToImageMatch__v_image_ground_connection1(self)

    @property
    def image_ground_connection1(self):
        return self._v_image_ground_connection1()

    def _v_image_ground_connection2(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::SurfaceImageToImageMatch::image_ground_connection2() const
        ImageGroundConnection for image 2. 
        """
        return _surface_image_to_image_match.SurfaceImageToImageMatch__v_image_ground_connection2(self)

    @property
    def image_ground_connection2(self):
        return self._v_image_ground_connection2()

    def _v_matcher(self):
        """
        const boost::shared_ptr<ImageMatcher>& GeoCal::SurfaceImageToImageMatch::matcher() const
        Matcher we are using. 
        """
        return _surface_image_to_image_match.SurfaceImageToImageMatch__v_matcher(self)

    @property
    def matcher(self):
        return self._v_matcher()

    def _v_map_project_on_demand(self):
        """
        bool GeoCal::SurfaceImageToImageMatch::map_project_on_demand() const
        Indicate if we are doing the map projection on demand, or if we
        already have the data map projected. 
        """
        return _surface_image_to_image_match.SurfaceImageToImageMatch__v_map_project_on_demand(self)

    @property
    def map_project_on_demand(self):
        return self._v_map_project_on_demand()

    def _v_surface_image1(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::SurfaceImageToImageMatch::surface_image1() const
        Surface projected image 1. 
        """
        return _surface_image_to_image_match.SurfaceImageToImageMatch__v_surface_image1(self)

    @property
    def surface_image1(self):
        return self._v_surface_image1()

    def _v_surface_image2(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::SurfaceImageToImageMatch::surface_image2() const
        Surface projected image 2. 
        """
        return _surface_image_to_image_match.SurfaceImageToImageMatch__v_surface_image2(self)

    @property
    def surface_image2(self):
        return self._v_surface_image2()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
        if(self.map_project_on_demand):
          return _new_from_init, (self.__class__, 1, self.image_ground_connection1,
    			      self.image_ground_connection2, 
    			      self.surface_image1.map_info(),
    			      self.matcher)
        else:
          return _new_from_init, (self.__class__, 1, self.image_ground_connection1,
    			      self.surface_image1,
    			      self.image_ground_connection2, 
    			      self.surface_image2,
    			      self.matcher)
      
    __swig_destroy__ = _surface_image_to_image_match.delete_SurfaceImageToImageMatch
SurfaceImageToImageMatch.match_surf = new_instancemethod(_surface_image_to_image_match.SurfaceImageToImageMatch_match_surf,None,SurfaceImageToImageMatch)
SurfaceImageToImageMatch._v_image_ground_connection1 = new_instancemethod(_surface_image_to_image_match.SurfaceImageToImageMatch__v_image_ground_connection1,None,SurfaceImageToImageMatch)
SurfaceImageToImageMatch._v_image_ground_connection2 = new_instancemethod(_surface_image_to_image_match.SurfaceImageToImageMatch__v_image_ground_connection2,None,SurfaceImageToImageMatch)
SurfaceImageToImageMatch._v_matcher = new_instancemethod(_surface_image_to_image_match.SurfaceImageToImageMatch__v_matcher,None,SurfaceImageToImageMatch)
SurfaceImageToImageMatch._v_map_project_on_demand = new_instancemethod(_surface_image_to_image_match.SurfaceImageToImageMatch__v_map_project_on_demand,None,SurfaceImageToImageMatch)
SurfaceImageToImageMatch._v_surface_image1 = new_instancemethod(_surface_image_to_image_match.SurfaceImageToImageMatch__v_surface_image1,None,SurfaceImageToImageMatch)
SurfaceImageToImageMatch._v_surface_image2 = new_instancemethod(_surface_image_to_image_match.SurfaceImageToImageMatch__v_surface_image2,None,SurfaceImageToImageMatch)
SurfaceImageToImageMatch_swigregister = _surface_image_to_image_match.SurfaceImageToImageMatch_swigregister
SurfaceImageToImageMatch_swigregister(SurfaceImageToImageMatch)



