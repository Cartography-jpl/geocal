#include "rsm_indirect_covariance_b.h"
#include "rsm_id.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "ostream_pad.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmBSubgroup::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmBSubgroup);
  ar & GEOCAL_NVP_(cov)
    & GEOCAL_NVP_(time_correlation)
    & GEOCAL_NVP_(time_correlation_domain_flag);
}

template<class Archive>
void RsmBUnmodeledCovariance::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmBUnmodeledCovariance);
  ar & GEOCAL_NVP_(cov)
    & GEOCAL_NVP_(row_time_correlation)
    & GEOCAL_NVP_(col_time_correlation);
}

template<class Archive>
void RsmIndirectCovarianceB::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmIndirectCovariance)
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBParameterDesc)
    & GEOCAL_NVP(cvdate)
    & GEOCAL_NVP_(subgroup)
    & GEOCAL_NVP_(m)
    & GEOCAL_NVP_(ucov);
}

GEOCAL_IMPLEMENT(RsmBSubgroup);
GEOCAL_IMPLEMENT(RsmBUnmodeledCovariance);
GEOCAL_IMPLEMENT(RsmIndirectCovarianceB);
#endif

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

static boost::format numint("%|1$02d|");
static boost::format numint1("%|1$01d|");
static boost::format numint_missing("%|1$2s|");
static boost::format num("%|1$+21.14E|");
static boost::format numpow("%|1$1d|%|2$1d|%|3$1d|");
static boost::format date("%|1$-8s|");

std::string RsmIndirectCovarianceB::tre_string() const
{
  check_consistent();
  std::string res = base_tre_string();
  res += (has_indirect_error_covariance() ? "Y" : "N");
  res += (has_unmodeled_error_covariance() ? "Y" : "N");
  if(has_indirect_error_covariance()) {
    res += str_check_size(numint % number_original_parameter(), 2);
    int num_subgroup = (int) subgroup_.size();
    res += str_check_size(numint % num_subgroup, 2);
    res += str_check_size(date % cvdate, 8);
    res += rsm_b_parm_tre_string();
    for(int i = 0; i < num_subgroup; ++i)
      res += subgroup_[i]->tre_string();
    for(int i = 0; i < m_.rows(); ++i)
      for(int j = 0; j < m_.rows(); ++j)
	res += str_check_size(num % m_(i,j), 21);
  }
  if(has_unmodeled_error_covariance()) {
    res += ucov_->tre_string();
  }
  return res;
}

//-----------------------------------------------------------------------
/// Read a TRE string. Note that the TRE does not contain all the
/// fields we have in a RsmRationalPolynomial. However the fields that
/// aren't contained are ones used for fitting the RSM, so in practice
/// this doesn't matter. We just set the various fields to the default
/// values found in the constructor.
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

boost::shared_ptr<RsmIndirectCovarianceB>
RsmIndirectCovarianceB::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmIndirectCovarianceB> res(new RsmIndirectCovarianceB);
  res->base_read_tre_string(in);
  std::string inclic = read_size<std::string>(in, 1);
  std::string incluc = read_size<std::string>(in, 1);
  if(inclic == "Y") {
    int norgparm = read_size<int>(in, 2);
    int num_subgroup = read_size<int>(in, 2);
    res->cvdate = read_size<std::string>(in, 8);
    boost::trim(res->cvdate);
    res->rsm_b_parm_read_tre_string(in);
    for(int i = 0; i < num_subgroup; ++i) {
      auto subgrp = boost::make_shared<RsmBSubgroup>();
      subgrp->read_tre_string(in);
      res->add_subgroup(subgrp);
    }
    res->m_.resize(res->number_parameter(), res->number_original_parameter());
    for(int i = 0; i < res->m_.rows(); ++i)
      for(int j = 0; j < res->m_.cols(); ++j)
	res->m_(i,j) = read_size<double>(in, 21);
  }
  if(incluc == "Y") {
    res->ucov_ = boost::make_shared<RsmBUnmodeledCovariance>();
    res->ucov_->read_tre_string(in);
  }
  res->check_consistent();
  return res;
}

//-----------------------------------------------------------------------
/// We have a few different pieces that need to match sizes. We don't
/// check this as the object is created/modified because we may be in
/// an inconsistent state during construction (e.g., we have added
/// subgroups, but haven't yet modified the mapping_matrix). This
/// routine does that consistency check, and throws an exception if
/// things aren't consistent. Called before creating and after reading
/// a TRE.
//-----------------------------------------------------------------------

