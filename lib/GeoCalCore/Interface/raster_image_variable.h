#ifndef RASTER_IMAGE_VARIABLE_H
#define RASTER_IMAGE_VARIABLE_H
#include "raster_image.h"

namespace GeoCal {
/****************************************************************//**
  Much of the time the constructor for a RasterImage determine the
  size, map_info, and tile size. This helper class holds these values
  in protected member function variables and fills in the
  corresponding functions.
*******************************************************************/
class RasterImageVariable : public RasterImage {
public:
  virtual ~RasterImageVariable() {}
  virtual int number_line() const {return number_line_;}
  virtual int number_sample() const {return number_sample_; }
  virtual int number_tile_line() const {return number_tile_line_;}
  virtual int number_tile_sample() const {return number_tile_sample_;}
  virtual bool has_map_info() const {return map_info_.get();}
  virtual const MapInfo& map_info() const 
  {
    if(!has_map_info())
      throw Exception("Do not have MapInfo for this RasterImage");
    return *map_info_;
  }
  virtual bool has_rpc() const {return rpc_.get();}
//-----------------------------------------------------------------------
/// Remove an RPC.
//-----------------------------------------------------------------------
  void remove_rpc() { rpc_.reset(); }

  virtual Rpc rpc() const 
  {
    if(!has_rpc())
      throw Exception("Do not have RPC for this RasterImage");
    return *rpc_;
  }

  virtual bool has_rsm() const {return rsm_.get();}
//-----------------------------------------------------------------------
/// Remove an RSM.
//-----------------------------------------------------------------------
  void remove_rsm() { rsm_.reset(); }

  virtual boost::shared_ptr<Rsm> rsm() const 
  {
    if(!has_rsm())
      throw Exception("Do not have RSM for this RasterImage");
    return rsm_;
  }

//-----------------------------------------------------------------------
/// We need special handling for maps that are crossing the
/// dateline. For most projections this isn't an issue - we just pick
/// one that doesn't do anything in particular crossing the dateline. 
/// But for the special case of a Geodetic map projection, we flip
/// signs so the point with longitude -180 is right next to 179.999.
/// To handle this, we can change the map projection to work with a
/// Geodetic360. This is really the same map projection, we just label
/// the longitude differently.
///
/// Note this is a noop if the coordinate converter is already a
/// Geodetic360Converter, and changes if it is a
/// GeodeticConverter. Any other projection will cause an exception to
/// be thrown.
//-----------------------------------------------------------------------

  virtual void change_to_geodetic360()
  {
    map_info_->change_to_geodetic360();
  }

//-----------------------------------------------------------------------
/// Change back to -180 to 180
//-----------------------------------------------------------------------

  virtual void change_to_geodetic()
  {
    map_info_->change_to_geodetic();
  }

//-----------------------------------------------------------------------
/// Simple test to see if we are Geodetic360, sometimes class need
/// special handling for this
//-----------------------------------------------------------------------

  bool is_geodetic_360() const { return map_info_->is_geodetic_360(); }
protected:
//-----------------------------------------------------------------------
/// Default constructor, derived classes should fill in protected
/// variables. 
//-----------------------------------------------------------------------

  RasterImageVariable() {}

//-----------------------------------------------------------------------
/// Constructor that we give a size to. The tile size is set to the
/// full size of the image, and map_info_ is set to null.
//-----------------------------------------------------------------------

  RasterImageVariable(int Nline, int Nsamp)
    : number_line_(Nline), number_sample_(Nsamp),
      number_tile_line_(Nline), number_tile_sample_(Nsamp)
  {}

//-----------------------------------------------------------------------
/// Constructor that we give a size and tile size to. 
/// map_info_ is set to null.
//-----------------------------------------------------------------------

  RasterImageVariable(int Nline, int Nsamp, int Ntileln, int Ntilesmp)
    : number_line_(Nline), number_sample_(Nsamp),
      number_tile_line_(Ntileln), number_tile_sample_(Ntilesmp)
  {}

//-----------------------------------------------------------------------
/// Constructor that takes the given MapInfo. The tile size is set to
/// the full size of the image.
//-----------------------------------------------------------------------

  RasterImageVariable(const MapInfo& Mi)
    : number_line_(Mi.number_y_pixel()), number_sample_(Mi.number_x_pixel()),
      number_tile_line_(Mi.number_y_pixel()), 
      number_tile_sample_(Mi.number_x_pixel()),
      map_info_(new MapInfo(Mi))
  { }

//-----------------------------------------------------------------------
/// Constructor that we find the size information from the given
/// sample image. We set the tile size to the full size of the image,
/// and copy the map and RPC information. 
///
/// Note that this is *not* a copy constructor, we don't copy the
/// underlying data.
//-----------------------------------------------------------------------
  RasterImageVariable(const RasterImage& Img)
    : number_line_(Img.number_line()), number_sample_(Img.number_sample()),
      number_tile_line_(Img.number_line()), 
      number_tile_sample_(Img.number_sample())
  {
    if(Img.has_map_info())
      map_info_.reset(new MapInfo(Img.map_info()));
    if(Img.has_rpc())
      rpc_.reset(new Rpc(Img.rpc()));
    if(Img.has_rsm())
      rsm_ = Img.rsm();;
  }      

  int number_line_, number_sample_, number_tile_line_, number_tile_sample_;
  boost::shared_ptr<MapInfo> map_info_;
  boost::shared_ptr<Rpc> rpc_;
  boost::shared_ptr<Rsm> rsm_;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}
GEOCAL_EXPORT_KEY(RasterImageVariable);
GEOCAL_CLASS_VERSION(RasterImageVariable, 1);
#endif
