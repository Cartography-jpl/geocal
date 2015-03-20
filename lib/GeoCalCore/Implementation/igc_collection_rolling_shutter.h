#ifndef IGC_COLLECTION_ROLLING_SHUTTER_H
#define IGC_COLLECTION_ROLLING_SHUTTER_H
#include "igc_collection.h"
#include "igc_rolling_shutter.h"

namespace GeoCal {
/****************************************************************//**
  This is a IgcCollection that is a collection of IgcRollingShutter.
  These all share the same Orbit and Camera.
*******************************************************************/

class IgcCollectionRollingShutter : public virtual IgcCollection,
		 public virtual WithParameterNested {
public:
  IgcCollectionRollingShutter
  (const std::vector<boost::shared_ptr<RasterImage> >& Img_list, 
   const std::vector<boost::shared_ptr<TimeTable> >& Tt_list,
   const std::vector<std::string>& Title_list,
   const boost::shared_ptr<Orbit>& Orb,
   const boost::shared_ptr<Camera>& Cam,
   const boost::shared_ptr<Dem>& D)
    : orb(Orb), cam(Cam), dem_(D), img_list(Img_list), tt_list(Tt_list),
      title_list(Title_list)
  {
    add_object(Orb);
    add_object(Cam);
    if(img_list.size() != tt_list.size() ||
       img_list.size() != title_list.size())
      throw Exception("All the lists need to be the same size.");
  }
  IgcCollectionRollingShutter
  (const boost::shared_ptr<Orbit>& Orb,
   const boost::shared_ptr<Camera>& Cam,
   const boost::shared_ptr<Dem>& D)
    : orb(Orb), cam(Cam), dem_(D)
  {
    add_object(Orb);
    add_object(Cam);
  }
  virtual ~IgcCollectionRollingShutter() {}
  virtual int number_image() const { return (int) tt_list.size(); }

  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<IgcCollection>  
 subset(const std::vector<int>& Index_set) const;

//-----------------------------------------------------------------------
/// Add a image and time table to create a ImageGroundConnection to
/// the end of our collection.
//-----------------------------------------------------------------------

  void add_image(const boost::shared_ptr<RasterImage>& Img,
		 const boost::shared_ptr<TimeTable>& Tt,
		 const std::string& Title)
  {
    img_list.push_back(Img);
    tt_list.push_back(Tt);
    title_list.push_back(Title);
  }

//-----------------------------------------------------------------------
/// Orbit we are using.
//-----------------------------------------------------------------------

  boost::shared_ptr<Orbit> orbit() const { return orb; }
  void orbit(const boost::shared_ptr<Orbit>& Orb)
  { 
    clear_object();
    clear_cache();
    orb = Orb;
    add_object(orb);
    add_object(cam);
  }

//-----------------------------------------------------------------------
/// Camera we are using.
//-----------------------------------------------------------------------
  boost::shared_ptr<Camera> camera() const { return cam; }
  void camera(const boost::shared_ptr<Camera>& Cam)
  { 
    clear_object();
    clear_cache();
    cam = Cam;
    add_object(orb);
    add_object(cam);
  }

  virtual blitz::Array<double, 1> parameter() const
  { return WithParameterNested::parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { WithParameterNested::parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return WithParameterNested::parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { WithParameterNested::parameter_with_derivative(Parm);}
private:
  boost::shared_ptr<Orbit> orb;
  boost::shared_ptr<Camera> cam;
  boost::shared_ptr<Dem> dem_;
  std::vector<boost::shared_ptr<RasterImage> > img_list;
  std::vector<boost::shared_ptr<TimeTable> > tt_list;
  std::vector<std::string> title_list;
  mutable std::vector<boost::shared_ptr<ImageGroundConnection> > igc_cache;
  void clear_cache() const
  {
    igc_cache.resize(0);
  }

  IgcCollectionRollingShutter() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcCollectionRollingShutter);
#endif