void RsmIndirectCovarianceB::check_consistent() const
{
  if(has_indirect_error_covariance()) {
    if(m_.rows() != number_parameter() ||
       m_.cols() != number_original_parameter()) {
      Exception e;
      e << "mapping_matrix needs to be number_parameter x number_original_parameter\n"
	<< "   number_parameter:          " << number_parameter() << "\n"
	<< "   number_original_parameter: " << number_original_parameter()
	<< "\n"
	<< "   mapping_matrix:            " << m_.rows() << " x "
	<< m_.cols() << "\n";
      throw e;
    }
  }
}

//-----------------------------------------------------------------------
/// Create a RsmIndirectCovarianceB. We create a LocalRcConverter
/// that matches the given Igc, set the Image_identifier and
/// Rsm_support_data_edition to match that of the given Rsm_id.
//-----------------------------------------------------------------------

RsmIndirectCovarianceB::RsmIndirectCovarianceB
(const ImageGroundConnection& Igc,
 double H_min, double H_max,
 const RsmId& Rsm_id,
 const std::string& Triangulation_id)
  : RsmBParameterDesc(Igc, H_min, H_max),
    RsmIndirectCovariance(Rsm_id.image_identifier(),
			  Rsm_id.rsm_support_data_edition(),
			  Triangulation_id),
    cvdate("20200707")
{
}

RsmIndirectCovarianceB::RsmIndirectCovarianceB(const RsmIndirectCovarianceB& V)
  : RsmIndirectCovariance(V.image_identifier(), V.rsm_support_data_edition(),
			  V.triangulation_id()),
    cvdate(V.covariance_version_date()),
    m_(V.mapping_matrix().copy())
{
  for(auto i : V.subgroup())
    add_subgroup(boost::make_shared<RsmBSubgroup>(*i));
  if(V.has_unmodeled_error_covariance())
    ucov_ = boost::make_shared<RsmBUnmodeledCovariance>(*V.unmodeled_covariance());
}

int RsmIndirectCovarianceB::number_original_parameter() const
{
  int res = 0;
  for(auto i : subgroup_)
    res += i->number_parameter();
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmIndirectCovarianceB::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmIndirectCovarianceB:\n"
     << "  Image Identifier: " << image_identifier() << "\n"
     << "  RSM Edition:      " << rsm_support_data_edition() << "\n"
     << "  Triangulation ID: " << triangulation_id() << "\n";
  RsmBParameterDesc::print(opad);
  opad.strict_sync();
  if(has_indirect_error_covariance()) {
    Os << "  Subgroups:\n";
    for(auto i : subgroup_)
      opad << *i;
    opad.strict_sync();
    Os << "  mapping_matrix:\n";
    opad << m_;
    opad.strict_sync();
  } else {
    Os << "  Subgroups: None\n";
  }
}

//-----------------------------------------------------------------------
/// Create a RsmBSubgroup. This is a collection of "original
/// parameters" that have a covariance matrix and time behavior.
///
/// The time_correlation_domain_flag should be 0, 1 or 2.
///
/// If this field is 0, the time correlation applies to all time
/// intervals, both within and between images. The associated errors
/// in the original adjustable parameters are “image element errors”.
/// 
/// If this field is 1, the time correlation applies to time intervals
/// between images only. Time correlation for time intervals within an
/// image is defined 100% positively correlated. The associated errors
/// in the original adjustable parameters are “image errors”.
///
/// If this field is 2, the time correlation applies to time intervals
/// within an image only. Time correlation for time intervals between
/// images is defined as zero. The associated errors in the original
/// adjustable parameters are “restricted image element errors”.
///
/// The time_correlation is described by a matrix of values. The
/// matrix is either numseg x 2 for the piecewise linear model
/// (first column is segment correlation value, second is segment
/// tau value) or 1 x 4 for the CSM model (first column is A
/// parameter, second is alpha, third is beta, and fourth is T
/// parameter). Note that is a little clumsy, I had considered
/// separate classes for these. But there is only 2 types (no other
/// types can fit into the TRE) and they really only differ in the
/// parameters we use to describe them. So for now we use this model. 
//-----------------------------------------------------------------------

RsmBSubgroup::RsmBSubgroup
(const blitz::Array<double, 2> Cov,
 int Time_correlation_domain_flag,
 const blitz::Array<double, 2> Time_correlation)
  : cov_(Cov.copy()),
    time_correlation_domain_flag_(Time_correlation_domain_flag),
    time_correlation_(Time_correlation.copy())
{
  if(Cov.rows() != Cov.cols())
    throw Exception("Cov needs to be square");
  range_check(Time_correlation_domain_flag, 0, 3);
  if(!(time_correlation_.cols() == 2 ||
       (time_correlation_.rows() == 1 && time_correlation_.cols() == 4))) {
    Exception e;
    e << "Time_correlation must be nseg x 2 (for piecewise linear model)"
      << "or 1 x 4 (for CSM model";
    throw e;
  }
}

