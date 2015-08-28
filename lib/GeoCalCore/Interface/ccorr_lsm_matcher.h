#ifndef CCORR_LSM_MATCHER_H
#define CCORR_LSM_MATCHER_H
#include "ccorr_matcher.h"
#include "lsm_matcher.h"
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace GeoCal {
/****************************************************************//**
  This class performs image matching. It first does a cross
  correlation match using CcorrMatcher to get to within a pixel, and
  then follows up with a least squares match using LsmMatcher to get
  subpixel accuracy. 
*******************************************************************/

class CcorrLsmMatcher : public ImageMatcher {
public:
//-----------------------------------------------------------------------
/// Constructor. This uses all the defaults for CcorrMatcher and
/// LsmMatcher. 
///
/// There are two possibilities when a LSM fails. We can either have
/// the whole match fail, or alternatively we can fall back on doing
/// the Cccor only. The choice depends on how you are using the
/// matches - are more matches of ok quality important, or do you only
/// want the best possible matches as the cost of some coverage?
//-----------------------------------------------------------------------

  CcorrLsmMatcher(bool Accept_ccorr_only = false) 
    : accept_ccorr_only_(Accept_ccorr_only),
      ccorr_matcher_(new CcorrMatcher),
      lsm_matcher_(new LsmMatcher) {}

//-----------------------------------------------------------------------
/// Constructor. This takes the CcorrMatcher and LsmMatcher, which
/// allows you to use something other than the default values
/// for the construction of these.
///
/// There are two possibilities when a LSM fails. We can either have
/// the whole match fail, or alternatively we can fall back on doing
/// the Cccor only. The choice depends on how you are using the
/// matches - are more matches of ok quality important, or do you only
/// want the best possible matches as the cost of some coverage?
//-----------------------------------------------------------------------

  CcorrLsmMatcher(const boost::shared_ptr<ImageMatcher>& Ccorr,
		  const boost::shared_ptr<ImageMatcher>& Lsm,
		  bool Accept_ccorr_only = false) 
    : accept_ccorr_only_(Accept_ccorr_only),
      ccorr_matcher_(Ccorr),
      lsm_matcher_(Lsm) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~CcorrLsmMatcher() {}

//-----------------------------------------------------------------------
/// ImageMatcher used.
//-----------------------------------------------------------------------
  
  const ImageMatcher& ccorr_matcher() const {return *ccorr_matcher_;}
  boost::shared_ptr<ImageMatcher> ccorr_matcher_ptr() const 
  {return ccorr_matcher_;}
//-----------------------------------------------------------------------
/// LsmMatcher used.
//-----------------------------------------------------------------------
  
  const ImageMatcher& lsm_matcher() const {return *lsm_matcher_;}
  boost::shared_ptr<ImageMatcher> lsm_matcher_ptr() const 
  {return lsm_matcher_;}
  virtual void match_mask(const RasterImage& Ref, const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, const
			  ImageCoordinate& New_guess, 
			  ImageCoordinate& New_res,
			  double& Line_sigma, double& Sample_sigma,
			  bool& Success,
			  int* Diagnostic = 0) const;
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// True if we will accept a point where the least squares matcher
/// fails but the cross correlation matcher succeeds. If allowed, a 
/// match like this will have a much larger line and sample sigma, but
/// will be marked as successful.
///
/// If this flag is false, then we require both matchers to be 
/// successful to match a point as successful. 
//-----------------------------------------------------------------------

  bool accept_ccorr_only() const { return accept_ccorr_only_; }
private:
  bool accept_ccorr_only_;
  boost::shared_ptr<ImageMatcher> ccorr_matcher_;
  boost::shared_ptr<ImageMatcher> lsm_matcher_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(CcorrLsmMatcher);
#endif

