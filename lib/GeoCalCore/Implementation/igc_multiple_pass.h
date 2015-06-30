#ifndef IGC_MULTIPLE_PASS_H
#define IGC_MULTIPLE_PASS_H
#include "igc_collection.h"

namespace GeoCal {
/****************************************************************//**
  This is a IgcCollection that stitches a collection of IgcCollection
  together. The use case is multiple passes , e.g., AirMSPI having
  multiple collections in a day. Typically you will have a shared set
  of parameters that you are jointly fitting for, e.g., fitting for
  the camera parameters over multiple passes of AirMSPI. This class is
  a WithParameterNested, but it doesn't automatically add anything
  into the parameter list. The normal case is you would create the
  IgcMultiplePass, and then add in the objects you want. For example:
  
      boost::shared_ptr<IgcCollection> igc_pass1(new AirMspiIgcCollection
       (orb1, cam, gim, Dem, l1b1_file_name1));
      boost::shared_ptr<IgcCollection> igc_pass2(new AirMspiIgcCollection
       (orb2, cam, gim, Dem, l1b1_file_name2));
      IgcMultiplePass igc;
      igc.add_igc(igc_pass1);
      igc.add_igc(igc_pass2);
      igc.add_object(cam);
      igc.add_object(gim);

  The images are stacked together. So if the first IgcCollection Igc1 has
  5 image, the second Igc2 6, the third Igc3 2 then in the
  IgcMultiplePass image 0 is 0th image in Igc1, image 5 is the 0th
  image in Igc2, image 12 is image 1 of Igc3. The total number of
  images is 13.
*******************************************************************/

class IgcMultiplePass : public virtual IgcCollection,
		 public virtual WithParameterNested {
public:
//-----------------------------------------------------------------------
/// Constructor that create an empty object.
//-----------------------------------------------------------------------

  IgcMultiplePass() {};

//-----------------------------------------------------------------------
/// Constructor that takes a list of IgcCollection to initialize with.
//-----------------------------------------------------------------------

  IgcMultiplePass(const std::vector<boost::shared_ptr<IgcCollection> >& Igc_list)
    : igc_list(Igc_list) {}

  virtual ~IgcMultiplePass() {}
  void add_igc(const boost::shared_ptr<IgcCollection> Igccol)
  { igc_list.push_back(Igccol); }
  virtual int number_image() const;
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;

  virtual blitz::Array<double, 1> parameter() const
  { return WithParameterNested::parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { WithParameterNested::parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return WithParameterNested::parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { WithParameterNested::parameter_with_derivative(Parm);}
private:
  std::vector<boost::shared_ptr<IgcCollection> > igc_list;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcMultiplePass);
#endif