RsmBSubgroup::RsmBSubgroup
(const blitz::Array<double, 2> Cov,
 int Time_correlation_domain_flag,
 double A, double alpha, double beta, double T)
  : cov_(Cov.copy()),
    time_correlation_domain_flag_(Time_correlation_domain_flag),
    time_correlation_(1, 4)
{
  if(Cov.rows() != Cov.cols())
    throw Exception("Cov needs to be square");
  range_check(Time_correlation_domain_flag, 0, 3);
  time_correlation_ = A, alpha, beta, T;
}

RsmBUnmodeledCovariance::RsmBUnmodeledCovariance
(const blitz::Array<double, 2> Cov,
 const blitz::Array<double, 2> Row_time_correlation,
 const blitz::Array<double, 2> Col_time_correlation)
  : cov_(Cov.copy()),
    row_time_correlation_(Row_time_correlation.copy()),
    col_time_correlation_(Col_time_correlation.copy())
{
  if(Cov.rows() != Cov.cols())
    throw Exception("Cov needs to be square");
  if(!(row_time_correlation_.cols() == 2 ||
       (row_time_correlation_.rows() == 1 && row_time_correlation_.cols() == 4))) {
    Exception e;
    e << "Time_correlation must be nseg x 2 (for piecewise linear model)"
      << "or 1 x 4 (for CSM model";
    throw e;
  }
  if(!(col_time_correlation_.cols() == 2 ||
       (col_time_correlation_.rows() == 1 && col_time_correlation_.cols() == 4))) {
    Exception e;
    e << "Time_correlation must be nseg x 2 (for piecewise linear model)"
      << "or 1 x 4 (for CSM model";
    throw e;
  }
  if(col_time_correlation_.cols() != row_time_correlation_.cols())
    throw Exception("row and col need to be the same kind of time correlation");
}

RsmBUnmodeledCovariance::RsmBUnmodeledCovariance
(const blitz::Array<double, 2> Cov,
 double Row_A, double Row_alpha, double Row_beta, double Row_T,
 double Col_A, double Col_alpha, double Col_beta, double Col_T)
: cov_(Cov.copy()),
  row_time_correlation_(1, 4),
  col_time_correlation_(1, 4)
{
  if(Cov.rows() != Cov.cols())
    throw Exception("Cov needs to be square");
  row_time_correlation_ = Row_A, Row_alpha, Row_beta, Row_T;
  col_time_correlation_ = Col_A, Col_alpha, Col_beta, Col_T;
}

std::string RsmBSubgroup::tre_string() const
{
  std::string res = str_check_size(numint % number_parameter(), 2);
  for(int i = 0; i < number_parameter(); ++i)
    for(int j = i; j < number_parameter(); ++j)
      res += str_check_size(num % cov_(i, j), 21);
  res += str_check_size(numint1 % time_correlation_domain_flag_, 1);
  res += (is_csm_correlation() ? "Y" : "N");
  if(!is_csm_correlation()) {
    res += str_check_size(numint1 % time_correlation_.rows(), 1);
    for(int i = 0; i < time_correlation_.rows(); ++i) {
      res += str_check_size(num % time_correlation_(i, 0), 21);
      res += str_check_size(num % time_correlation_(i, 1), 21);
    }
  } else {
    for(int i = 0; i < 4; ++i)
      res += str_check_size(num % time_correlation_(0, i), 21);
  }
  return res;
}

void RsmBSubgroup::read_tre_string(std::istream& In)
{
  int nparm = read_size<int>(In, 2);
  cov_.resize(nparm, nparm);
  for(int i = 0; i < number_parameter(); ++i)
    for(int j = i; j < number_parameter(); ++j) {
      cov_(i, j) = read_size<double>(In, 21);
      cov_(j, i) = cov_(i, j);
    }
  time_correlation_domain_flag_ = read_size<int>(In, 1);
  std::string is_csm = read_size<std::string>(In, 1);
  if(is_csm != "Y") {
    int nseg = read_size<int>(In, 1);
    time_correlation_.resize(nseg, 2);
    for(int i = 0; i < time_correlation_.rows(); ++i) {
      time_correlation_(i, 0) = read_size<double>(In, 21);
      time_correlation_(i, 1) = read_size<double>(In, 21);
    }
  } else {
    time_correlation_.resize(1,4);
    for(int i = 0; i < 4; ++i)
      time_correlation_(0, i) = read_size<double>(In, 21);
  }
}

