// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "quickbird_time_table.h"
%}
%base_import(time_table)
%geocal_shared_ptr(GeoCal::QuickBirdTimeTable);
namespace GeoCal {

class QuickBirdTimeTable : public TimeTable {
public:
  QuickBirdTimeTable(const std::string& Fname);
  const std::string& file_name() const;
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const;
  virtual int min_line() const;
  virtual int max_line() const;
  virtual Time min_time() const;
  virtual Time max_time() const;
};
}
// List of things "import *" will include
%python_export("QuickBirdTimeTable")
