// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "vicar_ogr.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%import "map_info.i"
%import "vicar_lite_file.i"
%import "vicar_file.i"
%geocal_shared_ptr(GeoCal::VicarOgr);

namespace GeoCal {
class VicarOgr: public GenericObject {
public:
  VicarOgr();
  void vicar_to_gtiff(const VicarFile& F, const std::string& Fname);
  void vicar_to_gtiff(const VicarLiteFile& F, const std::string& Fname);
  MapInfo from_vicar(const VicarFile& F);
  MapInfo from_vicar(const VicarLiteFile& F);
  void to_vicar(const MapInfo& Mi, VicarFile& F);
};

}

// List of things "import *" will include
%python_export("VicarOgr")
