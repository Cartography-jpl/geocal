#include "mspi_camera.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor, which creates a MspiCamera from the given
/// configuration file.
//-----------------------------------------------------------------------

MspiCamera::MspiCamera(const std::string& File_name)
  :fname(File_name)
{
}

// See base class for description
FrameCoordinate MspiCamera::frame_coordinate
(const ScLookVector& Sl, int Band) const
{
}

// See base class for description
ScLookVector MspiCamera::sc_look_vector
(const FrameCoordinate& F, int Band) const
{
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void MspiCamera::print(std::ostream& Os) const
{
  Os << "MspiCamera:\n"
     << "  File name: " << file_name() << "\n";
}