std::string RsmBUnmodeledCovariance::tre_string() const
{
  std::string res;
  for(int i = 0; i < cov_.rows(); ++i)
    for(int j = i; j < cov_.cols(); ++j)
      res += str_check_size(num % cov_(i, j), 21);
  res += (is_csm_correlation() ? "Y" : "N");
  if(!is_csm_correlation()) {
    res += str_check_size(numint1 % row_time_correlation_.rows(), 1);
    for(int i = 0; i < row_time_correlation_.rows(); ++i) {
      res += str_check_size(num % row_time_correlation_(i, 0), 21);
      res += str_check_size(num % row_time_correlation_(i, 1), 21);
    }
    res += str_check_size(numint1 % col_time_correlation_.rows(), 1);
    for(int i = 0; i < col_time_correlation_.rows(); ++i) {
      res += str_check_size(num % col_time_correlation_(i, 0), 21);
      res += str_check_size(num % col_time_correlation_(i, 1), 21);
    }
  } else {
    for(int i = 0; i < 4; ++i)
      res += str_check_size(num % row_time_correlation_(0, i), 21);
    for(int i = 0; i < 4; ++i)
      res += str_check_size(num % col_time_correlation_(0, i), 21);
  }
  return res;
}

void RsmBUnmodeledCovariance::read_tre_string(std::istream& In)
{
  cov_.resize(2, 2);
  for(int i = 0; i < cov_.rows(); ++i)
    for(int j = i; j < cov_.cols(); ++j) {
      cov_(i, j) = read_size<double>(In, 21);
      cov_(j, i) = cov_(i, j);
    }
  std::string is_csm = read_size<std::string>(In, 1);
  if(is_csm != "Y") {
    int nseg = read_size<int>(In, 1);
    row_time_correlation_.resize(nseg, 2);
    for(int i = 0; i < row_time_correlation_.rows(); ++i) {
      row_time_correlation_(i, 0) = read_size<double>(In, 21);
      row_time_correlation_(i, 1) = read_size<double>(In, 21);
    }
    nseg = read_size<int>(In, 1);
    col_time_correlation_.resize(nseg, 2);
    for(int i = 0; i < col_time_correlation_.rows(); ++i) {
      col_time_correlation_(i, 0) = read_size<double>(In, 21);
      col_time_correlation_(i, 1) = read_size<double>(In, 21);
    }
  } else {
    row_time_correlation_.resize(1,4);
    for(int i = 0; i < 4; ++i)
      row_time_correlation_(0, i) = read_size<double>(In, 21);
    col_time_correlation_.resize(1,4);
    for(int i = 0; i < 4; ++i)
      col_time_correlation_(0, i) = read_size<double>(In, 21);
  }
}

void RsmBSubgroup::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmBSubgroup:\n"
     << "  Time correlation domain flag: "
     << time_correlation_domain_flag() << "\n"
     << "  Covariance: \n";
  opad << covariance() << "\n";
  opad.strict_sync();
  Os << "  Time correlation:\n";
  if(is_csm_correlation()) {
    Os << "     A:     " << time_correlation_(0,0) << "\n"
       << "     alpha: " << time_correlation_(0,1) << "\n"
       << "     beta:  " << time_correlation_(0,2) << "\n"
       << "     T:     " << time_correlation_(0,3) << "\n";
  } else {
    opad << time_correlation_ << "\n";
  opad.strict_sync();
  }
}

void RsmBUnmodeledCovariance::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmBUnmodeledCovariance:\n"
     << "  Covariance: \n";
  opad << covariance() << "\n";
  opad.strict_sync();
  Os << "  Time correlation:\n";
  if(is_csm_correlation()) {
    Os << "     Row A:     " << row_time_correlation_(0,0) << "\n"
       << "     Row alpha: " << row_time_correlation_(0,1) << "\n"
       << "     Row beta:  " << row_time_correlation_(0,2) << "\n"
       << "     Row T:     " << row_time_correlation_(0,3) << "\n"
       << "     Col A:     " << col_time_correlation_(0,0) << "\n"
       << "     Col alpha: " << col_time_correlation_(0,1) << "\n"
       << "     Col beta:  " << col_time_correlation_(0,2) << "\n"
       << "     Col T:     " << col_time_correlation_(0,3) << "\n";
  } else {
    opad << row_time_correlation_ << "\n";
    opad << col_time_correlation_ << "\n";
    opad.strict_sync();
  }
}


