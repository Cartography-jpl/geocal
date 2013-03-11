
// File: index.xml

// File: a00001.xml

%feature("docstring") AffineWarp "
";
%feature("docstring")  AffineWarp::getTransformation "
cv::Mat AffineWarp::getTransformation(std::vector< GeoCal::GeoCalCore::ImageCoor > &Ref_locs, std::vector<
GeoCal::GeoCalCore::ImageCoor > &Target_locs)
";
%feature("docstring")  AffineWarp::warp "
cv::Mat AffineWarp::warp(GeoCal::GeoCalCore::RasterImage &target, cv::Mat &transform)
";

// File: a00002.xml

%feature("docstring") GeoCal::AircraftOrbitData "
This is a single navigation file record for aircraft data.

This was originally written for data as reported by Applanix ephemeris
and attitude, but it is likely to be useful for other INU untis.

Note that we frequently don't have access to a direct measurement of
the velocity. Rather than trying to do some clever estimate of the
velocity, we just set it to 0. The velocity is only used for the
abberation of light calculation, which isn't important for aircraft
speeds. We can revisit this choice if needed, it wouldn't be too hard
to use a couple of position measurements at nearby times to estimate
the velocity.

C++ includes: aircraft_orbit_data.h ";

%feature("docstring")  GeoCal::AircraftOrbitData::AircraftOrbitData "
GeoCal::AircraftOrbitData::AircraftOrbitData(const Time &Tm, const GroundCoordinate &Position, const boost::array<
double, 3 > &Vel_fixed, double Roll, double Pitch, double Heading)
Constructor. ";

%feature("docstring")  GeoCal::AircraftOrbitData::AircraftOrbitData "
AircraftOrbitData::AircraftOrbitData(const Time &Tm, const GroundCoordinate &Position, const Time &Tm2,
const GroundCoordinate &Position2, double Roll, double Pitch, double
Heading)
Often with Aircraft data we don't have a direct measure of velocity.

This calculates a simple velocity by taking a second position and a
different time and assuming a constant velocity between the values. ";

%feature("docstring")  GeoCal::AircraftOrbitData::cf_look_vector "
CartesianFixedLookVector QuaternionOrbitData::cf_look_vector(const ScLookVector &Sl) const
Convert to CartesianFixedLookVector. ";

%feature("docstring")  GeoCal::AircraftOrbitData::ci_look_vector "
CartesianInertialLookVector QuaternionOrbitData::ci_look_vector(const ScLookVector &Sl) const
Convert to CartesianInertialLookVector. ";

%feature("docstring")  GeoCal::AircraftOrbitData::footprint "
std::vector< boost::shared_ptr< GroundCoordinate > > OrbitData::footprint(const Camera &C, const Dem &D, double Resolution=30, int Band=0,
double Max_height=9000) const
Return the footprint on the ground for the given camera and dem.

This just calls surface_intersect for the four corner points. ";

%feature("docstring")  GeoCal::AircraftOrbitData::frame_coordinate "
FrameCoordinate OrbitData::frame_coordinate(const GroundCoordinate &Gc, const Camera &C, int Band=0) const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::AircraftOrbitData::from_cf "
bool GeoCal::QuaternionOrbitData::from_cf() const
Was this created form the cartesian fixed version of the constructor?
This is intended for use by python when we pickle this object so we
know which constructor to call.

It probably isn't of interest to anything else. ";

%feature("docstring")  GeoCal::AircraftOrbitData::heading "
double GeoCal::AircraftOrbitData::heading() const
Heading, in degrees. ";

%feature("docstring")  GeoCal::AircraftOrbitData::pitch "
double GeoCal::AircraftOrbitData::pitch() const
Pitch, in degrees. ";

%feature("docstring")  GeoCal::AircraftOrbitData::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::QuaternionOrbitData::position_cf() const
Return position as a ptr. ";

%feature("docstring")  GeoCal::AircraftOrbitData::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::QuaternionOrbitData::position_ci() const
Return position as a ptr. ";

%feature("docstring")  GeoCal::AircraftOrbitData::position_geodetic "
const Geodetic& GeoCal::AircraftOrbitData::position_geodetic() const
Position of aircraft. ";

%feature("docstring")  GeoCal::AircraftOrbitData::print "
void AircraftOrbitData::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::AircraftOrbitData::print_to_string "
std::string GeoCal::Printable< OrbitData  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::AircraftOrbitData::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianFixed > OrbitData::reference_surface_intersect_approximate(const Camera &C, const FrameCoordinate &Fc, int Band=0, double
Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::AircraftOrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the center
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::AircraftOrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, const FrameCoordinate &Fc, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the given
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::AircraftOrbitData::roll "
double GeoCal::AircraftOrbitData::roll() const
Roll, in degrees. ";

%feature("docstring")  GeoCal::AircraftOrbitData::sc_look_vector "
ScLookVector QuaternionOrbitData::sc_look_vector(const CartesianInertialLookVector &Ci) const
Convert to ScLookVector. ";

%feature("docstring")  GeoCal::AircraftOrbitData::sc_look_vector "
ScLookVector QuaternionOrbitData::sc_look_vector(const CartesianFixedLookVector &Cf) const
Convert to ScLookVector. ";

%feature("docstring")  GeoCal::AircraftOrbitData::sc_to_cf "
boost::math::quaternion<double> GeoCal::QuaternionOrbitData::sc_to_cf() const
Return the quaternion used to go from spacecraft to cartesian fixed.
";

%feature("docstring")  GeoCal::AircraftOrbitData::sc_to_ci "
boost::math::quaternion<double> GeoCal::QuaternionOrbitData::sc_to_ci() const
Return the quaternion used to go from spacecraft to cartesian
inertial. ";

%feature("docstring")  GeoCal::AircraftOrbitData::surface_intersect "
boost::shared_ptr< CartesianFixed > OrbitData::surface_intersect(const Camera &C, const FrameCoordinate &Fc, const Dem &D, double
Resolution=30, int Band=0, double Max_height=9000) const
Calculate the intersection with the surface.

Resolution is the dem postings in meters, you usually don't want this
much more accurate than the intrinsic accuracy of the Dem (e.g., the
Did is 30 meter, so resolution shouldn't be better than 30). ";

%feature("docstring")  GeoCal::AircraftOrbitData::time "
virtual Time GeoCal::QuaternionOrbitData::time() const
Return Time of OrbitData. ";

%feature("docstring")  GeoCal::AircraftOrbitData::velocity_cf "
boost::array< double, 3 > QuaternionOrbitData::velocity_cf() const
Velocity in CartisianFixed coordinates. ";

%feature("docstring")  GeoCal::AircraftOrbitData::velocity_ci "
boost::array< double, 3 > QuaternionOrbitData::velocity_ci() const
Return velocity. ";

%feature("docstring")  GeoCal::AircraftOrbitData::~AircraftOrbitData "
virtual GeoCal::AircraftOrbitData::~AircraftOrbitData()
Destructor. ";


// File: a01118.xml

%feature("docstring") std::allocator "
STL class. ";


// File: a00003.xml

%feature("docstring") GeoCal::ApplyMask "
This applies a mask to a RasterImage.

The data either passes through the mask, or it set to a value of 0
(the normal gore value we use) if a second RasterImage indicates that
this is masked.

C++ includes: apply_mask.h ";

%feature("docstring")  GeoCal::ApplyMask::ApplyMask "
GeoCal::ApplyMask::ApplyMask(const boost::shared_ptr< RasterImage > &Data, const
boost::shared_ptr< RasterImage > &M, int Good_value=0)
Constructor.

Parameters:
-----------

Data:  Underlying data

M:   Mask image

Good_value:  Pixels with this value in the the image M allow ";

%feature("docstring")  GeoCal::ApplyMask::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::ApplyMask::good_value "
int GeoCal::ApplyMask::good_value() const
";
%feature("docstring")  GeoCal::ApplyMask::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::ApplyMask::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::ApplyMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ApplyMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ApplyMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ApplyMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ApplyMask::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::ApplyMask::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::ApplyMask::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::ApplyMask::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::ApplyMask::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::ApplyMask::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::ApplyMask::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::ApplyMask::mask "
boost::shared_ptr<RasterImage> GeoCal::ApplyMask::mask() const
";
%feature("docstring")  GeoCal::ApplyMask::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::ApplyMask::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::ApplyMask::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::ApplyMask::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::ApplyMask::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::ApplyMask::print "
virtual void GeoCal::ApplyMask::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ApplyMask::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ApplyMask::raw_data "
boost::shared_ptr<RasterImage> GeoCal::ApplyMask::raw_data() const
";
%feature("docstring")  GeoCal::ApplyMask::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::ApplyMask::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ApplyMask::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ApplyMask::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ApplyMask::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::ApplyMask::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ApplyMask::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::ApplyMask::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::ApplyMask::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::ApplyMask::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::ApplyMask::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::ApplyMask::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::ApplyMask::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::ApplyMask::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::ApplyMask::~ApplyMask "
virtual GeoCal::ApplyMask::~ApplyMask()
";

// File: a00004.xml

%feature("docstring") GeoCal::ArgusCamera "
A model of the ARGUS camera.

Right now, this doesn't account for any lens distortion, we model this
as a pinhole camera. We may need to revisit this.

C++ includes: argus_camera.h ";

%feature("docstring")  GeoCal::ArgusCamera::ArgusCamera "
ArgusCamera::ArgusCamera(double Yaw=0, double Pitch=0, double Roll=0, double Focal_length=400)
Create a ARGUS camera.

We read one of the image jpeg files to get camera metadata used to
describe the camera. Yaw, Pitch and Roll are in degrees, Focal_length
is in mm. ";

%feature("docstring")  GeoCal::ArgusCamera::direction "
virtual Direction GeoCal::Camera::direction() const
This gives the camera direction.

This is intended for use with steep camera angles (e.g., MISR AF
camera). For cameras that are near nadir looking, we can just
arbitrarily pick a direction for it.

The default version returns FORWARD. ";

%feature("docstring")  GeoCal::ArgusCamera::focal_length "
double GeoCal::ArgusCamera::focal_length() const
Focal length, in mm. ";

%feature("docstring")  GeoCal::ArgusCamera::frame_coordinate "
FrameCoordinate ArgusCamera::frame_coordinate(const ScLookVector &Sl, int Band) const
This converts from ScLookVector to FrameCoordinate for a given band.

Note that the FrameCoordinate may be outside of the range (0,
number_line(band) - 1), (0, number_sample(band) - 1), this just means
the look vector is not with the active portion of the camera (i.e., it
is not seen). ";

%feature("docstring")  GeoCal::ArgusCamera::frame_line_coordinate "
virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::ArgusCamera::integration_time "
virtual double GeoCal::Camera::integration_time(int Band) const
This is the integration time in seconds.

The integration time is how long it takes the camera to collect data.
It is important to model this, so that we can include movement of the
spacecraft while the camera is in the middle of collecting data. If
the integration time is very short, or if a particular derived class
doesn't wish to model this, then the integration time can be set to 0
seconds.

The default version returns 0.0. ";

%feature("docstring")  GeoCal::ArgusCamera::line_pitch "
double GeoCal::ArgusCamera::line_pitch() const
CCD pitch, in mm. ";

%feature("docstring")  GeoCal::ArgusCamera::number_band "
virtual int GeoCal::ArgusCamera::number_band() const
Number of bands in camera.

We don't treat the bands separately (at least for now), so this is set
to 1. ";

%feature("docstring")  GeoCal::ArgusCamera::number_line "
virtual int GeoCal::ArgusCamera::number_line(int Band) const
Number of lines in camera for given band. ";

%feature("docstring")  GeoCal::ArgusCamera::number_sample "
virtual int GeoCal::ArgusCamera::number_sample(int Band) const
Number of samples in camera for given band. ";

%feature("docstring")  GeoCal::ArgusCamera::pitch "
double GeoCal::ArgusCamera::pitch() const
Pitch angle, in degrees. ";

%feature("docstring")  GeoCal::ArgusCamera::print "
void ArgusCamera::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::ArgusCamera::print_to_string "
std::string GeoCal::Printable< Camera  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ArgusCamera::roll "
double GeoCal::ArgusCamera::roll() const
Roll angle, in degrees. ";

%feature("docstring")  GeoCal::ArgusCamera::sample_pitch "
double GeoCal::ArgusCamera::sample_pitch() const
CCD pitch, in mm. ";

%feature("docstring")  GeoCal::ArgusCamera::sc_look_vector "
ScLookVector ArgusCamera::sc_look_vector(const FrameCoordinate &F, int Band) const
Convert from FrameCoordinate to ScLookVector.

It is perfectly allowable for F.line to be outside the range (0,
number_line(band) 1) or for F.sample to be outside the range (0,
number_sample(band) - 1). The conversion will just act as if the
camera has infinite extent. ";

%feature("docstring")  GeoCal::ArgusCamera::yaw "
double GeoCal::ArgusCamera::yaw() const
Yaw angle, in degrees. ";

%feature("docstring")  GeoCal::ArgusCamera::~ArgusCamera "
virtual GeoCal::ArgusCamera::~ArgusCamera()
Destructor. ";


// File: a00005.xml

%feature("docstring") GeoCal::ArgusOrbit "
This read a CSV navigation file and uses it to generate a Orbit.

We access the data in two ways. The first is the standard time index,
giving a OrbitData for that time (interpolating if needed). The second
is by \"row\" and camera number. All the orbit data for a given row
were acquired at nearly the same time. A row contains data for 1 or
more cameras - once the instrument is fully working a row has data for
cameras 1 through 13. We return a ArgusOrbitData, which in addition to
having the normal OrbitData stuff has additional metadata information.

C++ includes: argus_orbit.h ";

%feature("docstring")  GeoCal::ArgusOrbit::ArgusOrbit "
ArgusOrbit::ArgusOrbit(const std::string &Fname)
Open the given file and use it to provide Orbit information.

This file should be a CSV file containing one record per line, with
the fields \"File,Camera,Time,Lat,Lon,Alt,Roll,Pitch,Heading\". The
first line is assumed to be a header, and is discarded. The second
line is the epoch that the times are measured relative to.

Orbit data, indexed by the Time. This is a multimap because it is
entirely possible to have multiple entries in the navigation file with
the same time, e.g., two cameras collect data at the same time. ";

%feature("docstring")  GeoCal::ArgusOrbit::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::ArgusOrbit::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::ArgusOrbit::file_name "
const std::string& GeoCal::ArgusOrbit::file_name() const
";
%feature("docstring")  GeoCal::ArgusOrbit::focal_length "
double ArgusOrbit::focal_length(int camera_num) const
This finds the first row with orbit and image data for the given
camera.

We then read the metadata from the image do determine the reported
focal length. If we don't have any orbit or image data for the camera,
we return -1. ";

%feature("docstring")  GeoCal::ArgusOrbit::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::ArgusOrbit::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::ArgusOrbit::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::ArgusOrbit::nav "
boost::shared_ptr< ArgusOrbitData > ArgusOrbit::nav(int row, int camera_num) const
Return the ArgusOrbitData for the given row and camera, or if we don't
have any data there return a null pointer. ";

%feature("docstring")  GeoCal::ArgusOrbit::number_row "
int GeoCal::ArgusOrbit::number_row() const
Number of rows of data we have. ";

%feature("docstring")  GeoCal::ArgusOrbit::orbit_data "
boost::shared_ptr< OrbitData > OrbitQuaternionList::orbit_data(Time T) const
Return OrbitData for the given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::ArgusOrbit::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::ArgusOrbit::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::ArgusOrbit::print "
void ArgusOrbit::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ArgusOrbit::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ArgusOrbit::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::ArgusOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::ArgusOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::ArgusOrbit::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";


// File: a00006.xml

%feature("docstring") GeoCal::ArgusOrbitData "
This is a single ARGUS navigation file record.

In addition to the normal things in an OrbitData class, this contains
some other metadata. This matches what is stored in the ARGUS CSV
navigation file. The additional information is the file name of the
JPEG image file collected by the camera and the camera number.

Note that we don't have access to a direct measurement of the
velocity. Rather than trying to do some clever estimate of the
velocity, we just set it to 0.

C++ includes: argus_orbit.h ";

%feature("docstring")  GeoCal::ArgusOrbitData::add_ortho_to_image "
void ArgusOrbitData::add_ortho_to_image(const boost::shared_ptr< RasterImage > &M1, const boost::shared_ptr<
RasterImage > &M2, const boost::shared_ptr< RasterImage > &M3, const
Camera &Cam, const Dem &D, int Border=10) const
Add three color orthorectified image to an existing set of three
images.

We currently don't try to do any feathering, instead we simply add
imagery if it is currently fill (value of 0), or crop the data if it
already has data. ";

%feature("docstring")  GeoCal::ArgusOrbitData::ArgusOrbitData "
GeoCal::ArgusOrbitData::ArgusOrbitData(const Time &Tm, const std::string &File_name, int Camera_number,
const Geodetic &Position, const boost::array< double, 3 > &Vel_fixed,
double Roll, double Pitch, double Heading)
Constructor. ";

%feature("docstring")  GeoCal::ArgusOrbitData::camera_number "
int GeoCal::ArgusOrbitData::camera_number() const
Camera number. This is 1 - 13. ";

%feature("docstring")  GeoCal::ArgusOrbitData::cf_look_vector "
CartesianFixedLookVector QuaternionOrbitData::cf_look_vector(const ScLookVector &Sl) const
Convert to CartesianFixedLookVector. ";

%feature("docstring")  GeoCal::ArgusOrbitData::ci_look_vector "
CartesianInertialLookVector QuaternionOrbitData::ci_look_vector(const ScLookVector &Sl) const
Convert to CartesianInertialLookVector. ";

%feature("docstring")  GeoCal::ArgusOrbitData::file_name "
const std::string& GeoCal::ArgusOrbitData::file_name() const
File with JPEG data. ";

%feature("docstring")  GeoCal::ArgusOrbitData::footprint "
std::vector< boost::shared_ptr< GroundCoordinate > > OrbitData::footprint(const Camera &C, const Dem &D, double Resolution=30, int Band=0,
double Max_height=9000) const
Return the footprint on the ground for the given camera and dem.

This just calls surface_intersect for the four corner points. ";

%feature("docstring")  GeoCal::ArgusOrbitData::frame_coordinate "
FrameCoordinate OrbitData::frame_coordinate(const GroundCoordinate &Gc, const Camera &C, int Band=0) const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::ArgusOrbitData::from_cf "
bool GeoCal::QuaternionOrbitData::from_cf() const
Was this created form the cartesian fixed version of the constructor?
This is intended for use by python when we pickle this object so we
know which constructor to call.

It probably isn't of interest to anything else. ";

%feature("docstring")  GeoCal::ArgusOrbitData::heading "
double GeoCal::AircraftOrbitData::heading() const
Heading, in degrees. ";

%feature("docstring")  GeoCal::ArgusOrbitData::image "
GdalRasterImage ArgusOrbitData::image(int band=1) const
Access image for the given band.

The band is 1 based, because that is what Gdal uses. ";

%feature("docstring")  GeoCal::ArgusOrbitData::pitch "
double GeoCal::AircraftOrbitData::pitch() const
Pitch, in degrees. ";

%feature("docstring")  GeoCal::ArgusOrbitData::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::QuaternionOrbitData::position_cf() const
Return position as a ptr. ";

%feature("docstring")  GeoCal::ArgusOrbitData::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::QuaternionOrbitData::position_ci() const
Return position as a ptr. ";

%feature("docstring")  GeoCal::ArgusOrbitData::position_geodetic "
const Geodetic& GeoCal::AircraftOrbitData::position_geodetic() const
Position of aircraft. ";

%feature("docstring")  GeoCal::ArgusOrbitData::print "
void ArgusOrbitData::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ArgusOrbitData::print_to_string "
std::string GeoCal::Printable< OrbitData  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ArgusOrbitData::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianFixed > OrbitData::reference_surface_intersect_approximate(const Camera &C, const FrameCoordinate &Fc, int Band=0, double
Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::ArgusOrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the center
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::ArgusOrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, const FrameCoordinate &Fc, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the given
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::ArgusOrbitData::roll "
double GeoCal::AircraftOrbitData::roll() const
Roll, in degrees. ";

%feature("docstring")  GeoCal::ArgusOrbitData::save_ortho "
void ArgusOrbitData::save_ortho(const MapInfo &Mi, const Camera &Cam, const Dem &D, const std::string
&Fname, const std::string &Type, int Border=10, int Grid_spacing=1)
const
Save an three color orthorectified image to the given file.

The type should be \"img\", \"tif\", \"tifjpeg\" or \"vicar\" (\"tif\"
uses LZW lossless compression, \"tifjpeg\" uses jpeg lossy
compression).

The MapInfo governs the map projection and pixel size used. We however
only write out the subset of MapInfo that is needed to cover the
footprint of this orthorectified image.

To speed up the processing, you can give a Grid_spacing > 1. We
calculate the position in the input image exactly at the Grid_spacing
locations, and do a bilinear interpolation in between. If the
Grid_spacing is 1, then no interpolation is done.

This is a somewhat specific function, but I had similar functionality
in Ruby code working with ARGUS data. Ruby is a nice language, but
just sucks at garbage collection. So we move this functionality into
this routine where we can control the lifetimes of all of the data
involved. ";

%feature("docstring")  GeoCal::ArgusOrbitData::sc_look_vector "
ScLookVector QuaternionOrbitData::sc_look_vector(const CartesianInertialLookVector &Ci) const
Convert to ScLookVector. ";

%feature("docstring")  GeoCal::ArgusOrbitData::sc_look_vector "
ScLookVector QuaternionOrbitData::sc_look_vector(const CartesianFixedLookVector &Cf) const
Convert to ScLookVector. ";

%feature("docstring")  GeoCal::ArgusOrbitData::sc_to_cf "
boost::math::quaternion<double> GeoCal::QuaternionOrbitData::sc_to_cf() const
Return the quaternion used to go from spacecraft to cartesian fixed.
";

%feature("docstring")  GeoCal::ArgusOrbitData::sc_to_ci "
boost::math::quaternion<double> GeoCal::QuaternionOrbitData::sc_to_ci() const
Return the quaternion used to go from spacecraft to cartesian
inertial. ";

%feature("docstring")  GeoCal::ArgusOrbitData::surface_intersect "
boost::shared_ptr< CartesianFixed > OrbitData::surface_intersect(const Camera &C, const FrameCoordinate &Fc, const Dem &D, double
Resolution=30, int Band=0, double Max_height=9000) const
Calculate the intersection with the surface.

Resolution is the dem postings in meters, you usually don't want this
much more accurate than the intrinsic accuracy of the Dem (e.g., the
Did is 30 meter, so resolution shouldn't be better than 30). ";

%feature("docstring")  GeoCal::ArgusOrbitData::time "
virtual Time GeoCal::QuaternionOrbitData::time() const
Return Time of OrbitData. ";

%feature("docstring")  GeoCal::ArgusOrbitData::velocity_cf "
boost::array< double, 3 > QuaternionOrbitData::velocity_cf() const
Velocity in CartisianFixed coordinates. ";

%feature("docstring")  GeoCal::ArgusOrbitData::velocity_ci "
boost::array< double, 3 > QuaternionOrbitData::velocity_ci() const
Return velocity. ";

%feature("docstring")  GeoCal::ArgusOrbitData::~ArgusOrbitData "
virtual GeoCal::ArgusOrbitData::~ArgusOrbitData()
Destructor. ";

%feature("docstring")  GeoCal::ArgusOrbitData::mosaic "
void ArgusOrbitData::mosaic(const std::vector< boost::shared_ptr< ArgusOrbitData > > &Od, const
std::vector< boost::shared_ptr< Camera > > &Cam, const Dem &D, const
MapInfo &Mi, const std::string &Fname, const std::string &Type, int
Border=10)
Generate a mosaic that fits the given MapInfo, sampling each of the
orbit data.

We save this to the given file, with the given Type. ";


// File: a00007.xml

%feature("docstring") GeoCal::GeoCalCore::Array2d "
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::Array2d "
GeoCal::GeoCalCore::Array2d< T >::Array2d()
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::Array2d "
GeoCal::GeoCalCore::Array2d< T >::Array2d(size_type num_row, size_type num_col)
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::Array2d "
GeoCal::GeoCalCore::Array2d< T >::Array2d(size_type num_row, size_type num_col, const T &C)
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::begin "
const_iterator GeoCal::GeoCalCore::Array2d< T >::begin() const
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::begin "
iterator GeoCal::GeoCalCore::Array2d< T >::begin()
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::end "
const_iterator GeoCal::GeoCalCore::Array2d< T >::end() const
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::end "
iterator GeoCal::GeoCalCore::Array2d< T >::end()
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::number_col "
size_type GeoCal::GeoCalCore::Array2d< T >::number_col() const
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::number_row "
size_type GeoCal::GeoCalCore::Array2d< T >::number_row() const
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::rbegin "
const_reverse_iterator GeoCal::GeoCalCore::Array2d< T >::rbegin() const
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::rbegin "
reverse_iterator GeoCal::GeoCalCore::Array2d< T >::rbegin()
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::rend "
const_reverse_iterator GeoCal::GeoCalCore::Array2d< T >::rend() const
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::rend "
reverse_iterator GeoCal::GeoCalCore::Array2d< T >::rend()
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::resize "
void GeoCal::GeoCalCore::Array2d< T >::resize(size_type num_row, size_type num_col, const T &C)
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::resize "
void GeoCal::GeoCalCore::Array2d< T >::resize(size_type num_row, size_type num_col)
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::subset "
void GeoCal::GeoCalCore::Array2d< T >::subset(size_type Lstart, size_type Sstart, size_type Number_line, size_type
Number_sample, Array2d< T > &Res) const
";
%feature("docstring")  GeoCal::GeoCalCore::Array2d::~Array2d "
virtual GeoCal::GeoCalCore::Array2d< T >::~Array2d()
";

// File: a01119.xml

%feature("docstring") std::auto_ptr "
STL class. ";


// File: a01120.xml

%feature("docstring") std::bad_alloc "
STL class. ";


// File: a01121.xml

%feature("docstring") std::bad_cast "
STL class. ";


// File: a01122.xml

%feature("docstring") std::bad_exception "
STL class. ";


// File: a01123.xml

%feature("docstring") std::bad_typeid "
STL class. ";


// File: a01124.xml

%feature("docstring") std::basic_fstream "
STL class. ";


// File: a01132.xml

%feature("docstring") std::basic_ifstream "
STL class. ";


// File: a01129.xml

%feature("docstring") std::basic_ios "
STL class. ";


// File: a01127.xml

%feature("docstring") std::basic_iostream "
STL class. ";


// File: a01128.xml

%feature("docstring") std::basic_istream "
STL class. ";


// File: a01140.xml

%feature("docstring") std::basic_istringstream "
STL class. ";


// File: a01145.xml

%feature("docstring") std::basic_ofstream "
STL class. ";


// File: a01131.xml

%feature("docstring") std::basic_ostream "
STL class. ";


// File: a01148.xml

%feature("docstring") std::basic_ostringstream "
STL class. ";


// File: a00704.xml

%feature("docstring") std::basic_string "
STL class. ";


// File: a01137.xml

%feature("docstring") std::basic_stringstream "
STL class. ";


// File: a00008.xml

%feature("docstring") BasicPcorrMatcher "
";
%feature("docstring")  BasicPcorrMatcher::BasicPcorrMatcher "
BasicPcorrMatcher::BasicPcorrMatcher(int fft_size, int search_window, double thresh)
";
%feature("docstring")  BasicPcorrMatcher::match "
void BasicPcorrMatcher::match(cv::Mat &Ref, cv::Mat &New, cv::Point2f &Ref_loc, cv::Point2f
&New_guess, cv::Point2f &Result, bool *Success, double *Cov)
";

// File: a00009.xml

%feature("docstring") BilinearIntepolate "
";
%feature("docstring")  BilinearIntepolate::apply "
static void BilinearIntepolate::apply(T1 &A, T2 &B, T3, T4, T5, T6, T7, T8, T9, T10, T11)
";

// File: a01158.xml

%feature("docstring") std::bitset "
STL class. ";


// File: a00010.xml

%feature("docstring") GeoCal::CalcMapProjected "
This is a RasterImage that has been map projected by a ground to image
calculation.

There are several obvious variations of this - calculate by OrbitData
(e.g., Argus), by an Ipi (e.g., Quickbird), by an RPC. This base class
capture the common behavior.

To do this we do two steps:

We calculate roughly what the difference in resolution is between the
original data and final MapInfo. We do this by looking at the center
pixel of the original data and the pixel +1 in line and sample. We
then use RasterAveraged to average the original data to roughly the
resolution of the final MapInfo. If the final MapInfo is near the same
resolution as the original, or if it has a higher resolution, then we
don't do any averaging. Alternatively, you can pass in the averaging
factor.

We then interpolate the possibly averaged data to the final
projection.

It is ok if the final MapInfo contains areas outside of the original
data. For any pixel outside of the original data, we just return a
value of 0.

This class calculates the data on the fly. Sometimes this is what you
want, but if you are going to be using the resulting data a few times,
you may want to use a MemoryRasterImage to generate a copy once and
keep it in memory.

C++ includes: calc_map_projected.h ";

%feature("docstring")  GeoCal::CalcMapProjected::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::CalcMapProjected::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::CalcMapProjected::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::CalcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcMapProjected::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::CalcMapProjected::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::CalcMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::CalcMapProjected::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::CalcMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::CalcMapProjected::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::CalcMapProjected::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::CalcMapProjected::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::CalcMapProjected::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::CalcMapProjected::number_tile_line "
virtual int GeoCal::CalcMapProjected::number_tile_line() const
Number of lines in each tile. ";

%feature("docstring")  GeoCal::CalcMapProjected::number_tile_sample "
virtual int GeoCal::CalcMapProjected::number_tile_sample() const
Number of samples in each tile. ";

%feature("docstring")  GeoCal::CalcMapProjected::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::CalcMapProjected::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::CalcMapProjected::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CalcMapProjected::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::CalcMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcMapProjected::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcMapProjected::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::CalcMapProjected::read_ptr "
void CalcMapProjected::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcMapProjected::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::CalcMapProjected::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::CalcMapProjected::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::CalcMapProjected::unchecked_read "
int CalcMapProjected::unchecked_read(int Line, int Sample) const
Read pixel value at given line and sample. ";

%feature("docstring")  GeoCal::CalcMapProjected::unchecked_write "
void CalcMapProjected::unchecked_write(int Line, int Sample, int Val)
Write a value.

This isn't actually implemented for this type of RasterImage. ";

%feature("docstring")  GeoCal::CalcMapProjected::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::CalcMapProjected::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::CalcMapProjected::write_image "
void CalcMapProjected::write_image(RasterImage &Out, int Grid_spacing=1) const
input exactly at this grid spacing, and interpolate in between.

This is much faster than calculating ever point, and if the grid
spacing is small compared to the Dem and any nonlinearities give
results very close to the full calculation. ";

%feature("docstring")  GeoCal::CalcMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out) const
Write output to multiple RasterImage at once.

The number should be <= the number given to the constructor. ";

%feature("docstring")  GeoCal::CalcMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out, int
Grid_spacing) const
Write output to multiple RasterImage at once.

This variation takes a grid spacing to use. We calculate the image
coordinates in the input exactly at this grid spacing, and interpolate
in between. This is much faster than calculating ever point, and if
the grid spacing is small compared to the Dem and any Camera
nonlinearities give results very close to the full calculation. ";

%feature("docstring")  GeoCal::CalcMapProjected::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::CalcMapProjected::~CalcMapProjected "
virtual GeoCal::CalcMapProjected::~CalcMapProjected()
Destructor. ";


// File: a00011.xml

%feature("docstring") GeoCal::CalcRaster "
Some classes most naturally generate a RasterImage by calculating a
results for an array of data (e.g., a tile).

This class handles the common behavior for these classes.

C++ includes: calc_raster.h ";

%feature("docstring")  GeoCal::CalcRaster::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::CalcRaster::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::CalcRaster::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::CalcRaster::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcRaster::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcRaster::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcRaster::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::CalcRaster::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::CalcRaster::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::CalcRaster::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::CalcRaster::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::CalcRaster::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::CalcRaster::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::CalcRaster::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::CalcRaster::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::CalcRaster::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::CalcRaster::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::CalcRaster::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::CalcRaster::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::CalcRaster::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::CalcRaster::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CalcRaster::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::CalcRaster::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcRaster::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcRaster::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcRaster::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::CalcRaster::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::CalcRaster::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::CalcRaster::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::CalcRaster::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::CalcRaster::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::CalcRaster::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::CalcRaster::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::CalcRaster::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::CalcRaster::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::CalcRaster::~CalcRaster "
virtual GeoCal::CalcRaster::~CalcRaster()
";

// File: a00012.xml

%feature("docstring") GeoCal::CalcRasterMultiBand "
Some classes most naturally generate a RasterImageMultiBand by
calculating a results for an array of data (e.g., a tile).

This class handles the common behavior for these classes.

This is very similar to RasterImageMultiBand made upf of CalcRaster,
the difference is that for this class it makes sense to calculate all
the bands at once.

C++ includes: calc_raster_multi_band.h ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::add_raster_image "
void GeoCal::RasterImageMultiBandVariable::add_raster_image(const boost::shared_ptr< RasterImage > &Ri)
";
%feature("docstring")  GeoCal::CalcRasterMultiBand::number_band "
virtual int GeoCal::RasterImageMultiBandVariable::number_band() const
Return number of bands. ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::print "
virtual void GeoCal::RasterImageMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::CalcRasterMultiBand::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band) const
Return given band. ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band)
";
%feature("docstring")  GeoCal::CalcRasterMultiBand::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBandVariable::raster_image_ptr(int band) const
";
%feature("docstring")  GeoCal::CalcRasterMultiBand::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::read_double "
Array< double, 3 > CalcRasterMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::CalcRasterMultiBand::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";


// File: a00013.xml

%feature("docstring") GeoCal::Camera "
This class models a frame camera.

It is used to convert ScLookVector to FrameCoordinate and vice versa.

C++ includes: camera.h ";

%feature("docstring")  GeoCal::Camera::Camera "
GeoCal::Camera::Camera()
Default constructor. ";

%feature("docstring")  GeoCal::Camera::direction "
virtual Direction GeoCal::Camera::direction() const
This gives the camera direction.

This is intended for use with steep camera angles (e.g., MISR AF
camera). For cameras that are near nadir looking, we can just
arbitrarily pick a direction for it.

The default version returns FORWARD. ";

%feature("docstring")  GeoCal::Camera::frame_coordinate "
virtual FrameCoordinate GeoCal::Camera::frame_coordinate(const ScLookVector &Sl, int Band) const =0
This converts from ScLookVector to FrameCoordinate for a given band.

Note that the FrameCoordinate may be outside of the range (0,
number_line(band) - 1), (0, number_sample(band) - 1), this just means
the look vector is not with the active portion of the camera (i.e., it
is not seen). ";

%feature("docstring")  GeoCal::Camera::frame_line_coordinate "
virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::Camera::integration_time "
virtual double GeoCal::Camera::integration_time(int Band) const
This is the integration time in seconds.

The integration time is how long it takes the camera to collect data.
It is important to model this, so that we can include movement of the
spacecraft while the camera is in the middle of collecting data. If
the integration time is very short, or if a particular derived class
doesn't wish to model this, then the integration time can be set to 0
seconds.

The default version returns 0.0. ";

%feature("docstring")  GeoCal::Camera::number_band "
virtual int GeoCal::Camera::number_band() const =0
Number of bands in camera. ";

%feature("docstring")  GeoCal::Camera::number_line "
virtual int GeoCal::Camera::number_line(int Band) const =0
Number of lines in camera for given band. ";

%feature("docstring")  GeoCal::Camera::number_sample "
virtual int GeoCal::Camera::number_sample(int Band) const =0
Number of samples in camera for given band. ";

%feature("docstring")  GeoCal::Camera::print "
virtual void GeoCal::Camera::print(std::ostream &Os) const =0
Print to a stream. ";

%feature("docstring")  GeoCal::Camera::print_to_string "
std::string GeoCal::Printable< Camera  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Camera::sc_look_vector "
virtual ScLookVector GeoCal::Camera::sc_look_vector(const FrameCoordinate &F, int Band) const =0
Convert from FrameCoordinate to ScLookVector.

It is perfectly allowable for F.line to be outside the range (0,
number_line(band) 1) or for F.sample to be outside the range (0,
number_sample(band) - 1). The conversion will just act as if the
camera has infinite extent. ";

%feature("docstring")  GeoCal::Camera::~Camera "
virtual GeoCal::Camera::~Camera()
Destructor. ";


// File: a00014.xml

%feature("docstring") GeoCal::CartesianFixed "
This gives the fixed Cartesian Coordinates for a ground coordinate.

This is an abstract base class, there are CartesianFixed for the Earth
( Ecr), Mars (MarsFixed), etc.

We want to be able to convert to and from CartesianInertial
coordinates. However, we want to abstract out the toolkit used to do
this. We use the toolkit pointed to by
CartesianFixed::toolkit_coordinate_interface. If either SPICE or SDP
is available, then that toolkit is the one used for the conversion. If
we have both available, then we default to SPICE. You can change the
choice by updating the variable
CartesianFixed::toolkit_coordinate_interface.

C++ includes: ground_coordinate.h ";

%feature("docstring")  GeoCal::CartesianFixed::ci_to_cf "
virtual void GeoCal::CartesianFixed::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const =0
Calculate matrix that can be used to convert CartesianFixed to
CartesianInertial at the given Time.

The matrix for going from CartesianInertial to CartesianFixed is the
transpose of this matrix. ";

%feature("docstring")  GeoCal::CartesianFixed::ci_to_cf_quat "
boost::math::quaternion<double> GeoCal::CartesianFixed::ci_to_cf_quat(const Time &T) const
Calculate quaternion that can be used to convert CartesianFixed to
CartesianInertial at the given Time. ";

%feature("docstring")  GeoCal::CartesianFixed::convert_to_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianFixed::convert_to_cf() const
This calls create. ";

%feature("docstring")  GeoCal::CartesianFixed::convert_to_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::CartesianFixed::convert_to_ci(const Time &T) const =0
Convert to CartesianInertial. ";

%feature("docstring")  GeoCal::CartesianFixed::create "
virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianFixed::create(boost::array< double, 3 > P) const =0
Create an instance of whatever type of CartesianFixed this is. ";

%feature("docstring")  GeoCal::CartesianFixed::height_reference_surface "
double GeoCal::GroundCoordinate::height_reference_surface() const
Return height in meters above reference surface (e.g., WGS-84 for the
earth).

By default we just convert to CartesianFixed and then to height, but
derived classes can supply more efficient versions of these if needed.
";

%feature("docstring")  GeoCal::CartesianFixed::latitude "
double GeoCal::GroundCoordinate::latitude() const
Return latitude in degrees.

By default we just convert to CartesianFixed and then to latitude, but
derived classes can supply more efficient versions of these if needed.
Latitude is -90 to 90. ";

%feature("docstring")  GeoCal::CartesianFixed::longitude "
double GeoCal::GroundCoordinate::longitude() const
Return longitude in degrees.

By default we just convert to CartesianFixed and then to longitude,
but derived classes can supply more efficient versions of these if
needed. Longitude is -180 to 180. ";

%feature("docstring")  GeoCal::CartesianFixed::min_radius_reference_surface "
virtual double GeoCal::CartesianFixed::min_radius_reference_surface() const =0
This is the radius of the largest sphere that fits completely into the
reference surface (e.g., radius for a reference sphere, minimum of a
and b for a ellipsoid). ";

%feature("docstring")  GeoCal::CartesianFixed::print "
virtual void GeoCal::GroundCoordinate::print(std::ostream &Os) const =0
Print to given stream. ";

%feature("docstring")  GeoCal::CartesianFixed::print_to_string "
std::string GeoCal::Printable< GroundCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CartesianFixed::reference_surface_intersect_approximate "
virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianFixed::reference_surface_intersect_approximate(const CartesianFixedLookVector &Cl, double
Height_reference_surface=0) const =0
Find the intersection with the surface at the given height starting at
this point and looking in the given direction.

The intersection is done in an approximate but quicker manner, see the
derived classes for the approximation used. ";

%feature("docstring")  GeoCal::CartesianFixed::~CartesianFixed "
virtual GeoCal::CartesianFixed::~CartesianFixed()
Destructor. ";


// File: a00015.xml

%feature("docstring") GeoCal::CartesianFixedLookVector "
This is a look vector in CartesianFixed coordinates.

C++ includes: look_vector.h ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::CartesianFixedLookVector "
GeoCal::CartesianFixedLookVector::CartesianFixedLookVector()
Default constructor. Does not initialize look_vector. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::CartesianFixedLookVector "
GeoCal::CartesianFixedLookVector::CartesianFixedLookVector(const boost::array< double, 3 > &Lv)
Constructor. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::CartesianFixedLookVector "
GeoCal::CartesianFixedLookVector::CartesianFixedLookVector(double x, double y, double z)
Constructor. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::direction "
boost::array< double, 3 > LookVector::direction() const
This is the direction, as a unit vector. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::length "
double LookVector::length() const
";
%feature("docstring")  GeoCal::CartesianFixedLookVector::look_quaternion "
boost::math::quaternion<double> GeoCal::LookVector::look_quaternion() const
Look vector as a quaternion. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::look_quaternion "
void GeoCal::LookVector::look_quaternion(const boost::math::quaternion< double > &V)
Set look vector using a quaternion. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::print "
void CartesianFixedLookVector::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::print_to_string "
std::string GeoCal::Printable< LookVector  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CartesianFixedLookVector::~CartesianFixedLookVector "
virtual GeoCal::CartesianFixedLookVector::~CartesianFixedLookVector()
";

// File: a00016.xml

%feature("docstring") GeoCal::CartesianInertial "
This gives the location in a Cartesian inertial coordinate system
(.e.g., ECI).

See CartesianFixed for a discussion on converting to and from
CartesianFixed.

C++ includes: ground_coordinate.h ";

%feature("docstring")  GeoCal::CartesianInertial::ci_to_cf "
virtual void GeoCal::CartesianInertial::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const =0
Calculate matrix that can be used to convert CartesianFixed to
CartesianInertial at the given Time.

The matrix for going from CartesianInertial to CartesianFixed is the
transpose of this matrix. ";

%feature("docstring")  GeoCal::CartesianInertial::ci_to_cf_quat "
boost::math::quaternion<double> GeoCal::CartesianInertial::ci_to_cf_quat(const Time &T) const
Calculate quaternion that can be used to convert CartesianFixed to
CartesianInertial at the given Time. ";

%feature("docstring")  GeoCal::CartesianInertial::convert_to_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianInertial::convert_to_cf(const Time &T) const =0
Convert to a CartesianFixed. ";

%feature("docstring")  GeoCal::CartesianInertial::create "
virtual boost::shared_ptr<CartesianInertial> GeoCal::CartesianInertial::create(boost::array< double, 3 > P) const =0
Create an instance of whatever type of CartesianInertial this is. ";

%feature("docstring")  GeoCal::CartesianInertial::print "
virtual void GeoCal::CartesianInertial::print(std::ostream &Os) const =0
Print to given stream. ";

%feature("docstring")  GeoCal::CartesianInertial::print_to_string "
std::string GeoCal::Printable< CartesianInertial  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CartesianInertial::reference_surface_intersect_approximate "
virtual boost::shared_ptr<CartesianInertial> GeoCal::CartesianInertial::reference_surface_intersect_approximate(const CartesianInertialLookVector &Cl, double
Height_reference_surface=0) const =0
Find the intersection with the surface at the given height starting at
this point and looking in the given direction.

The intersection is done in an approximate but quicker manner, see the
derived classes for the approximation used. ";

%feature("docstring")  GeoCal::CartesianInertial::~CartesianInertial "
virtual GeoCal::CartesianInertial::~CartesianInertial()
Destructor. ";


// File: a00017.xml

%feature("docstring") GeoCal::CartesianInertialLookVector "
This is a look vector in CartesianInertial coordinates.

C++ includes: look_vector.h ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::CartesianInertialLookVector "
GeoCal::CartesianInertialLookVector::CartesianInertialLookVector()
Default constructor. Does not initialize look_vector. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::CartesianInertialLookVector "
GeoCal::CartesianInertialLookVector::CartesianInertialLookVector(const boost::array< double, 3 > &Lv)
Constructor. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::CartesianInertialLookVector "
GeoCal::CartesianInertialLookVector::CartesianInertialLookVector(double x, double y, double z)
Constructor. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::direction "
boost::array< double, 3 > LookVector::direction() const
This is the direction, as a unit vector. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::length "
double LookVector::length() const
";
%feature("docstring")  GeoCal::CartesianInertialLookVector::look_quaternion "
boost::math::quaternion<double> GeoCal::LookVector::look_quaternion() const
Look vector as a quaternion. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::look_quaternion "
void GeoCal::LookVector::look_quaternion(const boost::math::quaternion< double > &V)
Set look vector using a quaternion. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::print "
void CartesianInertialLookVector::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::print_to_string "
std::string GeoCal::Printable< LookVector  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CartesianInertialLookVector::~CartesianInertialLookVector "
virtual GeoCal::CartesianInertialLookVector::~CartesianInertialLookVector()
";

// File: a00018.xml

%feature("docstring") GeoCal::CcorrLsmMatcher "
This class performs image matching.

It first does a cross correlation match using CcorrMatcher to get to
within a pixel, and then follows up with a least squares match using
LsmMatcher to get subpixel accuracy.

C++ includes: ccorr_lsm_matcher.h ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::accept_ccorr_only "
bool GeoCal::CcorrLsmMatcher::accept_ccorr_only() const
True if we will accept a point where the least squares matcher fails
but the cross correlation matcher succeeds.

If allowed, a match like this will have a much larger line and sample
sigma, but will be marked as successful.

If this flag is false, then we require both matchers to be successful
to match a point as successful. ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::ccorr_matcher "
const CcorrMatcher& GeoCal::CcorrLsmMatcher::ccorr_matcher() const
CcorrMatcher used. ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::ccorr_matcher_ptr "
boost::shared_ptr<CcorrMatcher> GeoCal::CcorrLsmMatcher::ccorr_matcher_ptr() const
";
%feature("docstring")  GeoCal::CcorrLsmMatcher::CcorrLsmMatcher "
GeoCal::CcorrLsmMatcher::CcorrLsmMatcher(bool Accept_ccorr_only=false)
Constructor.

This uses all the defaults for CcorrMatcher and LsmMatcher.

There are two possibilities when a LSM fails. We can either have the
whole match fail, or alternatively we can fall back on doing the Cccor
only. The choice depends on how you are using the matches - are more
matches of ok quality important, or do you only want the best possible
matches as the cost of some coverage? ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::CcorrLsmMatcher "
GeoCal::CcorrLsmMatcher::CcorrLsmMatcher(const boost::shared_ptr< CcorrMatcher > &Ccorr, const
boost::shared_ptr< LsmMatcher > &Lsm, bool Accept_ccorr_only=false)
Constructor.

This takes the CcorrMatcher and LsmMatcher, which allows you to use
something other than the default values for the construction of these.

There are two possibilities when a LSM fails. We can either have the
whole match fail, or alternatively we can fall back on doing the Cccor
only. The choice depends on how you are using the matches - are more
matches of ok quality important, or do you only want the best possible
matches as the cost of some coverage? ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::lsm_matcher "
const LsmMatcher& GeoCal::CcorrLsmMatcher::lsm_matcher() const
LsmMatcher used. ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::lsm_matcher_ptr "
boost::shared_ptr<LsmMatcher> GeoCal::CcorrLsmMatcher::lsm_matcher_ptr() const
";
%feature("docstring")  GeoCal::CcorrLsmMatcher::match "
void CcorrLsmMatcher::match(const RasterImage &Ref, const RasterImage &New, const ImageCoordinate
&Ref_loc, const ImageCoordinate &New_guess, ImageCoordinate &New_res,
double &Line_sigma, double &Sample_sigma, bool &Success, int
*Diagnostic=0) const
Match a point found in the reference image with a point in the new
image.

A initial guess in the new image is supplied. We return an improved
image in the new image along with the uncertainty if Success is true,
otherwise the match failed.

The Diagnostic argument is used to indicate why a match failed. For
many purposes you can ignore this, because you need to know detailed
information about the specific ImageMatcher that a user passed to a
function and what the diagnostic codes mean. However, for some
purposes the failure reason can be useful (e.g., tuning parameters of
a LsmMatcher). This class just returns the Diagnostic returned by the
CcorrMatcher or the LsmMatcher. ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::print "
void CcorrLsmMatcher::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::print_to_string "
std::string GeoCal::Printable< ImageMatcher  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CcorrLsmMatcher::~CcorrLsmMatcher "
virtual GeoCal::CcorrLsmMatcher::~CcorrLsmMatcher()
Destructor. ";


// File: a00019.xml

%feature("docstring") GeoCal::CcorrMatcher "
This class performs image matching.

It does a normalized cross correlation match.

C++ includes: ccorr_matcher.h ";

%feature("docstring")  GeoCal::CcorrMatcher::CcorrMatcher "
CcorrMatcher::CcorrMatcher(int Target_nline=37, int Target_nsamp=37, int Template_nline=9, int
Template_nsamp=9, double Min_ccorr=0.3, double Min_variance=0)
";
%feature("docstring")  GeoCal::CcorrMatcher::match "
void CcorrMatcher::match(const RasterImage &Ref, const RasterImage &New, const ImageCoordinate
&Ref_loc, const ImageCoordinate &New_guess, ImageCoordinate &New_res,
double &Line_sigma, double &Sample_sigma, bool &Success, int
*Diagnostic=0) const
Match a point found in the reference image with a point in the new
image.

A initial guess in the new image is supplied. We return an improved
image in the new image along with the uncertainty if Success is true,
otherwise the match failed.

The Diagnostic argument is used to indicate why a match failed. For
many purposes you can ignore this, because you need to know detailed
information about the specific ImageMatcher that a user passed to a
function and what the diagnostic codes mean. However, for some
purposes the failure reason can be useful (e.g., tuning parameters of
a LsmMatcher). ";

%feature("docstring")  GeoCal::CcorrMatcher::min_correlation "
double GeoCal::CcorrMatcher::min_correlation() const
Minimum correlation threshold. ";

%feature("docstring")  GeoCal::CcorrMatcher::min_variance "
double GeoCal::CcorrMatcher::min_variance() const
Minimum variance threshold. ";

%feature("docstring")  GeoCal::CcorrMatcher::print "
void CcorrMatcher::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::CcorrMatcher::print_to_string "
std::string GeoCal::Printable< ImageMatcher  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CcorrMatcher::target_number_line "
int GeoCal::CcorrMatcher::target_number_line() const
Number of lines in target window. ";

%feature("docstring")  GeoCal::CcorrMatcher::target_number_sample "
int GeoCal::CcorrMatcher::target_number_sample() const
Number of samples in target window. ";

%feature("docstring")  GeoCal::CcorrMatcher::template_number_line "
int GeoCal::CcorrMatcher::template_number_line() const
Number of lines in template window. ";

%feature("docstring")  GeoCal::CcorrMatcher::template_number_sample "
int GeoCal::CcorrMatcher::template_number_sample() const
Number of samples in template window. ";

%feature("docstring")  GeoCal::CcorrMatcher::~CcorrMatcher "
virtual GeoCal::CcorrMatcher::~CcorrMatcher()
Destructor. ";


// File: a00020.xml

%feature("docstring") GeoCal::GeoCalCore::CodeLocation "
";
%feature("docstring")  GeoCal::GeoCalCore::CodeLocation::CodeLocation "
GeoCal::GeoCalCore::CodeLocation::CodeLocation(const char *File, int Line, const char *Function)
";
%feature("docstring")  GeoCal::GeoCalCore::CodeLocation::file "
const char* GeoCal::GeoCalCore::CodeLocation::file() const
";
%feature("docstring")  GeoCal::GeoCalCore::CodeLocation::function "
const char* GeoCal::GeoCalCore::CodeLocation::function() const
";
%feature("docstring")  GeoCal::GeoCalCore::CodeLocation::line "
int GeoCal::GeoCalCore::CodeLocation::line() const
";

// File: a00021.xml

%feature("docstring") GeoCal::CombinedMask "
This is combines a set of masks into a single one.

The combination just check each mask in turn, and if any of them
indicates a point or a region is masked, then we return true.

Note that this class handles the degenerate case of no masked. This is
particular useful in handling situations where you might or might not
have mask, e.g. a general routine that can take a mask but where you
don't happen to have any masks in a particular case.

When there are no masks, we return true.

C++ includes: mask.h ";

%feature("docstring")  GeoCal::CombinedMask::CombinedMask "
GeoCal::CombinedMask::CombinedMask()
Create a empty CombinedMask. ";

%feature("docstring")  GeoCal::CombinedMask::CombinedMask "
GeoCal::CombinedMask::CombinedMask(const std::vector< boost::shared_ptr< Mask > > &Ml)
Create a CombinedMask using the given set of masks. ";

%feature("docstring")  GeoCal::CombinedMask::CombinedMask "
GeoCal::CombinedMask::CombinedMask(It First, It Last)
Create a CombinedMask using the given set of masks, where we get this
from a general InputIterator. ";

%feature("docstring")  GeoCal::CombinedMask::mask "
bool CombinedMask::mask(const GroundCoordinate &Gc) const
Indicate if a particular point is masked.

If true, the point is masked and should not be used in processing
(e.g., don't do image matching) ";

%feature("docstring")  GeoCal::CombinedMask::print "
void CombinedMask::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::CombinedMask::print_to_string "
std::string GeoCal::Printable< Mask  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CombinedMask::region_masked "
bool CombinedMask::region_masked(const GroundCoordinate &Ulc, const GroundCoordinate &Lrc) const
Indicated if a region is all masked or not.

See the discussion in the comments of Mask for detailed discussion of
the check. ";

%feature("docstring")  GeoCal::CombinedMask::~CombinedMask "
virtual GeoCal::CombinedMask::~CombinedMask()
";

// File: a01159.xml

%feature("docstring") std::complex "
STL class. ";


// File: a01160.xml

%feature("docstring") std::map::const_iterator "
STL iterator class. ";


// File: a01161.xml

%feature("docstring") std::multimap::const_iterator "
STL iterator class. ";


// File: a01154.xml

%feature("docstring") std::basic_string::const_iterator "
STL iterator class. ";


// File: a01162.xml

%feature("docstring") std::set::const_iterator "
STL iterator class. ";


// File: a01163.xml

%feature("docstring") std::multiset::const_iterator "
STL iterator class. ";


// File: a01164.xml

%feature("docstring") std::string::const_iterator "
STL iterator class. ";


// File: a01165.xml

%feature("docstring") std::vector::const_iterator "
STL iterator class. ";


// File: a01166.xml

%feature("docstring") std::wstring::const_iterator "
STL iterator class. ";


// File: a01167.xml

%feature("docstring") std::deque::const_iterator "
STL iterator class. ";


// File: a01168.xml

%feature("docstring") std::list::const_iterator "
STL iterator class. ";


// File: a01169.xml

%feature("docstring") std::map::const_reverse_iterator "
STL iterator class. ";


// File: a01170.xml

%feature("docstring") std::multimap::const_reverse_iterator "
STL iterator class. ";


// File: a01171.xml

%feature("docstring") std::set::const_reverse_iterator "
STL iterator class. ";


// File: a01155.xml

%feature("docstring") std::basic_string::const_reverse_iterator "
STL iterator class. ";


// File: a01172.xml

%feature("docstring") std::multiset::const_reverse_iterator "
STL iterator class. ";


// File: a01173.xml

%feature("docstring") std::vector::const_reverse_iterator "
STL iterator class. ";


// File: a01174.xml

%feature("docstring") std::string::const_reverse_iterator "
STL iterator class. ";


// File: a01175.xml

%feature("docstring") std::wstring::const_reverse_iterator "
STL iterator class. ";


// File: a01176.xml

%feature("docstring") std::deque::const_reverse_iterator "
STL iterator class. ";


// File: a01177.xml

%feature("docstring") std::list::const_reverse_iterator "
STL iterator class. ";


// File: a00022.xml

%feature("docstring") GeoCal::ConstantSpacingTimeTable "
This is a time table that has a constant spacing between lines.

C++ includes: time_table.h ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::ConstantSpacingTimeTable "
ConstantSpacingTimeTable::ConstantSpacingTimeTable(Time Min_time, Time Max_time, double Time_space=40.8e-3)
Constructor, creates time table from Min_time to Max_time with given
Time spacing.

We adjust Max_time to exactly Min_time + i * Time_space, rounding to
nearest integer i, so it ok if Max_time is a little sloppy. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::image_coordinate "
ImageCoordinate ConstantSpacingTimeTable::image_coordinate(Time T, const FrameCoordinate &F) const
Convert from Time and FrameCoordinate to ImageCoordinate. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::max_line "
virtual int GeoCal::ConstantSpacingTimeTable::max_line() const
Maximum line table is valid for. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::max_time "
virtual Time GeoCal::ConstantSpacingTimeTable::max_time() const
Maximum time table is valid for. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::min_line "
virtual int GeoCal::ConstantSpacingTimeTable::min_line() const
Minimum line table is valid for. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::min_time "
virtual Time GeoCal::ConstantSpacingTimeTable::min_time() const
Minimum time table is valid for. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::print "
void ConstantSpacingTimeTable::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::print_to_string "
std::string GeoCal::Printable< TimeTable  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::time "
void ConstantSpacingTimeTable::time(const ImageCoordinate &Ic, Time &T, FrameCoordinate &F) const
Convert from ImageCoordinate to Time and FrameCoordinate. ";

%feature("docstring")  GeoCal::ConstantSpacingTimeTable::time_space "
double GeoCal::ConstantSpacingTimeTable::time_space() const
";
%feature("docstring")  GeoCal::ConstantSpacingTimeTable::~ConstantSpacingTimeTable "
virtual GeoCal::ConstantSpacingTimeTable::~ConstantSpacingTimeTable()
";

// File: a00023.xml

%feature("docstring") GeoCal::GdalType::Conv "
";

// File: a00024.xml

%feature("docstring") GeoCal::GdalType::Conv< boost::array< T, D > > "
";
%feature("docstring")  GeoCal::GdalType::Conv< boost::array< T, D > >::conv "
static boost::array<T, D> GeoCal::GdalType::Conv< boost::array< T, D > >::conv(const char *M)
";

// File: a00025.xml

%feature("docstring") GeoCal::GdalType::Conv< double > "
";
%feature("docstring")  GeoCal::GdalType::Conv< double >::conv "
static double GeoCal::GdalType::Conv< double >::conv(const char *M)
";

// File: a00026.xml

%feature("docstring") GeoCal::GdalType::Conv< int > "
";
%feature("docstring")  GeoCal::GdalType::Conv< int >::conv "
static int GeoCal::GdalType::Conv< int >::conv(const char *M)
";

// File: a00027.xml

%feature("docstring") GeoCal::GdalType::Conv< std::string > "
";
%feature("docstring")  GeoCal::GdalType::Conv< std::string >::conv "
static std::string GeoCal::GdalType::Conv< std::string >::conv(const char *M)
";

// File: a00028.xml

%feature("docstring") GeoCal::ConvergenceFailure "
Exception thrown if Math routine fails to converge.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::ConvergenceFailure::ConvergenceFailure "
GeoCal::ConvergenceFailure::ConvergenceFailure(const std::string &W=\"\")
Default constructor.

Can give an optional string describing the error. ";

%feature("docstring")  GeoCal::ConvergenceFailure::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::ConvergenceFailure::~ConvergenceFailure "
virtual GeoCal::ConvergenceFailure::~ConvergenceFailure()
 throw ()
Destructor.
";

// File: a00029.xml

%feature("docstring") GeoCal::CoordinateConverter "
This class is used to convert from and to a particular coordinate
system.

There are some functions that can work equal well with any map
coordinate system, and where we want the decision of what coordinate
system to be made outside of the function (e.g., the program
map_project, where the user selects the coordinate system to map
project to).

The solution we use here is to make use of the Strategy pattern (see
\"Design Patterns\" by Erich Gamma et. al.). This class is the base
class defining the interface to a general coordinate convertor.

This class allows conversion to and from whatever coordinate system
has been selected. The meaning of the X, Y, and Height value passed in
and out of the functions depends on what concrete specialization of
this class is used. For uses where the ambiguity isn't acceptable, it
is not appropriate to use this class (i.e., the various Strategies of
coordinate conversions aren't really exchangeable then).

C++ includes: coordinate_converter.h ";

%feature("docstring")  GeoCal::CoordinateConverter::convert_from_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::CoordinateConverter::convert_from_coordinate(double X, double Y, double Height=0) const =0
This converts from a particular coordinate system, returning a ground
coordinate for the given coordinates.

The specific meaning and units of X, Y, and height depend on which
coordinates system is used by a specialization of this class. ";

%feature("docstring")  GeoCal::CoordinateConverter::convert_to_coordinate "
virtual void GeoCal::CoordinateConverter::convert_to_coordinate(const GroundCoordinate &Gc, double &X, double &Y, double &Height)
const =0
This converts from a ground coordinate to a particular coordinate
system.

The specific meaning and units of X, Y, and height depend on which
coordinates system is used by a specialization of this class. ";

%feature("docstring")  GeoCal::CoordinateConverter::is_same "
virtual bool GeoCal::CoordinateConverter::is_same(const CoordinateConverter &Conv) const =0
Test if two CoordinateConverters are the same coordinate system. ";

%feature("docstring")  GeoCal::CoordinateConverter::print "
virtual void GeoCal::CoordinateConverter::print(std::ostream &Os) const =0
Print to given stream. ";

%feature("docstring")  GeoCal::CoordinateConverter::print_to_string "
std::string GeoCal::Printable< CoordinateConverter  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::CoordinateConverter::~CoordinateConverter "
virtual GeoCal::CoordinateConverter::~CoordinateConverter()
Destructor. ";


// File: a00030.xml

%feature("docstring") GeoCal::Covariance "
This calculates simple statistics on a pair of numbers.

It does this by keeping running sums, so we don't need all the numbers
to be available at once.

C++ includes: covariance.h ";

%feature("docstring")  GeoCal::Covariance::add "
void GeoCal::Covariance::add(double x1, double x2)
Add another pair to the running total. ";

%feature("docstring")  GeoCal::Covariance::count "
int GeoCal::Covariance::count() const
Return number of pairs that have been added. ";

%feature("docstring")  GeoCal::Covariance::Covariance "
GeoCal::Covariance::Covariance()
Constructor. ";

%feature("docstring")  GeoCal::Covariance::covariance "
double GeoCal::Covariance::covariance() const
Return covariance between numbers. Returns 0 if count < 2. ";

%feature("docstring")  GeoCal::Covariance::mean1 "
double GeoCal::Covariance::mean1() const
Return mean of first number. Returns 0 if count < 1. ";

%feature("docstring")  GeoCal::Covariance::mean2 "
double GeoCal::Covariance::mean2() const
Return mean of second number. Returns 0 if count < 1. ";

%feature("docstring")  GeoCal::Covariance::print "
void Covariance::print(std::ostream &os) const
Print to stream. ";

%feature("docstring")  GeoCal::Covariance::print_to_string "
std::string GeoCal::Printable< Covariance  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";


// File: a00031.xml

%feature("docstring") GeoCal::Datum "
This is a Datum, which returns the distance from mean sea level to a
reference ellipsoid.

C++ includes: geocal_datum.h ";

%feature("docstring")  GeoCal::Datum::print "
virtual void GeoCal::Datum::print(std::ostream &Os) const =0
Print to stream. ";

%feature("docstring")  GeoCal::Datum::print_to_string "
std::string GeoCal::Printable< Datum  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Datum::undulation "
virtual double GeoCal::Datum::undulation(const GroundCoordinate &Gc) const =0
Undulation, which is the distance form mean sea level to the reference
ellipsoid, for the given ground location.

This is in meters. ";

%feature("docstring")  GeoCal::Datum::~Datum "
virtual GeoCal::Datum::~Datum()
Destructor. ";


// File: a00032.xml

%feature("docstring") GeoCal::DatumGeoid96 "
This class read the EGM 96 geoid data to provide a Datum.

C++ includes: datum_geoid96.h ";

%feature("docstring")  GeoCal::DatumGeoid96::DatumGeoid96 "
DatumGeoid96::DatumGeoid96(const std::string &Fname=\"\")
Constructor.

You can pass the datum file to read, but if you leave this as blank we
use the file ${AFIDS_VDEV_DATA}/EGM96_20_x100.HLF. ";

%feature("docstring")  GeoCal::DatumGeoid96::file_name "
const std::string& GeoCal::DatumGeoid96::file_name() const
";
%feature("docstring")  GeoCal::DatumGeoid96::print "
void DatumGeoid96::print(std::ostream &Os) const
Print a description of the class. ";

%feature("docstring")  GeoCal::DatumGeoid96::print_to_string "
std::string GeoCal::Printable< Datum  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::DatumGeoid96::undulation "
double DatumGeoid96::undulation(const GroundCoordinate &Gc) const
Return the undulation for a given location. ";

%feature("docstring")  GeoCal::DatumGeoid96::~DatumGeoid96 "
virtual GeoCal::DatumGeoid96::~DatumGeoid96()
";

// File: a00033.xml

%feature("docstring") GeoCal::Dem "
This class gives an interface to a digital elevation model (DEM).

C++ includes: dem.h ";

%feature("docstring")  GeoCal::Dem::Dem "
GeoCal::Dem::Dem()
Default constructor. ";

%feature("docstring")  GeoCal::Dem::distance_to_surface "
virtual double GeoCal::Dem::distance_to_surface(const GroundCoordinate &Gp) const =0
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::Dem::height_reference_surface "
virtual double GeoCal::Dem::height_reference_surface(const GroundCoordinate &Gp) const =0
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::Dem::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::Dem::print "
virtual void GeoCal::Dem::print(std::ostream &Os) const =0
Print to stream. ";

%feature("docstring")  GeoCal::Dem::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Dem::surface_point "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::Dem::surface_point(const GroundCoordinate &Gp) const =0
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::Dem::~Dem "
virtual GeoCal::Dem::~Dem()
Destructor. ";


// File: a00034.xml

%feature("docstring") GeoCal::DemFit "
This is a set of equations for fitting a Dem to a stereo pair of RPC
images.

This is a work in progress, we are likely to change this.

C++ includes: dem_fit.h ";

%feature("docstring")  GeoCal::DemFit::correlation "
Array< double, 2 > DemFit::correlation() const
";
%feature("docstring")  GeoCal::DemFit::DemFit "
DemFit::DemFit(const boost::shared_ptr< RasterImage > &Raw_img1, const Rpc &R1,
const boost::shared_ptr< RasterImage > &Raw_img2, const Rpc &R2, const
boost::shared_ptr< MemoryDem > &D, const MapInfo &M, double Stiff)
Constructor. ";

%feature("docstring")  GeoCal::DemFit::value "
void DemFit::value(std::vector< double > &Res) const
Differences between 2 images. ";

%feature("docstring")  GeoCal::DemFit::write "
void DemFit::write(const std::string &fname1, const std::string &fname2, const
std::string &fname3, const std::string &fname4) const
";

// File: a00035.xml

%feature("docstring") GeoCal::DemMapInfo "
This handles a Dem from a file with a MapInfo.

A very common Dem is one where the height is a geodetic height
relative to a Datum on a regular grid in some map projection (e.g., a
GeoTiff file containing elevations on a latitude/longitude grid). This
class implements surface_gp and distance_to_surface in terms of a
derived class supplied \"elevation\" function.

We do a bilinear interpolation to get heights in between the values
given by elevation.

Depending on what we are doing, calling with a location outside of the
underlying DEM data might be ok, or might be something that should
trigger an error. If requested, this class can just return a
height_reference_surface of 0 everywhere outside of the Dem (so
height_datum is -datum().undulation).

Derived classes need to call the constructor or initialize(), and fill
in the elavation() function.

C++ includes: dem_map_info.h ";

%feature("docstring")  GeoCal::DemMapInfo::datum "
const Datum& GeoCal::DemMapInfo::datum() const
Datum height is relative to. ";

%feature("docstring")  GeoCal::DemMapInfo::datum_ptr "
const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
Pointer to datum. ";

%feature("docstring")  GeoCal::DemMapInfo::distance_to_surface "
double DemMapInfo::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::DemMapInfo::height_datum "
double DemMapInfo::height_datum(const GroundCoordinate &Gp) const
Height relative to datum. ";

%feature("docstring")  GeoCal::DemMapInfo::height_reference_surface "
double DemMapInfo::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::DemMapInfo::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::DemMapInfo::map_info "
const MapInfo& GeoCal::DemMapInfo::map_info() const
MapInfo of underlying data. ";

%feature("docstring")  GeoCal::DemMapInfo::outside_dem_is_error "
bool GeoCal::DemMapInfo::outside_dem_is_error() const
If true, then calling with values outside the range of area of the DEM
triggers an exception, otherwise we just return a height of 0. ";

%feature("docstring")  GeoCal::DemMapInfo::print "
virtual void GeoCal::Dem::print(std::ostream &Os) const =0
Print to stream. ";

%feature("docstring")  GeoCal::DemMapInfo::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::DemMapInfo::surface_point "
boost::shared_ptr< GroundCoordinate > DemMapInfo::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::DemMapInfo::~DemMapInfo "
virtual GeoCal::DemMapInfo::~DemMapInfo()
Destructor. ";


// File: a00036.xml

%feature("docstring") GeoCal::DemMatch "
This class does matching between two images, and then finds the
intersection of these two images.

This defines a Dem surface.

I'm not really sure how this class should be arranged. Currently, this
is used by the python class DemGenerate, and this is just the part
that we want to have in C++. This may evolve over time, and change
exactly what we do here.

C++ includes: dem_match.h ";

%feature("docstring")  GeoCal::DemMatch::DemMatch "
GeoCal::DemMatch::DemMatch(const boost::shared_ptr< ImageToImageMatch > &Match, const
boost::shared_ptr< RayIntersect > &Ray_intersect, double
Max_dist_good_point)
Constructor. ";

%feature("docstring")  GeoCal::DemMatch::match "
boost::shared_ptr<ImageToImageMatch> GeoCal::DemMatch::match() const
";
%feature("docstring")  GeoCal::DemMatch::max_distance "
double GeoCal::DemMatch::max_distance() const
";
%feature("docstring")  GeoCal::DemMatch::print "
virtual void GeoCal::DemMatch::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::DemMatch::print_to_string "
std::string GeoCal::Printable< DemMatch  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::DemMatch::ray_intersect "
boost::shared_ptr<RayIntersect> GeoCal::DemMatch::ray_intersect() const
";
%feature("docstring")  GeoCal::DemMatch::surface_point "
blitz::Array< double, 2 > DemMatch::surface_point(int Lstart, int Sstart, int Lend, int Send, int Lstride=1, int
Sstride=1) const
Look through the given range in line and sample for the first image.

Try image matching, and where it succeeds look at the intersection of
the points. We allow a little slop, but the intersection need to be
close to where each image would put the point (e.g., epipolar
constraint), so we reject any point where the intersection is farther
than the maximum distance in meters that was passed to the
constructor.

We return an array of points that we find. This is a N X 3 array, with
each row corresponding to a point. The columns are latitude,
longitude, and height (in degrees and meters). This is a bit odd way
to return this, but it works well with the python class that uses this
code. ";

%feature("docstring")  GeoCal::DemMatch::~DemMatch "
virtual GeoCal::DemMatch::~DemMatch()
";

// File: a00037.xml

%feature("docstring") GeoCal::DemTiledFile "
This is a DemMapInfo that is implemented as a TiledFile<T, 2>

C++ includes: dem_tiled_file.h ";

%feature("docstring")  GeoCal::DemTiledFile::datum "
const Datum& GeoCal::DemMapInfo::datum() const
Datum height is relative to. ";

%feature("docstring")  GeoCal::DemTiledFile::datum_ptr "
const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
Pointer to datum. ";

%feature("docstring")  GeoCal::DemTiledFile::DemTiledFile "
GeoCal::DemTiledFile::DemTiledFile(const boost::shared_ptr< TiledFileBase< 2 > > &F, const
boost::shared_ptr< Datum > &D, const MapInfo &M, bool
Outside_dem_is_error=false, double Scale=1.0)
Constructor.

Parameters:
-----------

D:   Datum to use when translating to elevation about ellipsoid

M:   MapInfo going with file F

Outside_dem_is_error:  If true, we treat requests for elevation
outside of the MapInfo M as error. Otherwise, we just return 0.

Scale:  Value to multiple underlying data by to get meters (e.g., DEM
might be in US survey foot). ";

%feature("docstring")  GeoCal::DemTiledFile::distance_to_surface "
double DemMapInfo::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::DemTiledFile::elevation "
virtual double GeoCal::DemTiledFile::elevation(int Y_index, int X_index) const
Return height in meters relative to datum().

Note that the call is in line, sample order, which means Y and then X.
";

%feature("docstring")  GeoCal::DemTiledFile::height_datum "
double DemMapInfo::height_datum(const GroundCoordinate &Gp) const
Height relative to datum. ";

%feature("docstring")  GeoCal::DemTiledFile::height_reference_surface "
double DemMapInfo::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::DemTiledFile::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::DemTiledFile::map_info "
const MapInfo& GeoCal::DemMapInfo::map_info() const
MapInfo of underlying data. ";

%feature("docstring")  GeoCal::DemTiledFile::number_line_per_tile "
int GeoCal::DemTiledFile::number_line_per_tile() const
";
%feature("docstring")  GeoCal::DemTiledFile::number_sample_per_tile "
int GeoCal::DemTiledFile::number_sample_per_tile() const
";
%feature("docstring")  GeoCal::DemTiledFile::number_swap "
int GeoCal::DemTiledFile::number_swap() const
";
%feature("docstring")  GeoCal::DemTiledFile::number_tile "
unsigned int GeoCal::DemTiledFile::number_tile() const
";
%feature("docstring")  GeoCal::DemTiledFile::outside_dem_is_error "
bool GeoCal::DemMapInfo::outside_dem_is_error() const
If true, then calling with values outside the range of area of the DEM
triggers an exception, otherwise we just return a height of 0. ";

%feature("docstring")  GeoCal::DemTiledFile::print "
virtual void GeoCal::Dem::print(std::ostream &Os) const =0
Print to stream. ";

%feature("docstring")  GeoCal::DemTiledFile::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::DemTiledFile::reset_number_swap "
void GeoCal::DemTiledFile::reset_number_swap()
";
%feature("docstring")  GeoCal::DemTiledFile::surface_point "
boost::shared_ptr< GroundCoordinate > DemMapInfo::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::DemTiledFile::~DemTiledFile "
virtual GeoCal::DemTiledFile::~DemTiledFile()
Destructor. ";


// File: a00038.xml

%feature("docstring") GeoCal::DemUpdater "
This is used to update a Dem that is used to map project a pair of
images using Rpc.

This class is currently experimental, I'm not exactly sure what the
interface and functionality should look like.

C++ includes: dem_updater.h ";

%feature("docstring")  GeoCal::DemUpdater::compare_image "
double DemUpdater::compare_image(int Line, int Sample, double H) const
Compare two images.

The Line and Sample are relative to the MapInfo passed in. We assign
the given height to this point, and then check how similar the images
are to each other. This returns a value, the smaller the value the
closer the images are to each other.

I'm not exactly sure how we should calculate this metric, but right
now we scale and add a constant to image 2 to have the same mean and
variance as image 1, and then take the sum of the square of the
differences between image 1 and 2. ";

%feature("docstring")  GeoCal::DemUpdater::DemUpdater "
DemUpdater::DemUpdater(const boost::shared_ptr< RasterImage > &Raw_img1, const
boost::shared_ptr< RasterImage > &Raw_img2, const Rpc &R1, const Rpc
&R2, const boost::shared_ptr< Dem > &D, const MapInfo &Mi, int
Template_size=21, double Max_height_correction=50, double
Initial_step=10)
Create a DemUpdater given the 2 raw images, the 2 RPCs for the raw
images, the original Dem, and the MapInfo for where we want the final
Dem.

The Rpc should give map projected images that only differ because of
errors in the Dem, other errors should have been already removed
(e.g., use fit of RpcImage). ";

%feature("docstring")  GeoCal::DemUpdater::find_height "
double DemUpdater::find_height(int Line, int Sample) const
Find the height that gives the minimum compare_image.

Note that this algorithm can probably be improved on. ";

%feature("docstring")  GeoCal::DemUpdater::generate_dem "
blitz::Array< double, 2 > DemUpdater::generate_dem() const
Generate the full Dem. ";

%feature("docstring")  GeoCal::DemUpdater::print "
void DemUpdater::print(std::ostream &Os) const
Print to stream. ";


// File: a00039.xml

%feature("docstring") GeoCal::DemUpdater2 "
This is a second try at DemUpdater.

This class is currently experimental, I'm not exactly sure what the
interface and functionality should look like.

C++ includes: dem_updater2.h ";

%feature("docstring")  GeoCal::DemUpdater2::compare_image "
double DemUpdater2::compare_image(int Line, int Sample, double H) const
Compare two images.

The Line and Sample are relative to the MapInfo of Image 1. We assign
the given height to this point, and then check how similar the images
are to each other. This returns a value, the smaller the value the
closer the images are to each other.

I'm not exactly sure how we should calculate this metric, but right
now we scale and add a constant to image 2 to have the same mean and
variance as image 1, and then take the sum of the square of the
differences between image 1 and 2. ";

%feature("docstring")  GeoCal::DemUpdater2::DemUpdater2 "
DemUpdater2::DemUpdater2(const boost::shared_ptr< MapProjectedImage > &Img_ref_surf1, const
boost::shared_ptr< MapProjectedImage > &Img_ref_surf2, const Rpc &R1,
const Rpc &R2, const boost::shared_ptr< Dem > &D, double
Min_correlation=0.2, int Template_size=15, double
Max_height_correction=50, double Max_diff_neighbor=5.0, double
Step_size=1.0)
Create a DemUpdater given the 2 images projected to a reference
surface at height 0, the 2 RPCs for the raw images, and the original
Dem. ";

%feature("docstring")  GeoCal::DemUpdater2::find_height "
void DemUpdater2::find_height(int Line, int Sample, double Hstart, double &Hres, bool &Success,
double *Max_correlation=0) const
Find the height that gives the minimum compare_image.

Note that this algorithm can probably be improved on. The Line and
Sample are for the data projected to the reference surface. We return
the point in 3-space that is seen in image 1. ";

%feature("docstring")  GeoCal::DemUpdater2::height "
double DemUpdater2::height(int Line, int Sample) const
";
%feature("docstring")  GeoCal::DemUpdater2::height_func "
double DemUpdater2::height_func(int Line, int Sample, double Height) const
";
%feature("docstring")  GeoCal::DemUpdater2::height_reference_surface "
double DemUpdater2::height_reference_surface(double Line, double Sample) const
";
%feature("docstring")  GeoCal::DemUpdater2::height_reference_surface "
double DemUpdater2::height_reference_surface(int Line, int Sample) const
";
%feature("docstring")  GeoCal::DemUpdater2::ic_ref_surf2 "
ImageCoordinate DemUpdater2::ic_ref_surf2(int Line, int Sample, double H) const
Figure out conjugate point in projected reference surface 2 for a
point in reference surface 1 if it is at a given height. ";

%feature("docstring")  GeoCal::DemUpdater2::print "
void DemUpdater2::print(std::ostream &Os) const
Print to stream. ";


// File: a01178.xml

%feature("docstring") std::deque "
STL class. ";


// File: a00040.xml

%feature("docstring") GeoCal::DidDatum "
This is a Datum used for the the DID.

C++ includes: did_datum.h ";

%feature("docstring")  GeoCal::DidDatum::DidDatum "
DidDatum::DidDatum(const std::string &Fname)
This creates a DidDatum.

This takes the name of the mean sea level data, i.e.,
/data/bank/anc/DID/MeanSeaLevel/current/geoid_2160h_4320w.half ";

%feature("docstring")  GeoCal::DidDatum::file_name "
const std::string& GeoCal::DidDatum::file_name() const
";
%feature("docstring")  GeoCal::DidDatum::print "
virtual void GeoCal::DidDatum::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::DidDatum::print_to_string "
std::string GeoCal::Printable< Datum  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::DidDatum::undulation "
double DidDatum::undulation(const GroundCoordinate &Gc) const
Undulation, which is the distance form mean sea level to the reference
ellipsoid, for the given ground location.

This is in meters. ";

%feature("docstring")  GeoCal::DidDatum::~DidDatum "
virtual GeoCal::DidDatum::~DidDatum()
Destructor. ";


// File: a00041.xml

%feature("docstring") GeoCal::DirChange "
Utility class.

This changes to a new directory, and on destruction changes back.

C++ includes: dir_change.h ";

%feature("docstring")  GeoCal::DirChange::DirChange "
GeoCal::DirChange::DirChange(const std::string &newdir)
";
%feature("docstring")  GeoCal::DirChange::~DirChange "
GeoCal::DirChange::~DirChange()
";

// File: a00042.xml

%feature("docstring") GeoCal::DisplayStretch "
This stretches data for displaying as 8 bit data.

C++ includes: display_stretch.h ";

%feature("docstring")  GeoCal::DisplayStretch::print "
virtual void GeoCal::DisplayStretch::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::DisplayStretch::stretch "
virtual void GeoCal::DisplayStretch::stretch(const blitz::Array< int, 2 > &Raw_data, blitz::Array< unsigned char,
2 > &Stretched_data) const =0
Take the given data and stretch it to 8 bit display data.

We take the array to write this to, because often we already have a
sink for the data (e.g., creating a gdk_pixbuf). ";

%feature("docstring")  GeoCal::DisplayStretch::~DisplayStretch "
virtual GeoCal::DisplayStretch::~DisplayStretch()
";

// File: a00043.xml

%feature("docstring") GeoCal::DisplayStretchStdDev "
This stretches data linearly between a two values.

The values are given by min_dev and max_dev, the default is to use 2
sigma.

Note that the advantage of this is simplicity. For most images, it
doesn't give a particularly good stretch for viewing the data. If you
are looking for a good default, then do DisplayStretchStdDev.

C++ includes: display_stretch_std_dev.h ";

%feature("docstring")  GeoCal::DisplayStretchStdDev::DisplayStretchStdDev "
GeoCal::DisplayStretchStdDev::DisplayStretchStdDev(double Min_sig=-2.0, double Max_sig=2.0)
Constructor.

We stretch between mean + sigma * Min_sig to mean + sigma * Max_sig.
";

%feature("docstring")  GeoCal::DisplayStretchStdDev::print "
virtual void GeoCal::DisplayStretchStdDev::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::DisplayStretchStdDev::stretch "
void DisplayStretchStdDev::stretch(const blitz::Array< int, 2 > &Raw_data, blitz::Array< unsigned char,
2 > &Stretched_data) const
Take the given data and stretch it to 8 bit display data.

We take the array to write this to, because often we already have a
sink for the data (e.g., creating a gdk_pixbuf). ";

%feature("docstring")  GeoCal::DisplayStretchStdDev::~DisplayStretchStdDev "
virtual GeoCal::DisplayStretchStdDev::~DisplayStretchStdDev()
";

// File: a01181.xml

%feature("docstring") std::domain_error "
STL class. ";


// File: a00044.xml

%feature("docstring") GeoCal::DoughnutAverage "
This class does a \"doughnut average\" of an underlying RasterImage.

This produces the average value of all the pixels in a given window
size, excluding a smaller doughnut. We exclude all points that a value
of 0 in the average (e.g., the normal gore used in VICAR images), as
well as all points that are beyond the edge of the image. We also
preserve gore, so if a point has a value of 0 in any of the bands then
we set the doughnut average to 0 for that point.

Depending on the application, you may or may not want to include
points that have gore within the doughnut window. You can specify this
in the constructor. If \"Allow_gore\" is true, then any point other
than the center can be gore and we just exclude it from the average.
If \"Allow_gore\" is false, we set a value to 0 if any value in the
window is 0. In all cases, we set a value to 0 if the center is 0.

This is one of the building block used to do the \"cvdnorm\", you can
consult that program for details. But basically this is used to
examine small features (smaller than the doughnut whole) that are
significantly different than the nearby background.

C++ includes: doughnut_average.h ";

%feature("docstring")  GeoCal::DoughnutAverage::add_raster_image "
void GeoCal::RasterImageMultiBandVariable::add_raster_image(const boost::shared_ptr< RasterImage > &Ri)
";
%feature("docstring")  GeoCal::DoughnutAverage::allow_gore "
bool GeoCal::DoughnutAverage::allow_gore() const
If true, we allow gore in a window and just exclude it in the average.

Otherwise we set a point to 0 if any point in the window is 0. ";

%feature("docstring")  GeoCal::DoughnutAverage::cvdnorm "
Array< double, 2 > DoughnutAverage::cvdnorm(int band, int Lstart, int Sstart, int Number_line, int Number_sample)
const
Very closely related in the doughnut average is the cvdnorm for one of
the bands.

So we provide the calculation for that. ";

%feature("docstring")  GeoCal::DoughnutAverage::cvdnorm_raster_image "
boost::shared_ptr< RasterImage > DoughnutAverage::cvdnorm_raster_image(int band) const
Present the cvdnorm as a RasterImage. ";

%feature("docstring")  GeoCal::DoughnutAverage::doughnut_size "
int GeoCal::DoughnutAverage::doughnut_size() const
The doughnut size. ";

%feature("docstring")  GeoCal::DoughnutAverage::DoughnutAverage "
DoughnutAverage::DoughnutAverage(const boost::shared_ptr< RasterImageMultiBand > &Rimg, int
Window_size=11, int Doughnut_size=5, bool Allow_gore=true, int
Number_tile_line=-1, int Number_tile_sample=-1, int Number_tile=4)
Constructor.

Parameters:
-----------

Rimg:  The underlying image we are producing the average for.

Window_size:  The window size we are averaging. Should be odd number.

Doughnut_size:  The doughnut we are excluding in the average. Should
be odd, and < Window_size. ";

%feature("docstring")  GeoCal::DoughnutAverage::number_band "
virtual int GeoCal::RasterImageMultiBandVariable::number_band() const
Return number of bands. ";

%feature("docstring")  GeoCal::DoughnutAverage::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::DoughnutAverage::pandif "
blitz::Array< double, 2 > DoughnutAverage::pandif(int Lstart, int Sstart, int Number_line, int Number_sample) const
This is the difference of the first two cvdnorm bands. ";

%feature("docstring")  GeoCal::DoughnutAverage::pandif_raster_image "
boost::shared_ptr< RasterImage > DoughnutAverage::pandif_raster_image() const
Present the pandif as a RasterImage. ";

%feature("docstring")  GeoCal::DoughnutAverage::print "
virtual void GeoCal::RasterImageMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::DoughnutAverage::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::DoughnutAverage::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band) const
Return given band. ";

%feature("docstring")  GeoCal::DoughnutAverage::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band)
";
%feature("docstring")  GeoCal::DoughnutAverage::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBandVariable::raster_image_ptr(int band) const
";
%feature("docstring")  GeoCal::DoughnutAverage::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::DoughnutAverage::read_double "
Array< double, 3 > CalcRasterMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::DoughnutAverage::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::DoughnutAverage::window_size "
int GeoCal::DoughnutAverage::window_size() const
The window size. ";

%feature("docstring")  GeoCal::DoughnutAverage::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";

%feature("docstring")  GeoCal::DoughnutAverage::~DoughnutAverage "
virtual GeoCal::DoughnutAverage::~DoughnutAverage()
";

// File: a00045.xml

%feature("docstring") DummyToolkitCoordinateInterface "
Each toolkit supplies methods to convert to and from
CartesianInertial. ";

%feature("docstring")  DummyToolkitCoordinateInterface::to_fixed "
virtual void DummyToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, const CartesianInertial &From,
CartesianFixed &To)
This converts from CartesianInertial to CartesianFixed for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). We use this because it is a unique coding, the underlying
toolkit doesn't need to be SPICE. ";

%feature("docstring")  DummyToolkitCoordinateInterface::to_fixed "
virtual void DummyToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, double Ci_to_cf[3][3])
Return a matrix for converting from CartesianInertial to
CartesianFixed. ";

%feature("docstring")  DummyToolkitCoordinateInterface::to_inertial "
virtual void DummyToolkitCoordinateInterface::to_inertial(int Body_id, const Time &T, const CartesianFixed &From,
CartesianInertial &To)
This converts from CartesianFixed to CartesianInertial for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). We use this because it is a unique coding, the underlying
toolkit doesn't need to be SPICE. ";

%feature("docstring")  DummyToolkitCoordinateInterface::to_inertial "
virtual void DummyToolkitCoordinateInterface::to_inertial(int Body_id, const Time &T, double Cf_to_ci[3][3])
";
%feature("docstring")  DummyToolkitCoordinateInterface::~DummyToolkitCoordinateInterface "
virtual DummyToolkitCoordinateInterface::~DummyToolkitCoordinateInterface()
";

// File: a00046.xml

%feature("docstring") GeoCal::Eci "
This is a ECI coordinate (J2000)

C++ includes: eci.h ";

%feature("docstring")  GeoCal::Eci::ci_to_cf "
virtual void GeoCal::Eci::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const
Matrix to convert Eci to Ecr.

The transpose of this will convert Ecr to Eci. ";

%feature("docstring")  GeoCal::Eci::ci_to_cf_quat "
boost::math::quaternion<double> GeoCal::CartesianInertial::ci_to_cf_quat(const Time &T) const
Calculate quaternion that can be used to convert CartesianFixed to
CartesianInertial at the given Time. ";

%feature("docstring")  GeoCal::Eci::convert_to_cf "
boost::shared_ptr< CartesianFixed > Eci::convert_to_cf(const Time &T) const
Convert to CartesianFixed. ";

%feature("docstring")  GeoCal::Eci::create "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Eci::create(boost::array< double, 3 > P) const
Create an instance of whatever type of CartesianInertial this is. ";

%feature("docstring")  GeoCal::Eci::Eci "
GeoCal::Eci::Eci()
Default constructor, doesn't initialize position. ";

%feature("docstring")  GeoCal::Eci::Eci "
GeoCal::Eci::Eci(double X, double Y, double Z)
Make an Eci with the given position, in meters. ";

%feature("docstring")  GeoCal::Eci::Eci "
GeoCal::Eci::Eci(const boost::array< double, 3 > &Pos)
Create an Eci with the given position in meters. ";

%feature("docstring")  GeoCal::Eci::print "
void Eci::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::Eci::print_to_string "
std::string GeoCal::Printable< CartesianInertial  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Eci::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianInertial > Eci::reference_surface_intersect_approximate(const CartesianInertialLookVector &Cl, double
Height_reference_surface=0) const
Find the intersection with the surface at the given height starting at
this point and looking in the given direction.

The intersection is done in an approximate but quicker manner.

We find the intersection with the ellipsoid that has the given height
added to the equatorial and polar axis. This is not the same as the
figure with a height h above the ellipsoid (which isn't an ellipsoid),
but for many purposes this is close enough. ";

%feature("docstring")  GeoCal::Eci::~Eci "
virtual GeoCal::Eci::~Eci()
Destructor. ";


// File: a00047.xml

%feature("docstring") GeoCal::EciTod "
This is a ECI true of date coordinate.

This is a dynamic frame that uses the true equator and equinox of a
particular data.

C++ includes: eci_tod.h ";

%feature("docstring")  GeoCal::EciTod::ci_to_cf "
void EciTod::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const
Matrix to convert EciTod to Ecr.

The transpose of this will convert Ecr to EciTod. ";

%feature("docstring")  GeoCal::EciTod::ci_to_cf_quat "
boost::math::quaternion<double> GeoCal::CartesianInertial::ci_to_cf_quat(const Time &T) const
Calculate quaternion that can be used to convert CartesianFixed to
CartesianInertial at the given Time. ";

%feature("docstring")  GeoCal::EciTod::convert_to_cf "
boost::shared_ptr< CartesianFixed > EciTod::convert_to_cf(const Time &T) const
Convert to CartesianFixed. ";

%feature("docstring")  GeoCal::EciTod::create "
virtual boost::shared_ptr<CartesianInertial> GeoCal::EciTod::create(boost::array< double, 3 > P) const
Create an instance of whatever type of CartesianInertial this is. ";

%feature("docstring")  GeoCal::EciTod::EciTod "
GeoCal::EciTod::EciTod()
Default constructor, doesn't initialize position. ";

%feature("docstring")  GeoCal::EciTod::EciTod "
EciTod::EciTod(const Eci &Eci_coor, const Time &T)
";
%feature("docstring")  GeoCal::EciTod::EciTod "
GeoCal::EciTod::EciTod(double X, double Y, double Z)
Make an EciTod with the given position, in meters. ";

%feature("docstring")  GeoCal::EciTod::EciTod "
GeoCal::EciTod::EciTod(const boost::array< double, 3 > &Pos)
Create an Eci with the given position in meters. ";

%feature("docstring")  GeoCal::EciTod::print "
void EciTod::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::EciTod::print_to_string "
std::string GeoCal::Printable< CartesianInertial  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::EciTod::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianInertial > EciTod::reference_surface_intersect_approximate(const CartesianInertialLookVector &Cl, double
Height_reference_surface=0) const
Find the intersection with the surface at the given height starting at
this point and looking in the given direction.

The intersection is done in an approximate but quicker manner.

We find the intersection with the ellipsoid that has the given height
added to the equatorial and polar axis. This is not the same as the
figure with a height h above the ellipsoid (which isn't an ellipsoid),
but for many purposes this is close enough. ";

%feature("docstring")  GeoCal::EciTod::to_eci "
Eci EciTod::to_eci(const Time &T)
";
%feature("docstring")  GeoCal::EciTod::~EciTod "
virtual GeoCal::EciTod::~EciTod()
Destructor. ";


// File: a00048.xml

%feature("docstring") GeoCal::EciTodBurl "
This is a ECI true of date coordinate.

This is a dynamic frame that uses the true equator and equinox of a
particular data.

This implementation uses Mike Burl's code. I believe EciTod is more
accurate since it accounts for polar drift, but I'm not positive. We
want to be able to compare between the two, so I'll put this in place.

C++ includes: eci_tod_burl.h ";

%feature("docstring")  GeoCal::EciTodBurl::ci_to_cf "
void EciTodBurl::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const
Matrix to convert EciTodBurl to Ecr.

The transpose of this will convert Ecr to EciTodBurl. ";

%feature("docstring")  GeoCal::EciTodBurl::ci_to_cf_quat "
boost::math::quaternion<double> GeoCal::CartesianInertial::ci_to_cf_quat(const Time &T) const
Calculate quaternion that can be used to convert CartesianFixed to
CartesianInertial at the given Time. ";

%feature("docstring")  GeoCal::EciTodBurl::convert_to_cf "
boost::shared_ptr< CartesianFixed > EciTodBurl::convert_to_cf(const Time &T) const
Convert to CartesianFixed. ";

%feature("docstring")  GeoCal::EciTodBurl::create "
virtual boost::shared_ptr<CartesianInertial> GeoCal::EciTodBurl::create(boost::array< double, 3 > P) const
Create an instance of whatever type of CartesianInertial this is. ";

%feature("docstring")  GeoCal::EciTodBurl::EciTodBurl "
GeoCal::EciTodBurl::EciTodBurl()
Default constructor, doesn't initialize position. ";

%feature("docstring")  GeoCal::EciTodBurl::EciTodBurl "
GeoCal::EciTodBurl::EciTodBurl(double X, double Y, double Z)
Make an EciTod with the given position, in meters. ";

%feature("docstring")  GeoCal::EciTodBurl::EciTodBurl "
GeoCal::EciTodBurl::EciTodBurl(const boost::array< double, 3 > &Pos)
Create an Eci with the given position in meters. ";

%feature("docstring")  GeoCal::EciTodBurl::print "
void EciTodBurl::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::EciTodBurl::print_to_string "
std::string GeoCal::Printable< CartesianInertial  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::EciTodBurl::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianInertial > EciTodBurl::reference_surface_intersect_approximate(const CartesianInertialLookVector &Cl, double
Height_reference_surface=0) const
Find the intersection with the surface at the given height starting at
this point and looking in the given direction.

The intersection is done in an approximate but quicker manner.

We find the intersection with the ellipsoid that has the given height
added to the equatorial and polar axis. This is not the same as the
figure with a height h above the ellipsoid (which isn't an ellipsoid),
but for many purposes this is close enough. ";

%feature("docstring")  GeoCal::EciTodBurl::~EciTodBurl "
virtual GeoCal::EciTodBurl::~EciTodBurl()
Destructor. ";


// File: a00049.xml

%feature("docstring") GeoCal::Ecr "
This is a ground coordinate, represented in ECR coordinates.

C++ includes: ecr.h ";

%feature("docstring")  GeoCal::Ecr::ci_to_cf "
virtual void GeoCal::Ecr::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const
Matrix to convert Eci to Ecr.

The transpose of this will convert Ecr to Eci. ";

%feature("docstring")  GeoCal::Ecr::ci_to_cf_quat "
boost::math::quaternion<double> GeoCal::CartesianFixed::ci_to_cf_quat(const Time &T) const
Calculate quaternion that can be used to convert CartesianFixed to
CartesianInertial at the given Time. ";

%feature("docstring")  GeoCal::Ecr::convert_to_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianFixed::convert_to_cf() const
This calls create. ";

%feature("docstring")  GeoCal::Ecr::convert_to_ci "
boost::shared_ptr< CartesianInertial > Ecr::convert_to_ci(const Time &T) const
Convert to CartesianInertial. ";

%feature("docstring")  GeoCal::Ecr::convert_to_geodetic "
Geodetic Ecr::convert_to_geodetic() const
Convert to Geodetic coordinates. ";

%feature("docstring")  GeoCal::Ecr::create "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Ecr::create(boost::array< double, 3 > P) const
Create an instance of Ecr. ";

%feature("docstring")  GeoCal::Ecr::Ecr "
Ecr::Ecr(const GroundCoordinate &Gc)
Convert from GroundCoor. ";

%feature("docstring")  GeoCal::Ecr::Ecr "
GeoCal::Ecr::Ecr(double X, double Y, double Z)
Make an Ecr with the given position, in meters. ";

%feature("docstring")  GeoCal::Ecr::Ecr "
GeoCal::Ecr::Ecr(const boost::array< double, 3 > &Pos)
Create an Ecr with the given position in meters. ";

%feature("docstring")  GeoCal::Ecr::Ecr "
GeoCal::Ecr::Ecr()
Default constructor. ";

%feature("docstring")  GeoCal::Ecr::height_reference_surface "
double Ecr::height_reference_surface() const
Return height above/below the reference surface (e.g., WGS-84 for the
earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::Ecr::latitude "
double Ecr::latitude() const
Return latitude in degrees.

By default we just convert to CartesianFixed and then to latitude, but
derived classes can supply more efficient versions of these if needed.
Latitude is -90 to 90. ";

%feature("docstring")  GeoCal::Ecr::longitude "
double Ecr::longitude() const
Return longitude in degrees.

By default we just convert to CartesianFixed and then to longitude,
but derived classes can supply more efficient versions of these if
needed. Longitude is -180 to 180. ";

%feature("docstring")  GeoCal::Ecr::min_radius_reference_surface "
double Ecr::min_radius_reference_surface() const
This is the radius of the largest sphere that fits completely into the
reference surface (e.g., radius for a reference sphere, minimum of a
and b for a ellipsoid). ";

%feature("docstring")  GeoCal::Ecr::print "
void Ecr::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::Ecr::print_to_string "
std::string GeoCal::Printable< GroundCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Ecr::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianFixed > Ecr::reference_surface_intersect_approximate(const CartesianFixedLookVector &Cl, double
Height_reference_surface=0) const
Find the intersection with the surface at the given height starting at
this point and looking in the given direction.

The intersection is done in an approximate but quicker manner.

We find the intersection with the ellipsoid that has the given height
added to the equatorial and polar axis. This is not the same as the
figure with a height h above the ellipsoid (which isn't an ellipsoid),
but for many purposes this is close enough. ";

%feature("docstring")  GeoCal::Ecr::~Ecr "
virtual GeoCal::Ecr::~Ecr()
Destructor. ";


// File: a00050.xml

%feature("docstring") ErdasBands "
This is a simple class that has a number of bands all in memory.

When close() or the destructor is called, this is then written out to
an Erdas file. The includes calculating statistics and image pyramid.
";

%feature("docstring")  ErdasBands::close "
void ErdasBands::close()
Close the file.

This copies the data from memory and writes it out to the Erdas file.
At the same time, we generate statistics and the image pyramids. ";

%feature("docstring")  ErdasBands::ErdasBands "
ErdasBands::ErdasBands(const std::string &Fname, int Number_line, int Number_sample, int
Number_band, int Dt, int Fill_value=0)
Constructor. ";

%feature("docstring")  ErdasBands::image "
const GdalRasterImage& ErdasBands::image(int band) const
Return the GdalRasterImage for the given band. ";

%feature("docstring")  ErdasBands::image "
GdalRasterImage& ErdasBands::image(int band)
Return the GdalRasterImage for the given band. ";

%feature("docstring")  ErdasBands::number_band "
int ErdasBands::number_band() const
Number of bands of data. ";

%feature("docstring")  ErdasBands::set_map_info "
void ErdasBands::set_map_info(const MapInfo &Mi)
Set the mapinfo. ";

%feature("docstring")  ErdasBands::~ErdasBands "
virtual ErdasBands::~ErdasBands()
";

// File: a00051.xml

%feature("docstring") GeoCal::Exception "
This is the base of the exception hierarchy for GeoCal code.

This can be written to like a stream to contain information about the
exception. This is derived from the standard library std::exception

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::Exception::Exception "
GeoCal::Exception::Exception(const std::string &W=\"\")
Default constructor.

Can give an optional string describing the error. ";

%feature("docstring")  GeoCal::Exception::Exception "
GeoCal::Exception::Exception(const Exception &E)
Copy constructor. ";

%feature("docstring")  GeoCal::Exception::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::Exception::~Exception "
virtual GeoCal::Exception::~Exception()
 throw ()
Destructor.
";

// File: a00052.xml

%feature("docstring") GeoCal::GeoCalCore::Exception "
";
%feature("docstring")  GeoCal::GeoCalCore::Exception::Exception "
GeoCal::GeoCalCore::Exception::Exception()
 throw ()";
%feature("docstring")  GeoCal::GeoCalCore::Exception::Exception "
GeoCal::GeoCalCore::Exception::Exception(const Exception &E)
 throw ()";
%feature("docstring")  GeoCal::GeoCalCore::Exception::what "
virtual const char* GeoCal::GeoCalCore::Exception::what() const
 throw ()";
%feature("docstring")  GeoCal::GeoCalCore::Exception::~Exception "
virtual GeoCal::GeoCalCore::Exception::~Exception()
 throw ()";

// File: a00627.xml

%feature("docstring") std::exception "
STL class. ";


// File: a00053.xml

%feature("docstring") F "
";
%feature("docstring")  F::F "
F::F(const DemUpdater2 &D, int Line, int Sample)
";
%feature("docstring")  F::~F "
virtual F::~F()
";

// File: a01183.xml

%feature("docstring") std::ios_base::failure "
STL class. ";


// File: a00054.xml

%feature("docstring") GeoCal::FeatureDetector "
This class detects interest points in an image.

An interest point is one that is good for image matching by some sort
of criteria. For example, a common feature detector is one that tries
to detect corners, with the idea that these are good choices for doing
image matching on.

C++ includes: feature_detector.h ";

%feature("docstring")  GeoCal::FeatureDetector::FeatureDetector "
GeoCal::FeatureDetector::FeatureDetector()
Constructor. ";

%feature("docstring")  GeoCal::FeatureDetector::interest_point_detect "
virtual std::vector<InterestPoint> GeoCal::FeatureDetector::interest_point_detect(const RasterImage &Img) const =0
Go through a RasterImage, and return a list of InterstPoints for it.

The list isn't in any particular order, but note that InterstPoints
have a natural order by the weight, and you can sort the points if
desired using std::sort, or just pick at the \"best\" point by
std::max_element. ";

%feature("docstring")  GeoCal::FeatureDetector::interest_point_grid "
std::vector< ImageCoordinate > FeatureDetector::interest_point_grid(const RasterImage &Img, const Mask &M, int Number_grid_line, int
Number_grid_sample, int Border=0) const
This is used to generate a set of interest points that can then be
used to do image matching over a whole raster image.

We divide up the raster image into Number_grid_line x
Number_grid_sample boxes. In each box, we select one point that has
the largest weight as an interest point. Since these points are
typically used for image matching, which can't be done too close to
the edge of the image, an optional border to exclude can be supplied.
None of the points returned will be within the border of the edge of
the image.

We exclude points that are marked by the Mask. ";

%feature("docstring")  GeoCal::FeatureDetector::interest_point_grid "
std::vector< ImageCoordinate > FeatureDetector::interest_point_grid(const RasterImage &Img, int Number_grid_line, int Number_grid_sample,
int Border=0) const
Variation of interest_point_grid that does not use as Mask. ";

%feature("docstring")  GeoCal::FeatureDetector::interest_point_grid_ptr "
boost::shared_ptr< ImageCoordinate > FeatureDetector::interest_point_grid_ptr(const RasterImage &Img, const Mask &M, int i, int j, int
Number_grid_line, int Number_grid_sample, int Border=0) const
Python much prefers getting shared_ptr back, so we have this variation
that wraps that.

We also return only a single point. This is a somewhat awkward
interface, but it gets wrapped into a interest_point_grid function in
python, possibly doing processing in parallel. ";

%feature("docstring")  GeoCal::FeatureDetector::interest_point_grid_ptr "
boost::shared_ptr< ImageCoordinate > FeatureDetector::interest_point_grid_ptr(const RasterImage &Img, int i, int j, int Number_grid_line, int
Number_grid_sample, int Border=0) const
Python much prefers getting shared_ptr back, so we have this variation
that wraps that.

We also return only a single point. This is a somewhat awkward
interface, but it gets wrapped into a interest_point_grid function in
python, possibly doing processing in parallel. ";

%feature("docstring")  GeoCal::FeatureDetector::print "
virtual void GeoCal::FeatureDetector::print(std::ostream &Os) const =0
Print to stream. ";

%feature("docstring")  GeoCal::FeatureDetector::print_to_string "
std::string GeoCal::Printable< FeatureDetector  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::FeatureDetector::~FeatureDetector "
virtual GeoCal::FeatureDetector::~FeatureDetector()
Destructor. ";


// File: a00055.xml

%feature("docstring") GeoCal::Fftw2dBackward "
This is a light wrapper around the FFTW library.

This maintains the lifetime of plans. This is for a 2D real transform,
going backward from a complex value to real results.

Note that the FFTW library calculates an unnormalized transform. This
means that a forward calculation followed by a backward calculation
will return a results Nrow * Ncol * original data.

C++ includes: geocal_fftw.h ";

%feature("docstring")  GeoCal::Fftw2dBackward::Fftw2dBackward "
Fftw2dBackward::Fftw2dBackward(int Nrow, int Ncol)
Constructor. ";

%feature("docstring")  GeoCal::Fftw2dBackward::print "
virtual void GeoCal::Fftw2dBackward::print(std::ostream &Os)
Print to stream. ";

%feature("docstring")  GeoCal::Fftw2dBackward::print_to_string "
std::string GeoCal::Printable< Fftw2dBackward  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Fftw2dBackward::run_fft "
void GeoCal::Fftw2dBackward::run_fft()
Run the FFT on the data in data_in, with the results going to
data_out.

Note that data_in may possibly be destroyed. ";

%feature("docstring")  GeoCal::Fftw2dBackward::~Fftw2dBackward "
Fftw2dBackward::~Fftw2dBackward()
Destructor. ";


// File: a00056.xml

%feature("docstring") GeoCal::Fftw2dForward "
This is a light wrapper around the FFTW library.

This maintains the lifetime of plans. This is for a 2D real transform,
going forward to a complex results.

C++ includes: geocal_fftw.h ";

%feature("docstring")  GeoCal::Fftw2dForward::Fftw2dForward "
Fftw2dForward::Fftw2dForward(int Nrow, int Ncol)
Constructor. ";

%feature("docstring")  GeoCal::Fftw2dForward::print "
virtual void GeoCal::Fftw2dForward::print(std::ostream &Os)
Print to stream. ";

%feature("docstring")  GeoCal::Fftw2dForward::print_to_string "
std::string GeoCal::Printable< Fftw2dForward  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Fftw2dForward::run_fft "
void GeoCal::Fftw2dForward::run_fft()
Run the FFT on the data in data_in, with the results going to
data_out.

Note that data_in may possibly be destroyed. ";

%feature("docstring")  GeoCal::Fftw2dForward::~Fftw2dForward "
Fftw2dForward::~Fftw2dForward()
Destructor. ";


// File: a00057.xml

%feature("docstring") GeoCal::ForstnerFeatureDetector "
This class detects interest points in an image using Forstner feature
detection.

C++ includes: forstner_feature_detector.h ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::basic_ip_large_number "
int GeoCal::ForstnerFeatureDetector::basic_ip_large_number() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::basic_robert_threshold "
double GeoCal::ForstnerFeatureDetector::basic_robert_threshold() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::forstner_max_window_large_size "
int GeoCal::ForstnerFeatureDetector::forstner_max_window_large_size() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::forstner_max_window_small_size "
int GeoCal::ForstnerFeatureDetector::forstner_max_window_small_size() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::forstner_window_size "
int GeoCal::ForstnerFeatureDetector::forstner_window_size() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::ForstnerFeatureDetector "
BZ_END_STENCIL ForstnerFeatureDetector::ForstnerFeatureDetector(int Basic_ip_large_number=1000, int Maximum_weight_threshold=10000,
int Forstner_max_window_large_size=9, int
Forstner_max_window_small_size=7, int Forstner_window_size=3, double
Roundness_threshold=0.2, double Weight_threshold=1.0, double
Basic_robert_threshold=1.2)
Constructor. ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::interest_point_detect "
std::vector< InterestPoint > ForstnerFeatureDetector::interest_point_detect(const RasterImage &Img) const
Go through a RasterImage, and return a list of InterstPoints for it.

Put all of the raster data into a blitz::array.

Compute the Robert gradient ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::interest_point_grid "
std::vector< ImageCoordinate > FeatureDetector::interest_point_grid(const RasterImage &Img, const Mask &M, int Number_grid_line, int
Number_grid_sample, int Border=0) const
This is used to generate a set of interest points that can then be
used to do image matching over a whole raster image.

We divide up the raster image into Number_grid_line x
Number_grid_sample boxes. In each box, we select one point that has
the largest weight as an interest point. Since these points are
typically used for image matching, which can't be done too close to
the edge of the image, an optional border to exclude can be supplied.
None of the points returned will be within the border of the edge of
the image.

We exclude points that are marked by the Mask. ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::interest_point_grid "
std::vector< ImageCoordinate > FeatureDetector::interest_point_grid(const RasterImage &Img, int Number_grid_line, int Number_grid_sample,
int Border=0) const
Variation of interest_point_grid that does not use as Mask. ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::interest_point_grid_ptr "
boost::shared_ptr< ImageCoordinate > FeatureDetector::interest_point_grid_ptr(const RasterImage &Img, const Mask &M, int i, int j, int
Number_grid_line, int Number_grid_sample, int Border=0) const
Python much prefers getting shared_ptr back, so we have this variation
that wraps that.

We also return only a single point. This is a somewhat awkward
interface, but it gets wrapped into a interest_point_grid function in
python, possibly doing processing in parallel. ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::interest_point_grid_ptr "
boost::shared_ptr< ImageCoordinate > FeatureDetector::interest_point_grid_ptr(const RasterImage &Img, int i, int j, int Number_grid_line, int
Number_grid_sample, int Border=0) const
Python much prefers getting shared_ptr back, so we have this variation
that wraps that.

We also return only a single point. This is a somewhat awkward
interface, but it gets wrapped into a interest_point_grid function in
python, possibly doing processing in parallel. ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::maximum_weight_threshold "
int GeoCal::ForstnerFeatureDetector::maximum_weight_threshold() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::print "
void ForstnerFeatureDetector::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::print_to_string "
std::string GeoCal::Printable< FeatureDetector  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ForstnerFeatureDetector::roundness_threshold "
double GeoCal::ForstnerFeatureDetector::roundness_threshold() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::weight_threshold "
double GeoCal::ForstnerFeatureDetector::weight_threshold() const
";
%feature("docstring")  GeoCal::ForstnerFeatureDetector::~ForstnerFeatureDetector "
virtual GeoCal::ForstnerFeatureDetector::~ForstnerFeatureDetector()
Destructor. ";


// File: a00058.xml

%feature("docstring") GeoCal::FrameCoordinate "
This is the coordinates of a frame camera.

C++ includes: frame_coordinate.h ";

%feature("docstring")  GeoCal::FrameCoordinate::FrameCoordinate "
GeoCal::FrameCoordinate::FrameCoordinate()
Default constructor. ";

%feature("docstring")  GeoCal::FrameCoordinate::FrameCoordinate "
GeoCal::FrameCoordinate::FrameCoordinate(double L, double S)
Create a FrameCoordinate with the given coordinates. ";

%feature("docstring")  GeoCal::FrameCoordinate::print "
void FrameCoordinate::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::FrameCoordinate::print_to_string "
std::string GeoCal::Printable< FrameCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";


// File: a01125.xml

%feature("docstring") std::fstream "
STL class. ";


// File: a00059.xml

%feature("docstring") GeoCal::Functor "
This is a general Functor (Function object).

This maps a type T to type S. We have typedefs for common mappings.

C++ includes: functor.h ";

%feature("docstring")  GeoCal::Functor::~Functor "
virtual GeoCal::Functor< T, S >::~Functor()
Destructor. ";


// File: a00060.xml

%feature("docstring") GeoCal::Gdal "
Read and write files supported by GDAL library.

The GDAL library is a library that supports a number of raster
formats. It is the library used by GRASS, although it is separate and
you don't need to use GRASS or even have it installed to use GDAL.
There is a large number of formats supported, including ERDAS imagine,
Arc/Info, TIFF and GeoTIFF, NITF, HDF, netCDF, and many others.
Documentation can be found athttp://www.gdal.org.

This class provides a generic TiledFile interface. Other classes can
read additional metadata to support additional features (e.g., create
a Dem or RasterImage).

Note that the GDAL library reads each band separate. So if you have
multiple bands, you'll need to create multiple Gdals.

Note when using MapInfo, we have X (e.g., longitude) going in the
sample direction, and Y (e.g., latitude) going in the line direction.
This means that when reading data which is in line, sample order you
need to swap these - e.g., we have (Y_index, X_index).

C++ includes: geocal_gdal.h ";

%feature("docstring")  GeoCal::Gdal::can_set_map_info "
virtual bool GeoCal::Gdal< T >::can_set_map_info() const
We can set MapInfo for this kind of file. ";

%feature("docstring")  GeoCal::Gdal::close "
void GeoCal::GdalBase::close()
";
%feature("docstring")  GeoCal::Gdal::data_set "
const boost::shared_ptr<GDALDataset>& GeoCal::GdalBase::data_set() const
The underlying dataset object. ";

%feature("docstring")  GeoCal::Gdal::file_names "
std::vector< std::string > GdalBase::file_names() const
File names.

This can contain zero or more files (zero for instance for a virtual
dataset). ";

%feature("docstring")  GeoCal::Gdal::flush "
virtual void GeoCal::Gdal< T >::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::Gdal::Gdal "
GeoCal::Gdal< T >::Gdal(const boost::shared_ptr< GDALDataset > &Data_set, int Band_id=1, int
Number_tile=4, int Tile_number_line=-1, int Tile_number_sample=-1)
Constructor to read and/or write a new file (or an existing file where
you want to share the GDALDataset with some other object).

This constructor doesn't try to actually create the file, that is done
outside of this class. Normally, you just call GDALDriver::Create for
whatever driver you are using (see the GDAL documentation at the web
site listed above for details) ";

%feature("docstring")  GeoCal::Gdal::Gdal "
GeoCal::Gdal< T >::Gdal(const boost::shared_ptr< GDALDataset > &Data_set, GDALRasterBand *Rb,
int Number_tile=4, int Tile_number_line=-1, int Tile_number_sample=-1)
";
%feature("docstring")  GeoCal::Gdal::Gdal "
GeoCal::Gdal< T >::Gdal(const std::string &Fname, const std::string &Driver_name, int
Number_line, int Number_sample, int Number_band, const std::string
&Options, int Number_tile=4, int Tile_number_line=-1, int
Tile_number_sample=-1)
Constructor to create a new file.

This is a thin wrapper around the GDAL library that gets the requested
driver and creates the file with the given options.

Because the option interface of GDAL is a bit awkward to use, we
supply an interface that takes a single string, with the options
separated by \" \", so for example for a TIFF file we might have
\"TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024\" ";

%feature("docstring")  GeoCal::Gdal::Gdal "
GeoCal::Gdal< T >::Gdal(const std::string &Fname, int Band_id=1, int Number_tile=4, bool
Update=false, int Tile_number_line=-1, int Tile_number_sample=-1)
Constructor to read and/or update an existing file. ";

%feature("docstring")  GeoCal::Gdal::get_double "
virtual double GeoCal::TiledFile< T, D >::get_double(const boost::array< index, D > &Index) const
Get data an convert to double. ";

%feature("docstring")  GeoCal::Gdal::get_int "
virtual int GeoCal::TiledFile< T, D >::get_int(const boost::array< index, D > &Index) const
Get data an convert to integer. ";

%feature("docstring")  GeoCal::Gdal::has_map_info "
virtual bool GeoCal::Gdal< T >::has_map_info() const
True if we have map info. ";

%feature("docstring")  GeoCal::Gdal::has_metadata "
bool GeoCal::GdalBase::has_metadata(const std::string &M, const std::string &Domain=\"\") const
This is a thin wrapper around GDALGetMetadataItem().

We determine if requested metadata can be read. ";

%feature("docstring")  GeoCal::Gdal::has_rpc "
virtual bool GeoCal::Gdal< T >::has_rpc() const
True if we have RPC. ";

%feature("docstring")  GeoCal::Gdal::is_closed "
bool GeoCal::GdalBase::is_closed() const
";
%feature("docstring")  GeoCal::Gdal::linear_unit_name "
std::string GdalBase::linear_unit_name() const
Return linear units name. ";

%feature("docstring")  GeoCal::Gdal::linear_unit_scale "
double GdalBase::linear_unit_scale() const
Return linear units scale, which is the factor needed to multiple
linear distance by to get meters. ";

%feature("docstring")  GeoCal::Gdal::map_info "
virtual MapInfo GeoCal::Gdal< T >::map_info() const
Return MapInfo. ";

%feature("docstring")  GeoCal::Gdal::map_info "
virtual void GeoCal::Gdal< T >::map_info(const MapInfo &M)
Set MapInfo. ";

%feature("docstring")  GeoCal::Gdal::metadata "
S GeoCal::GdalBase::metadata(const std::string &M, const std::string &Domain=\"\") const
This is a thin wrapper around GDALGetMetadataItem().

We read the requested metadata, and cast to the desired type. A
MetadataMissing is thrown if we don't find the requested metadata. ";

%feature("docstring")  GeoCal::Gdal::number_swap "
unsigned int GeoCal::TiledFileBase< D >::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::Gdal::number_tile "
int GeoCal::TiledFile< T, D >::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::Gdal::print "
void GdalBase::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::Gdal::print_to_string "
std::string GeoCal::Printable< GdalBase  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Gdal::raster_band "
GDALRasterBand& GeoCal::GdalBase::raster_band()
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::Gdal::raster_band "
const GDALRasterBand& GeoCal::GdalBase::raster_band() const
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::Gdal::raster_band_ptr "
GDALRasterBand* GeoCal::GdalBase::raster_band_ptr()
";
%feature("docstring")  GeoCal::Gdal::read "
boost::multi_array<T, D> GeoCal::TiledFile< T, D >::read(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index) const
Read a subset of the data. ";

%feature("docstring")  GeoCal::Gdal::read_double "
virtual void GeoCal::TiledFile< T, D >::read_double(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, double *Res) const
Read a subset of the data, and convert to double. ";

%feature("docstring")  GeoCal::Gdal::read_int "
virtual void GeoCal::TiledFile< T, D >::read_int(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, int *Res) const
Read a subset of the data, and convert to int. ";

%feature("docstring")  GeoCal::Gdal::read_ptr "
virtual void GeoCal::TiledFile< T, D >::read_ptr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, T *Res) const
Read a subset of the data.

Check first to see if entire requested data is in a single tile. If
not, the we read from disk (rather than trying to stitch together
multiple tiles). ";

%feature("docstring")  GeoCal::Gdal::reset_number_swap "
void GeoCal::TiledFileBase< D >::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::Gdal::rpc "
Rpc GeoCal::GdalBase::rpc() const
Read metadata to determine Rpc. ";

%feature("docstring")  GeoCal::Gdal::rpc "
void GeoCal::GdalBase::rpc(const Rpc &R)
";
%feature("docstring")  GeoCal::Gdal::set_metadata "
void GeoCal::GdalBase::set_metadata(const std::string &M, const std::string &Val, const std::string
&Domain=\"\")
This is a thin wrapper around GDALSetMetadataItem().

We write the requested metadata. ";

%feature("docstring")  GeoCal::Gdal::size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::size() const
File size. ";

%feature("docstring")  GeoCal::Gdal::tile_size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::tile_size() const
Tile size. ";

%feature("docstring")  GeoCal::Gdal::write "
void GeoCal::TiledFile< T, D >::write(const boost::array< index, D > &Min_index, const boost::multi_array<
T, D > &V) const
Write a subset of the data. ";

%feature("docstring")  GeoCal::Gdal::write_double "
virtual void GeoCal::TiledFile< T, D >::write_double(const boost::array< index, D > &Index, double Val)
Convert a double to type T, and write to file. ";

%feature("docstring")  GeoCal::Gdal::write_double_arr "
virtual void GeoCal::TiledFile< T, D >::write_double_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const double *V)
Convert a double to type T, and write to file. ";

%feature("docstring")  GeoCal::Gdal::write_int "
virtual void GeoCal::TiledFile< T, D >::write_int(const boost::array< index, D > &Index, int Val)
Convert an int to type T, and write to file. ";

%feature("docstring")  GeoCal::Gdal::write_int_arr "
virtual void GeoCal::TiledFile< T, D >::write_int_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const int *V)
Convert an int to type T, and write to file. ";

%feature("docstring")  GeoCal::Gdal::write_ptr "
virtual void GeoCal::TiledFile< T, D >::write_ptr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const T *V) const
Write a subset of the data. ";

%feature("docstring")  GeoCal::Gdal::~Gdal "
virtual GeoCal::Gdal< T >::~Gdal()
Destructor. ";


// File: a00061.xml

%feature("docstring") GeoCal::GdalBase "
Base class of Gdal<T>.

This contains the portion that doesn't depend on T.

C++ includes: geocal_gdal.h ";

%feature("docstring")  GeoCal::GdalBase::close "
void GeoCal::GdalBase::close()
";
%feature("docstring")  GeoCal::GdalBase::data_set "
const boost::shared_ptr<GDALDataset>& GeoCal::GdalBase::data_set() const
The underlying dataset object. ";

%feature("docstring")  GeoCal::GdalBase::file_names "
std::vector< std::string > GdalBase::file_names() const
File names.

This can contain zero or more files (zero for instance for a virtual
dataset). ";

%feature("docstring")  GeoCal::GdalBase::flush "
void GeoCal::GdalBase::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::GdalBase::has_map_info "
bool GdalBase::has_map_info() const
Returns true if the file contains projection information, and false
otherwise. ";

%feature("docstring")  GeoCal::GdalBase::has_metadata "
bool GeoCal::GdalBase::has_metadata(const std::string &M, const std::string &Domain=\"\") const
This is a thin wrapper around GDALGetMetadataItem().

We determine if requested metadata can be read. ";

%feature("docstring")  GeoCal::GdalBase::has_rpc "
virtual bool GeoCal::GdalBase::has_rpc() const
";
%feature("docstring")  GeoCal::GdalBase::is_closed "
bool GeoCal::GdalBase::is_closed() const
";
%feature("docstring")  GeoCal::GdalBase::linear_unit_name "
std::string GdalBase::linear_unit_name() const
Return linear units name. ";

%feature("docstring")  GeoCal::GdalBase::linear_unit_scale "
double GdalBase::linear_unit_scale() const
Return linear units scale, which is the factor needed to multiple
linear distance by to get meters. ";

%feature("docstring")  GeoCal::GdalBase::map_info "
MapInfo GdalBase::map_info() const
Read the MapInfo for the file.

An exception is thrown if there isn't map projection information in
the file or if it is an unsupported map projection. ";

%feature("docstring")  GeoCal::GdalBase::map_info "
virtual void GeoCal::GdalBase::map_info(const MapInfo &M)
";
%feature("docstring")  GeoCal::GdalBase::metadata "
S GeoCal::GdalBase::metadata(const std::string &M, const std::string &Domain=\"\") const
This is a thin wrapper around GDALGetMetadataItem().

We read the requested metadata, and cast to the desired type. A
MetadataMissing is thrown if we don't find the requested metadata. ";

%feature("docstring")  GeoCal::GdalBase::print "
void GdalBase::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::GdalBase::print_to_string "
std::string GeoCal::Printable< GdalBase  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::GdalBase::raster_band "
GDALRasterBand& GeoCal::GdalBase::raster_band()
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::GdalBase::raster_band "
const GDALRasterBand& GeoCal::GdalBase::raster_band() const
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::GdalBase::raster_band_ptr "
GDALRasterBand* GeoCal::GdalBase::raster_band_ptr()
";
%feature("docstring")  GeoCal::GdalBase::rpc "
Rpc GeoCal::GdalBase::rpc() const
Read metadata to determine Rpc. ";

%feature("docstring")  GeoCal::GdalBase::rpc "
void GeoCal::GdalBase::rpc(const Rpc &R)
";
%feature("docstring")  GeoCal::GdalBase::set_metadata "
void GeoCal::GdalBase::set_metadata(const std::string &M, const std::string &Val, const std::string
&Domain=\"\")
This is a thin wrapper around GDALSetMetadataItem().

We write the requested metadata. ";

%feature("docstring")  GeoCal::GdalBase::~GdalBase "
virtual GeoCal::GdalBase::~GdalBase()
Destructor. ";


// File: a00062.xml

%feature("docstring") GeoCal::GdalDatum "
This uses a GdalRasterImage to provide a datum.

This file should have the offset from the datum to reference ellipsoid
(e.g., WGS-84).

C++ includes: gdal_datum.h ";

%feature("docstring")  GeoCal::GdalDatum::GdalDatum "
GeoCal::GdalDatum::GdalDatum(const std::string &Fname, int Band_id=1, int Number_tile=4, int
Tile_number_line=-1, int Tile_number_sample=-1)
Constructor. ";

%feature("docstring")  GeoCal::GdalDatum::GdalDatum "
GeoCal::GdalDatum::GdalDatum(const boost::shared_ptr< GdalRasterImage > &Map_file)
";
%feature("docstring")  GeoCal::GdalDatum::map_file "
boost::shared_ptr<GdalRasterImage> GeoCal::GdalDatum::map_file() const
";
%feature("docstring")  GeoCal::GdalDatum::print "
virtual void GeoCal::GdalDatum::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::GdalDatum::print_to_string "
std::string GeoCal::Printable< Datum  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::GdalDatum::undulation "
virtual double GeoCal::GdalDatum::undulation(const GroundCoordinate &Gc) const
Undulation, which is the distance form mean sea level to the reference
ellipsoid, for the given ground location.

This is in meters. ";

%feature("docstring")  GeoCal::GdalDatum::~GdalDatum "
virtual GeoCal::GdalDatum::~GdalDatum()
Destructor. ";


// File: a00063.xml

%feature("docstring") GeoCal::GdalDem "
This uses a Gdal<T> as a DEM.

The type of T should be one we can convert to a double, and should
give the height in meters above sea level. We do a bilinear
interpolation to get values between points, and we optionally return a
height of 0 outside of the range of where we have data (or
alternatively, throw an exception).

Right now, the datum needs to be passed in. I don't believe there is
any information available in the files to let us know which datum to
use.

C++ includes: gdal_dem.h ";

%feature("docstring")  GeoCal::GdalDem::band_id "
int GeoCal::GdalDem::band_id() const
";
%feature("docstring")  GeoCal::GdalDem::datum "
const Datum& GeoCal::DemMapInfo::datum() const
Datum height is relative to. ";

%feature("docstring")  GeoCal::GdalDem::datum_ptr "
const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
Pointer to datum. ";

%feature("docstring")  GeoCal::GdalDem::distance_to_surface "
double DemMapInfo::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::GdalDem::elevation "
virtual double GeoCal::DemTiledFile::elevation(int Y_index, int X_index) const
Return height in meters relative to datum().

Note that the call is in line, sample order, which means Y and then X.
";

%feature("docstring")  GeoCal::GdalDem::file_name "
const std::string& GeoCal::GdalDem::file_name() const
";
%feature("docstring")  GeoCal::GdalDem::GdalDem "
GdalDem::GdalDem(const std::string &Fname, const boost::shared_ptr< Datum >
&D=boost::shared_ptr< Datum >(new NoDatum()), int Band_id=1, bool
Outside_dem_is_error=false, int Number_tile=4, int
Tile_number_line=-1, int Tile_number_sample=-1)
Constructor that reads an existing file to get the Dem data.

The default Datum used if none is supplied is DidDatum, which is
relative to WGS-84. If Outside_dem_is_error is false, we return a
height of 0 outside of the range of the data, otherwise an exception
is thrown when an attempt is made to retrieve an height outside of the
range.

You can optionally pass the desired tile size. If you don't pass this
in, we use the blocking size of the underlying dataset. ";

%feature("docstring")  GeoCal::GdalDem::height_datum "
double DemMapInfo::height_datum(const GroundCoordinate &Gp) const
Height relative to datum. ";

%feature("docstring")  GeoCal::GdalDem::height_reference_surface "
double DemMapInfo::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::GdalDem::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::GdalDem::map_info "
const MapInfo& GeoCal::DemMapInfo::map_info() const
MapInfo of underlying data. ";

%feature("docstring")  GeoCal::GdalDem::number_line_per_tile "
int GeoCal::DemTiledFile::number_line_per_tile() const
";
%feature("docstring")  GeoCal::GdalDem::number_sample_per_tile "
int GeoCal::DemTiledFile::number_sample_per_tile() const
";
%feature("docstring")  GeoCal::GdalDem::number_swap "
int GeoCal::DemTiledFile::number_swap() const
";
%feature("docstring")  GeoCal::GdalDem::number_tile "
unsigned int GeoCal::DemTiledFile::number_tile() const
";
%feature("docstring")  GeoCal::GdalDem::outside_dem_is_error "
bool GeoCal::DemMapInfo::outside_dem_is_error() const
If true, then calling with values outside the range of area of the DEM
triggers an exception, otherwise we just return a height of 0. ";

%feature("docstring")  GeoCal::GdalDem::print "
virtual void GeoCal::GdalDem::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::GdalDem::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::GdalDem::reset_number_swap "
void GeoCal::DemTiledFile::reset_number_swap()
";
%feature("docstring")  GeoCal::GdalDem::surface_point "
boost::shared_ptr< GroundCoordinate > DemMapInfo::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::GdalDem::~GdalDem "
virtual GeoCal::GdalDem::~GdalDem()
Destructor. ";


// File: a00064.xml

%feature("docstring") GeoCal::GdalMultiBand "
This is a set of Gdal RasterImages, one for each band.

C++ includes: gdal_multi_band.h ";

%feature("docstring")  GeoCal::GdalMultiBand::add_raster_image "
void GeoCal::RasterImageMultiBandVariable::add_raster_image(const boost::shared_ptr< RasterImage > &Ri)
";
%feature("docstring")  GeoCal::GdalMultiBand::close "
void GdalMultiBand::close() const
Flush data to disk for all bands and close file.

You don't normally need to call this directly, since the destructor of
the files automatically does this. But this can be useful in some
cases. ";

%feature("docstring")  GeoCal::GdalMultiBand::data_set "
const boost::shared_ptr<GDALDataset>& GeoCal::GdalMultiBand::data_set() const
The underlying dataset object. ";

%feature("docstring")  GeoCal::GdalMultiBand::flush "
void GdalMultiBand::flush() const
Flush data to disk for all bands. ";

%feature("docstring")  GeoCal::GdalMultiBand::gdal_raster_image "
boost::shared_ptr<GdalRasterImage> GeoCal::GdalMultiBand::gdal_raster_image(int band) const
RasterImage, cast to GdalRasterImage. ";

%feature("docstring")  GeoCal::GdalMultiBand::GdalMultiBand "
GdalMultiBand::GdalMultiBand(const std::string &Fname, int Number_tile=4, int Tile_number_line=-1,
int Tile_number_sample=-1)
Constructor. ";

%feature("docstring")  GeoCal::GdalMultiBand::GdalMultiBand "
GdalMultiBand::GdalMultiBand(const std::string &Fname, const std::string &Driver_name, const
GdalRasterImage &Source, const std::string &Options=\"\", bool
Log_progress=false, int Number_tile=4, int Tile_number_line=-1, int
Tile_number_sample=-1)
Create a new GdalMultiBand from and an existing GDAL dataset using the
CreateCopy command. ";

%feature("docstring")  GeoCal::GdalMultiBand::GdalMultiBand "
GdalMultiBand::GdalMultiBand(const std::string &Fname, const std::string &Driver_name, const
GdalMultiBand &Source, const std::string &Options=\"\", bool
Log_progress=false, int Number_tile=4, int Tile_number_line=-1, int
Tile_number_sample=-1)
Create a new GdalMultiBand from and an existing GDAL dataset using the
CreateCopy command. ";

%feature("docstring")  GeoCal::GdalMultiBand::GdalMultiBand "
GdalMultiBand::GdalMultiBand(const std::string &Fname, const std::string &Driver_name, int
Number_line, int Number_sample, int Number_band, int Dt, const
std::string &Options=\"\", int Number_tile=4, int Tile_number_line=-1,
int Tile_number_sample=-1)
Create a new file.

All the arguments here are the same as for GdalRasterImage. ";

%feature("docstring")  GeoCal::GdalMultiBand::number_band "
virtual int GeoCal::RasterImageMultiBandVariable::number_band() const
Return number of bands. ";

%feature("docstring")  GeoCal::GdalMultiBand::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::GdalMultiBand::print "
virtual void GeoCal::RasterImageMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::GdalMultiBand::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::GdalMultiBand::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band) const
Return given band. ";

%feature("docstring")  GeoCal::GdalMultiBand::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band)
";
%feature("docstring")  GeoCal::GdalMultiBand::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBandVariable::raster_image_ptr(int band) const
";
%feature("docstring")  GeoCal::GdalMultiBand::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::GdalMultiBand::read_double "
Array< double, 3 > RasterImageMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::GdalMultiBand::set_map_info "
void GeoCal::GdalMultiBand::set_map_info(const MapInfo &M)
";
%feature("docstring")  GeoCal::GdalMultiBand::set_rpc "
void GeoCal::GdalMultiBand::set_rpc(const Rpc &R)
";
%feature("docstring")  GeoCal::GdalMultiBand::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::GdalMultiBand::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";

%feature("docstring")  GeoCal::GdalMultiBand::~GdalMultiBand "
virtual GeoCal::GdalMultiBand::~GdalMultiBand()
";
%feature("docstring")  GeoCal::GdalMultiBand::save "
void GdalMultiBand::save(const std::string &Oname, const std::string &Driver, const
RasterImageMultiBand &d, bool Log_progress=false, int Dt=GDT_Byte,
const std::string &Options=\"\")
Copy the RasterImageMultiBand into a new file.

We copy map and RPC information (if found) from the first band in
RasterImageMultiBand. ";


// File: a00065.xml

%feature("docstring") GeoCal::GdalMultiFile "
This gives read access to a large dataset that is represented as a
number of tiles.

The tiles all need to have the same map projection, and all need to
have an origin a integral number of pixels from a common origin.

This class gives an interface to this set of VICAR files that looks
like one really big MapProjectedImage.

Note that the files might not cover the full extent of the area. If
you try to read a pixel without a file covering it, then depending on
the application this is either an error, or you might just want a fill
value returned. This can be controlled by passing in a true or false
value for No_coverage_is_error.

C++ includes: gdal_multi_file.h ";

%feature("docstring")  GeoCal::GdalMultiFile::GdalMultiFile "
GdalMultiFile::GdalMultiFile(const std::vector< std::string > &File_list, int band=1, bool
No_coverage_is_error=true, int Size_multiple_of=1, int
Number_line_per_tile=-1, int Number_sample_per_tile=-1, int
Number_tile_each_file=4, int Number_file=4, int
No_coverage_fill_value=0)
Read the given list of files to create a multifile.

The files all need to have the same map projection, same resolution,
and be an integral number of pixels from a common origin. For each
file, we read the given band.

If you would like areas without file coverage to simple return a 0
rather than an error, set No_coverage_is_error to false.

For some applications (e.g., image pyramids for google earth) it is
convenient to restrict the size of the full image to multiple of a
fixed tile size, e.g. 256. If this is desired, then you can pass this
in as Size_multiple_of.

There are two kinds of tiling going on. At the top level, we have a
number of files open at one time, given by Number_file. For each file,
we read that it tiles with the given Number_line_per_tile x
Number_sample_per_tile Number_tile_each_file tiles. ";

%feature("docstring")  GeoCal::GdalMultiFile::get_file "
MapProjectedMultifileTile GdalMultiFile::get_file(int Line, int Sample) const
Get a file that covers the given location. ";

%feature("docstring")  GeoCal::GdalMultiFile::print "
void GdalMultiFile::print(std::ostream &Os) const
Write to a stream. ";

%feature("docstring")  GeoCal::GdalMultiFile::~GdalMultiFile "
virtual GeoCal::GdalMultiFile::~GdalMultiFile()
";
%feature("docstring")  GeoCal::GdalMultiFile::tile_for_google_earth "
void GdalMultiFile::tile_for_google_earth(const std::vector< std::string > &File_list, const std::string
&Fbase, std::vector< std::string > &Out_list, std::vector< MapInfo >
Out_mapinfo)
Create a GdalMultiFile for the given file list, and write out into
fixed size tiles PNG (256 x 256) for google earth.

We create the file name as Fbase + line_tile + \"_\" + sample_tile +
\".png\", e.g. foo/0_1.png. We treat 0 as transparent fill value.

Note that this only generates the PNG files, we don't produce the KML
files or image pyramid. This is intended as a low level function to be
used with some higher level tool that handles the other pieces (e.g.,
Ruby, which is better than C++ for generating XML files)

We return the list of files created and the MapInfo for each file. ";


// File: a00066.xml

%feature("docstring") GeoCal::GdalNotAvailableException "
Exception thrown if GDAL isn't available.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::GdalNotAvailableException::GdalNotAvailableException "
GeoCal::GdalNotAvailableException::GdalNotAvailableException()
Constructor. ";

%feature("docstring")  GeoCal::GdalNotAvailableException::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::GdalNotAvailableException::~GdalNotAvailableException "
virtual GeoCal::GdalNotAvailableException::~GdalNotAvailableException()
 throw ()
Destructor.
";

// File: a00067.xml

%feature("docstring") GeoCal::GdalRasterImage "
This wraps a Gdal<T> to make a RasterImage.

The type of T should be one we can convert to and from a int, since
this is what a RasterImage works in.

C++ includes: gdal_raster_image.h ";

%feature("docstring")  GeoCal::GdalRasterImage::band_id "
int GeoCal::GdalRasterImage::band_id() const
";
%feature("docstring")  GeoCal::GdalRasterImage::close "
void GeoCal::GdalRasterImage::close()
";
%feature("docstring")  GeoCal::GdalRasterImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::GdalRasterImage::data_set "
const boost::shared_ptr<GDALDataset>& GeoCal::GdalRasterImage::data_set() const
The underlying dataset object. ";

%feature("docstring")  GeoCal::GdalRasterImage::file_names "
std::vector<std::string> GeoCal::GdalRasterImage::file_names() const
";
%feature("docstring")  GeoCal::GdalRasterImage::flush "
virtual void GeoCal::GdalRasterImage::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::GdalRasterImage::gdal_data_base "
const GdalBase& GeoCal::GdalRasterImage::gdal_data_base() const
Underlying data. ";

%feature("docstring")  GeoCal::GdalRasterImage::gdal_data_base "
GdalBase& GeoCal::GdalRasterImage::gdal_data_base()
Underlying data. ";

%feature("docstring")  GeoCal::GdalRasterImage::gdal_data_base_ptr "
boost::shared_ptr<GdalBase> GeoCal::GdalRasterImage::gdal_data_base_ptr() const
";
%feature("docstring")  GeoCal::GdalRasterImage::GdalRasterImage "
GdalRasterImage::GdalRasterImage(const boost::shared_ptr< GDALDataset > &Data_set, int Band_id=1, int
Number_tile=4, int Tile_number_line=-1, int Tile_number_sample=-1)
Constructor to read and/or write a new file (or an existing file where
you want to share the GDALDataset with some other object).

This constructor doesn't try to actually create the file, that is done
outside of this class. Normally, you just call GDALDriver::Create for
whatever driver you are using (see the GDAL documentation at the web
site listed above for details).

You can optionally pass the desired tile size. If you don't pass this
in, we use the blocking size of the underlying dataset. ";

%feature("docstring")  GeoCal::GdalRasterImage::GdalRasterImage "
GdalRasterImage::GdalRasterImage(const std::string &Fname, int Band_id=1, int Number_tile=4, bool
Update=false, int Tile_number_line=-1, int Tile_number_sample=-1)
Constructor to read and/or update an existing file. ";

%feature("docstring")  GeoCal::GdalRasterImage::GdalRasterImage "
GdalRasterImage::GdalRasterImage(const std::string &Fname, const std::string &Driver_name, int
Number_line, int Number_sample, int Number_band, int Dt, const
std::string &Options=\"\", int Number_tile=4, int Tile_number_line=-1,
int Tile_number_sample=-1)
Constructor to create a new file.

This is a thin wrapper around the GDAL library that gets the requested
driver and creates the file with the given options.

We create a file with the given number of bands, but this always opens
band 1. However you can use the GDALDataset created in other
GdalRasterImage to write the other bands.

Because the option interface of GDAL is a bit awkward to use, we
supply an interface that takes a single string, with the options
separated by \" \", so for example for a TIFF file we might have
\"TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024\" ";

%feature("docstring")  GeoCal::GdalRasterImage::GdalRasterImage "
GdalRasterImage::GdalRasterImage(const std::string &Fname, const std::string &Driver_name, const
MapInfo &Mi, int Number_band, int Dt, const std::string &Options=\"\",
int Number_tile=4, int Tile_number_line=-1, int Tile_number_sample=-1)
Constructor to create a new file.

This is a thin wrapper around the GDAL library that gets the requested
driver and creates the file with the given options.

We create a file with the given number of bands, but this always opens
band 1. However you can use the GDALDataset created in other
GdalRasterImage to write the other bands.

Because the option interface of GDAL is a bit awkward to use, we
supply an interface that takes a single string, with the options
separated by \" \", so for example for a TIFF file we might have
\"TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024\" ";

%feature("docstring")  GeoCal::GdalRasterImage::GdalRasterImage "
GdalRasterImage::GdalRasterImage(const std::string &Fname, const std::string &Driver_name, const
GdalRasterImage &Source, const std::string &Options=\"\", bool
Log_progress=false, int Band_id=1, int Number_tile=4, int
Tile_number_line=-1, int Tile_number_sample=-1)
Copy an existing GdalRasterImage using CreateCopy. ";

%feature("docstring")  GeoCal::GdalRasterImage::GdalRasterImage "
GeoCal::GdalRasterImage::GdalRasterImage(const boost::shared_ptr< Gdal< T > > &Data)
Constructor. ";

%feature("docstring")  GeoCal::GdalRasterImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::GdalRasterImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::GdalRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::GdalRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::GdalRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::GdalRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::GdalRasterImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::GdalRasterImage::has_metadata "
bool GeoCal::GdalRasterImage::has_metadata(const std::string &Metadata_item, const std::string &Domain=\"\")
const
Indicates if a metadata item is found in the file. ";

%feature("docstring")  GeoCal::GdalRasterImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::GdalRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::GdalRasterImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::GdalRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::GdalRasterImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::GdalRasterImage::is_closed "
bool GeoCal::GdalRasterImage::is_closed() const
";
%feature("docstring")  GeoCal::GdalRasterImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::GdalRasterImage::map_info_from_nitf_corner "
boost::shared_ptr< MapInfo > GdalRasterImage::map_info_from_nitf_corner() const
In AFIDS, we calculate what is called the \"nitf corners\".

This gives approximate corner coordinates for an image. Depending on
the application, it can be useful to treat this as an approximate
MapInfo.

This return a null pointer if we don't have the NITF corner metadata.
";

%feature("docstring")  GeoCal::GdalRasterImage::metadata "
S GeoCal::GdalRasterImage::metadata(const std::string &Metadata_item, const std::string &Domain=\"\")
const
Read a metadata item and cast it to the given type. ";

%feature("docstring")  GeoCal::GdalRasterImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::GdalRasterImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::GdalRasterImage::number_swap "
unsigned int GeoCal::RasterImageTiledFile::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::GdalRasterImage::number_tile "
int GeoCal::RasterImageTiledFile::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::GdalRasterImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::GdalRasterImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::GdalRasterImage::overview "
boost::shared_ptr< RasterImage > GdalRasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::GdalRasterImage::print "
virtual void GeoCal::GdalRasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::GdalRasterImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::GdalRasterImage::raster_band "
GDALRasterBand& GeoCal::GdalRasterImage::raster_band()
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::GdalRasterImage::raster_band "
const GDALRasterBand& GeoCal::GdalRasterImage::raster_band() const
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::GdalRasterImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::GdalRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::GdalRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::GdalRasterImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::GdalRasterImage::read_double "
virtual blitz::Array<double, 2> GeoCal::RasterImageTiledFile::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::GdalRasterImage::read_ptr "
virtual void GeoCal::RasterImageTiledFile::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::GdalRasterImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::GdalRasterImage::reset_number_swap "
void GeoCal::RasterImageTiledFile::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::GdalRasterImage::rpc "
Rpc GeoCal::GdalRasterImage::rpc() const
Return Rpc for image. ";

%feature("docstring")  GeoCal::GdalRasterImage::set_map_info "
void GdalRasterImage::set_map_info(const MapInfo &Mi)
Set the map info. This overwrites whatever was previously set. ";

%feature("docstring")  GeoCal::GdalRasterImage::set_metadata "
void GeoCal::GdalRasterImage::set_metadata(const std::string &M, const std::string &Val, const std::string
&Domain=\"\")
Write the requested metadata. ";

%feature("docstring")  GeoCal::GdalRasterImage::set_rpc "
void GdalRasterImage::set_rpc(const Rpc &R)
Set the RPC info. This overwrites whatever was previously set. ";

%feature("docstring")  GeoCal::GdalRasterImage::tile_file_base "
const TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base() const
Underlying data. ";

%feature("docstring")  GeoCal::GdalRasterImage::tile_file_base "
TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base()
Underlying data. ";

%feature("docstring")  GeoCal::GdalRasterImage::tile_file_base_ptr "
const boost::shared_ptr<TiledFileBase<2> >& GeoCal::RasterImageTiledFile::tile_file_base_ptr() const
Underlying data. ";

%feature("docstring")  GeoCal::GdalRasterImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::GdalRasterImage::unchecked_read "
virtual int GeoCal::RasterImageTiledFile::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::GdalRasterImage::unchecked_write "
virtual void GeoCal::RasterImageTiledFile::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::GdalRasterImage::update "
bool GeoCal::GdalRasterImage::update() const
";
%feature("docstring")  GeoCal::GdalRasterImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::GdalRasterImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::GdalRasterImage::write_ptr "
virtual void GeoCal::RasterImageTiledFile::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the image. ";

%feature("docstring")  GeoCal::GdalRasterImage::~GdalRasterImage "
virtual GeoCal::GdalRasterImage::~GdalRasterImage()
Destructor. ";

%feature("docstring")  GeoCal::GdalRasterImage::read_all "
std::vector< boost::shared_ptr< RasterImage > > GdalRasterImage::read_all(const std::string &Fname)
For some formats, reading all of the data for all the bands at one
time is much quicker than reading each band separately (assuming you
actually want all the data).

For example, for jpeg reading three separate bands requires
decompressing the data three times while reading all at once only
decompresses once. Since almost the entire time reading jpeg is used
in decompressing the data this means it takes about three times as
long to read all the bands as to read them all at once.

This function reads all of the data into memory and returns
RasterImages that can be used to read the data. ";

%feature("docstring")  GeoCal::GdalRasterImage::save "
static void GeoCal::GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
RasterImage &Mi, int Dt=GDT_Byte, const std::string &Option=\"\", bool
Require_copy=false, int Fill_value=-1000)
Copy the given RasterImage into a new file, using the given driver and
options.

Some drivers only allow \"CreateCopy\" rather than \"Create\",
requiring us to use an intermediate memory version of Gdal. In that
case, set \"require_copy\" to true. ";

%feature("docstring")  GeoCal::GdalRasterImage::save "
static void GeoCal::GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
RasterImage &Mi_r, const RasterImage &Mi_g, const RasterImage &Mi_b,
int Dt=GDT_Byte, const std::string &Option=\"\", bool
Require_copy=false, int Fill_value=-1000)
Copy the given RasterImages into a new file, using the given driver
and options.

Some drivers only allow \"CreateCopy\" rather than \"Create\",
requiring us to use an intermediate memory version of Gdal. In that
case, set \"require_copy\" to true. ";

%feature("docstring")  GeoCal::GdalRasterImage::save "
void GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
std::vector< const RasterImage * > &d, int Dt=GDT_Byte, const
std::string &Option=\"\", bool Require_copy=false, int
Fill_value=-1000)
Copy the given RasterImages into a new file, using the given driver
and options.

Some drivers only allow \"CreateCopy\" rather than \"Create\",
requiring us to use an intermediate memory version of Gdal. In that
case, set \"require_copy\" to true. ";

%feature("docstring")  GeoCal::GdalRasterImage::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImage &Mi, int Dt=GDT_Byte, int
Fill_value=0)
Copy the given RasterImage into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";

%feature("docstring")  GeoCal::GdalRasterImage::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImage &Mi_r, const RasterImage
&Mi_g, const RasterImage &Mi_b, int Dt=GDT_Byte, int Fill_value=0)
Copy the given RasterImages into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";

%feature("docstring")  GeoCal::GdalRasterImage::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImage &Mi_r, const RasterImage
&Mi_g, const RasterImage &Mi_b, const RasterImage &Mi_ir, int
Dt=GDT_Byte, int Fill_value=0)
Copy the given RasterImages into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";

%feature("docstring")  GeoCal::GdalRasterImage::save_to_erdas "
static void GeoCal::GdalRasterImage::save_to_erdas(const std::string &Oname, const std::vector< boost::shared_ptr<
GeoCal::RasterImage > > &d, int Dt=GDT_Byte, int Fill_value=0)
";
%feature("docstring")  GeoCal::GdalRasterImage::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImageMultiBand &d, int
Dt=GDT_Byte, int Fill_value=0)
Copy the given RasterImages into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";


// File: a00068.xml

%feature("docstring") GeoCal::Geocentric "
This is a ground coordinate, represented in Geocentric coordinates.

C++ includes: geodetic.h ";

%feature("docstring")  GeoCal::Geocentric::convert_to_cf "
boost::shared_ptr< CartesianFixed > Geocentric::convert_to_cf() const
Convert to CartesianFixed. ";

%feature("docstring")  GeoCal::Geocentric::Geocentric "
Geocentric::Geocentric(const GroundCoordinate &Gc)
Convert from GroundCoor. ";

%feature("docstring")  GeoCal::Geocentric::Geocentric "
GeoCal::Geocentric::Geocentric(double Latitude, double Longitude, double Height_ellipsoid=0)
Make an Geocentric with the given latitude, longitude, and height.

Latitude and longitude are in degrees, height is in meters. Longitude
should be between -180 and 180 and latitude -90 and 90. ";

%feature("docstring")  GeoCal::Geocentric::Geocentric "
GeoCal::Geocentric::Geocentric()
Default constructor. ";

%feature("docstring")  GeoCal::Geocentric::height_reference_surface "
virtual double GeoCal::Geocentric::height_reference_surface() const
Height above ellipsoid, in meters. ";

%feature("docstring")  GeoCal::Geocentric::latitude "
virtual double GeoCal::Geocentric::latitude() const
Return latitude in degrees. Latitude is -90 to 90. ";

%feature("docstring")  GeoCal::Geocentric::longitude "
virtual double GeoCal::Geocentric::longitude() const
Return longitude in degrees. Longitude is -180 to 180. ";

%feature("docstring")  GeoCal::Geocentric::print "
void Geocentric::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::Geocentric::print_to_string "
std::string GeoCal::Printable< GroundCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Geocentric::~Geocentric "
virtual GeoCal::Geocentric::~Geocentric()
Destructor. ";


// File: a00069.xml

%feature("docstring") GeoCal::Geodetic "
This is a ground coordinate, represented in Geodetic coordinates.

C++ includes: geodetic.h ";

%feature("docstring")  GeoCal::Geodetic::convert_to_cf "
boost::shared_ptr< CartesianFixed > Geodetic::convert_to_cf() const
Convert to CartesianFixed. ";

%feature("docstring")  GeoCal::Geodetic::Geodetic "
Geodetic::Geodetic(const GroundCoordinate &Gc)
Convert from GroundCoor. ";

%feature("docstring")  GeoCal::Geodetic::Geodetic "
GeoCal::Geodetic::Geodetic(double Latitude, double Longitude, double Height_ellipsoid=0)
Make an Geodetic with the given latitude, longitude, and height.

Latitude and longitude are in degrees, height is in meters. Longitude
should be between -180 and 180 and latitude -90 and 90. ";

%feature("docstring")  GeoCal::Geodetic::Geodetic "
GeoCal::Geodetic::Geodetic()
Default constructor. ";

%feature("docstring")  GeoCal::Geodetic::height_reference_surface "
virtual double GeoCal::Geodetic::height_reference_surface() const
Height above ellipsoid, in meters. ";

%feature("docstring")  GeoCal::Geodetic::latitude "
virtual double GeoCal::Geodetic::latitude() const
Return latitude in degrees. Latitude is -90 to 90. ";

%feature("docstring")  GeoCal::Geodetic::longitude "
virtual double GeoCal::Geodetic::longitude() const
Return longitude in degrees. Longitude is -180 to 180. ";

%feature("docstring")  GeoCal::Geodetic::print "
void Geodetic::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::Geodetic::print_to_string "
std::string GeoCal::Printable< GroundCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Geodetic::~Geodetic "
virtual GeoCal::Geodetic::~Geodetic()
Destructor. ";


// File: a00070.xml

%feature("docstring") GeoCal::GeodeticConverter "
CoordinateConverter that goes to and from Geodetic coordinates.

C++ includes: coordinate_converter.h ";

%feature("docstring")  GeoCal::GeodeticConverter::convert_from_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::GeodeticConverter::convert_from_coordinate(double X, double Y, double Height=0) const
Convert to geodetic.

X and Y are longitude and latitude in degrees, and height is in
meters. ";

%feature("docstring")  GeoCal::GeodeticConverter::convert_to_coordinate "
virtual void GeoCal::GeodeticConverter::convert_to_coordinate(const GroundCoordinate &Gc, double &X, double &Y, double &Height)
const
Convert to geodetic.

X and Y are longitude and latitude in degrees, and height is in
meters. ";

%feature("docstring")  GeoCal::GeodeticConverter::is_same "
virtual bool GeoCal::GeodeticConverter::is_same(const CoordinateConverter &Conv) const
Test if two CoordinateConverters are the same coordinate system. ";

%feature("docstring")  GeoCal::GeodeticConverter::print "
virtual void GeoCal::GeodeticConverter::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::GeodeticConverter::print_to_string "
std::string GeoCal::Printable< CoordinateConverter  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::GeodeticConverter::~GeodeticConverter "
virtual GeoCal::GeodeticConverter::~GeodeticConverter()
Destructor. ";


// File: a00071.xml

%feature("docstring") GeoCal::GlobalFixture "
This is a global fixture that is available to all unit tests.

C++ includes: global_fixture.h ";

%feature("docstring")  GeoCal::GlobalFixture::GlobalFixture "
GlobalFixture::GlobalFixture()
Setup for all unit tests. ";

%feature("docstring")  GeoCal::GlobalFixture::shiva_test_data_dir "
std::string GeoCal::GlobalFixture::shiva_test_data_dir() const
";
%feature("docstring")  GeoCal::GlobalFixture::stereo_test_data_dir "
std::string GeoCal::GlobalFixture::stereo_test_data_dir() const
";
%feature("docstring")  GeoCal::GlobalFixture::test_data_dir "
std::string GlobalFixture::test_data_dir() const
Directory where test data is.

This already includes the trailing slash, so you can just do
test_data_data() + \"foo.txt\" in your unit tests. ";

%feature("docstring")  GeoCal::GlobalFixture::~GlobalFixture "
virtual GeoCal::GlobalFixture::~GlobalFixture()
";

// File: a00072.xml

%feature("docstring") GeoCal::GroundCoordinate "
This represents a location on the ground.

It is a base class, each of the derived classes represent different
coordinate systems that can be used to express the ground location.
Any ground coordinate can be converted to CartesianFixed coordinates.

Because it is commonly needed, we also can convert to latitude,
longitude and height_reference_surface. The default is just convert
first to CartesianFixed, and then convert to latitude, longitude or
height_reference_surface, but derived classes can supply more
efficient versions of this if desired.

C++ includes: ground_coordinate.h ";

%feature("docstring")  GeoCal::GroundCoordinate::convert_to_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::GroundCoordinate::convert_to_cf() const =0
Convert to CartesianFixed. ";

%feature("docstring")  GeoCal::GroundCoordinate::height_reference_surface "
double GeoCal::GroundCoordinate::height_reference_surface() const
Return height in meters above reference surface (e.g., WGS-84 for the
earth).

By default we just convert to CartesianFixed and then to height, but
derived classes can supply more efficient versions of these if needed.
";

%feature("docstring")  GeoCal::GroundCoordinate::latitude "
double GeoCal::GroundCoordinate::latitude() const
Return latitude in degrees.

By default we just convert to CartesianFixed and then to latitude, but
derived classes can supply more efficient versions of these if needed.
Latitude is -90 to 90. ";

%feature("docstring")  GeoCal::GroundCoordinate::longitude "
double GeoCal::GroundCoordinate::longitude() const
Return longitude in degrees.

By default we just convert to CartesianFixed and then to longitude,
but derived classes can supply more efficient versions of these if
needed. Longitude is -180 to 180. ";

%feature("docstring")  GeoCal::GroundCoordinate::print "
virtual void GeoCal::GroundCoordinate::print(std::ostream &Os) const =0
Print to given stream. ";

%feature("docstring")  GeoCal::GroundCoordinate::print_to_string "
std::string GeoCal::Printable< GroundCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::GroundCoordinate::~GroundCoordinate "
virtual GeoCal::GroundCoordinate::~GroundCoordinate()
Destructor. ";


// File: a00073.xml

%feature("docstring") GeoCal::GslFsolver "
";
%feature("docstring")  GeoCal::GslFsolver::GslFsolver "
GeoCal::GslFsolver::GslFsolver()
";
%feature("docstring")  GeoCal::GslFsolver::~GslFsolver "
GeoCal::GslFsolver::~GslFsolver()
";

// File: a00074.xml

%feature("docstring") GeoCal::GslMatrix "
This provides thin wrapper around the GNU Scientific Library
gsl_matrix.

The GSL is a pretty complete scientific library. However, it is C
based and out of the box doesn't place nicely with other C++ classes
(such as blitz::Array). We provide some thin wrapper code around the
GSL code to interface these two classes. This provides both a
blitz::Array and a gsl_matrix view of the same underlying data.

C++ includes: geocal_gsl_matrix.h ";

%feature("docstring")  GeoCal::GslMatrix::blitz_array "
const blitz::Array<double, 2>& GeoCal::GslMatrix::blitz_array() const
Return blitz::Array look at data. ";

%feature("docstring")  GeoCal::GslMatrix::blitz_array "
blitz::Array<double, 2>& GeoCal::GslMatrix::blitz_array()
Return blitz::Array look at data. ";

%feature("docstring")  GeoCal::GslMatrix::gsl "
const gsl_matrix* GeoCal::GslMatrix::gsl() const
Return gsl_matrix look at data. ";

%feature("docstring")  GeoCal::GslMatrix::gsl "
gsl_matrix* GeoCal::GslMatrix::gsl()
Return gsl_matrix look at data. ";

%feature("docstring")  GeoCal::GslMatrix::GslMatrix "
GeoCal::GslMatrix::GslMatrix()
Default constructor. ";

%feature("docstring")  GeoCal::GslMatrix::GslMatrix "
GeoCal::GslMatrix::GslMatrix(gsl_matrix *M, bool Owned=true)
Use data owned by gsl_matrix.

This data can either have ownership passed to this class (in which
case we delete it when done with it), or just a reference (in which
case the lifetime is handled outside of this class). ";

%feature("docstring")  GeoCal::GslMatrix::GslMatrix "
GeoCal::GslMatrix::GslMatrix(blitz::Array< double, 2 > &M)
Use data owned by blitz::Array.

Note that if this data isStorageContiguous(), then we use the data in
place. If it isn't be make a contiguous copy of it. This means that in
one case the original matrix will be modified if the gsl_matrix view
of the data is modified, and the second case it won't. If you don't
want this behaviour, you can form a copy before you pass it to this
constructor. ";

%feature("docstring")  GeoCal::GslMatrix::reset "
void GslMatrix::reset(gsl_matrix *M, bool Owned=true)
Reset class to point to new matrix.

This data can either have ownership passed to this class (in which
case we delete it when done with it), or just a reference (in which
case the lifetime is handled outside of this class). ";

%feature("docstring")  GeoCal::GslMatrix::reset "
void GslMatrix::reset(blitz::Array< double, 2 > &M)
Reset to Use data owned by blitz::Array.

Note that if this data isStorageContiguous(), then we use the data in
place. If it isn't be make a contiguous copy of it. This means that in
one case the original matrix will be modified if the gsl_matrix view
of the data is modified, and the second case it won't. If you don't
want this behaviour, you can form a copy before you pass it to this
constructor. ";

%feature("docstring")  GeoCal::GslMatrix::~GslMatrix "
GslMatrix::~GslMatrix()
Destructor. ";


// File: a00075.xml

%feature("docstring") GeoCal::GslMultiFdfsolver "
";
%feature("docstring")  GeoCal::GslMultiFdfsolver::GslMultiFdfsolver "
GeoCal::GslMultiFdfsolver::GslMultiFdfsolver(size_t n)
";
%feature("docstring")  GeoCal::GslMultiFdfsolver::~GslMultiFdfsolver "
GeoCal::GslMultiFdfsolver::~GslMultiFdfsolver()
";

// File: a00076.xml

%feature("docstring") GeoCal::GslMultiFsolver "
";
%feature("docstring")  GeoCal::GslMultiFsolver::GslMultiFsolver "
GeoCal::GslMultiFsolver::GslMultiFsolver(size_t n)
";
%feature("docstring")  GeoCal::GslMultiFsolver::~GslMultiFsolver "
GeoCal::GslMultiFsolver::~GslMultiFsolver()
";

// File: a00077.xml

%feature("docstring") GeoCal::GslVector "
This provides thin wrapper around the GNU Scientific Library
gsl_vector.

The GSL is a pretty complete scientific library. However, is C based
and out of the box doesn't place nicely with other C++ classes (such
as blitz::Array). We provide some thin wrapper code around the GSL
code to interface these two classes. This provides both a blitz::Array
and a gsl_vector view of the same underlying data.

C++ includes: geocal_gsl_matrix.h ";

%feature("docstring")  GeoCal::GslVector::blitz_array "
const blitz::Array<double, 1>& GeoCal::GslVector::blitz_array() const
Return blitz::Array look at data. ";

%feature("docstring")  GeoCal::GslVector::blitz_array "
blitz::Array<double, 1>& GeoCal::GslVector::blitz_array()
Return blitz::Array look at data. ";

%feature("docstring")  GeoCal::GslVector::gsl "
const gsl_vector* GeoCal::GslVector::gsl() const
Return gsl_vector look at data. ";

%feature("docstring")  GeoCal::GslVector::gsl "
gsl_vector* GeoCal::GslVector::gsl()
Return gsl_vector look at data. ";

%feature("docstring")  GeoCal::GslVector::GslVector "
GeoCal::GslVector::GslVector()
Default constructor. ";

%feature("docstring")  GeoCal::GslVector::GslVector "
GeoCal::GslVector::GslVector(gsl_vector *M, bool Owned=true)
Use data owned by gsl_vector.

This data can either have ownership passed to this class (in which
case we delete it when done with it), or just a reference (in which
case the lifetime is handled outside of this class). ";

%feature("docstring")  GeoCal::GslVector::GslVector "
GeoCal::GslVector::GslVector(blitz::Array< double, 1 > &M)
Use data owned by blitz::Array.

Note that if this data isStorageContiguous(), then we use the data in
place. If it isn't be make a contiguous copy of it. This means that in
one case the original vector will be modified if the gsl_vector view
of the data is modified, and the second case it won't. If you don't
want this behaviour, you can form a copy before you pass it to this
constructor. ";

%feature("docstring")  GeoCal::GslVector::reset "
void GslVector::reset(gsl_vector *M, bool Owned=true)
Reset class to point to new vector.

This data can either have ownership passed to this class (in which
case we delete it when done with it), or just a reference (in which
case the lifetime is handled outside of this class). ";

%feature("docstring")  GeoCal::GslVector::reset "
void GslVector::reset(blitz::Array< double, 1 > &M)
Reset to use data owned by blitz::Array.

Note that if this data isStorageContiguous(), then we use the data in
place. If it isn't be make a contiguous copy of it. This means that in
one case the original vector will be modified if the gsl_vector view
of the data is modified, and the second case it won't. If you don't
want this behaviour, you can form a copy before you pass it to this
constructor. ";

%feature("docstring")  GeoCal::GslVector::~GslVector "
GslVector::~GslVector()
Destructor. ";


// File: a00078.xml

%feature("docstring") GeoCal::GslWorkspace "
";
%feature("docstring")  GeoCal::GslWorkspace::GslWorkspace "
GeoCal::GslWorkspace::GslWorkspace(size_t n, size_t p)
";
%feature("docstring")  GeoCal::GslWorkspace::~GslWorkspace "
GeoCal::GslWorkspace::~GslWorkspace()
";

// File: a00079.xml

%feature("docstring") GeoCal::VicarOgrNsp::Gtif "
";
%feature("docstring")  GeoCal::VicarOgrNsp::Gtif::Gtif "
GeoCal::VicarOgrNsp::Gtif::Gtif(const Tif &T)
";
%feature("docstring")  GeoCal::VicarOgrNsp::Gtif::~Gtif "
GeoCal::VicarOgrNsp::Gtif::~Gtif()
";

// File: a00080.xml

%feature("docstring") HDFEOS5Image "
";
%feature("docstring")  HDFEOS5Image::HDFEOS5Image "
HDFEOS5Image::HDFEOS5Image(const std::string &filename, const std::string &grid, const
std::string &datafield)
";
%feature("docstring")  HDFEOS5Image::HDFEOS5Image "
HDFEOS5Image::HDFEOS5Image(int Number_line, int Number_sample)
";
%feature("docstring")  HDFEOS5Image::interpolate "
double GeoCal::GeoCalCore::RasterImage::interpolate(double Line, double Sample) const
";
%feature("docstring")  HDFEOS5Image::interpolate "
double GeoCal::GeoCalCore::RasterImage::interpolate(const ImageCoor &Ic) const
";
%feature("docstring")  HDFEOS5Image::number_line "
int GeoCal::GeoCalCore::RasterImage::number_line() const
";
%feature("docstring")  HDFEOS5Image::number_sample "
int GeoCal::GeoCalCore::RasterImage::number_sample() const
";
%feature("docstring")  HDFEOS5Image::number_tile_line "
virtual int GeoCal::GeoCalCore::RasterImage::number_tile_line() const
";
%feature("docstring")  HDFEOS5Image::number_tile_sample "
virtual int GeoCal::GeoCalCore::RasterImage::number_tile_sample() const
";
%feature("docstring")  HDFEOS5Image::subset "
void HDFEOS5Image::subset(int Lstart, int Sstart, int Number_line, int Number_sample,
GeoCal::GeoCalCore::Array2d< int > &Res) const
";
%feature("docstring")  HDFEOS5Image::write "
void HDFEOS5Image::write(int Line, int Sample, int Val)
";
%feature("docstring")  HDFEOS5Image::~HDFEOS5Image "
HDFEOS5Image::~HDFEOS5Image()
";

// File: a00081.xml

%feature("docstring") GeoCal::HdfFile "
This class reads a HDF5 file.

Note that this is just a thin layer on top of the HDF 5 libraries to
make the file operations we need to do in GeoCal easier. There are
many other things that can be done with a HDF 5 than what this class
exposes.

Right now, we don't support writing. This is just because I don't
happen to need this. We can add this if we ever want to write files.

C++ includes: hdf_file.h ";

%feature("docstring")  GeoCal::HdfFile::close "
void GeoCal::HdfFile::close()
Close the underlying file.

This is automatically done by the destructor, so you only need to call
this if you want to force a close (e.g., for a unit test) ";

%feature("docstring")  GeoCal::HdfFile::file_name "
const std::string& GeoCal::HdfFile::file_name() const
File name. ";

%feature("docstring")  GeoCal::HdfFile::has_attribute "
bool HdfFile::has_attribute(const std::string &Aname) const
Check to see if a attribute is in the file.

Determine if attribute is present. ";

%feature("docstring")  GeoCal::HdfFile::has_object "
bool GeoCal::HdfFile::has_object(const std::string &Objname) const
Check to see if an object (such as a Dataset) is in the file. ";

%feature("docstring")  GeoCal::HdfFile::HdfFile "
HdfFile::HdfFile(const std::string &Fname, Mode M=READ)
Open the given file with the given mode. ";

%feature("docstring")  GeoCal::HdfFile::pred_arr "
H5::PredType GeoCal::HdfFile::pred_arr< double >() const
";
%feature("docstring")  GeoCal::HdfFile::pred_data "
H5::PredType GeoCal::HdfFile::pred_data< double >() const
";
%feature("docstring")  GeoCal::HdfFile::print "
void HdfFile::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::HdfFile::print_to_string "
std::string GeoCal::Printable< HdfFile  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::HdfFile::read_attribute "
T GeoCal::HdfFile::read_attribute(const std::string &Aname) const
Read the given attribute attached to a group or dataset. ";

%feature("docstring")  GeoCal::HdfFile::read_attribute "
T GeoCal::HdfFile::read_attribute(const std::string &Aname) const
";
%feature("docstring")  GeoCal::HdfFile::read_field "
blitz::Array< T, D > GeoCal::HdfFile::read_field(const std::string &Dataname) const
Read a given field. ";

%feature("docstring")  GeoCal::HdfFile::read_field "
blitz::Array< T, D > GeoCal::HdfFile::read_field(const std::string &Dataname, const blitz::TinyVector< int, D >
&Start, const blitz::TinyVector< int, D > &Size) const
Read a given field.

This reads a subset of the data, given by Start and Size. ";

%feature("docstring")  GeoCal::HdfFile::~HdfFile "
virtual GeoCal::HdfFile::~HdfFile()
";
%feature("docstring")  GeoCal::HdfFile::is_hdf "
static bool GeoCal::HdfFile::is_hdf(const std::string &Fname)
Return true if the given file is an HDF file. ";


// File: a00082.xml

%feature("docstring") GeoCal::HdfOrbit "
This is an implementation of an Orbit that reads position, velocity,
and attitude quaternion from an HDF file.

For times that fall between the values given in the file, we
interpolate to get the OrbitData.

The file should have the following fields:

<Base group>=\"\">/Attitude/Time - natt in size, time of attitude
measurement as doubles. <Base group>=\"\">/Attitude/Quaternion - natt
x 4 in size, quaternion to take same coordinate system as Position
(e.g., ECI) <Base group>=\"\">/Ephemeris/Time - neph in size, time of
ephemeris measurement as doubles <Base group>=\"\">/Ephemeris/Position
- neph x 3 in size, position measurement as doubles in meters <Base
group>=\"\">/Ephemeris/Velocity - neph x 3 in size, velocity
measurement as doubles in meter/second

Because it is useful, we allow the type of position measurement and
time to be changed. The measurement class is passed in, e.g., Eci, as
is a small wrapper to give the conversion to Time.

C++ includes: hdf_orbit.h ";

%feature("docstring")  GeoCal::HdfOrbit::base_group "
const std::string& GeoCal::HdfOrbit< PositionType, TimeCreatorType >::base_group() const
Return the base group. ";

%feature("docstring")  GeoCal::HdfOrbit::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::HdfOrbit::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::HdfOrbit::file_name "
const std::string& GeoCal::HdfOrbit< PositionType, TimeCreatorType >::file_name() const
Return the file name. ";

%feature("docstring")  GeoCal::HdfOrbit::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::HdfOrbit::HdfOrbit "
GeoCal::HdfOrbit< PositionType, TimeCreatorType >::HdfOrbit(const std::string &Fname, const std::string &Base_group=\"Orbit\")
Read the given orbit data file.

You can optional pass the base group of the HDF file, the default is
\"/Orbit\" ";

%feature("docstring")  GeoCal::HdfOrbit::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::HdfOrbit::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::HdfOrbit::orbit_data "
boost::shared_ptr< OrbitData > GeoCal::HdfOrbit< PositionType, TimeCreatorType >::orbit_data(Time T) const
Return orbit data for given time. ";

%feature("docstring")  GeoCal::HdfOrbit::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::HdfOrbit::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::HdfOrbit::print "
virtual void GeoCal::HdfOrbit< PositionType, TimeCreatorType >::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::HdfOrbit::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::HdfOrbit::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::HdfOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::HdfOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::HdfOrbit::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";

%feature("docstring")  GeoCal::HdfOrbit::~HdfOrbit "
virtual GeoCal::HdfOrbit< PositionType, TimeCreatorType >::~HdfOrbit()
";

// File: a00083.xml

%feature("docstring") GeoCal::IbisColumn "
This class handles reading and writing a single column of a IBIS file.

If you update a column and want the results to be written out, then
you need to mark the \"updated\" flag as true, otherwise whatever
changes are made won't be written to disk.

C++ includes: ibis_file.h ";

%feature("docstring")  GeoCal::IbisColumn::column_index "
int GeoCal::IbisColumnBase::column_index() const
Column index.

This is 0-based rather than the 1-based used by some VICAR routines.
";

%feature("docstring")  GeoCal::IbisColumn::flush "
void GeoCal::IbisColumn< T >::flush()
Flush data to disk. ";

%feature("docstring")  GeoCal::IbisColumn::flush "
void GeoCal::IbisColumn< std::string >::flush()
Flush data to disk. ";

%feature("docstring")  GeoCal::IbisColumn::ibis_file "
const IbisFile& GeoCal::IbisColumnBase::ibis_file() const
IbisFile that this column is for. ";

%feature("docstring")  GeoCal::IbisColumn::print "
void GeoCal::IbisColumn< T >::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::IbisColumn::print "
void GeoCal::IbisColumn< std::string >::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::IbisColumn::print_to_string "
std::string GeoCal::Printable< IbisColumnBase  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::IbisColumn::size_byte "
int GeoCal::IbisColumnBase::size_byte() const
Size in bytes of column element.

This is mostly only interesting for the VICAR_ASCII types, since the
other types are just the size of the underlying type. ";

%feature("docstring")  GeoCal::IbisColumn::~IbisColumn "
virtual GeoCal::IbisColumn< T >::~IbisColumn()
";

// File: a00084.xml

%feature("docstring") GeoCal::IbisColumnBase "
Base class for all IBIS columns.

C++ includes: ibis_file.h ";

%feature("docstring")  GeoCal::IbisColumnBase::column_index "
int GeoCal::IbisColumnBase::column_index() const
Column index.

This is 0-based rather than the 1-based used by some VICAR routines.
";

%feature("docstring")  GeoCal::IbisColumnBase::flush "
virtual void GeoCal::IbisColumnBase::flush()
Flush data to disk. ";

%feature("docstring")  GeoCal::IbisColumnBase::ibis_file "
const IbisFile& GeoCal::IbisColumnBase::ibis_file() const
IbisFile that this column is for. ";

%feature("docstring")  GeoCal::IbisColumnBase::print "
virtual void GeoCal::IbisColumnBase::print(std::ostream &Os) const =0
";
%feature("docstring")  GeoCal::IbisColumnBase::print_to_string "
std::string GeoCal::Printable< IbisColumnBase  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::IbisColumnBase::size_byte "
int GeoCal::IbisColumnBase::size_byte() const
Size in bytes of column element.

This is mostly only interesting for the VICAR_ASCII types, since the
other types are just the size of the underlying type. ";

%feature("docstring")  GeoCal::IbisColumnBase::~IbisColumnBase "
virtual GeoCal::IbisColumnBase::~IbisColumnBase()
";

// File: a00085.xml

%feature("docstring") GeoCal::IbisFile "
This provides read and write for a IBIS file.

IBIS is VICAR format used for tabulating raster format data -
basically it is a table. This is used to interact with existing VICAR
routines.

The ASCII column type is a bit of a special case. For IBIS files, the
ASCII column type is a fixed number of characters. We access this a
std::string type. If you attempt to write a string that is too large
for a column, then we silently truncate the string. Alternatively, we
could have triggered an error in that case, but after thinking about
it truncating seemed like the better choice. We can revisit this
decision if it becomes clear that triggering an error would be better.

As an implementation detail, the class VicarFile and IbisFile need to
coordinate in translating file names to unit numbers since the same
underlying Vicar routine zvunit is used by both. We arbitrarily have
VicarFile handle this, and IbisFile accesses VicarFile when it needs
to do this.

C++ includes: ibis_file.h ";

%feature("docstring")  GeoCal::IbisFile::access "
access_type GeoCal::IbisFile::access() const
Access type of file. ";

%feature("docstring")  GeoCal::IbisFile::close "
void IbisFile::close()
Close a file.

You don't normally need to call this directly, it is done by the
destructor. But it is useful to have for use by Ruby. ";

%feature("docstring")  GeoCal::IbisFile::column "
const IbisColumn<T>& GeoCal::IbisFile::column(int Index) const
Give access to the given column. ";

%feature("docstring")  GeoCal::IbisFile::column "
IbisColumn<T>& GeoCal::IbisFile::column(int Index)
Give access to the given column. ";

%feature("docstring")  GeoCal::IbisFile::column_data_type "
data_type GeoCal::IbisFile::column_data_type(int I) const
Data type for given column.

Note that this is 0-based rather than 1-based. ";

%feature("docstring")  GeoCal::IbisFile::data "
const T& GeoCal::IbisFile::data(int i, int j) const
Short cut for column<T>(j).data[i]. ";

%feature("docstring")  GeoCal::IbisFile::data "
T& GeoCal::IbisFile::data(int i, int j)
Short cut for column<T>(j).data[i]. ";

%feature("docstring")  GeoCal::IbisFile::file_name "
const std::string& GeoCal::IbisFile::file_name() const
File name. ";

%feature("docstring")  GeoCal::IbisFile::flush "
void IbisFile::flush()
Flush data to disk, including any changes.

This is automatically called by the destructor. ";

%feature("docstring")  GeoCal::IbisFile::ibis_fh "
int IbisFile::ibis_fh() const
IBIS file handle number used in IBIS calls. ";

%feature("docstring")  GeoCal::IbisFile::IbisFile "
IbisFile::IbisFile(const std::string &Fname, access_type Access=READ)
Open given file for read or update. ";

%feature("docstring")  GeoCal::IbisFile::IbisFile "
IbisFile::IbisFile(const std::string &Fname, int Number_row, const std::vector<
std::string > &Format, const std::string &Organization=\"COLUMN\")
Create a new IBIS file.

We pass in the number of rows and the format for each column. The
format is given as a list of strings, with each string being one of
the format types taken by the IBIS library - \"BYTE\", \"HALF\",
\"FULL\", \"REAL\", \"DOUB\", \"Ann\" (e.g., \"A10\" for 10 character
ASCII). The organization of the file can be given as \"COLUMN\" or
\"ROW\". ";

%feature("docstring")  GeoCal::IbisFile::number_col "
int GeoCal::IbisFile::number_col() const
Number of columns in the file. ";

%feature("docstring")  GeoCal::IbisFile::number_row "
int GeoCal::IbisFile::number_row() const
Number of rows in the file. ";

%feature("docstring")  GeoCal::IbisFile::print "
void IbisFile::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::IbisFile::print_to_string "
std::string GeoCal::Printable< IbisFile  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::IbisFile::unit "
int GeoCal::IbisFile::unit() const
Unit number for VicarFile. ";

%feature("docstring")  GeoCal::IbisFile::~IbisFile "
IbisFile::~IbisFile()
Destructor. ";

%feature("docstring")  GeoCal::IbisFile::IBISColumnReadw "
int IbisFile::IBISColumnReadw(int ibis_id, char *buffer, int column, int srow, int nrows)
This is a thin wrapper around the p1 function IBISColumnRead.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::IbisFile::IBISColumnWritew "
int IbisFile::IBISColumnWritew(int ibis_id, char *buffer, int column, int srow, int nrows)
This is a thin wrapper around the p1 function IBISColumnWrite.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";


// File: a01133.xml

%feature("docstring") std::ifstream "
STL class. ";


// File: a00086.xml

%feature("docstring") GeoCal::IgcImageToImageMatch "
ImageToImageMatch where the images and prediction comes from
ImageGroundConnection objects.

C++ includes: igc_image_to_image_match.h ";

%feature("docstring")  GeoCal::IgcImageToImageMatch::IgcImageToImageMatch "
GeoCal::IgcImageToImageMatch::IgcImageToImageMatch(const boost::shared_ptr< ImageGroundConnection > &Igc1, const
boost::shared_ptr< ImageGroundConnection > &Igc2, const
boost::shared_ptr< ImageMatcher > &Matcher)
";
%feature("docstring")  GeoCal::IgcImageToImageMatch::image_ground_connection1 "
const boost::shared_ptr<ImageGroundConnection>& GeoCal::IgcImageToImageMatch::image_ground_connection1() const
ImageGroundConnection for image 1. ";

%feature("docstring")  GeoCal::IgcImageToImageMatch::image_ground_connection2 "
const boost::shared_ptr<ImageGroundConnection>& GeoCal::IgcImageToImageMatch::image_ground_connection2() const
ImageGroundConnection for image 2. ";

%feature("docstring")  GeoCal::IgcImageToImageMatch::match "
void IgcImageToImageMatch::match(const ImageCoordinate &Ic1, ImageCoordinate &Ic2, double &Line_sigma,
double &Sample_sigma, bool &Success, int *Diagnostic=0) const
Match point Ic1 in the first image with the second image.

We return the location in the second image along with the uncertainty
if Success is true, otherwise the match failed.

The Diagnostic argument is used to indicate why a match failed. For
many purposes you can ignore this, because you need to know detailed
information about the specific ImageMatcher that a user passed to a
function and what the diagnostic codes mean. However, for some
purposes the failure reason can be useful (e.g., tuning parameters of
a LsmMatcher). ";

%feature("docstring")  GeoCal::IgcImageToImageMatch::matcher "
const boost::shared_ptr<ImageMatcher>& GeoCal::IgcImageToImageMatch::matcher() const
Matcher we are using. ";

%feature("docstring")  GeoCal::IgcImageToImageMatch::print "
virtual void GeoCal::IgcImageToImageMatch::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::IgcImageToImageMatch::print_to_string "
std::string GeoCal::Printable< ImageToImageMatch  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::IgcImageToImageMatch::~IgcImageToImageMatch "
virtual GeoCal::IgcImageToImageMatch::~IgcImageToImageMatch()
";

// File: a00087.xml

%feature("docstring") GeoCal::IgcMapProjected "
This is a RasterImage that has been map projected using an
ImageGroundConnection and a Camera.

To do this we do two steps:

We calculate roughly what the difference in resolution is between the
original data and final MapInfo. We do this by looking at the center
pixel of the original data and the pixel +1 in line and sample. We
then use RasterAveraged to average the original data to roughly the
resolution of the final MapInfo. If the final MapInfo is near the same
resolution as the original, or if it has a higher resolution, then we
don't do any averaging. Alternatively, you can pass in the averaging
factor (include a value of 1 which turns this behavior off).

We then interpolate the possibly averaged data to the final
projection.

It is ok if the final MapInfo contains areas outside of the original
data. For any pixel outside of the original data, we just return a
value of 0.

This class calculates the data on the fly. Sometimes this is what you
want, but if you are going to be using the resulting data a few times,
you may want to use a MemoryRasterImage to generate a copy once and
keep it in memory.

C++ includes: igc_map_projected.h ";

%feature("docstring")  GeoCal::IgcMapProjected::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::IgcMapProjected::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::IgcMapProjected::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::IgcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IgcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IgcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IgcMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IgcMapProjected::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::IgcMapProjected::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::IgcMapProjected::IgcMapProjected "
IgcMapProjected::IgcMapProjected(const MapInfo &Mi, const boost::shared_ptr< ImageGroundConnection >
&Igc, const boost::shared_ptr< RasterImage > &R, int Avg_fact=-1, bool
Read_into_memory=true)
Constructor.

We average the data either by the factor given as Avg_fact, or by
ratio of the Mapinfo resolution and the camera resolution.

Most of the time, you want to use the raster image associated with the
Igc. This constructor allows you to use a different RasterImage
(perhaps a related band, or something like that). ";

%feature("docstring")  GeoCal::IgcMapProjected::IgcMapProjected "
IgcMapProjected::IgcMapProjected(const MapInfo &Mi, const boost::shared_ptr< ImageGroundConnection >
&Igc, int Avg_fact=-1, bool Read_into_memory=true)
Constructor.

We average the data either by the factor given as Avg_fact, or by
ratio of the Mapinfo resolution and the camera resolution.

This uses the Igc.image(). ";

%feature("docstring")  GeoCal::IgcMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::IgcMapProjected::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::IgcMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::IgcMapProjected::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::IgcMapProjected::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::IgcMapProjected::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::IgcMapProjected::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::IgcMapProjected::number_tile_line "
virtual int GeoCal::CalcMapProjected::number_tile_line() const
Number of lines in each tile. ";

%feature("docstring")  GeoCal::IgcMapProjected::number_tile_sample "
virtual int GeoCal::CalcMapProjected::number_tile_sample() const
Number of samples in each tile. ";

%feature("docstring")  GeoCal::IgcMapProjected::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::IgcMapProjected::print "
void IgcMapProjected::print(std::ostream &Os) const
Print out a description of the object. ";

%feature("docstring")  GeoCal::IgcMapProjected::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::IgcMapProjected::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::IgcMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IgcMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IgcMapProjected::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IgcMapProjected::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::IgcMapProjected::read_ptr "
void CalcMapProjected::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IgcMapProjected::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::IgcMapProjected::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::IgcMapProjected::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::IgcMapProjected::unchecked_read "
int CalcMapProjected::unchecked_read(int Line, int Sample) const
Read pixel value at given line and sample. ";

%feature("docstring")  GeoCal::IgcMapProjected::unchecked_write "
void CalcMapProjected::unchecked_write(int Line, int Sample, int Val)
Write a value.

This isn't actually implemented for this type of RasterImage. ";

%feature("docstring")  GeoCal::IgcMapProjected::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::IgcMapProjected::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::IgcMapProjected::write_image "
void CalcMapProjected::write_image(RasterImage &Out, int Grid_spacing=1) const
input exactly at this grid spacing, and interpolate in between.

This is much faster than calculating ever point, and if the grid
spacing is small compared to the Dem and any nonlinearities give
results very close to the full calculation. ";

%feature("docstring")  GeoCal::IgcMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out) const
Write output to multiple RasterImage at once.

The number should be <= the number given to the constructor. ";

%feature("docstring")  GeoCal::IgcMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out, int
Grid_spacing) const
Write output to multiple RasterImage at once.

This variation takes a grid spacing to use. We calculate the image
coordinates in the input exactly at this grid spacing, and interpolate
in between. This is much faster than calculating ever point, and if
the grid spacing is small compared to the Dem and any Camera
nonlinearities give results very close to the full calculation. ";

%feature("docstring")  GeoCal::IgcMapProjected::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::IgcMapProjected::~IgcMapProjected "
virtual GeoCal::IgcMapProjected::~IgcMapProjected()
Destructor. ";


// File: a00088.xml

%feature("docstring") GeoCal::GeoCalCore::ImageCoor "
";
%feature("docstring")  GeoCal::GeoCalCore::ImageCoor::ImageCoor "
GeoCal::GeoCalCore::ImageCoor::ImageCoor(double Line, double Sample)
";
%feature("docstring")  GeoCal::GeoCalCore::ImageCoor::ImageCoor "
GeoCal::GeoCalCore::ImageCoor::ImageCoor()
";
%feature("docstring")  GeoCal::GeoCalCore::ImageCoor::line "
double GeoCal::GeoCalCore::ImageCoor::line() const
";
%feature("docstring")  GeoCal::GeoCalCore::ImageCoor::sample "
double GeoCal::GeoCalCore::ImageCoor::sample() const
";

// File: a00089.xml

%feature("docstring") GeoCal::ImageCoordinate "
This describes an image coordinate.

We also define the print operator <<, and equality operator ==. Two
image coordinates are considers equal if they are within 0.01 in the
line and sample.

The convention used throughout GeoCal is that an integer value of the
ImageCoordinate line or sample refers to the center of a pixel. This
means the upper left corner of the bounding box of an image is at
(-0.5, -0.5).

C++ includes: image_coordinate.h ";

%feature("docstring")  GeoCal::ImageCoordinate::ImageCoordinate "
GeoCal::ImageCoordinate::ImageCoordinate(double Line, double Sample)
Constructor. ";

%feature("docstring")  GeoCal::ImageCoordinate::ImageCoordinate "
GeoCal::ImageCoordinate::ImageCoordinate(const VicarImageCoordinate &Vic)
Convert from VicarImageCoordinate. ";

%feature("docstring")  GeoCal::ImageCoordinate::ImageCoordinate "
GeoCal::ImageCoordinate::ImageCoordinate()
Default constructor, line and sample aren't initialized. ";

%feature("docstring")  GeoCal::ImageCoordinate::print "
void ImageCoordinate::print(std::ostream &Os) const
Print an ImageCoordinate to a stream. ";

%feature("docstring")  GeoCal::ImageCoordinate::print_to_string "
std::string GeoCal::Printable< ImageCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ImageCoordinate::~ImageCoordinate "
virtual GeoCal::ImageCoordinate::~ImageCoordinate()
";

// File: a00090.xml

%feature("docstring") GeoCal::ImageGroundConnection "
Depending on the the data we are using, we may connect a location in
an image to a ground location in one of several ways.

For example, we might have a rigorous camera model and orbit data
allowing us to use an Ipi. Or we might have a RPC (Rational Polynomial
Coefficients) that connect an image to the ground. Or we might be
working with data that has already been map projected, so a MapInfo
gives us a direct connection.

For many purposes, we don't care exactly how the connection is done,
we just use the fact that there is a connection.

This class gives a generic interface that can be used for any kind of
a connection between the ground and an image.

C++ includes: image_ground_connection.h ";

%feature("docstring")  GeoCal::ImageGroundConnection::cf_look_vector "
virtual void GeoCal::ImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
boost::shared_ptr< CartesianFixed > &P) const =0
Return look vector for given coordinate, along with a position that
lies along the direction of the look vector (so position of satellite,
or a position on the surface. ";

%feature("docstring")  GeoCal::ImageGroundConnection::cover "
MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
Find a MapInfo that covers the ground coordinate of this
ImageGroundConnection.

We calculate the ground coordinate of the four corners, then find the
MapInfo that covers those corners, optionally adding a boundary in map
pixels. Depending on the actual warping involved, this may or may not
fully cover the edges of the image. ";

%feature("docstring")  GeoCal::ImageGroundConnection::dem "
void GeoCal::ImageGroundConnection::dem(const boost::shared_ptr< Dem > &D)
Set the DEM to use in ground_coordinate. ";

%feature("docstring")  GeoCal::ImageGroundConnection::dem "
const Dem& GeoCal::ImageGroundConnection::dem() const
Dem used by ground_coordinate. ";

%feature("docstring")  GeoCal::ImageGroundConnection::dem_ptr "
const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
DEM used by ground_coordinate. ";

%feature("docstring")  GeoCal::ImageGroundConnection::ground_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::ImageGroundConnection::ground_coordinate_dem "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const =0
Return ground coordinate that goes with a particular image coordinate.

This version supplies a Dem to use. ";

%feature("docstring")  GeoCal::ImageGroundConnection::image "
virtual boost::shared_ptr<RasterImage> GeoCal::ImageGroundConnection::image() const
Underlying image. ";

%feature("docstring")  GeoCal::ImageGroundConnection::image_coordinate "
virtual ImageCoordinate GeoCal::ImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const =0
Return image coordinate that goes with a particular GroundCoordinate.

For some types of ImageGroundConnection, we might not be able to
calculate image_coordinate for all values (e.g., Ipi might fail). In
those cases, we will through a ImageGroundConnectionFailed exception.
This means that nothing is wrong, other than that we can't calculate
the image_coordinate. Callers can catch this exception if they have
some way of handling no image coordinate data. ";

%feature("docstring")  GeoCal::ImageGroundConnection::image_coordinate_jac_ecr "
blitz::Array< double, 2 > ImageGroundConnection::image_coordinate_jac_ecr(const Ecr &Gc) const
Return the Jacobian of the image coordinates with respect to the X, Y,
and Z components of the Ecr ground location. ";

%feature("docstring")  GeoCal::ImageGroundConnection::image_coordinate_jac_parm "
virtual blitz::Array<double, 2> GeoCal::ImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Return the Jacobian of the image coordinates with respect to the
parameters. ";

%feature("docstring")  GeoCal::ImageGroundConnection::number_line "
virtual int GeoCal::ImageGroundConnection::number_line() const
Return number of lines in underlying image. ";

%feature("docstring")  GeoCal::ImageGroundConnection::number_sample "
virtual int GeoCal::ImageGroundConnection::number_sample() const
Return number of samples in underlying image. ";

%feature("docstring")  GeoCal::ImageGroundConnection::parameter "
virtual blitz::Array<double, 1> GeoCal::ImageGroundConnection::parameter() const
A image to ground connection may depend on a set of parameters, which
can by modified (e.g., during a simultaneous bundle adjustment).

This returns those parameters. ";

%feature("docstring")  GeoCal::ImageGroundConnection::parameter "
virtual void GeoCal::ImageGroundConnection::parameter(const blitz::Array< double, 1 > &Parm)
Set the value of the parameters. ";

%feature("docstring")  GeoCal::ImageGroundConnection::parameter_name "
virtual std::vector<std::string> GeoCal::ImageGroundConnection::parameter_name() const
Descriptive name of each parameter. ";

%feature("docstring")  GeoCal::ImageGroundConnection::print "
virtual void GeoCal::ImageGroundConnection::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ImageGroundConnection::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter(const ImageCoordinate &Ic) const
Calculate the approximate resolution on the ground of a given
ImageCoordinate.

This finds the intersection with the reference surface for the given
pixel, + 1 in the line and sample direction. We find the difference in
meters between these points, and select the maximum value. ";

%feature("docstring")  GeoCal::ImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter() const
Variation of resolution_meter that find the resolution of the center
pixel. ";

%feature("docstring")  GeoCal::ImageGroundConnection::title "
virtual std::string GeoCal::ImageGroundConnection::title() const
Title that we can use to describe the image.

This can be any string that is useful as a label. ";

%feature("docstring")  GeoCal::ImageGroundConnection::title "
virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
Set image title. ";

%feature("docstring")  GeoCal::ImageGroundConnection::~ImageGroundConnection "
virtual GeoCal::ImageGroundConnection::~ImageGroundConnection()
Destructor. ";


// File: a00091.xml

%feature("docstring") GeoCal::ImageGroundConnectionFailed "
Exception thrown if ImageGroundConnection fails to calculate a image
coordinate.

C++ includes: image_ground_connection.h ";

%feature("docstring")  GeoCal::ImageGroundConnectionFailed::ImageGroundConnectionFailed "
GeoCal::ImageGroundConnectionFailed::ImageGroundConnectionFailed(const std::string &W=\"\")
Default constructor.

Can give an optional string describing the error. ";

%feature("docstring")  GeoCal::ImageGroundConnectionFailed::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::ImageGroundConnectionFailed::~ImageGroundConnectionFailed "
virtual GeoCal::ImageGroundConnectionFailed::~ImageGroundConnectionFailed()
 throw ()
Destructor.
";

// File: a00092.xml

%feature("docstring") GeoCal::ImageMatcher "
This class performs image matching, finding the location of a point in
a reference image in a new image.

C++ includes: image_matcher.h ";

%feature("docstring")  GeoCal::ImageMatcher::match "
virtual void GeoCal::ImageMatcher::match(const RasterImage &Ref, const RasterImage &New, const ImageCoordinate
&Ref_loc, const ImageCoordinate &New_guess, ImageCoordinate &New_res,
double &Line_sigma, double &Sample_sigma, bool &Success, int
*Diagnostic=0) const =0
Match a point found in the reference image with a point in the new
image.

A initial guess in the new image is supplied. We return an improved
image in the new image along with the uncertainty if Success is true,
otherwise the match failed.

The Diagnostic argument is used to indicate why a match failed. For
many purposes you can ignore this, because you need to know detailed
information about the specific ImageMatcher that a user passed to a
function and what the diagnostic codes mean. However, for some
purposes the failure reason can be useful (e.g., tuning parameters of
a LsmMatcher). ";

%feature("docstring")  GeoCal::ImageMatcher::print "
virtual void GeoCal::ImageMatcher::print(std::ostream &Os) const =0
Print to stream. ";

%feature("docstring")  GeoCal::ImageMatcher::print_to_string "
std::string GeoCal::Printable< ImageMatcher  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ImageMatcher::~ImageMatcher "
virtual GeoCal::ImageMatcher::~ImageMatcher()
Destructor. ";


// File: a00093.xml

%feature("docstring") GeoCal::ImagePointDisplay "
This class is used to generate a display of image points.

This is useful for example to show the difference between initial and
final location during image matching

C++ includes: image_point_display.h ";

%feature("docstring")  GeoCal::ImagePointDisplay::band_id "
int GeoCal::GdalRasterImage::band_id() const
";
%feature("docstring")  GeoCal::ImagePointDisplay::close "
void GeoCal::GdalRasterImage::close()
";
%feature("docstring")  GeoCal::ImagePointDisplay::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::ImagePointDisplay::data_set "
const boost::shared_ptr<GDALDataset>& GeoCal::GdalRasterImage::data_set() const
The underlying dataset object. ";

%feature("docstring")  GeoCal::ImagePointDisplay::file_names "
std::vector<std::string> GeoCal::GdalRasterImage::file_names() const
";
%feature("docstring")  GeoCal::ImagePointDisplay::flush "
virtual void GeoCal::GdalRasterImage::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::ImagePointDisplay::gdal_data_base "
const GdalBase& GeoCal::GdalRasterImage::gdal_data_base() const
Underlying data. ";

%feature("docstring")  GeoCal::ImagePointDisplay::gdal_data_base "
GdalBase& GeoCal::GdalRasterImage::gdal_data_base()
Underlying data. ";

%feature("docstring")  GeoCal::ImagePointDisplay::gdal_data_base_ptr "
boost::shared_ptr<GdalBase> GeoCal::GdalRasterImage::gdal_data_base_ptr() const
";
%feature("docstring")  GeoCal::ImagePointDisplay::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::ImagePointDisplay::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::ImagePointDisplay::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ImagePointDisplay::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ImagePointDisplay::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ImagePointDisplay::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ImagePointDisplay::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::ImagePointDisplay::has_metadata "
bool GeoCal::GdalRasterImage::has_metadata(const std::string &Metadata_item, const std::string &Domain=\"\")
const
Indicates if a metadata item is found in the file. ";

%feature("docstring")  GeoCal::ImagePointDisplay::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::ImagePointDisplay::image_point "
void ImagePointDisplay::image_point(const ImageCoordinate &Ic, int Cindex)
Display Point with the given color index. ";

%feature("docstring")  GeoCal::ImagePointDisplay::image_point "
void GeoCal::ImagePointDisplay::image_point(const GroundCoordinate &Gc, int Cindex)
Display Point with the given color index. ";

%feature("docstring")  GeoCal::ImagePointDisplay::ImagePointDisplay "
GeoCal::ImagePointDisplay::ImagePointDisplay(const boost::shared_ptr< GDALDataset > &Data_set, int Band_id=1, int
Number_tile=4)
Constructor to write a new file. ";

%feature("docstring")  GeoCal::ImagePointDisplay::ImagePointDisplay "
GeoCal::ImagePointDisplay::ImagePointDisplay(const std::string &Fname, const std::string &Driver_name, const
MapInfo &M, int Dt, const std::string &Options=\"\", int
Number_tile=4, int Tile_number_line=-1, int Tile_number_sample=-1)
Constructor to write a new file. ";

%feature("docstring")  GeoCal::ImagePointDisplay::ImagePointDisplay "
GeoCal::ImagePointDisplay::ImagePointDisplay(const std::string &Fname, const std::string &Driver_name, int
Number_line, int Number_sample, int Dt, const std::string
&Options=\"\", int Number_tile=4, int Tile_number_line=-1, int
Tile_number_sample=-1)
Constructor to write a new file. ";

%feature("docstring")  GeoCal::ImagePointDisplay::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::ImagePointDisplay::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::ImagePointDisplay::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::ImagePointDisplay::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::ImagePointDisplay::is_closed "
bool GeoCal::GdalRasterImage::is_closed() const
";
%feature("docstring")  GeoCal::ImagePointDisplay::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::map_info_from_nitf_corner "
boost::shared_ptr< MapInfo > GdalRasterImage::map_info_from_nitf_corner() const
In AFIDS, we calculate what is called the \"nitf corners\".

This gives approximate corner coordinates for an image. Depending on
the application, it can be useful to treat this as an approximate
MapInfo.

This return a null pointer if we don't have the NITF corner metadata.
";

%feature("docstring")  GeoCal::ImagePointDisplay::metadata "
S GeoCal::GdalRasterImage::metadata(const std::string &Metadata_item, const std::string &Domain=\"\")
const
Read a metadata item and cast it to the given type. ";

%feature("docstring")  GeoCal::ImagePointDisplay::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::number_swap "
unsigned int GeoCal::RasterImageTiledFile::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::ImagePointDisplay::number_tile "
int GeoCal::RasterImageTiledFile::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::ImagePointDisplay::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::overview "
boost::shared_ptr< RasterImage > GdalRasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::ImagePointDisplay::print "
virtual void GeoCal::GdalRasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ImagePointDisplay::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ImagePointDisplay::raster_band "
GDALRasterBand& GeoCal::GdalRasterImage::raster_band()
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::ImagePointDisplay::raster_band "
const GDALRasterBand& GeoCal::GdalRasterImage::raster_band() const
The underlying GDALRasterBand object. ";

%feature("docstring")  GeoCal::ImagePointDisplay::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::ImagePointDisplay::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::read_double "
virtual blitz::Array<double, 2> GeoCal::RasterImageTiledFile::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::ImagePointDisplay::read_ptr "
virtual void GeoCal::RasterImageTiledFile::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::ImagePointDisplay::reset_number_swap "
void GeoCal::RasterImageTiledFile::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::ImagePointDisplay::rpc "
Rpc GeoCal::GdalRasterImage::rpc() const
Return Rpc for image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::set_map_info "
void GdalRasterImage::set_map_info(const MapInfo &Mi)
Set the map info. This overwrites whatever was previously set. ";

%feature("docstring")  GeoCal::ImagePointDisplay::set_metadata "
void GeoCal::GdalRasterImage::set_metadata(const std::string &M, const std::string &Val, const std::string
&Domain=\"\")
Write the requested metadata. ";

%feature("docstring")  GeoCal::ImagePointDisplay::set_rpc "
void GdalRasterImage::set_rpc(const Rpc &R)
Set the RPC info. This overwrites whatever was previously set. ";

%feature("docstring")  GeoCal::ImagePointDisplay::tile_file_base "
const TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base() const
Underlying data. ";

%feature("docstring")  GeoCal::ImagePointDisplay::tile_file_base "
TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base()
Underlying data. ";

%feature("docstring")  GeoCal::ImagePointDisplay::tile_file_base_ptr "
const boost::shared_ptr<TiledFileBase<2> >& GeoCal::RasterImageTiledFile::tile_file_base_ptr() const
Underlying data. ";

%feature("docstring")  GeoCal::ImagePointDisplay::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::ImagePointDisplay::unchecked_read "
virtual int GeoCal::RasterImageTiledFile::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::ImagePointDisplay::unchecked_write "
virtual void GeoCal::RasterImageTiledFile::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::ImagePointDisplay::update "
bool GeoCal::GdalRasterImage::update() const
";
%feature("docstring")  GeoCal::ImagePointDisplay::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::ImagePointDisplay::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::ImagePointDisplay::write_ptr "
virtual void GeoCal::RasterImageTiledFile::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the image. ";

%feature("docstring")  GeoCal::ImagePointDisplay::~ImagePointDisplay "
virtual GeoCal::ImagePointDisplay::~ImagePointDisplay()
Destructor. ";

%feature("docstring")  GeoCal::ImagePointDisplay::read_all "
std::vector< boost::shared_ptr< RasterImage > > GdalRasterImage::read_all(const std::string &Fname)
For some formats, reading all of the data for all the bands at one
time is much quicker than reading each band separately (assuming you
actually want all the data).

For example, for jpeg reading three separate bands requires
decompressing the data three times while reading all at once only
decompresses once. Since almost the entire time reading jpeg is used
in decompressing the data this means it takes about three times as
long to read all the bands as to read them all at once.

This function reads all of the data into memory and returns
RasterImages that can be used to read the data. ";

%feature("docstring")  GeoCal::ImagePointDisplay::save "
static void GeoCal::GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
RasterImage &Mi, int Dt=GDT_Byte, const std::string &Option=\"\", bool
Require_copy=false, int Fill_value=-1000)
Copy the given RasterImage into a new file, using the given driver and
options.

Some drivers only allow \"CreateCopy\" rather than \"Create\",
requiring us to use an intermediate memory version of Gdal. In that
case, set \"require_copy\" to true. ";

%feature("docstring")  GeoCal::ImagePointDisplay::save "
static void GeoCal::GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
RasterImage &Mi_r, const RasterImage &Mi_g, const RasterImage &Mi_b,
int Dt=GDT_Byte, const std::string &Option=\"\", bool
Require_copy=false, int Fill_value=-1000)
Copy the given RasterImages into a new file, using the given driver
and options.

Some drivers only allow \"CreateCopy\" rather than \"Create\",
requiring us to use an intermediate memory version of Gdal. In that
case, set \"require_copy\" to true. ";

%feature("docstring")  GeoCal::ImagePointDisplay::save "
void GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
std::vector< const RasterImage * > &d, int Dt=GDT_Byte, const
std::string &Option=\"\", bool Require_copy=false, int
Fill_value=-1000)
Copy the given RasterImages into a new file, using the given driver
and options.

Some drivers only allow \"CreateCopy\" rather than \"Create\",
requiring us to use an intermediate memory version of Gdal. In that
case, set \"require_copy\" to true. ";

%feature("docstring")  GeoCal::ImagePointDisplay::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImage &Mi, int Dt=GDT_Byte, int
Fill_value=0)
Copy the given RasterImage into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";

%feature("docstring")  GeoCal::ImagePointDisplay::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImage &Mi_r, const RasterImage
&Mi_g, const RasterImage &Mi_b, int Dt=GDT_Byte, int Fill_value=0)
Copy the given RasterImages into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";

%feature("docstring")  GeoCal::ImagePointDisplay::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImage &Mi_r, const RasterImage
&Mi_g, const RasterImage &Mi_b, const RasterImage &Mi_ir, int
Dt=GDT_Byte, int Fill_value=0)
Copy the given RasterImages into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";

%feature("docstring")  GeoCal::ImagePointDisplay::save_to_erdas "
static void GeoCal::GdalRasterImage::save_to_erdas(const std::string &Oname, const std::vector< boost::shared_ptr<
GeoCal::RasterImage > > &d, int Dt=GDT_Byte, int Fill_value=0)
";
%feature("docstring")  GeoCal::ImagePointDisplay::save_to_erdas "
void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImageMultiBand &d, int
Dt=GDT_Byte, int Fill_value=0)
Copy the given RasterImages into a new Erdas file.

This calculates the statistics and image pyramid needed by Erdas also.
The given Fill_value is excluded from the statistics calculation. ";


// File: a00094.xml

%feature("docstring") GeoCal::ImageToImageMatch "
This class find matches between two images.

This is similar to the class ImageMatcher, but in addition to doing
the matching it also keeps track of which two images we are matching,
and how we do the initial guess for the location in the second image.

C++ includes: image_to_image_match.h ";

%feature("docstring")  GeoCal::ImageToImageMatch::match "
virtual void GeoCal::ImageToImageMatch::match(const ImageCoordinate &Ic1, ImageCoordinate &Ic2, double &Line_sigma,
double &Sample_sigma, bool &Success, int *Diagnostic=0) const =0
Match point Ic1 in the first image with the second image.

We return the location in the second image along with the uncertainty
if Success is true, otherwise the match failed.

The Diagnostic argument is used to indicate why a match failed. For
many purposes you can ignore this, because you need to know detailed
information about the specific ImageMatcher that a user passed to a
function and what the diagnostic codes mean. However, for some
purposes the failure reason can be useful (e.g., tuning parameters of
a LsmMatcher). ";

%feature("docstring")  GeoCal::ImageToImageMatch::print "
virtual void GeoCal::ImageToImageMatch::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ImageToImageMatch::print_to_string "
std::string GeoCal::Printable< ImageToImageMatch  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ImageToImageMatch::~ImageToImageMatch "
virtual GeoCal::ImageToImageMatch::~ImageToImageMatch()
";

// File: a00095.xml

%feature("docstring") ImageVis "
";
%feature("docstring")  ImageVis::convertGrayToSingleColor "
cv::Mat ImageVis::convertGrayToSingleColor(cv::Mat &img, RGBColor colorType)
";
%feature("docstring")  ImageVis::displayAllMatchedPoints "
void ImageVis::displayAllMatchedPoints(cv::Mat &ref, cv::Mat &target, std::vector< cv::Point2f > &Ref_locs,
std::vector< cv::Point2f > &Results)
";
%feature("docstring")  ImageVis::displayCombinedWarped "
void ImageVis::displayCombinedWarped(cv::Mat ref, cv::Mat target, cv::Mat warped)
";
%feature("docstring")  ImageVis::displayEntireMatchedRegion "
void ImageVis::displayEntireMatchedRegion(cv::Mat &ref, std::vector< cv::Rect > &rects)
";
%feature("docstring")  ImageVis::displayResultsFromPoints "
void ImageVis::displayResultsFromPoints(cv::Mat &ref, std::vector< cv::Point2f > &Ref_locs, std::vector<
cv::Point2f > &Results)
";
%feature("docstring")  ImageVis::displaySingleMatch "
void ImageVis::displaySingleMatch(cv::Mat &ref, cv::Mat &target, cv::Point2f &ref_loc, cv::Point2f
&new_loc, bool printMatchingPoints=false)
";
%feature("docstring")  ImageVis::drawImBorder "
bool ImageVis::drawImBorder(cv::Mat &img, cv::Mat &img2, bool showBorder=true)
";
%feature("docstring")  ImageVis::drawImBoundingBox "
bool ImageVis::drawImBoundingBox(cv::Mat &img, cv::Rect &box, bool showBox=false)
";
%feature("docstring")  ImageVis::drawPoint "
void ImageVis::drawPoint(cv::Mat &ref, cv::Point2f &center, bool rgb=false)
";
%feature("docstring")  ImageVis::randomColor "
cv::Scalar ImageVis::randomColor(cv::RNG &rng)
";

// File: a00096.xml

%feature("docstring") GeoCal::InterestPoint "
This is an interest point, determined by a FeatureDetector.

C++ includes: feature_detector.h ";

%feature("docstring")  GeoCal::InterestPoint::InterestPoint "
GeoCal::InterestPoint::InterestPoint()
Default constructor. ";

%feature("docstring")  GeoCal::InterestPoint::InterestPoint "
GeoCal::InterestPoint::InterestPoint(const ImageCoordinate &Ic, double Weight)
Constructor. ";

%feature("docstring")  GeoCal::InterestPoint::print "
void InterestPoint::print(std::ostream &Os) const
Print InterestPoint to a stream. ";

%feature("docstring")  GeoCal::InterestPoint::print_to_string "
std::string GeoCal::Printable< InterestPoint  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";


// File: a01191.xml


// File: a01185.xml

%feature("docstring") std::invalid_argument "
STL class. ";


// File: a01135.xml

%feature("docstring") std::ios "
STL class. ";


// File: a01130.xml

%feature("docstring") std::ios_base "
STL class. ";


// File: a00097.xml

%feature("docstring") GeoCal::Ipi "
This performs an image point intersection (IPI), finding the time or
image coordinates that a given ground point is seen by a camera.

This solves the collinearity equation in order to find the time and
frame coordinates that a ground point is seen. Note however that the
collinearity equation can give \"false\" solutions, because it is
perfectly happy to find points seen by looking straight through the
earth (since it does not account for an obstructed view).

We also get \"false\" solutions if the camera model diverges (i.e, we
pass in a look vector parallel to the camera CCD plane, so the point
is not seen by the camera). This is because the root finder doesn't
actually look for a zero, but rather a change in the sign of the
equation. When the camera model diverges, we go from negative infinity
to positive infinity. We recognize these \"false\" solutions by
comparing the collinearity equation results to a user supplied
tolerance.

We find all the solutions to the collinearity equation, in the range
Tmin to Tmax. The solutions found must be seperated by a time larger
then Root_min_separation.

We then inspect the list of roots, and reject those that are \"false\"
solutions. If 1 solution is left, we return it with the success flag
set to true. Otherwise, we return with the success flag set to false.

It is very common to call the IPI with a ground point near the last
call. As a performance improvement, we remember the time results from
the previous IPI calculation. We first look in a local time range
around the previous time result. If no solution is found, we then look
over the entire time range.

C++ includes: ipi.h ";

%feature("docstring")  GeoCal::Ipi::band "
int GeoCal::Ipi::band() const
Band that we are using. ";

%feature("docstring")  GeoCal::Ipi::camera "
const PushBroomCamera& GeoCal::Ipi::camera() const
Camera that we are using. ";

%feature("docstring")  GeoCal::Ipi::camera_ptr "
boost::shared_ptr<PushBroomCamera> GeoCal::Ipi::camera_ptr() const
";
%feature("docstring")  GeoCal::Ipi::footprint "
std::vector< boost::shared_ptr< GroundCoordinate > > Ipi::footprint(const Dem &D) const
Return the footprint of the data on the ground.

This is just the footprint of the orbit data at the start and end of
the time range. ";

%feature("docstring")  GeoCal::Ipi::image_coordinate "
void Ipi::image_coordinate(const GroundCoordinate &Gp, ImageCoordinate &Ic, bool &Success) const
Look for the image location that a point is seen.

Note that unlike time, if a point is seen only outside of the sample
range of the image then we consider the Ipi a failure. If Success is
true, then Ic will have a line and sample number inside of the image
given by the Camera and TimeTable. ";

%feature("docstring")  GeoCal::Ipi::image_coordinate_extended "
void Ipi::image_coordinate_extended(const GroundCoordinate &Gp, ImageCoordinate &Ic, bool &Success) const
This is like image_coordinate, by we try to handle points out of the
time and sample range of the data (so this returns line and samples
outside of the range of the TimeTable and Camera).

In some cases this is more useful than having image_coordinate fail.
Note that we still might have Success false, if for example the point
is outside the range of the Orbit.

For points outside of the TimeTable range, we just extrapolate the
last two lines in the time table. ";

%feature("docstring")  GeoCal::Ipi::Ipi "
Ipi::Ipi(const boost::shared_ptr< Orbit > &Orb, const boost::shared_ptr<
PushBroomCamera > &Cam, int Band, Time Tmin, Time Tmax, const
boost::shared_ptr< TimeTable > &Tt=boost::shared_ptr< TimeTable >(),
double Local_time_window_size=5.0, double Root_min_separation=30.0,
double Time_tolerance=40.8e-3 *1.0/16)
Constructor.

If you only want to get the Time from the Ipi and not ImageCoordinate,
it is ok for Tt to be null.

We find solutions to the Collinearity equations over the full time
range Tmin to Tmax. However, as a performance enhancement we first
look around the given Local_time_window_size in seconds around the
solution from the last call to time() or image_coordinate(). We look
for all solutions that are Root_min_separation apart. We find time
with a accuracy given by Time_tolerance. ";

%feature("docstring")  GeoCal::Ipi::local_time_window_size "
double GeoCal::Ipi::local_time_window_size() const
How large of a window to search around last solution, in seconds. ";

%feature("docstring")  GeoCal::Ipi::max_time "
Time GeoCal::Ipi::max_time() const
Maximum time that we are using. ";

%feature("docstring")  GeoCal::Ipi::min_time "
Time GeoCal::Ipi::min_time() const
Minimum time that we are using. ";

%feature("docstring")  GeoCal::Ipi::orbit "
const Orbit& GeoCal::Ipi::orbit() const
Orbit that we are using. ";

%feature("docstring")  GeoCal::Ipi::orbit_ptr "
boost::shared_ptr<Orbit> GeoCal::Ipi::orbit_ptr() const
";
%feature("docstring")  GeoCal::Ipi::print "
void Ipi::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::Ipi::print_to_string "
std::string GeoCal::Printable< Ipi  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Ipi::resolution_meter "
double Ipi::resolution_meter() const
Give the approximate ground resolution in meters.

This is for the center pixel in the area covered by the Ipi. ";

%feature("docstring")  GeoCal::Ipi::root_min_separation "
double GeoCal::Ipi::root_min_separation() const
How far apart in seconds we look for solutions to the collinearity
equation. ";

%feature("docstring")  GeoCal::Ipi::time "
void Ipi::time(const GroundCoordinate &Gp, Time &Tres, FrameCoordinate &Fres, bool
&Success) const
Look for solution for when the GroundCoordinate Gp is seen.

If we don't find a solution, then Success will be false.

Note that we may return a solution where Fres has a sample number < 0
or > cam.number_sample(). This means that a point is not actually seen
in the image, but would be if we had an infinite extent camera. Often
this is useful information, however depending on the application you
may want to reject points with Fres outside of the image range.

If it is more useful to you, image_coordinate has the opposite
behavior, it rejects any points that are outside of the actual image.
";

%feature("docstring")  GeoCal::Ipi::time_table "
const TimeTable& GeoCal::Ipi::time_table() const
TimeTable that we are using. ";

%feature("docstring")  GeoCal::Ipi::time_table_ptr "
boost::shared_ptr<TimeTable> GeoCal::Ipi::time_table_ptr() const
";
%feature("docstring")  GeoCal::Ipi::time_tolerance "
double GeoCal::Ipi::time_tolerance() const
How accurate we find the time. ";

%feature("docstring")  GeoCal::Ipi::~Ipi "
virtual GeoCal::Ipi::~Ipi()
";

// File: a00098.xml

%feature("docstring") GeoCal::IpiImageGroundConnection "
This is a ImageGroundConnection where the connection is made by a Ipi.

C++ includes: ipi_image_ground_connection.h ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::band "
int GeoCal::IpiImageGroundConnection::band() const
Band we are working with. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::cf_look_vector "
virtual void GeoCal::IpiImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
boost::shared_ptr< CartesianFixed > &P) const
Return look vector for given coordinate, along with a position that
lies along the direction of the look vector (so position of satellite,
or a position on the surface. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::cover "
MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
Find a MapInfo that covers the ground coordinate of this
ImageGroundConnection.

We calculate the ground coordinate of the four corners, then find the
MapInfo that covers those corners, optionally adding a boundary in map
pixels. Depending on the actual warping involved, this may or may not
fully cover the edges of the image. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::dem "
void GeoCal::ImageGroundConnection::dem(const boost::shared_ptr< Dem > &D)
Set the DEM to use in ground_coordinate. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::dem "
const Dem& GeoCal::ImageGroundConnection::dem() const
Dem used by ground_coordinate. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::dem_ptr "
const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
DEM used by ground_coordinate. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::ground_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::IpiImageGroundConnection::ground_coordinate_dem "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::IpiImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const
Return ground coordinate that goes with a particular image coordinate.

This version supplies a Dem to use. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::image "
virtual boost::shared_ptr<RasterImage> GeoCal::ImageGroundConnection::image() const
Underlying image. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::image_coordinate "
virtual ImageCoordinate GeoCal::IpiImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const
Return image coordinate that goes with a particular GroundCoordinate.

For some types of ImageGroundConnection, we might not be able to
calculate image_coordinate for all values (e.g., Ipi might fail). In
those cases, we will through a ImageGroundConnectionFailed exception.
This means that nothing is wrong, other than that we can't calculate
the image_coordinate. Callers can catch this exception if they have
some way of handling no image coordinate data. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::image_coordinate_jac_ecr "
blitz::Array< double, 2 > ImageGroundConnection::image_coordinate_jac_ecr(const Ecr &Gc) const
Return the Jacobian of the image coordinates with respect to the X, Y,
and Z components of the Ecr ground location. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::image_coordinate_jac_parm "
virtual blitz::Array<double, 2> GeoCal::ImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Return the Jacobian of the image coordinates with respect to the
parameters. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::ipi "
const Ipi& GeoCal::IpiImageGroundConnection::ipi() const
";
%feature("docstring")  GeoCal::IpiImageGroundConnection::ipi_ptr "
const boost::shared_ptr<Ipi>& GeoCal::IpiImageGroundConnection::ipi_ptr() const
IPI that we are using. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::IpiImageGroundConnection "
GeoCal::IpiImageGroundConnection::IpiImageGroundConnection(const boost::shared_ptr< Ipi > &I, const boost::shared_ptr< Dem > &D,
const boost::shared_ptr< RasterImage > &Img, const std::string
&Title=\"Image\", double Resolution=30, int Band=0, double
Max_height=9000)
";
%feature("docstring")  GeoCal::IpiImageGroundConnection::maximum_height "
double GeoCal::IpiImageGroundConnection::maximum_height() const
Maximum height we look through Dem at. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::number_line "
virtual int GeoCal::IpiImageGroundConnection::number_line() const
Return number of lines in underlying image. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::number_sample "
virtual int GeoCal::IpiImageGroundConnection::number_sample() const
Return number of samples in underlying image. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::parameter "
virtual blitz::Array<double, 1> GeoCal::ImageGroundConnection::parameter() const
A image to ground connection may depend on a set of parameters, which
can by modified (e.g., during a simultaneous bundle adjustment).

This returns those parameters. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::parameter "
virtual void GeoCal::ImageGroundConnection::parameter(const blitz::Array< double, 1 > &Parm)
Set the value of the parameters. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::parameter_name "
virtual std::vector<std::string> GeoCal::ImageGroundConnection::parameter_name() const
Descriptive name of each parameter. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::print "
virtual void GeoCal::IpiImageGroundConnection::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::resolution "
double GeoCal::IpiImageGroundConnection::resolution() const
Resolution we step through Dem at, in meters. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter(const ImageCoordinate &Ic) const
Calculate the approximate resolution on the ground of a given
ImageCoordinate.

This finds the intersection with the reference surface for the given
pixel, + 1 in the line and sample direction. We find the difference in
meters between these points, and select the maximum value. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter() const
Variation of resolution_meter that find the resolution of the center
pixel. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::title "
virtual std::string GeoCal::ImageGroundConnection::title() const
Title that we can use to describe the image.

This can be any string that is useful as a label. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::title "
virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
Set image title. ";

%feature("docstring")  GeoCal::IpiImageGroundConnection::~IpiImageGroundConnection "
virtual GeoCal::IpiImageGroundConnection::~IpiImageGroundConnection()
Destructor. ";


// File: a00099.xml

%feature("docstring") GeoCal::IpiMapProjected "
This is a CalcMapProjected where the calculation of the image
coordinates is done by an Ipi.

This class calculates the data on the fly. Sometimes this is what you
want, but if you are going to be using the resulting data a few times,
you may want to use a MemoryRasterImage to generate a copy once and
keep it in memory.

C++ includes: ipi_map_projected.h ";

%feature("docstring")  GeoCal::IpiMapProjected::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::IpiMapProjected::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::IpiMapProjected::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::IpiMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IpiMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IpiMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IpiMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::IpiMapProjected::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::IpiMapProjected::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::IpiMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::IpiMapProjected::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::IpiMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::IpiMapProjected::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::IpiMapProjected::IpiMapProjected "
IpiMapProjected::IpiMapProjected(const MapInfo &Mi, const boost::shared_ptr< Ipi > &I, const
boost::shared_ptr< RasterImage > &R, const boost::shared_ptr< Dem >
&D, int Avg_fact=-1, bool Read_into_memory=true)
Constructor.

We average the data either by the factor given as Avg_fact, or by
ratio of the Mapinfo resolution and the camera resolution. ";

%feature("docstring")  GeoCal::IpiMapProjected::IpiMapProjected "
IpiMapProjected::IpiMapProjected(const MapInfo &Mi, const boost::shared_ptr< Ipi > &I, const
std::vector< boost::shared_ptr< RasterImage > > &R, const
boost::shared_ptr< Dem > &D, int Avg_fact=-1, bool
Read_into_memory=true)
Constructor.

We average the data either by the factor given as Avg_fact, or by
ratio of the Mapinfo resolution and the camera resolution.

This is a variation that handles multiple bands at one time, you give
it the set of raster image to process. ";

%feature("docstring")  GeoCal::IpiMapProjected::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::IpiMapProjected::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::IpiMapProjected::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::IpiMapProjected::number_tile_line "
virtual int GeoCal::CalcMapProjected::number_tile_line() const
Number of lines in each tile. ";

%feature("docstring")  GeoCal::IpiMapProjected::number_tile_sample "
virtual int GeoCal::CalcMapProjected::number_tile_sample() const
Number of samples in each tile. ";

%feature("docstring")  GeoCal::IpiMapProjected::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::IpiMapProjected::print "
void IpiMapProjected::print(std::ostream &Os) const
Print out a description of the object. ";

%feature("docstring")  GeoCal::IpiMapProjected::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::IpiMapProjected::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::IpiMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IpiMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IpiMapProjected::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IpiMapProjected::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::IpiMapProjected::read_ptr "
void CalcMapProjected::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::IpiMapProjected::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::IpiMapProjected::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::IpiMapProjected::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::IpiMapProjected::unchecked_read "
int CalcMapProjected::unchecked_read(int Line, int Sample) const
Read pixel value at given line and sample. ";

%feature("docstring")  GeoCal::IpiMapProjected::unchecked_write "
void CalcMapProjected::unchecked_write(int Line, int Sample, int Val)
Write a value.

This isn't actually implemented for this type of RasterImage. ";

%feature("docstring")  GeoCal::IpiMapProjected::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::IpiMapProjected::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::IpiMapProjected::write_image "
void CalcMapProjected::write_image(RasterImage &Out, int Grid_spacing=1) const
input exactly at this grid spacing, and interpolate in between.

This is much faster than calculating ever point, and if the grid
spacing is small compared to the Dem and any nonlinearities give
results very close to the full calculation. ";

%feature("docstring")  GeoCal::IpiMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out) const
Write output to multiple RasterImage at once.

The number should be <= the number given to the constructor. ";

%feature("docstring")  GeoCal::IpiMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out, int
Grid_spacing) const
Write output to multiple RasterImage at once.

This variation takes a grid spacing to use. We calculate the image
coordinates in the input exactly at this grid spacing, and interpolate
in between. This is much faster than calculating ever point, and if
the grid spacing is small compared to the Dem and any Camera
nonlinearities give results very close to the full calculation. ";

%feature("docstring")  GeoCal::IpiMapProjected::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::IpiMapProjected::~IpiMapProjected "
virtual GeoCal::IpiMapProjected::~IpiMapProjected()
Destructor. ";


// File: a01143.xml

%feature("docstring") std::istream "
STL class. ";


// File: a01141.xml

%feature("docstring") std::istringstream "
STL class. ";


// File: a01192.xml

%feature("docstring") std::multimap::iterator "
STL iterator class. ";


// File: a01193.xml

%feature("docstring") std::set::iterator "
STL iterator class. ";


// File: a01156.xml

%feature("docstring") std::basic_string::iterator "
STL iterator class. ";


// File: a01194.xml

%feature("docstring") std::multiset::iterator "
STL iterator class. ";


// File: a01195.xml

%feature("docstring") std::vector::iterator "
STL iterator class. ";


// File: a01196.xml

%feature("docstring") std::string::iterator "
STL iterator class. ";


// File: a01197.xml

%feature("docstring") std::wstring::iterator "
STL iterator class. ";


// File: a01179.xml

%feature("docstring") std::deque::iterator "
STL iterator class. ";


// File: a01198.xml

%feature("docstring") std::list::iterator "
STL iterator class. ";


// File: a01199.xml

%feature("docstring") std::map::iterator "
STL iterator class. ";


// File: a00100.xml

%feature("docstring") KeplerEquation "
Kepler's equation, see Goldstein \"Classical Mechanics 2nd Edition\"
for details. ";

%feature("docstring")  KeplerEquation::KeplerEquation "
KeplerEquation::KeplerEquation(double wt, double e)
";
%feature("docstring")  KeplerEquation::~KeplerEquation "
virtual KeplerEquation::~KeplerEquation()
";

// File: a00101.xml

%feature("docstring") GeoCal::KeplerOrbit "
This is a simple implementation of an Orbit.

It just uses Kepler's equations. This is intended primarily for easy
testing of other classes that need an Orbit, rather than being a
realistic orbit simulation for real use.

The default parameters are for a nominal MISR orbit.

C++ includes: orbit.h ";

%feature("docstring")  GeoCal::KeplerOrbit::argument_of_perigee "
double GeoCal::KeplerOrbit::argument_of_perigee() const
Argument of perigee at epoch, in degrees. ";

%feature("docstring")  GeoCal::KeplerOrbit::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::KeplerOrbit::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::KeplerOrbit::eccentricity "
double GeoCal::KeplerOrbit::eccentricity() const
Eccentricity of orbit. ";

%feature("docstring")  GeoCal::KeplerOrbit::epoch "
const Time& GeoCal::KeplerOrbit::epoch() const
Epoch that rest of data is for. ";

%feature("docstring")  GeoCal::KeplerOrbit::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::KeplerOrbit::inclination "
double GeoCal::KeplerOrbit::inclination() const
Inclination of orbit, in degrees. ";

%feature("docstring")  GeoCal::KeplerOrbit::KeplerOrbit "
KeplerOrbit::KeplerOrbit(Time Min_time=Time::min_valid_time, Time
Max_time=Time::max_valid_time, Time
Epoch=Time::time_pgs(173357492.32), double Semimajor_axis=7086930,
double Eccentricity=0.001281620, double Inclination=98.199990, double
Ra_ascending_node=255.355971130, double Ap_at_epoch=69.086962170,
double Mean_anomaly_at_epoch=290.912925280)
Create a Kepler orbit with the given elements, valid over the given
time range.

Distances are in meters and angles are in degrees. The Epoch gives the
Time that the rest of the data is valid for. The default values are a
nominal orbit for MISR. ";

%feature("docstring")  GeoCal::KeplerOrbit::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::KeplerOrbit::mean_anomoly "
double GeoCal::KeplerOrbit::mean_anomoly() const
Mean anomoly at epoch, in degrees. ";

%feature("docstring")  GeoCal::KeplerOrbit::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::KeplerOrbit::orbit_data "
boost::shared_ptr< OrbitData > KeplerOrbit::orbit_data(Time T) const
Return OrbitData for the given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::KeplerOrbit::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::KeplerOrbit::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::KeplerOrbit::print "
void KeplerOrbit::print(std::ostream &Os) const
Print out description of Orbit. ";

%feature("docstring")  GeoCal::KeplerOrbit::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::KeplerOrbit::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::KeplerOrbit::right_ascension "
double GeoCal::KeplerOrbit::right_ascension() const
Right ascension of ascending node, in degrees. ";

%feature("docstring")  GeoCal::KeplerOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::KeplerOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::KeplerOrbit::semimajor_axis "
double GeoCal::KeplerOrbit::semimajor_axis() const
Semimajor axis in meters. ";

%feature("docstring")  GeoCal::KeplerOrbit::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";

%feature("docstring")  GeoCal::KeplerOrbit::~KeplerOrbit "
virtual GeoCal::KeplerOrbit::~KeplerOrbit()
";

// File: a01186.xml

%feature("docstring") std::length_error "
STL class. ";


// File: a01200.xml

%feature("docstring") std::list "
STL class. ";


// File: a00102.xml

%feature("docstring") GeoCal::LocationToFile "
A common implementation of MapProjectedMultifile is to have some sort
of a look up table that gives the file name for a particular location.

This class is a thin wrapper around RTree to give a mapping between
location and file name. We do this both because RTree's interface is a
bit different than how we want to access this, and because we want to
abstract out the use of the libsdbx library since this may go away at
some point.

C++ includes: location_to_file.h ";

%feature("docstring")  GeoCal::LocationToFile::add "
void GeoCal::LocationToFile::add(int x1, int y1, int x2, int y2, const std::string &fname)
Add a file with the given range.

The range is in x1 <= x < x2, y1 <= y < y. ";

%feature("docstring")  GeoCal::LocationToFile::find "
std::string GeoCal::LocationToFile::find(int x, int y) const
Find the filename that covers a given point.

If multiple files cover a point, this just returns one of them. If no
filenames cover the point, then we return the empty string \"\". ";

%feature("docstring")  GeoCal::LocationToFile::LocationToFile "
GeoCal::LocationToFile::LocationToFile()
Constructor. ";

%feature("docstring")  GeoCal::LocationToFile::print "
void GeoCal::LocationToFile::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::LocationToFile::print_to_string "
std::string GeoCal::Printable< LocationToFile  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";


// File: a01182.xml

%feature("docstring") std::logic_error "
STL class. ";


// File: a00103.xml

%feature("docstring") GeoCal::LookVector "
This is a look vector in an unspecified coordinate system.

Derived classes specify the coordinate system.

C++ includes: look_vector.h ";

%feature("docstring")  GeoCal::LookVector::direction "
boost::array< double, 3 > LookVector::direction() const
This is the direction, as a unit vector. ";

%feature("docstring")  GeoCal::LookVector::length "
double LookVector::length() const
";
%feature("docstring")  GeoCal::LookVector::look_quaternion "
boost::math::quaternion<double> GeoCal::LookVector::look_quaternion() const
Look vector as a quaternion. ";

%feature("docstring")  GeoCal::LookVector::look_quaternion "
void GeoCal::LookVector::look_quaternion(const boost::math::quaternion< double > &V)
Set look vector using a quaternion. ";

%feature("docstring")  GeoCal::LookVector::print "
virtual void GeoCal::LookVector::print(std::ostream &Os) const =0
";
%feature("docstring")  GeoCal::LookVector::print_to_string "
std::string GeoCal::Printable< LookVector  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::LookVector::~LookVector "
virtual GeoCal::LookVector::~LookVector()
";

// File: a00104.xml

%feature("docstring") GeoCal::LsmMatcher "
This class performs image matching.

This does a nonlinear least squares match. It also calculates an
estimate of the error in its matching.

We attempt to reach a goal of precision_goal in determining a0 and b0.
If at any iteration, the uncertainty in determining a0 and b0 is
greater than max_sigma, then we give up on doing LSM. If the final
uncertainty in determining a0 and b0 is larger than
precision_requirement, then we give up on doing LSM. We now also check
radiometric uncertainty at each iteration as described in MISR SDS
DFM-0245-I. The maximum allowed radiometric uncertainty is calculated
by multiplying the factor radiometric_uncertainty_factor by the
minimum of the template and target window sigmas. After each iteration
in the lsm the uncertainty in the calculation of h0 is greater than
this threshold we stop doing LSM and return unsuccessful.

The minimum uncertainty that will be returned is precision_goal (i.e.,
if the estimated uncertainty is smaller than this value, then
precision_goal is returned instead).

The model used is to resample the target using an affine
transformation combined with a linear correction to the radiometry:

g'(i, j) = h0 + h1 * g(a0 + a1 * i + a2 * j, b0 + b1 * i + b2 * j)

Where g is the target, g' is the template.

As a convention, internal to this class we index things so g'(0, 0) is
the center of the template.

We solve for the parameters giving the best fit against the template.

C++ includes: lsm_matcher.h ";

%feature("docstring")  GeoCal::LsmMatcher::border_size "
int GeoCal::LsmMatcher::border_size() const
Border added to target to allow for template/target shift. ";

%feature("docstring")  GeoCal::LsmMatcher::LsmMatcher "
BZ_END_STENCIL LsmMatcher::LsmMatcher(int Number_line=21, int Number_sample=21, int Border_size=3, double
Precision_goal=0.0625, double Precision_requirement=0.2, double
Max_sigma=0.5, double Rad_uncertainty_factor=2.0, double
Precision_min_geo_goal=0.15, double Precision_min_rad_goal=1)
Constructor.

Default values were tuned for MISR imagery, but work well for other
imagery. ";

%feature("docstring")  GeoCal::LsmMatcher::LsmMatcher "
LsmMatcher::LsmMatcher(const LsmMatcher &M)
Copy constructor. ";

%feature("docstring")  GeoCal::LsmMatcher::match "
void LsmMatcher::match(const RasterImage &Ref, const RasterImage &New, const ImageCoordinate
&Ref_loc, const ImageCoordinate &New_guess, ImageCoordinate &New_res,
double &Line_sigma, double &Sample_sigma, bool &Success, int
*Diagnostic=0) const
Match a point found in the reference image with a point in the new
image.

A initial guess in the new image is supplied. We return an improved
image in the new image along with the uncertainty if Success is true,
otherwise the match failed.

The Diagnostic argument is used to indicate why a match failed. For
many purposes you can ignore this, because you need to know detailed
information about the specific ImageMatcher that a user passed to a
function and what the diagnostic codes mean. However, for some
purposes the failure reason can be useful (e.g., tuning parameters of
a LsmMatcher). ";

%feature("docstring")  GeoCal::LsmMatcher::max_sigma "
double GeoCal::LsmMatcher::max_sigma() const
If at any stagee in solving LSM uncertainty is greater than this
value, then give up doing image matching. ";

%feature("docstring")  GeoCal::LsmMatcher::number_line "
int GeoCal::LsmMatcher::number_line() const
Number of lines in target and template. ";

%feature("docstring")  GeoCal::LsmMatcher::number_sample "
int GeoCal::LsmMatcher::number_sample() const
Number of samples in target and template. ";

%feature("docstring")  GeoCal::LsmMatcher::precision_goal "
double GeoCal::LsmMatcher::precision_goal() const
Goal in doing solving LSM. ";

%feature("docstring")  GeoCal::LsmMatcher::precision_min_geo_goal "
double GeoCal::LsmMatcher::precision_min_geo_goal() const
Minimum geometric goal for LSM to success. ";

%feature("docstring")  GeoCal::LsmMatcher::precision_min_rad_goal "
double GeoCal::LsmMatcher::precision_min_rad_goal() const
Minimum radiometric goal for LSM to success. ";

%feature("docstring")  GeoCal::LsmMatcher::precision_requirement "
double GeoCal::LsmMatcher::precision_requirement() const
Requirement for LSM success. ";

%feature("docstring")  GeoCal::LsmMatcher::print "
void LsmMatcher::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::LsmMatcher::print_to_string "
std::string GeoCal::Printable< ImageMatcher  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::LsmMatcher::rad_uncertainty_factor "
double GeoCal::LsmMatcher::rad_uncertainty_factor() const
This factor is used to scale the maximum allowable radiometric
uncertainty while solving LSM. ";

%feature("docstring")  GeoCal::LsmMatcher::~LsmMatcher "
virtual GeoCal::LsmMatcher::~LsmMatcher()
Destructor. ";


// File: a00105.xml

%feature("docstring") GeoCal::MagnifyBilinear "
This creates a magnified image of a RasterImage.

We do a bilinear interpolation to get the values

C++ includes: magnify_bilinear.h ";

%feature("docstring")  GeoCal::MagnifyBilinear::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinear::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::MagnifyBilinear::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::MagnifyBilinear::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinear::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinear::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinear::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinear::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::MagnifyBilinear::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::MagnifyBilinear::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MagnifyBilinear::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::MagnifyBilinear::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MagnifyBilinear::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::MagnifyBilinear::MagnifyBilinear "
MagnifyBilinear::MagnifyBilinear(const boost::shared_ptr< RasterImage > &Data, int Magfactor, int
Number_tile=4)
Constructor. ";

%feature("docstring")  GeoCal::MagnifyBilinear::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::MagnifyBilinear::print "
virtual void GeoCal::MagnifyBilinear::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MagnifyBilinear::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MagnifyBilinear::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::MagnifyBilinear::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::MagnifyBilinear::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::MagnifyBilinear::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::MagnifyBilinear::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::MagnifyBilinear::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MagnifyBilinear::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MagnifyBilinear::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::MagnifyBilinear::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::MagnifyBilinear::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::MagnifyBilinear::~MagnifyBilinear "
virtual GeoCal::MagnifyBilinear::~MagnifyBilinear()
";

// File: a00106.xml

%feature("docstring") GeoCal::MagnifyBilinearImageGroundConnection "
This create an ImageGroundConnection that is magnified the same way
MagnifyBilinear does.

C++ includes: magnify_bilinear.h ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::cf_look_vector "
virtual void GeoCal::MagnifyBilinearImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
boost::shared_ptr< CartesianFixed > &P) const
Return look vector for given coordinate, along with a position that
lies along the direction of the look vector (so position of satellite,
or a position on the surface. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::cover "
MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
Find a MapInfo that covers the ground coordinate of this
ImageGroundConnection.

We calculate the ground coordinate of the four corners, then find the
MapInfo that covers those corners, optionally adding a boundary in map
pixels. Depending on the actual warping involved, this may or may not
fully cover the edges of the image. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::dem "
void GeoCal::ImageGroundConnection::dem(const boost::shared_ptr< Dem > &D)
Set the DEM to use in ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::dem "
const Dem& GeoCal::ImageGroundConnection::dem() const
Dem used by ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::dem_ptr "
const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
DEM used by ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::ground_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::ground_coordinate_dem "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::MagnifyBilinearImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::image "
virtual boost::shared_ptr<RasterImage> GeoCal::ImageGroundConnection::image() const
Underlying image. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::image_coordinate "
virtual ImageCoordinate GeoCal::MagnifyBilinearImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const
Return image coordinate that goes with a particular GroundCoordinate.
";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::image_coordinate_jac_ecr "
virtual blitz::Array<double, 2> GeoCal::MagnifyBilinearImageGroundConnection::image_coordinate_jac_ecr(const Ecr &Gc) const
Return the Jacobian of the image coordinates with respect to the X, Y,
and Z components of the Ecr ground location. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::image_coordinate_jac_parm "
virtual blitz::Array<double, 2> GeoCal::MagnifyBilinearImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Return the Jacobian of the image coordinates with respect to the
parameters. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::magnification_factor "
int GeoCal::MagnifyBilinearImageGroundConnection::magnification_factor() const
Return magnification factor. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::MagnifyBilinearImageGroundConnection "
GeoCal::MagnifyBilinearImageGroundConnection::MagnifyBilinearImageGroundConnection(const boost::shared_ptr< ImageGroundConnection > &Ig_original, int
Magfactor)
Constructor. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::number_line "
virtual int GeoCal::MagnifyBilinearImageGroundConnection::number_line() const
Return number of lines in underlying image. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::number_sample "
virtual int GeoCal::MagnifyBilinearImageGroundConnection::number_sample() const
Return number of samples in underlying image. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::original_image_ground_connection "
boost::shared_ptr<ImageGroundConnection> GeoCal::MagnifyBilinearImageGroundConnection::original_image_ground_connection() const
Underlying ImageGroundConnection. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::parameter "
virtual blitz::Array<double, 1> GeoCal::MagnifyBilinearImageGroundConnection::parameter() const
A image to ground connection may depend on a set of parameters, which
can by modified (e.g., during a simultaneous bundle adjustment).

This returns those parameters. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::parameter "
virtual void GeoCal::MagnifyBilinearImageGroundConnection::parameter(const blitz::Array< double, 1 > &Parm)
Set the value of the parameters. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::parameter_name "
virtual std::vector<std::string> GeoCal::MagnifyBilinearImageGroundConnection::parameter_name() const
Descriptive name of each parameter. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::print "
virtual void GeoCal::MagnifyBilinearImageGroundConnection::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter(const ImageCoordinate &Ic) const
Calculate the approximate resolution on the ground of a given
ImageCoordinate.

This finds the intersection with the reference surface for the given
pixel, + 1 in the line and sample direction. We find the difference in
meters between these points, and select the maximum value. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter() const
Variation of resolution_meter that find the resolution of the center
pixel. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::title "
virtual std::string GeoCal::ImageGroundConnection::title() const
Title that we can use to describe the image.

This can be any string that is useful as a label. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::title "
virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
Set image title. ";

%feature("docstring")  GeoCal::MagnifyBilinearImageGroundConnection::~MagnifyBilinearImageGroundConnection "
virtual GeoCal::MagnifyBilinearImageGroundConnection::~MagnifyBilinearImageGroundConnection()
Destructor. ";


// File: a00107.xml

%feature("docstring") GeoCal::MagnifyReplicate "
This creates a magnified image of a RasterImage.

We do replicatione to get the values.

C++ includes: magnify_replicate.h ";

%feature("docstring")  GeoCal::MagnifyReplicate::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::MagnifyReplicate::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::MagnifyReplicate::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::MagnifyReplicate::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyReplicate::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyReplicate::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyReplicate::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MagnifyReplicate::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::MagnifyReplicate::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::MagnifyReplicate::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MagnifyReplicate::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::MagnifyReplicate::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MagnifyReplicate::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::MagnifyReplicate::magfactor "
int GeoCal::MagnifyReplicate::magfactor() const
";
%feature("docstring")  GeoCal::MagnifyReplicate::MagnifyReplicate "
MagnifyReplicate::MagnifyReplicate(const boost::shared_ptr< RasterImage > &Data, int Magfactor, int
Number_tile=4)
Constructor. ";

%feature("docstring")  GeoCal::MagnifyReplicate::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::MagnifyReplicate::print "
virtual void GeoCal::MagnifyReplicate::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MagnifyReplicate::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MagnifyReplicate::raw_data_ptr "
const boost::shared_ptr<RasterImage>& GeoCal::MagnifyReplicate::raw_data_ptr() const
";
%feature("docstring")  GeoCal::MagnifyReplicate::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::MagnifyReplicate::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::MagnifyReplicate::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::MagnifyReplicate::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::MagnifyReplicate::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::MagnifyReplicate::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MagnifyReplicate::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MagnifyReplicate::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::MagnifyReplicate::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::MagnifyReplicate::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::MagnifyReplicate::~MagnifyReplicate "
virtual GeoCal::MagnifyReplicate::~MagnifyReplicate()
";

// File: a01202.xml

%feature("docstring") std::map "
STL class. ";


// File: a00108.xml

%feature("docstring") GeoCal::MapInfo "
This contains information describing a map.

This class is used to treat map projection information the same,
regardless of the specific map projection used. However, the pixels of
the map are required to be equally spaced in the X and Y coordinates
of the underlying map projection.

Note that the various coordinates (e.g., lrc_x()) are for the bounding
box, not for the center of the lrc or ulc pixel.

The parameters used in MapInfo are the affine parameters, see for
example GeoTiff. Note though that by convention we use the middle of
the ulc pixel is at (0,0), and the upper left corner of the pixel is
at (-0.5, -0.5). The parameters on the other hand have the center of
the ulc pixel at (0.5, 0.5) and the upper left corner of the pixel at
(0, 0). This class handles this difference, but we mention this in
this comment so you can understand where the various \"0.5\" terms
come in.

C++ includes: map_info.h ";

%feature("docstring")  GeoCal::MapInfo::coordinate "
void MapInfo::coordinate(const GroundCoordinate &Gc, double &Pixel_x_index, double
&Pixel_y_index) const
Determine pixel coordinates for the given ground coordinates.

Note that this routine can be called with ground coordiantes outside
of the bounding box of the map, it just returns pixel coordinates
outside of the map in that case. ";

%feature("docstring")  GeoCal::MapInfo::coordinate_converter "
const CoordinateConverter& GeoCal::MapInfo::coordinate_converter() const
Coordinate converter used by map projection. ";

%feature("docstring")  GeoCal::MapInfo::coordinate_converter_ptr "
const boost::shared_ptr<CoordinateConverter>& GeoCal::MapInfo::coordinate_converter_ptr() const
Pointer to Coordinate converter used by map projection. ";

%feature("docstring")  GeoCal::MapInfo::coordinate_to_index "
void GeoCal::MapInfo::coordinate_to_index(double Pixel_x_coordinate, double Pixel_y_coordinate, double
&Pixel_x_index, double &Pixel_y_index) const
Convert pixel coordinate to pixel index. ";

%feature("docstring")  GeoCal::MapInfo::cover "
MapInfo MapInfo::cover(const std::vector< boost::shared_ptr< GroundCoordinate > > &Pt, int
boundary=0) const
Create a MapInfo that has the same map projection and pixel resolution
as this one, but covers the given set of points.

An optional boundary can be given, this adds a boundary of the given
number of pixels on each edge of the resulting MapInfo. ";

%feature("docstring")  GeoCal::MapInfo::ground_coordinate "
boost::shared_ptr< GroundCoordinate > MapInfo::ground_coordinate(double Pixel_x_index, double Pixel_y_index, const Dem &D) const
Convert pixel coordinates to ground coordinates, and place on surface
using DEM. ";

%feature("docstring")  GeoCal::MapInfo::ground_coordinate "
boost::shared_ptr< GroundCoordinate > MapInfo::ground_coordinate(double Pixel_x_index, double Pixel_y_index) const
Convert pixel coordinates to ground coordinates. ";

%feature("docstring")  GeoCal::MapInfo::index_to_coordinate "
void GeoCal::MapInfo::index_to_coordinate(double Pixel_x_index, double Pixel_y_index, double
&Pixel_x_coordinate, double &Pixel_y_coordinate) const
Convert pixel index to pixel coordinate. ";

%feature("docstring")  GeoCal::MapInfo::intersection "
MapInfo MapInfo::intersection(const MapInfo &Mi) const
This finds a rectangle box in the map projection and pixel size of
this class that contains the intersection with the second map info Mi.
";

%feature("docstring")  GeoCal::MapInfo::lrc_x "
double GeoCal::MapInfo::lrc_x() const
X of lower right corner.

Note that this is for the outer edge of the pixel, not the center
(i.e., for the bounding box). ";

%feature("docstring")  GeoCal::MapInfo::lrc_y "
double GeoCal::MapInfo::lrc_y() const
Y of lower right corner.

Note that this is for the outer edge of the pixel, not the center
(i.e., for the bounding box). ";

%feature("docstring")  GeoCal::MapInfo::map_union "
MapInfo MapInfo::map_union(const MapInfo &Mi) const
This finds a rectangle box in the map projection and pixel size of
this class that contains the union with the second map info Mi.

Note we call this \"map_union\" because \"union\" is a reserved word
in C++. ";

%feature("docstring")  GeoCal::MapInfo::MapInfo "
GeoCal::MapInfo::MapInfo()
Default constructor. ";

%feature("docstring")  GeoCal::MapInfo::MapInfo "
GeoCal::MapInfo::MapInfo(const MapInfo &Mi)
";
%feature("docstring")  GeoCal::MapInfo::MapInfo "
MapInfo::MapInfo(const boost::shared_ptr< CoordinateConverter > &Conv, double Ulc_x,
double Ulc_y, double Lrc_x, double Lrc_y, int Number_x_pixel, int
Number_y_pixel)
Constructor.

Note that ulc and lrc are for the bounding box of the map, not for the
center of the upper left hand or lower right hand pixel.

Note that it is allowed for Number_x_pixel or Number_y_pixel to be 0.
This obviously isn't a very useful map, but it is nice for edge cases
for us to allow an empty map (e.g., intersection of two MapInfo that
don't actually intersect). ";

%feature("docstring")  GeoCal::MapInfo::MapInfo "
MapInfo::MapInfo(const boost::shared_ptr< CoordinateConverter > &Conv, const
blitz::Array< double, 1 > &Param, int Number_x_pixel, int
Number_y_pixel)
Constructor that takes the affine parameters.

Note that the parameters should be such that the ulc is at coordinates
-0.5, 0.5. This is the same as \"area based pixels\", if you are using
Geotiff. ";

%feature("docstring")  GeoCal::MapInfo::number_x_pixel "
int GeoCal::MapInfo::number_x_pixel() const
Number of pixels in X direction. ";

%feature("docstring")  GeoCal::MapInfo::number_y_pixel "
int GeoCal::MapInfo::number_y_pixel() const
Number of pixels in Y direction. ";

%feature("docstring")  GeoCal::MapInfo::print "
void MapInfo::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::MapInfo::print_to_string "
std::string GeoCal::Printable< MapInfo  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MapInfo::resolution_meter "
double MapInfo::resolution_meter() const
Return the approximate resolution of the MapInfo in meters.

This looks at the center of the map, + 1 in the X and Y direction. We
determine the distance between these points, and select the maximum of
them. ";

%feature("docstring")  GeoCal::MapInfo::scale "
MapInfo MapInfo::scale(double Number_x_per_pixel, double Number_y_per_pixel) const
Create a MapInfo that has the same upper left corner, and a pixel size
scaled by the given number of line and samples per pixel.

Note that in general this won't exactly cover this MapInfo, we trim
lower left corner if needed to be the largest MapInfo that fits in
this MapInfo. ";

%feature("docstring")  GeoCal::MapInfo::subset "
MapInfo MapInfo::subset(int x_index, int y_index, int nx_pixel, int ny_pixel) const
Return a MapInfo for a subset of this map info.

Note that it is ok for x_index and y_index to be outside the range of
the MapInfo, so you could use this function to get a MapInfo for a
larger area or an area next to this one. ";

%feature("docstring")  GeoCal::MapInfo::transform "
blitz::Array<double, 1> GeoCal::MapInfo::transform() const
This is the affine geometric transformation used by the MapInfo.

You don't normally use this directly, but it is useful when writing
out file metadata, e.g., with Gdal SetGeoTransform. ";

%feature("docstring")  GeoCal::MapInfo::ulc_x "
double GeoCal::MapInfo::ulc_x() const
X of upper left corner.

Note that this is for the outer edge of the pixel, not the center
(i.e., for the bounding box). ";

%feature("docstring")  GeoCal::MapInfo::ulc_y "
double GeoCal::MapInfo::ulc_y() const
Y of upper left corner.

Note that this is for the outer edge of the pixel, not the center
(i.e., for the bounding box). ";


// File: a00109.xml

%feature("docstring") GeoCal::MapInfoImageGroundConnection "
This class creates an ImageGroundConnection that simply maps map
registered image to the ground.

C++ includes: map_info_image_ground_connection.h ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::cf_look_vector "
virtual void GeoCal::MapInfoImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
boost::shared_ptr< CartesianFixed > &P) const
Return look vector for given coordinate, along with a position that
lies along the direction of the look vector (so position of satellite,
or a position on the surface. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::cover "
MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
Find a MapInfo that covers the ground coordinate of this
ImageGroundConnection.

We calculate the ground coordinate of the four corners, then find the
MapInfo that covers those corners, optionally adding a boundary in map
pixels. Depending on the actual warping involved, this may or may not
fully cover the edges of the image. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::dem "
void GeoCal::ImageGroundConnection::dem(const boost::shared_ptr< Dem > &D)
Set the DEM to use in ground_coordinate. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::dem "
const Dem& GeoCal::ImageGroundConnection::dem() const
Dem used by ground_coordinate. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::dem_ptr "
const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
DEM used by ground_coordinate. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::ground_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::ground_coordinate_dem "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::MapInfoImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::image "
virtual boost::shared_ptr<RasterImage> GeoCal::ImageGroundConnection::image() const
Underlying image. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::image_coordinate "
virtual ImageCoordinate GeoCal::MapInfoImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const
Return image coordinate that goes with a particular GroundCoordinate.
";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::image_coordinate_jac_ecr "
virtual blitz::Array<double, 2> GeoCal::MapInfoImageGroundConnection::image_coordinate_jac_ecr(const Ecr &Gc) const
Return the Jacobian of the image coordinates with respect to the X, Y,
and Z components of the Ecr ground location. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::image_coordinate_jac_parm "
virtual blitz::Array<double, 2> GeoCal::ImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Return the Jacobian of the image coordinates with respect to the
parameters. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::MapInfoImageGroundConnection "
GeoCal::MapInfoImageGroundConnection::MapInfoImageGroundConnection(const boost::shared_ptr< RasterImage > &Img, const boost::shared_ptr<
Dem > &D, const std::string &Title=\"Image\")
Constructor. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::number_line "
virtual int GeoCal::ImageGroundConnection::number_line() const
Return number of lines in underlying image. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::number_sample "
virtual int GeoCal::ImageGroundConnection::number_sample() const
Return number of samples in underlying image. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::parameter "
virtual blitz::Array<double, 1> GeoCal::ImageGroundConnection::parameter() const
A image to ground connection may depend on a set of parameters, which
can by modified (e.g., during a simultaneous bundle adjustment).

This returns those parameters. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::parameter "
virtual void GeoCal::ImageGroundConnection::parameter(const blitz::Array< double, 1 > &Parm)
Set the value of the parameters. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::parameter_name "
virtual std::vector<std::string> GeoCal::ImageGroundConnection::parameter_name() const
Descriptive name of each parameter. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::print "
virtual void GeoCal::MapInfoImageGroundConnection::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter(const ImageCoordinate &Ic) const
Calculate the approximate resolution on the ground of a given
ImageCoordinate.

This finds the intersection with the reference surface for the given
pixel, + 1 in the line and sample direction. We find the difference in
meters between these points, and select the maximum value. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter() const
Variation of resolution_meter that find the resolution of the center
pixel. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::title "
virtual std::string GeoCal::ImageGroundConnection::title() const
Title that we can use to describe the image.

This can be any string that is useful as a label. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::title "
virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
Set image title. ";

%feature("docstring")  GeoCal::MapInfoImageGroundConnection::~MapInfoImageGroundConnection "
virtual GeoCal::MapInfoImageGroundConnection::~MapInfoImageGroundConnection()
Destructor. ";


// File: a00110.xml

%feature("docstring") GeoCal::MapReprojectedImage "
This is a RasterImage that has been resampled to a different map
projection and/or a different resolution.

To do this we do two steps:

We calculate roughly what the difference in resolution is between the
original and final MapInfo. We do this by looking at the center pixel
of the original data and the pixel +1 in line and sample. We then use
RasterAveraged to average the original data to roughly the resolution
of the final MapInfo. If the final MapInfo is near the same resolution
as the original, or if it has a higher resolution, then we don't do
any averaging.

We then interpolate the possibly averaged data to the final
projection.

It is ok if the final MapInfo contains areas outside of the original
data. For any pixel outside of the original data, we just return a
value of 0.

This class calculates the reprojected data on the fly. Sometimes this
is what you want, but if you are going to be using the resulting data
a few times, you may want to use a MemoryRasterImage to generate a
copy once and keep it in memory.

C++ includes: map_reprojected_image.h ";

%feature("docstring")  GeoCal::MapReprojectedImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::MapReprojectedImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::MapReprojectedImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::MapReprojectedImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MapReprojectedImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MapReprojectedImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MapReprojectedImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MapReprojectedImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::MapReprojectedImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::MapReprojectedImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MapReprojectedImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::MapReprojectedImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MapReprojectedImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::MapReprojectedImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::MapReprojectedImage "
MapReprojectedImage::MapReprojectedImage(const boost::shared_ptr< RasterImage > Img_in, const MapInfo &Mi)
Constructor.

We resample Img_in to the projection and resolution given by MapInfo.
";

%feature("docstring")  GeoCal::MapReprojectedImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::MapReprojectedImage::print "
void MapReprojectedImage::print(std::ostream &Os) const
Print out a description of the object. ";

%feature("docstring")  GeoCal::MapReprojectedImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MapReprojectedImage::raw_image_ptr "
boost::shared_ptr<RasterImage> GeoCal::MapReprojectedImage::raw_image_ptr() const
";
%feature("docstring")  GeoCal::MapReprojectedImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::MapReprojectedImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::MapReprojectedImage::read_ptr "
void MapReprojectedImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::MapReprojectedImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::MapReprojectedImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::MapReprojectedImage::unchecked_read "
int MapReprojectedImage::unchecked_read(int Line, int Sample) const
Read pixel value at given line and sample. ";

%feature("docstring")  GeoCal::MapReprojectedImage::unchecked_write "
void MapReprojectedImage::unchecked_write(int Line, int Sample, int Val)
Write a value.

This isn't actually implemented for this type of RasterImage. ";

%feature("docstring")  GeoCal::MapReprojectedImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::MapReprojectedImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::MapReprojectedImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::MapReprojectedImage::~MapReprojectedImage "
virtual GeoCal::MapReprojectedImage::~MapReprojectedImage()
Destructor. ";


// File: a00111.xml

%feature("docstring") GeoCal::Mask "
This provides a Mask, which can be used to prevent doing some kind of
processing.

An example is a using a Land/Water mask to restrict image matching to
land, or a cloud mask to restrict image matching to cloud free areas.

The mask can look at specific GroundCoordinate points indicating if
the that point is masked or not. You can also look at a region to see
if the entire area is masked (e.g., avoid processing on this area
because it is masked).

The region search is a bit loose, you give the corners of the area to
search. The region selected depends on the underlying map projection
used by the mask data - it might be a region in latitude/longitude, a
region in UTM, or something else. The region search is useful only for
small areas where these difference aren't very important. If you need
detailed, accurate information about a region in a specific map
projection then you should simply look at each individual point. The
region is useful for the \"don't bother   looking here for tie points,
look somewhere else\" sorts of uses.

If the region search returns true, then all of the region is masked.
If this is false, then some of the region might be unmasked. Note that
this is conservative in the true direction - a value of false doesn't
promise that there is an unmasked pixel, just that we are pretty sure
that there is one. Edge cases where a false value clips the corner of
a region might have region search return false, while when you look at
it pixel by pixel every point ends up being masked.

Typically a Mask has an underlying RasterImage or something similar. A
particular GroundCoordinate probably doesn't fall exactly on a pixel
of the Mask. How we interpolate is defined by the particular derived
class uses - a common approach is to mask if any of the 4 neighboring
pixels are masked (but check the derived class for details).

C++ includes: mask.h ";

%feature("docstring")  GeoCal::Mask::mask "
virtual bool GeoCal::Mask::mask(const GroundCoordinate &Gc) const =0
Indicate if a particular point is masked.

If true, the point is masked and should not be used in processing
(e.g., don't do image matching) ";

%feature("docstring")  GeoCal::Mask::print "
virtual void GeoCal::Mask::print(std::ostream &Os) const =0
Print to given stream. ";

%feature("docstring")  GeoCal::Mask::print_to_string "
std::string GeoCal::Printable< Mask  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Mask::region_masked "
virtual bool GeoCal::Mask::region_masked(const GroundCoordinate &Ulc, const GroundCoordinate &Lrc) const =0
Indicated if a region is all masked or not.

See the discussion in the comments of Mask for detailed discussion of
the check. ";

%feature("docstring")  GeoCal::Mask::~Mask "
virtual GeoCal::Mask::~Mask()
Destructor. ";


// File: a00112.xml

%feature("docstring") GeoCal::MaskImage "
This provides a Mask where the underlying data is a RasterImage.

We use the underlying map projection of the RasterImage we defining a
region.

For any point, we look at the four neighboring pixels. If any of the
pixels are the mask_value, we say the point is masked, otherwise it is
not masked. If we are outside of the RasterImage, then we say it is no
masked.

An example of this kind of Mask is the Land/Water mask used by Vicar
(\"world_30as_lwm.img\")

C++ includes: mask_image.h ";

%feature("docstring")  GeoCal::MaskImage::mask "
bool MaskImage::mask(const GroundCoordinate &Gc) const
Indicate if a particular point is masked.

If true, the point is masked and should not be used in processing
(e.g., don't do image matching) ";

%feature("docstring")  GeoCal::MaskImage::masked_value "
int GeoCal::MaskImage::masked_value() const
Value indicating masked data. ";

%feature("docstring")  GeoCal::MaskImage::MaskImage "
GeoCal::MaskImage::MaskImage(boost::shared_ptr< RasterImage > Img, int Mask_value=0)
Constructor. ";

%feature("docstring")  GeoCal::MaskImage::print "
void MaskImage::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::MaskImage::print_to_string "
std::string GeoCal::Printable< Mask  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MaskImage::raster_image "
const RasterImage& GeoCal::MaskImage::raster_image() const
Underlying RasterImage. ";

%feature("docstring")  GeoCal::MaskImage::raster_image_ptr "
const boost::shared_ptr<RasterImage>& GeoCal::MaskImage::raster_image_ptr() const
Underlying RasterImage. ";

%feature("docstring")  GeoCal::MaskImage::region_masked "
bool MaskImage::region_masked(const GroundCoordinate &Ulc, const GroundCoordinate &Lrc) const
Indicated if a region is all masked or not.

See the discussion in the comments of Mask for detailed discussion of
the check. ";

%feature("docstring")  GeoCal::MaskImage::~MaskImage "
virtual GeoCal::MaskImage::~MaskImage()
Destructor. ";


// File: a00113.xml

%feature("docstring") GeoCal::MatcherFixture "
";
%feature("docstring")  GeoCal::MatcherFixture::MatcherFixture "
GeoCal::MatcherFixture::MatcherFixture()
";
%feature("docstring")  GeoCal::MatcherFixture::shiva_test_data_dir "
std::string GeoCal::GlobalFixture::shiva_test_data_dir() const
";
%feature("docstring")  GeoCal::MatcherFixture::stereo_test_data_dir "
std::string GeoCal::GlobalFixture::stereo_test_data_dir() const
";
%feature("docstring")  GeoCal::MatcherFixture::test_data_dir "
std::string GlobalFixture::test_data_dir() const
Directory where test data is.

This already includes the trailing slash, so you can just do
test_data_data() + \"foo.txt\" in your unit tests. ";


// File: a00114.xml

%feature("docstring") GeoCal::MaterialDetect::MaterialClass "
";

// File: a00115.xml

%feature("docstring") GeoCal::MaterialDetect "
This class is used to do change detection and identify the material
for pixels that have changed.

This duplicates what was done in the VICAR proc \"detwvpan\", and in
particular the \"f2ratio\" process.

We read a IBIS file that describes the thresholds for a number of
material classes. For each material class, we do the following:

First, the difference in the pan bands is compared to a threshold. The
difference is normally calculated by DoughnutAverage, using the
pandif_raster_image. But something else could be used, all this class
cares about is having a difference it can compare against a threshold.
Compare pan data to a shadow threshold. This masks at very dark pixels
that we assume are in shadow.

For each point, we calculate the required ratios between the
multispectral bands, take the difference with the supplied class mean
and divide by the class sigma.

We sum the abs value of for each of the band ratios divided by the
number of band ratios (i.e, we use a L1 norm). This is compared
against a second threshold.

We may have more than one class that passes the second threshold. In
that case, we sort the classes first by a class priority (with the
lower number being selected first). For ties, we then pick the class
that has the smallest difference norm.

C++ includes: material_detect.h ";

%feature("docstring")  GeoCal::MaterialDetect::closest_material_dif "
blitz::Array< double, 2 > MaterialDetect::closest_material_dif(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate distance for every point from the nearest material.

By convention we multiple this by 100. ";

%feature("docstring")  GeoCal::MaterialDetect::closest_material_raster_image "
boost::shared_ptr< RasterImage > MaterialDetect::closest_material_raster_image() const
This returns an image that gives the distance to the closest material
(whose class id will vary from pixel to pixel).

By convention we multiple this difference by 100 so it can be viewed
more easily in xvd (which prefers integers). ";

%feature("docstring")  GeoCal::MaterialDetect::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::MaterialDetect::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::MaterialDetect::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::MaterialDetect::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MaterialDetect::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MaterialDetect::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MaterialDetect::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MaterialDetect::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::MaterialDetect::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::MaterialDetect::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MaterialDetect::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::MaterialDetect::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MaterialDetect::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::MaterialDetect::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::MaterialDetect::material_dif "
blitz::Array< double, 2 > MaterialDetect::material_dif(int Lstart, int Sstart, int Number_line, int Number_sample, int
Class_id) const
Calculate the distance for every point from the given material.

By convention we multiple this by 100. ";

%feature("docstring")  GeoCal::MaterialDetect::material_raster_image "
boost::shared_ptr< RasterImageMultiBand > MaterialDetect::material_raster_image() const
This returns a set of images that gives the distance to each of the
materials.

This is ordered by class id. By convention we multiple this difference
by 100 so it can be viewed more easily in xvd (which prefers
integers). ";

%feature("docstring")  GeoCal::MaterialDetect::material_raster_image "
boost::shared_ptr< RasterImage > MaterialDetect::material_raster_image(int Class_id) const
This returns a set image that gives the distance to the given
material.

By convention we multiple this difference by 100 so it can be viewed
more easily in xvd (which prefers integers). ";

%feature("docstring")  GeoCal::MaterialDetect::MaterialDetect "
MaterialDetect::MaterialDetect(const boost::shared_ptr< RasterImage > &Pan_data, const
boost::shared_ptr< RasterImage > &Pan_diff, const boost::shared_ptr<
RasterImageMultiBand > &Mulspect, const blitz::Array< double, 1 >
&Pan_diff_threshold, const blitz::Array< double, 1 >
&Spectral_diff_threshold, const blitz::Array< int, 1 >
&Class_priority, const std::string &Ibis_fname, double
Pan_shadow_threshold)
Constructor.

This takes the pan difference band (normally pandif_raster_image from
DoughnutAverage), the multispectral bands, the threshold for each
material for the pan difference and spectral difference, the class
priority for eahc class, and the name of the IBIS file to get the
material information.

The IBIS file should have at least 5 columns (which is all that we
read). The first two columns should be full word values, and are the
band indices (1 based, rather than the 0 based we use elsewhere). The
third should be double (for no good reason, it just is), which is the
class id. The fourth and fifth are also double, and are the mean and
sigma for that band ratio.

In generate, the class ID doesn't start from 0. We find the minimum
value in the table, and use that as the index into Pan_diff_threshold
etc. (so if first id is 8881 then the threshold for 8881 is
Pan_diff_threshold[0] and 8885 is Pan_diff_threshold[4]). Obviously we
could have just used a map instead, but this convention fits better
with the current way we supply this values in the Shiva scripts. ";

%feature("docstring")  GeoCal::MaterialDetect::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::MaterialDetect::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::MaterialDetect::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MaterialDetect::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MaterialDetect::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::MaterialDetect::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MaterialDetect::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MaterialDetect::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::MaterialDetect::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MaterialDetect::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MaterialDetect::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MaterialDetect::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::MaterialDetect::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MaterialDetect::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::MaterialDetect::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::MaterialDetect::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::MaterialDetect::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MaterialDetect::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MaterialDetect::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::MaterialDetect::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::MaterialDetect::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::MaterialDetect::~MaterialDetect "
virtual GeoCal::MaterialDetect::~MaterialDetect()
";

// File: a00116.xml

%feature("docstring") GeoCal::MemoryDem "
This is a DemMapInfo that has everything stored in memory.

This Dem can be updated, and we can also supply the jacobian with
respect to the Dem height values.

C++ includes: memory_dem.h ";

%feature("docstring")  GeoCal::MemoryDem::datum "
const Datum& GeoCal::DemMapInfo::datum() const
Datum height is relative to. ";

%feature("docstring")  GeoCal::MemoryDem::datum_ptr "
const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
Pointer to datum. ";

%feature("docstring")  GeoCal::MemoryDem::distance_to_surface "
double DemMapInfo::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::MemoryDem::elevation "
virtual double GeoCal::MemoryDem::elevation(int Y_index, int X_index) const
Return height in meters relative to datum().

Note that the call is in line, sample order, which means Y and then X.
";

%feature("docstring")  GeoCal::MemoryDem::height_datum "
double DemMapInfo::height_datum(const GroundCoordinate &Gp) const
Height relative to datum. ";

%feature("docstring")  GeoCal::MemoryDem::height_reference_surface "
double DemMapInfo::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::MemoryDem::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::MemoryDem::jacobian "
void GeoCal::MemoryDem::jacobian(const GroundCoordinate &Gc, T V, int parameter_offset=0) const
Jacobian of how height_reference_surface would vary if we varied the
underlying elevation_data.

This fills in a vector. We index the parameters like a 1 dimensional
flattened version of elevation_data, plus an optional offset (used if
these are only a some of the parameters you are using).

You can pass in any kind of vector, as long as we can assign to it by
V[i] = b. One example is gmm::wsvector<double>. ";

%feature("docstring")  GeoCal::MemoryDem::map_info "
const MapInfo& GeoCal::DemMapInfo::map_info() const
MapInfo of underlying data. ";

%feature("docstring")  GeoCal::MemoryDem::MemoryDem "
GeoCal::MemoryDem::MemoryDem(const boost::shared_ptr< Datum > &D, const MapInfo &M, bool
Outside_dem_is_error=false)
Constructor. ";

%feature("docstring")  GeoCal::MemoryDem::outside_dem_is_error "
bool GeoCal::DemMapInfo::outside_dem_is_error() const
If true, then calling with values outside the range of area of the DEM
triggers an exception, otherwise we just return a height of 0. ";

%feature("docstring")  GeoCal::MemoryDem::print "
virtual void GeoCal::MemoryDem::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MemoryDem::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MemoryDem::surface_point "
boost::shared_ptr< GroundCoordinate > DemMapInfo::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::MemoryDem::~MemoryDem "
virtual GeoCal::MemoryDem::~MemoryDem()
Destructor. ";


// File: a00117.xml

%feature("docstring") GeoCal::MemoryMapArray "
Access a binary file as an array.

This memory maps a file and supplies a boost::multi_array_ref
interface to the file. Note that in general binary files aren't
portable across different machines because the endian and/or size of
types aren't portable. Never the less, these binary files can be
useful.

C++ includes: memory_map_array.h ";

%feature("docstring")  GeoCal::MemoryMapArray::data "
const boost::multi_array_ref<T, D>& GeoCal::MemoryMapArray< T, D >::data() const
File data that is memory mapped to binary file. ";

%feature("docstring")  GeoCal::MemoryMapArray::data "
boost::multi_array_ref<T, D>& GeoCal::MemoryMapArray< T, D >::data()
File data that is memory mapped to binary file. ";

%feature("docstring")  GeoCal::MemoryMapArray::file_name "
const std::string& GeoCal::MemoryMapArray< T, D >::file_name() const
";
%feature("docstring")  GeoCal::MemoryMapArray::flush "
void GeoCal::MemoryMapArray< T, D >::flush() const
Synchronize the file with the memory map. ";

%feature("docstring")  GeoCal::MemoryMapArray::MemoryMapArray "
GeoCal::MemoryMapArray< T, D >::MemoryMapArray(const std::string &Fname, ExtentList const &Extents, Mode M=READ,
offset_type Offset=0, const boost::general_storage_order< D >
&So=boost::c_storage_order())
Open an existing file for reading or update, or if the mode is CREATE
then create a new file.

The extents to use for the data is passed in. An offset to the first
byte to read/write can be given, in bytes (e.g., there is a header
that should be skipped). ";

%feature("docstring")  GeoCal::MemoryMapArray::~MemoryMapArray "
virtual GeoCal::MemoryMapArray< T, D >::~MemoryMapArray()
Destructor. ";


// File: a00118.xml

%feature("docstring") GeoCal::MemoryMultiBand "
This reads a RasterImageMultiBand into memory, and allow access that
memory.

C++ includes: memory_multi_band.h ";

%feature("docstring")  GeoCal::MemoryMultiBand::add_raster_image "
void GeoCal::RasterImageMultiBandVariable::add_raster_image(const boost::shared_ptr< RasterImage > &Ri)
";
%feature("docstring")  GeoCal::MemoryMultiBand::data "
const blitz::Array<int, 3>& GeoCal::MemoryMultiBand::data() const
This the underlying data. This is ordered by band, line, and sample.
";

%feature("docstring")  GeoCal::MemoryMultiBand::MemoryMultiBand "
MemoryMultiBand::MemoryMultiBand(const RasterImageMultiBand &R)
Read all of the data from the given RasterImageBand into memory, and
provide access to it.

Note that we require that all the bands have the same number of lines
and samples. ";

%feature("docstring")  GeoCal::MemoryMultiBand::number_band "
virtual int GeoCal::RasterImageMultiBandVariable::number_band() const
Return number of bands. ";

%feature("docstring")  GeoCal::MemoryMultiBand::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::MemoryMultiBand::print "
virtual void GeoCal::MemoryMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::MemoryMultiBand::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MemoryMultiBand::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band) const
Return given band. ";

%feature("docstring")  GeoCal::MemoryMultiBand::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band)
";
%feature("docstring")  GeoCal::MemoryMultiBand::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBandVariable::raster_image_ptr(int band) const
";
%feature("docstring")  GeoCal::MemoryMultiBand::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::MemoryMultiBand::read_double "
Array< double, 3 > RasterImageMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::MemoryMultiBand::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::MemoryMultiBand::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";

%feature("docstring")  GeoCal::MemoryMultiBand::~MemoryMultiBand "
virtual GeoCal::MemoryMultiBand::~MemoryMultiBand()
";

// File: a00119.xml

%feature("docstring") GeoCal::MemoryRasterImage "
This is a simple implementation of a RasterImage, where the data is
just stored in a 2D array.

This is useful both for testing and to read all of another RasterImage
into memory.

C++ includes: memory_raster_image.h ";

%feature("docstring")  GeoCal::MemoryRasterImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImage::data "
boost::multi_array<int, 2>& GeoCal::MemoryRasterImage::data()
Underlying data. ";

%feature("docstring")  GeoCal::MemoryRasterImage::data "
const boost::multi_array<int, 2>& GeoCal::MemoryRasterImage::data() const
Underlying data. ";

%feature("docstring")  GeoCal::MemoryRasterImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::MemoryRasterImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::MemoryRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::MemoryRasterImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::MemoryRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MemoryRasterImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::MemoryRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MemoryRasterImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::MemoryRasterImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::MemoryRasterImage "
GeoCal::MemoryRasterImage::MemoryRasterImage(int Number_line=0, int Number_sample=0)
Construct a MemoryRasterImage of the given size. ";

%feature("docstring")  GeoCal::MemoryRasterImage::MemoryRasterImage "
MemoryRasterImage::MemoryRasterImage(const RasterImage &Img, int Number_line_to_read=-1, int
Number_sample_to_read=-1)
Copy another RasterImage into memory.

This can be useful to read a disk based RasterImage completely into
memory if you are going to be doing a lot of processing on it.

We read data in blocks of the given number of lines and sample. The
default value of -1 means we read the data in the tile size of the
image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::MemoryRasterImage "
GeoCal::MemoryRasterImage::MemoryRasterImage(const MapInfo &Mi)
Construct a MemoryRasterImage of the given MapInfo. ";

%feature("docstring")  GeoCal::MemoryRasterImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::MemoryRasterImage::print "
void MemoryRasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MemoryRasterImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MemoryRasterImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::MemoryRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::MemoryRasterImage::read_ptr "
virtual void GeoCal::MemoryRasterImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::MemoryRasterImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::MemoryRasterImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::MemoryRasterImage::unchecked_read "
virtual int GeoCal::MemoryRasterImage::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MemoryRasterImage::unchecked_write "
virtual void GeoCal::MemoryRasterImage::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::MemoryRasterImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::MemoryRasterImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::MemoryRasterImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::MemoryRasterImage::~MemoryRasterImage "
virtual GeoCal::MemoryRasterImage::~MemoryRasterImage()
";

// File: a00120.xml

%feature("docstring") GeoCal::MemoryRasterImageRef "
This is a variation of MemoryRasterImage where the data comes from
outside of this class.

C++ includes: memory_raster_image.h ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::data "
boost::multi_array_ref<T, 2>& GeoCal::MemoryRasterImageRef< T >::data()
Underlying data. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::data "
const boost::multi_array_ref<T, 2>& GeoCal::MemoryRasterImageRef< T >::data() const
Underlying data. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::MemoryRasterImageRef "
GeoCal::MemoryRasterImageRef< T >::MemoryRasterImageRef(T *d, int Number_line, int Number_sample)
Construct a MemoryRasterImageRes of the given size, pointing to the
given data.

Note that the lifetime of the data is managed outside of this class.
";

%feature("docstring")  GeoCal::MemoryRasterImageRef::MemoryRasterImageRef "
GeoCal::MemoryRasterImageRef< T >::MemoryRasterImageRef(const typename boost::shared_ptr< std::vector< T > > &Raw_data,
typename std::vector< T >::size_type Offset, int Number_line, int
Number_sample)
Variation of constructor that takes ownership of an underlying vector
with the data.

This can give a simpler interface in instances where we don't want to
maintain ownership outside of this class. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::print "
virtual void GeoCal::MemoryRasterImageRef< T >::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::read_ptr "
virtual void GeoCal::MemoryRasterImageRef< T >::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::unchecked_read "
virtual int GeoCal::MemoryRasterImageRef< T >::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::unchecked_write "
virtual void GeoCal::MemoryRasterImageRef< T >::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::MemoryRasterImageRef::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::MemoryRasterImageRef::~MemoryRasterImageRef "
virtual GeoCal::MemoryRasterImageRef< T >::~MemoryRasterImageRef()
";

// File: a00121.xml

%feature("docstring") GeoCal::MetadataMissing "
Exception thrown if Metadata is missing.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::MetadataMissing::MetadataMissing "
GeoCal::MetadataMissing::MetadataMissing(const std::string &W=\"\")
Default constructor.

Can give an optional string describing the error. ";

%feature("docstring")  GeoCal::MetadataMissing::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::MetadataMissing::~MetadataMissing "
virtual GeoCal::MetadataMissing::~MetadataMissing()
 throw ()
Destructor.
";

// File: a00122.xml

%feature("docstring") MspiPcorrMatcher "
";
%feature("docstring")  MspiPcorrMatcher::match "
void MspiPcorrMatcher::match(GeoCal::GeoCalCore::RasterImage &Ref, GeoCal::GeoCalCore::RasterImage
&New, std::vector< GeoCal::GeoCalCore::ImageCoor > &Ref_locs, double
New_guess[12], std::vector< GeoCal::GeoCalCore::ImageCoor > &Results,
std::vector< float * > &Cov, std::vector< bool > &Success)
";
%feature("docstring")  MspiPcorrMatcher::MspiPcorrMatcher "
MspiPcorrMatcher::MspiPcorrMatcher(int Search_window=128, int FFT_size=32, int Min_search_window=32,
bool FFT_half=false, int nRedo=12, int Autofit=0, bool Mag_shrk=false,
double Rmagtae[2]=NULL, double Rmagmin[2]=NULL, double Thr_res=10.0,
double Fit_min=0, bool Predfunc=false, bool Nohpf=false, bool
Subpix=true, float Retryparm[3]=NULL, float Zero_limit=5.0, float
Zero_limit2=-1.0, int Zero_thr=0, double Predictor_adjustment[2]=NULL)
";
%feature("docstring")  MspiPcorrMatcher::~MspiPcorrMatcher "
MspiPcorrMatcher::~MspiPcorrMatcher()
";

// File: a01204.xml

%feature("docstring") std::multimap "
STL class. ";


// File: a01206.xml

%feature("docstring") std::multiset "
STL class. ";


// File: a00123.xml

%feature("docstring") GeoCal::NoCoverage "
Exception thrown if no coverage.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::NoCoverage::NoCoverage "
GeoCal::NoCoverage::NoCoverage(const std::string &W=\"\")
Default constructor.

Can give an optional string describing the error. ";

%feature("docstring")  GeoCal::NoCoverage::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::NoCoverage::~NoCoverage "
virtual GeoCal::NoCoverage::~NoCoverage()
 throw ()
Destructor.
";

// File: a00124.xml

%feature("docstring") boost::noncopyable "
";

// File: a00125.xml

%feature("docstring") GeoCal::null_deleter "
This is a class that can be used to create a boost pointer to a value
that does not actually claim ownership of the object.

This won't delete anything when the pointer goes out of scope.

C++ includes: null_deleter.h ";


// File: a00126.xml

%feature("docstring") null_deleter "
";

// File: a00127.xml

%feature("docstring") GeoCal::OffsetImageGroundConnection "
This class creates an ImageGroundConnection that that is an offset of
a given one.

This would be used for example when a image is subsetted to create a
ImageGroundConnection to go with that subsetted image.

C++ includes: image_ground_connection.h ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::cf_look_vector "
virtual void GeoCal::OffsetImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
boost::shared_ptr< CartesianFixed > &P) const
Return look vector for given coordinate, along with a position that
lies along the direction of the look vector (so position of satellite,
or a position on the surface. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::cover "
MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
Find a MapInfo that covers the ground coordinate of this
ImageGroundConnection.

We calculate the ground coordinate of the four corners, then find the
MapInfo that covers those corners, optionally adding a boundary in map
pixels. Depending on the actual warping involved, this may or may not
fully cover the edges of the image. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::dem "
void GeoCal::ImageGroundConnection::dem(const boost::shared_ptr< Dem > &D)
Set the DEM to use in ground_coordinate. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::dem "
const Dem& GeoCal::ImageGroundConnection::dem() const
Dem used by ground_coordinate. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::dem_ptr "
const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
DEM used by ground_coordinate. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::ground_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::ground_coordinate_dem "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::OffsetImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::image "
virtual boost::shared_ptr<RasterImage> GeoCal::ImageGroundConnection::image() const
Underlying image. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::image_coordinate "
virtual ImageCoordinate GeoCal::OffsetImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const
Return image coordinate that goes with a particular GroundCoordinate.
";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::image_coordinate_jac_ecr "
virtual blitz::Array<double, 2> GeoCal::OffsetImageGroundConnection::image_coordinate_jac_ecr(const Ecr &Gc) const
Return the Jacobian of the image coordinates with respect to the X, Y,
and Z components of the Ecr ground location. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::image_coordinate_jac_parm "
virtual blitz::Array<double, 2> GeoCal::OffsetImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Return the Jacobian of the image coordinates with respect to the
parameters. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::line_offset "
double GeoCal::OffsetImageGroundConnection::line_offset() const
Return line offset. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::number_line "
virtual int GeoCal::OffsetImageGroundConnection::number_line() const
Return number of lines in underlying image. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::number_sample "
virtual int GeoCal::OffsetImageGroundConnection::number_sample() const
Return number of samples in underlying image. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::OffsetImageGroundConnection "
GeoCal::OffsetImageGroundConnection::OffsetImageGroundConnection(const boost::shared_ptr< ImageGroundConnection > &Ig_original, double
Line_offset, double Sample_offset, int Number_line, int Number_sample)
Constructor. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::original_image_ground_connection "
boost::shared_ptr<ImageGroundConnection> GeoCal::OffsetImageGroundConnection::original_image_ground_connection() const
Underlying ImageGroundConnection. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::parameter "
virtual blitz::Array<double, 1> GeoCal::OffsetImageGroundConnection::parameter() const
A image to ground connection may depend on a set of parameters, which
can by modified (e.g., during a simultaneous bundle adjustment).

This returns those parameters. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::parameter "
virtual void GeoCal::OffsetImageGroundConnection::parameter(const blitz::Array< double, 1 > &Parm)
Set the value of the parameters. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::parameter_name "
virtual std::vector<std::string> GeoCal::OffsetImageGroundConnection::parameter_name() const
Descriptive name of each parameter. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::print "
virtual void GeoCal::OffsetImageGroundConnection::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter(const ImageCoordinate &Ic) const
Calculate the approximate resolution on the ground of a given
ImageCoordinate.

This finds the intersection with the reference surface for the given
pixel, + 1 in the line and sample direction. We find the difference in
meters between these points, and select the maximum value. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter() const
Variation of resolution_meter that find the resolution of the center
pixel. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::sample_offset "
double GeoCal::OffsetImageGroundConnection::sample_offset() const
Return sample offset. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::title "
virtual std::string GeoCal::ImageGroundConnection::title() const
Title that we can use to describe the image.

This can be any string that is useful as a label. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::title "
virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
Set image title. ";

%feature("docstring")  GeoCal::OffsetImageGroundConnection::~OffsetImageGroundConnection "
virtual GeoCal::OffsetImageGroundConnection::~OffsetImageGroundConnection()
Destructor. ";


// File: a01146.xml

%feature("docstring") std::ofstream "
STL class. ";


// File: a00128.xml

%feature("docstring") GeoCal::OgrCoordinate "
This is a ground coordinate, implemented by the OGRSpatialReference
class supplied with GDAL.

This class is particularly useful for working with
GdalMapProjectedImage or VicarMapProjectedImage. The
OGRSpatialReference can work with a number of ways of describing a
coordinate system, including Well Known Text format (WKT) of the
OpenGIS Transformation specification and the EPSG specification. Note
in particular the EPSG specification is what is used by geotiff, the
format used by the AFIDS system.

You can see the documentation for OGRSpatialReference
athttp://www.gdal.org/ogr/osr_tutorial.html.

C++ includes: ogr_coordinate.h ";

%feature("docstring")  GeoCal::OgrCoordinate::convert_to_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::OgrCoordinate::convert_to_cf() const
Convert to CartesianFixed. ";

%feature("docstring")  GeoCal::OgrCoordinate::height_reference_surface "
virtual double GeoCal::OgrCoordinate::height_reference_surface() const
Height above ellipsoid, in meters. ";

%feature("docstring")  GeoCal::OgrCoordinate::latitude "
virtual double GeoCal::OgrCoordinate::latitude() const
Return latitude in degrees. Latitude is -90 to 90. ";

%feature("docstring")  GeoCal::OgrCoordinate::longitude "
virtual double GeoCal::OgrCoordinate::longitude() const
Return longitude in degrees. Longitude is -180 to 180. ";

%feature("docstring")  GeoCal::OgrCoordinate::ogr "
const OgrWrapper& GeoCal::OgrCoordinate::ogr() const
Underlying OgrWrapper. ";

%feature("docstring")  GeoCal::OgrCoordinate::ogr_ptr "
const boost::shared_ptr<OgrWrapper>& GeoCal::OgrCoordinate::ogr_ptr() const
Underlying OgrWrapper. ";

%feature("docstring")  GeoCal::OgrCoordinate::OgrCoordinate "
GeoCal::OgrCoordinate::OgrCoordinate(const boost::shared_ptr< OgrWrapper > &Ogr, double x, double y,
double z)
Constructor. X, Y, and Z are in whatever coordinates Ogr are in. ";

%feature("docstring")  GeoCal::OgrCoordinate::OgrCoordinate "
OgrCoordinate::OgrCoordinate(const boost::shared_ptr< OgrWrapper > &Ogr, const Geodetic &G)
Convert from Geodetic to the coordinate system given by Ogr. ";

%feature("docstring")  GeoCal::OgrCoordinate::print "
void OgrCoordinate::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::OgrCoordinate::print_to_string "
std::string GeoCal::Printable< GroundCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OgrCoordinate::to_geodetic "
Geodetic OgrCoordinate::to_geodetic() const
Convert to Geodetic coordinates. ";

%feature("docstring")  GeoCal::OgrCoordinate::~OgrCoordinate "
virtual GeoCal::OgrCoordinate::~OgrCoordinate()
Destructor. ";

%feature("docstring")  GeoCal::OgrCoordinate::to_utm "
OgrCoordinate OgrCoordinate::to_utm(const Geodetic &Gc, int zone=-999)
Convert a GroundCoordinate to UTM.

If desired, you can pass in the zone number (use negative number for
southern zones), or if left blank we determine the appropriate zone to
use. ";


// File: a00129.xml

%feature("docstring") GeoCal::OgrCoordinateConverter "
This is a CoordinateConverter for working with OgrCoordinates.

C++ includes: ogr_coordinate.h ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::convert_from_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::OgrCoordinateConverter::convert_from_coordinate(double X, double Y, double Height=0) const
Create a OgrCoordinate.

X, Y, and Height are in whatever X, Y, and Z are for the underlying
OgrWrapper projection. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::convert_to_coordinate "
virtual void GeoCal::OgrCoordinateConverter::convert_to_coordinate(const GroundCoordinate &Gc, double &X, double &Y, double &Height)
const
Convert to OgrCoordinate.

X, Y and Height are in whatever X, Y and Z are for the underlying
OgrWrapper. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::is_same "
bool OgrCoordinateConverter::is_same(const CoordinateConverter &Conv) const
Test if two CoordinateConverters are the same coordinate system. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::ogr "
const OgrWrapper& GeoCal::OgrCoordinateConverter::ogr() const
Underlying OgrWrapper. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::ogr_ptr "
const boost::shared_ptr<OgrWrapper>& GeoCal::OgrCoordinateConverter::ogr_ptr() const
Underlying OgrWrapper. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::OgrCoordinateConverter "
GeoCal::OgrCoordinateConverter::OgrCoordinateConverter(const boost::shared_ptr< OgrWrapper > Ogr)
Constructor. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::print "
void OgrCoordinateConverter::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::print_to_string "
std::string GeoCal::Printable< CoordinateConverter  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OgrCoordinateConverter::~OgrCoordinateConverter "
virtual GeoCal::OgrCoordinateConverter::~OgrCoordinateConverter()
Destructor. ";


// File: a00130.xml

%feature("docstring") GeoCal::OgrWrapper "
This is a wrapper around the OGRSpatialReference class.

We hold onto a OGRSpatialReference class and a transformation from
that coordinate system to the Geodetic coordinate system. This class
handles the lifetime of these objects. This is separated out from
OgrCoordinate because typically we'll have lots of OgrCoordinates that
have the same OgrWrapper.

C++ includes: ogr_coordinate.h ";

%feature("docstring")  GeoCal::OgrWrapper::inverse_transform "
const OGRCoordinateTransformation& GeoCal::OgrWrapper::inverse_transform() const
Return inverse of transform().

This goes from Geodetic to our coordinate system. ";

%feature("docstring")  GeoCal::OgrWrapper::ogr "
const OGRSpatialReference& GeoCal::OgrWrapper::ogr() const
Return underlying OGRSpatialReference. ";

%feature("docstring")  GeoCal::OgrWrapper::ogr_ptr "
const boost::shared_ptr<OGRSpatialReference>& GeoCal::OgrWrapper::ogr_ptr() const
";
%feature("docstring")  GeoCal::OgrWrapper::OgrWrapper "
OgrWrapper::OgrWrapper(const std::string &Wkt)
Constructor that creates a OGRSpatialReference from a WKT (Well Known
Text) string. ";

%feature("docstring")  GeoCal::OgrWrapper::OgrWrapper "
OgrWrapper::OgrWrapper(const boost::shared_ptr< OGRSpatialReference > &Ogr)
Constructor, from an existing OGRSpatialReference. ";

%feature("docstring")  GeoCal::OgrWrapper::pcs_citation_geo_key "
std::string OgrWrapper::pcs_citation_geo_key() const
When converting to the GEOTIFF header format used by VICAR, this is
the value that the PCSCitationGeoKey should have.

This is a short text description of the projection. ";

%feature("docstring")  GeoCal::OgrWrapper::pretty_wkt "
std::string OgrWrapper::pretty_wkt() const
Write out a prettified version of the WKT for ogr_. ";

%feature("docstring")  GeoCal::OgrWrapper::print "
void OgrWrapper::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::OgrWrapper::print_to_string "
std::string GeoCal::Printable< OgrWrapper  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OgrWrapper::projected_cs_type_geo_key "
std::string OgrWrapper::projected_cs_type_geo_key() const
When converting to the GEOTIFF header format used by VICAR, this is
the value that the PROJECTEDCSTYPEGEOKEY should have.

This is just the EPSG id written as text. ";

%feature("docstring")  GeoCal::OgrWrapper::transform "
const OGRCoordinateTransformation& GeoCal::OgrWrapper::transform() const
Return transformation that takes us from our coordinate system to
Geodetic. ";

%feature("docstring")  GeoCal::OgrWrapper::wkt "
std::string OgrWrapper::wkt() const
Write out a the WKT (Well Known Text) for ogr_. ";

%feature("docstring")  GeoCal::OgrWrapper::~OgrWrapper "
OgrWrapper::~OgrWrapper()
Destructor. ";

%feature("docstring")  GeoCal::OgrWrapper::from_epsg "
boost::shared_ptr< OgrWrapper > OgrWrapper::from_epsg(int Epsg_id)
Create a OgrWrapper for a coordinate system given by the EPSG ID.

You can look the EPSG code up for various coordinate systems
athttp://www.epsg-registry.org ";


// File: a00131.xml

%feature("docstring") GeoCal::Orbit "
This class is used to model orbit data, allowing conversions from
spacecraft coordinates to CartesianInertial and CartesianFixed
coordinates.

This class is used to return orbit data at a given time. OrbitData is
a class that is able to convert from spacecraft coordinates to
CartesianInertial coordinates and vice-versa at a given time, as well
as giving the platforms position.

As an optimization, methods for direct conversion from spacecraft
coordinates to CartesianInertial and vice-versa, which don't use the
intermediate OrbitData classes, are supplied. The default methods just
use the OrbitData methods, but derived classes can supply more
optimized versions of these methods.

An orbit has a min_time() and a max_time() that orbit data is
available for. Requesting data outside of this range will cause an
exception to be thrown. For Time T, we must have min_time() <= T <
max_time().

C++ includes: orbit.h ";

%feature("docstring")  GeoCal::Orbit::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::Orbit::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::Orbit::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::Orbit::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::Orbit::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::Orbit::Orbit "
GeoCal::Orbit::Orbit(Time Min_time=Time::min_valid_time, Time
Max_time=Time::max_valid_time)
Constructor.

The Orbit is valid for the given range of minimum to maximum time. ";

%feature("docstring")  GeoCal::Orbit::orbit_data "
virtual boost::shared_ptr<OrbitData> GeoCal::Orbit::orbit_data(Time T) const =0
Return OrbitData for the given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::Orbit::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::Orbit::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::Orbit::print "
virtual void GeoCal::Orbit::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::Orbit::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Orbit::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::Orbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::Orbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::Orbit::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";

%feature("docstring")  GeoCal::Orbit::~Orbit "
virtual GeoCal::Orbit::~Orbit()
Destructor. ";


// File: a00132.xml

%feature("docstring") GeoCal::OrbitData "
This class is used to convert ScLookVector,
CartesianInertialLookVector and CartesianFixedLookVector to and from
each other at a given time.

C++ includes: orbit.h ";

%feature("docstring")  GeoCal::OrbitData::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::OrbitData::cf_look_vector(const ScLookVector &Sl) const =0
Convert from ScLookVector to CartesianFixedLookVector. ";

%feature("docstring")  GeoCal::OrbitData::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::OrbitData::ci_look_vector(const ScLookVector &Sl) const =0
Convert from ScLookVector to CartesianInertialLookVector. ";

%feature("docstring")  GeoCal::OrbitData::footprint "
std::vector< boost::shared_ptr< GroundCoordinate > > OrbitData::footprint(const Camera &C, const Dem &D, double Resolution=30, int Band=0,
double Max_height=9000) const
Return the footprint on the ground for the given camera and dem.

This just calls surface_intersect for the four corner points. ";

%feature("docstring")  GeoCal::OrbitData::frame_coordinate "
FrameCoordinate OrbitData::frame_coordinate(const GroundCoordinate &Gc, const Camera &C, int Band=0) const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::OrbitData::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::OrbitData::position_cf() const =0
Return position as a pointer. ";

%feature("docstring")  GeoCal::OrbitData::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::OrbitData::position_ci() const =0
Return position as a pointer. ";

%feature("docstring")  GeoCal::OrbitData::print "
virtual void GeoCal::OrbitData::print(std::ostream &Os) const =0
";
%feature("docstring")  GeoCal::OrbitData::print_to_string "
std::string GeoCal::Printable< OrbitData  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OrbitData::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianFixed > OrbitData::reference_surface_intersect_approximate(const Camera &C, const FrameCoordinate &Fc, int Band=0, double
Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::OrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the center
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::OrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, const FrameCoordinate &Fc, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the given
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::OrbitData::sc_look_vector "
virtual ScLookVector GeoCal::OrbitData::sc_look_vector(const CartesianInertialLookVector &Ci) const =0
Convert from CartesianInertialLookVector to ScLookVector. ";

%feature("docstring")  GeoCal::OrbitData::sc_look_vector "
virtual ScLookVector GeoCal::OrbitData::sc_look_vector(const CartesianFixedLookVector &Cf) const =0
Convert from CartesianFixedLookVector to ScLookVector. ";

%feature("docstring")  GeoCal::OrbitData::surface_intersect "
boost::shared_ptr< CartesianFixed > OrbitData::surface_intersect(const Camera &C, const FrameCoordinate &Fc, const Dem &D, double
Resolution=30, int Band=0, double Max_height=9000) const
Calculate the intersection with the surface.

Resolution is the dem postings in meters, you usually don't want this
much more accurate than the intrinsic accuracy of the Dem (e.g., the
Did is 30 meter, so resolution shouldn't be better than 30). ";

%feature("docstring")  GeoCal::OrbitData::time "
virtual Time GeoCal::OrbitData::time() const =0
Return Time of OrbitData. ";

%feature("docstring")  GeoCal::OrbitData::velocity_ci "
virtual boost::array<double, 3> GeoCal::OrbitData::velocity_ci() const =0
Return velocity.

This is in meters per second, in same CartesianInertial coordinate
system as position (e.g., ECI). ";

%feature("docstring")  GeoCal::OrbitData::~OrbitData "
virtual GeoCal::OrbitData::~OrbitData()
";

// File: a00133.xml

%feature("docstring") GeoCal::OrbitDataImageGroundConnection "
This is a ImageGroundConnection where the connection is made by
OrbitData and a Camera.

C++ includes: orbit_data_image_ground_connection.h ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::band "
int GeoCal::OrbitDataImageGroundConnection::band() const
Camera band we are using. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::camera_ptr "
const boost::shared_ptr<Camera>& GeoCal::OrbitDataImageGroundConnection::camera_ptr() const
Camera that we are using. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::cf_look_vector "
virtual void GeoCal::OrbitDataImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
boost::shared_ptr< CartesianFixed > &P) const
Return look vector for given coordinate, along with a position that
lies along the direction of the look vector (so position of satellite,
or a position on the surface. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::cover "
MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
Find a MapInfo that covers the ground coordinate of this
ImageGroundConnection.

We calculate the ground coordinate of the four corners, then find the
MapInfo that covers those corners, optionally adding a boundary in map
pixels. Depending on the actual warping involved, this may or may not
fully cover the edges of the image. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::dem "
void GeoCal::ImageGroundConnection::dem(const boost::shared_ptr< Dem > &D)
Set the DEM to use in ground_coordinate. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::dem "
const Dem& GeoCal::ImageGroundConnection::dem() const
Dem used by ground_coordinate. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::dem_ptr "
const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
DEM used by ground_coordinate. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::ground_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::ground_coordinate_dem "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::OrbitDataImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const
Return ground coordinate that goes with a particular image coordinate.

This version supplies a Dem to use. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::image "
virtual boost::shared_ptr<RasterImage> GeoCal::ImageGroundConnection::image() const
Underlying image. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::image_coordinate "
virtual ImageCoordinate GeoCal::OrbitDataImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const
Return image coordinate that goes with a particular GroundCoordinate.

For some types of ImageGroundConnection, we might not be able to
calculate image_coordinate for all values (e.g., Ipi might fail). In
those cases, we will through a ImageGroundConnectionFailed exception.
This means that nothing is wrong, other than that we can't calculate
the image_coordinate. Callers can catch this exception if they have
some way of handling no image coordinate data. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::image_coordinate_jac_ecr "
blitz::Array< double, 2 > ImageGroundConnection::image_coordinate_jac_ecr(const Ecr &Gc) const
Return the Jacobian of the image coordinates with respect to the X, Y,
and Z components of the Ecr ground location. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::image_coordinate_jac_parm "
virtual blitz::Array<double, 2> GeoCal::ImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Return the Jacobian of the image coordinates with respect to the
parameters. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::max_height "
int GeoCal::OrbitDataImageGroundConnection::max_height() const
Maximum height that we expect to see in the Dem. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::number_line "
virtual int GeoCal::ImageGroundConnection::number_line() const
Return number of lines in underlying image. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::number_sample "
virtual int GeoCal::ImageGroundConnection::number_sample() const
Return number of samples in underlying image. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::orbit_data_ptr "
const boost::shared_ptr<OrbitData>& GeoCal::OrbitDataImageGroundConnection::orbit_data_ptr() const
Orbit data that we are using. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::OrbitDataImageGroundConnection "
GeoCal::OrbitDataImageGroundConnection::OrbitDataImageGroundConnection(const boost::shared_ptr< OrbitData > &Od, const boost::shared_ptr<
Camera > &Cam, const boost::shared_ptr< Dem > &D, const
boost::shared_ptr< RasterImage > &Img, const std::string Title=\"\",
const boost::shared_ptr< Refraction > &Ref=boost::shared_ptr<
Refraction >(), bool Include_refraction=false, double Resolution=30,
int Band=0, double Max_height=9000)
Constructor.

You can optionally include a approximate refraction correction, the
default is not to. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::parameter "
virtual blitz::Array<double, 1> GeoCal::ImageGroundConnection::parameter() const
A image to ground connection may depend on a set of parameters, which
can by modified (e.g., during a simultaneous bundle adjustment).

This returns those parameters. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::parameter "
virtual void GeoCal::ImageGroundConnection::parameter(const blitz::Array< double, 1 > &Parm)
Set the value of the parameters. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::parameter_name "
virtual std::vector<std::string> GeoCal::ImageGroundConnection::parameter_name() const
Descriptive name of each parameter. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::print "
virtual void GeoCal::OrbitDataImageGroundConnection::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::refraction "
boost::shared_ptr<Refraction> GeoCal::OrbitDataImageGroundConnection::refraction() const
Refraction object we are using.

May be null if we aren't including refraction. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::resolution "
double GeoCal::OrbitDataImageGroundConnection::resolution() const
Resolution in meters that we examine Dem out.

This affects how long ground_coordinate takes to figure out. It should
be about the resolution of the Dem ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter(const ImageCoordinate &Ic) const
Calculate the approximate resolution on the ground of a given
ImageCoordinate.

This finds the intersection with the reference surface for the given
pixel, + 1 in the line and sample direction. We find the difference in
meters between these points, and select the maximum value. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter() const
Variation of resolution_meter that find the resolution of the center
pixel. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::title "
virtual std::string GeoCal::ImageGroundConnection::title() const
Title that we can use to describe the image.

This can be any string that is useful as a label. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::title "
virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
Set image title. ";

%feature("docstring")  GeoCal::OrbitDataImageGroundConnection::~OrbitDataImageGroundConnection "
virtual GeoCal::OrbitDataImageGroundConnection::~OrbitDataImageGroundConnection()
Destructor. ";


// File: a00134.xml

%feature("docstring") GeoCal::OrbitMapProjected "
Note that this class is deprecated, it is redundant with
IgcMapProjected just passing in a OrbitDataGroundConnection.

But leave this is place now until we are ready to remove this.

This is a RasterImage that has been map projected using OrbitData and
a Camera.

To do this we do two steps:

We calculate roughly what the difference in resolution is between the
original data and final MapInfo. We do this by looking at the center
pixel of the original data and the pixel +1 in line and sample. We
then use RasterAveraged to average the original data to roughly the
resolution of the final MapInfo. If the final MapInfo is near the same
resolution as the original, or if it has a higher resolution, then we
don't do any averaging. Alternatively, you can pass in the averaging
factor.

We then interpolate the possibly averaged data to the final
projection.

It is ok if the final MapInfo contains areas outside of the original
data. For any pixel outside of the original data, we just return a
value of 0.

This class calculates the data on the fly. Sometimes this is what you
want, but if you are going to be using the resulting data a few times,
you may want to use a MemoryRasterImage to generate a copy once and
keep it in memory.

C++ includes: orbit_map_projected.h ";

%feature("docstring")  GeoCal::OrbitMapProjected::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::OrbitMapProjected::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::OrbitMapProjected::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::OrbitMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::OrbitMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::OrbitMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::OrbitMapProjected::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::OrbitMapProjected::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::OrbitMapProjected::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::OrbitMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::OrbitMapProjected::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::OrbitMapProjected::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::OrbitMapProjected::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::OrbitMapProjected::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::number_tile_line "
virtual int GeoCal::CalcMapProjected::number_tile_line() const
Number of lines in each tile. ";

%feature("docstring")  GeoCal::OrbitMapProjected::number_tile_sample "
virtual int GeoCal::CalcMapProjected::number_tile_sample() const
Number of samples in each tile. ";

%feature("docstring")  GeoCal::OrbitMapProjected::OrbitMapProjected "
OrbitMapProjected::OrbitMapProjected(const MapInfo &Mi, const boost::shared_ptr< OrbitData > &Od, const
boost::shared_ptr< RasterImage > &R, const boost::shared_ptr< Camera >
&C, const boost::shared_ptr< Dem > &D, int Band=0, int Avg_fact=-1,
bool Read_into_memory=true)
Constructor.

We average the data either by the factor given as Avg_fact, or by
ratio of the Mapinfo resolution and the camera resolution. ";

%feature("docstring")  GeoCal::OrbitMapProjected::OrbitMapProjected "
OrbitMapProjected::OrbitMapProjected(const MapInfo &Mi, const boost::shared_ptr< OrbitData > &Od, const
std::vector< boost::shared_ptr< RasterImage > > &R, const
boost::shared_ptr< Camera > &C, const boost::shared_ptr< Dem > &D, int
Band=0, int Avg_fact=-1, bool Read_into_memory=true)
Constructor.

We average the data either by the factor given as Avg_fact, or by
ratio of the Mapinfo resolution and the camera resolution.

This is a variation that handles multiple bands at one time, you give
it the set of raster image to process. ";

%feature("docstring")  GeoCal::OrbitMapProjected::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::OrbitMapProjected::print "
void OrbitMapProjected::print(std::ostream &Os) const
Print out a description of the object. ";

%feature("docstring")  GeoCal::OrbitMapProjected::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OrbitMapProjected::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::OrbitMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::OrbitMapProjected::read_ptr "
void CalcMapProjected::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::OrbitMapProjected::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::OrbitMapProjected::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::OrbitMapProjected::unchecked_read "
int CalcMapProjected::unchecked_read(int Line, int Sample) const
Read pixel value at given line and sample. ";

%feature("docstring")  GeoCal::OrbitMapProjected::unchecked_write "
void CalcMapProjected::unchecked_write(int Line, int Sample, int Val)
Write a value.

This isn't actually implemented for this type of RasterImage. ";

%feature("docstring")  GeoCal::OrbitMapProjected::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::OrbitMapProjected::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::OrbitMapProjected::write_image "
void CalcMapProjected::write_image(RasterImage &Out, int Grid_spacing=1) const
input exactly at this grid spacing, and interpolate in between.

This is much faster than calculating ever point, and if the grid
spacing is small compared to the Dem and any nonlinearities give
results very close to the full calculation. ";

%feature("docstring")  GeoCal::OrbitMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out) const
Write output to multiple RasterImage at once.

The number should be <= the number given to the constructor. ";

%feature("docstring")  GeoCal::OrbitMapProjected::write_multiple "
void CalcMapProjected::write_multiple(const std::vector< boost::shared_ptr< RasterImage > > &Out, int
Grid_spacing) const
Write output to multiple RasterImage at once.

This variation takes a grid spacing to use. We calculate the image
coordinates in the input exactly at this grid spacing, and interpolate
in between. This is much faster than calculating ever point, and if
the grid spacing is small compared to the Dem and any Camera
nonlinearities give results very close to the full calculation. ";

%feature("docstring")  GeoCal::OrbitMapProjected::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::OrbitMapProjected::~OrbitMapProjected "
virtual GeoCal::OrbitMapProjected::~OrbitMapProjected()
Destructor. ";


// File: a00135.xml

%feature("docstring") GeoCal::OrbitQuaternionList "
This is an implementation of an Orbit that is a list of
QuaternionOrbitData values.

For times that fall between these values, we interpolate to get the
OrbitData.

C++ includes: orbit_quaternion_list.h ";

%feature("docstring")  GeoCal::OrbitQuaternionList::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::OrbitQuaternionList::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::OrbitQuaternionList::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::orbit_data "
boost::shared_ptr< OrbitData > OrbitQuaternionList::orbit_data(Time T) const
Return OrbitData for the given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::OrbitQuaternionList::OrbitQuaternionList "
GeoCal::OrbitQuaternionList::OrbitQuaternionList(const std::vector< boost::shared_ptr< QuaternionOrbitData > > &Data)
Constructor that takes a list of QuaternionOrbitData values.

Note that the data doesn't need to be sorted, we handle sorting as we
ingest the data. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::OrbitQuaternionList "
GeoCal::OrbitQuaternionList::OrbitQuaternionList(iterator ibeg, iterator iend)
Constructor that takes a list of QuaternionOrbitData values.

Note that the data doesn't need to be sorted, we handle sorting as we
ingest the data. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::OrbitQuaternionList::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::OrbitQuaternionList::print "
void OrbitQuaternionList::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::OrbitQuaternionList::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::OrbitQuaternionList::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::OrbitQuaternionList::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";

%feature("docstring")  GeoCal::OrbitQuaternionList::~OrbitQuaternionList "
virtual GeoCal::OrbitQuaternionList::~OrbitQuaternionList()
Destructor. ";


// File: a01151.xml

%feature("docstring") std::ostream "
STL class. ";


// File: a00136.xml

%feature("docstring") GeoCal::OstreamPad "
This is a filtering stream that adds a pad to the front of every line
written out.

This can be used to do simple formating (among other things) of adding
space for a nested item.

C++ includes: ostream_pad.h ";

%feature("docstring")  GeoCal::OstreamPad::OstreamPad "
GeoCal::OstreamPad::OstreamPad(std::ostream &Os, const std::string &Pad)
";

// File: a00137.xml

%feature("docstring") GeoCal::OstreamPadFilter "
";
%feature("docstring")  GeoCal::OstreamPadFilter::OstreamPadFilter "
GeoCal::OstreamPadFilter::OstreamPadFilter(const std::string &Pad)
";
%feature("docstring")  GeoCal::OstreamPadFilter::put "
bool GeoCal::OstreamPadFilter::put(Sink &snk, char c)
";

// File: a01149.xml

%feature("docstring") std::ostringstream "
STL class. ";


// File: a01187.xml

%feature("docstring") std::out_of_range "
STL class. ";


// File: a01188.xml

%feature("docstring") std::overflow_error "
STL class. ";


// File: a00138.xml

%feature("docstring") GeoCal::PaintClass "
This takes two images, a image with the class ID and a pan band.

It then produces a three color image that is either the given pan band
or a color given by the class ID if the class ID is > 0.

This duplicates what was done with f2multi. This is a pretty specific
routine, and is likely to be replaced with something else at some
point.

C++ includes: paint_class.h ";

%feature("docstring")  GeoCal::PaintClass::add_raster_image "
void GeoCal::RasterImageMultiBandVariable::add_raster_image(const boost::shared_ptr< RasterImage > &Ri)
";
%feature("docstring")  GeoCal::PaintClass::number_band "
virtual int GeoCal::RasterImageMultiBandVariable::number_band() const
Return number of bands. ";

%feature("docstring")  GeoCal::PaintClass::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::PaintClass::PaintClass "
PaintClass::PaintClass(const boost::shared_ptr< RasterImage > &Class_id_img, const
boost::shared_ptr< RasterImage > &Pan_img, const blitz::Array< int, 2
> &Color, int Class_id_first_color=8881)
";
%feature("docstring")  GeoCal::PaintClass::print "
virtual void GeoCal::RasterImageMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::PaintClass::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::PaintClass::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band) const
Return given band. ";

%feature("docstring")  GeoCal::PaintClass::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band)
";
%feature("docstring")  GeoCal::PaintClass::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBandVariable::raster_image_ptr(int band) const
";
%feature("docstring")  GeoCal::PaintClass::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::PaintClass::read_double "
Array< double, 3 > CalcRasterMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::PaintClass::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::PaintClass::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";

%feature("docstring")  GeoCal::PaintClass::~PaintClass "
virtual GeoCal::PaintClass::~PaintClass()
";

// File: a00139.xml

%feature("docstring") GeoCal::PanSharpen "
This create a set of pan sharpened images from a pan band and a set of
multispectral bands (at lower resolution).

This uses the pan sharpening algorithm described in \"WorldView-2 Pan-
Sharpening\" by Chris Padwick et. al. (ASPRS 2010 Annual Conference).

Because of the way this is calculated, it is most efficient to
calculate all bands for a given tile at one time. So you should access
all bands for a particular tile before going to the next tile for this
object. Nothing bad happens if you don't, it is just slower to access.

C++ includes: pan_sharpen.h ";

%feature("docstring")  GeoCal::PanSharpen::add_raster_image "
void GeoCal::RasterImageMultiBandVariable::add_raster_image(const boost::shared_ptr< RasterImage > &Ri)
";
%feature("docstring")  GeoCal::PanSharpen::number_band "
virtual int GeoCal::RasterImageMultiBandVariable::number_band() const
Return number of bands. ";

%feature("docstring")  GeoCal::PanSharpen::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::PanSharpen::PanSharpen "
PanSharpen::PanSharpen(const boost::shared_ptr< RasterImage > &Pan, const
RasterImageMultiBand &Mul, bool Force_rpc=false, bool
Log_progress=true, const boost::shared_ptr< RasterImage >
&Pan_overview=boost::shared_ptr< RasterImage >(), const
boost::shared_ptr< RasterImageMultiBand >
&Mul_overview=boost::shared_ptr< RasterImageMultiBand >())
Constructor.

This takes in a pan image and a multi spectral image. We subset to the
area we can calculate PanSharpen over.

Parameters:
-----------

Pan:  The panchromatic image to use

Mul:  The multispectral images to use

Force_rpc:  Sometimes an image will have both map information and an
RPC. In this case, we use the map information by preference, unless
directed by Force_rpc being true.

Pan_overview:  Optional overview to use instead of Pan for calculating
the initial statistics.

Mul_overview:  Optional overview to use instead of Mul for calculating
the initial statistics.

Log_progress:  If true, write progress message to std::cout as we work
through the data. ";

%feature("docstring")  GeoCal::PanSharpen::print "
virtual void GeoCal::RasterImageMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::PanSharpen::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::PanSharpen::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band) const
Return given band. ";

%feature("docstring")  GeoCal::PanSharpen::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band)
";
%feature("docstring")  GeoCal::PanSharpen::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBandVariable::raster_image_ptr(int band) const
";
%feature("docstring")  GeoCal::PanSharpen::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::PanSharpen::read_double "
Array< double, 3 > CalcRasterMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::PanSharpen::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::PanSharpen::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";

%feature("docstring")  GeoCal::PanSharpen::~PanSharpen "
virtual GeoCal::PanSharpen::~PanSharpen()
";

// File: a00140.xml

%feature("docstring") GeoCal::PosExportOrbit "
This read a POS export file to supply an Orbit.

This is a text file that is written by the Applanix POS Export
Utility.

Note that the time in the POS export file is in GPS second of the
week. This starts at midnight UTC time between Saturday and Sunday.
Presumably if we happen to be flying at this time, the time will
reset. We don't have any handling in place for this, we'll need to add
handling if this ever becomes an issue.

C++ includes: pos_export_orbit.h ";

%feature("docstring")  GeoCal::PosExportOrbit::aircraft_orbit_data "
const AircraftOrbitData & PosExportOrbit::aircraft_orbit_data(const Time &T) const
Return the AircraftOrbitData closest to a given Time.

This doesn't interpolate, it just return the data point in the file
closest to the given time. ";

%feature("docstring")  GeoCal::PosExportOrbit::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::PosExportOrbit::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::PosExportOrbit::file_epoch "
const Time& GeoCal::PosExportOrbit::file_epoch() const
";
%feature("docstring")  GeoCal::PosExportOrbit::file_name "
const std::string& GeoCal::PosExportOrbit::file_name() const
";
%feature("docstring")  GeoCal::PosExportOrbit::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::PosExportOrbit::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::PosExportOrbit::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::PosExportOrbit::orbit_data "
boost::shared_ptr< OrbitData > OrbitQuaternionList::orbit_data(Time T) const
Return OrbitData for the given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::PosExportOrbit::PosExportOrbit "
PosExportOrbit::PosExportOrbit(const std::string &Fname, const Time &Epoch)
Read the given text file.

Note that the time in the Applanix file is in GPS seconds of the week,
but the actual week isn't in the file. This means we need to pass in
the Epoch that the times are relative to. ";

%feature("docstring")  GeoCal::PosExportOrbit::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::PosExportOrbit::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::PosExportOrbit::print "
void OrbitQuaternionList::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::PosExportOrbit::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::PosExportOrbit::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::PosExportOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::PosExportOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::PosExportOrbit::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";

%feature("docstring")  GeoCal::PosExportOrbit::~PosExportOrbit "
virtual GeoCal::PosExportOrbit::~PosExportOrbit()
";

// File: a00141.xml

%feature("docstring") GeoCal::Printable "
This is a Mixin for classes that can be printed.

There are two different functions we want to use for printing. For
normal C++ use, we want the usual \"<<\" notation, e.g., cout << foo.
For use with languages such as Ruby that don't have a native stream
type, we want to \"print_to_string()\", which returns a string that
Ruby can directly work with.

We implement both of these functions in terms of a third function,
\"print(ostream& Os)\".

Classes T that want to be printable in both C++ and other languages
should derive from Printable<T>, and then define print(ostream& Os).

C++ includes: printable.h ";

%feature("docstring")  GeoCal::Printable::print_to_string "
std::string GeoCal::Printable< T >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";


// File: a01208.xml

%feature("docstring") std::priority_queue "
STL class. ";


// File: a00142.xml

%feature("docstring") GeoCal::PushBroomCamera "
This is a camera specialized to 1 line per band.

C++ includes: camera.h ";

%feature("docstring")  GeoCal::PushBroomCamera::direction "
virtual Direction GeoCal::Camera::direction() const
This gives the camera direction.

This is intended for use with steep camera angles (e.g., MISR AF
camera). For cameras that are near nadir looking, we can just
arbitrarily pick a direction for it.

The default version returns FORWARD. ";

%feature("docstring")  GeoCal::PushBroomCamera::frame_coordinate "
virtual FrameCoordinate GeoCal::Camera::frame_coordinate(const ScLookVector &Sl, int Band) const =0
This converts from ScLookVector to FrameCoordinate for a given band.

Note that the FrameCoordinate may be outside of the range (0,
number_line(band) - 1), (0, number_sample(band) - 1), this just means
the look vector is not with the active portion of the camera (i.e., it
is not seen). ";

%feature("docstring")  GeoCal::PushBroomCamera::frame_line_coordinate "
virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::PushBroomCamera::integration_time "
virtual double GeoCal::Camera::integration_time(int Band) const
This is the integration time in seconds.

The integration time is how long it takes the camera to collect data.
It is important to model this, so that we can include movement of the
spacecraft while the camera is in the middle of collecting data. If
the integration time is very short, or if a particular derived class
doesn't wish to model this, then the integration time can be set to 0
seconds.

The default version returns 0.0. ";

%feature("docstring")  GeoCal::PushBroomCamera::number_band "
virtual int GeoCal::Camera::number_band() const =0
Number of bands in camera. ";

%feature("docstring")  GeoCal::PushBroomCamera::number_line "
virtual int GeoCal::PushBroomCamera::number_line(int Band) const
Number of lines in camera for given band. ";

%feature("docstring")  GeoCal::PushBroomCamera::number_sample "
virtual int GeoCal::Camera::number_sample(int Band) const =0
Number of samples in camera for given band. ";

%feature("docstring")  GeoCal::PushBroomCamera::print "
virtual void GeoCal::Camera::print(std::ostream &Os) const =0
Print to a stream. ";

%feature("docstring")  GeoCal::PushBroomCamera::print_to_string "
std::string GeoCal::Printable< Camera  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::PushBroomCamera::sc_look_vector "
virtual ScLookVector GeoCal::Camera::sc_look_vector(const FrameCoordinate &F, int Band) const =0
Convert from FrameCoordinate to ScLookVector.

It is perfectly allowable for F.line to be outside the range (0,
number_line(band) 1) or for F.sample to be outside the range (0,
number_sample(band) - 1). The conversion will just act as if the
camera has infinite extent. ";

%feature("docstring")  GeoCal::PushBroomCamera::~PushBroomCamera "
virtual GeoCal::PushBroomCamera::~PushBroomCamera()
";

// File: a00143.xml

%feature("docstring") GeoCal::QuaternionCamera "
This is a simple frame camera.

This doesn't account for any lens distortion, we model this as a
pinhole camera. The camera has an orientation to the spacecraft frame
by a given quaternion.

C++ includes: quaternion_camera.h ";

%feature("docstring")  GeoCal::QuaternionCamera::direction "
virtual Direction GeoCal::Camera::direction() const
This gives the camera direction.

This is intended for use with steep camera angles (e.g., MISR AF
camera). For cameras that are near nadir looking, we can just
arbitrarily pick a direction for it.

The default version returns FORWARD. ";

%feature("docstring")  GeoCal::QuaternionCamera::focal_length "
double GeoCal::QuaternionCamera::focal_length() const
Focal length, in mm. ";

%feature("docstring")  GeoCal::QuaternionCamera::frame_coordinate "
FrameCoordinate QuaternionCamera::frame_coordinate(const ScLookVector &Sl, int Band) const
This converts from ScLookVector to FrameCoordinate for a given band.

Note that the FrameCoordinate may be outside of the range (0,
number_line(band) - 1), (0, number_sample(band) - 1), this just means
the look vector is not with the active portion of the camera (i.e., it
is not seen). ";

%feature("docstring")  GeoCal::QuaternionCamera::frame_line_coordinate "
virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::QuaternionCamera::frame_to_sc "
const boost::math::quaternion<double>& GeoCal::QuaternionCamera::frame_to_sc() const
Frame to spacecraft quaternion. ";

%feature("docstring")  GeoCal::QuaternionCamera::integration_time "
virtual double GeoCal::Camera::integration_time(int Band) const
This is the integration time in seconds.

The integration time is how long it takes the camera to collect data.
It is important to model this, so that we can include movement of the
spacecraft while the camera is in the middle of collecting data. If
the integration time is very short, or if a particular derived class
doesn't wish to model this, then the integration time can be set to 0
seconds.

The default version returns 0.0. ";

%feature("docstring")  GeoCal::QuaternionCamera::line_pitch "
double GeoCal::QuaternionCamera::line_pitch() const
CCD pitch, in mm. ";

%feature("docstring")  GeoCal::QuaternionCamera::line_scale "
double GeoCal::QuaternionCamera::line_scale() const
Scaling of line. ";

%feature("docstring")  GeoCal::QuaternionCamera::line_scale "
void GeoCal::QuaternionCamera::line_scale(double Line_scale)
";
%feature("docstring")  GeoCal::QuaternionCamera::number_band "
virtual int GeoCal::QuaternionCamera::number_band() const
Number of bands in camera.

We don't treat the bands separately (at least for now), so this is set
to 1. ";

%feature("docstring")  GeoCal::QuaternionCamera::number_line "
virtual int GeoCal::QuaternionCamera::number_line(int Band) const
Number of lines in camera for given band. ";

%feature("docstring")  GeoCal::QuaternionCamera::number_sample "
virtual int GeoCal::QuaternionCamera::number_sample(int Band) const
Number of samples in camera for given band. ";

%feature("docstring")  GeoCal::QuaternionCamera::principal_point "
const FrameCoordinate& GeoCal::QuaternionCamera::principal_point() const
Principal point of camera. ";

%feature("docstring")  GeoCal::QuaternionCamera::print "
void QuaternionCamera::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::QuaternionCamera::print_to_string "
std::string GeoCal::Printable< Camera  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::QuaternionCamera::QuaternionCamera "
GeoCal::QuaternionCamera::QuaternionCamera(boost::math::quaternion< double > frame_to_sc_q, double Number_line,
double Number_sample, double Line_pitch, double Sample_pitch, double
Focal_length, const FrameCoordinate &Principal_point, double
Line_scale=1.0, double Sample_scale=1.0)
Create a QuaternionCamera.

The orientation of the camera to the spacecraft to given by the
quaternion that takes frame coordinates to spacecraft coordinates. The
size of the camera and the line pitch, sample pitch, and focal length
are given. By convention, these are given in mm. Finally the
Principal_point (coordinates at center) are given.

Note that by convention the quaternion has line going in the +y
direction and sample in the +x direction. This is different convention
that we have used in other cameras, but matches the use for the
initial use we have for this camera. ";

%feature("docstring")  GeoCal::QuaternionCamera::sample_pitch "
double GeoCal::QuaternionCamera::sample_pitch() const
CCD pitch, in mm. ";

%feature("docstring")  GeoCal::QuaternionCamera::sample_scale "
double GeoCal::QuaternionCamera::sample_scale() const
Scaling of sample. ";

%feature("docstring")  GeoCal::QuaternionCamera::sample_scale "
void GeoCal::QuaternionCamera::sample_scale(double Sample_scale)
";
%feature("docstring")  GeoCal::QuaternionCamera::sc_look_vector "
ScLookVector QuaternionCamera::sc_look_vector(const FrameCoordinate &F, int Band) const
Convert from FrameCoordinate to ScLookVector.

It is perfectly allowable for F.line to be outside the range (0,
number_line(band) 1) or for F.sample to be outside the range (0,
number_sample(band) - 1). The conversion will just act as if the
camera has infinite extent. ";

%feature("docstring")  GeoCal::QuaternionCamera::~QuaternionCamera "
virtual GeoCal::QuaternionCamera::~QuaternionCamera()
Destructor. ";


// File: a00144.xml

%feature("docstring") GeoCal::QuaternionOrbitData "
This class implements the most common way of doing OrbitData
conversions, which just uses fixed quaternions.

This accounts for both the orientation of the spacecraft relative to
the planet, as well as the aberration of light (a small correction due
to the movement of the spacecraft relative to the planet).

This does not account for atmospheric refraction. Depending on the
zenith angle, this can be somewhat important for satellites. From the
approximate atmospheric model described in \"Theoretical Basis of
the SDP Toolkit Geolocation package for the ECS\", Table 6-5 the
linear displacement for a zenith angle of 10 is 0.549 meters, 20
degrees is 1.223 meters, and 30 degrees is 2.221.

We may want to add a atmospheric refraction correction in the future,
but this hasn't been done yet.

We need to have one of the toolkit available if we want to convert for
the CartesianFixed coordinates used by this class to
CartesianInertial. If you stick to working with CartesianFixed only,
you can avoid the need of using one of these toolkits.

C++ includes: orbit.h ";

%feature("docstring")  GeoCal::QuaternionOrbitData::cf_look_vector "
CartesianFixedLookVector QuaternionOrbitData::cf_look_vector(const ScLookVector &Sl) const
Convert to CartesianFixedLookVector. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::ci_look_vector "
CartesianInertialLookVector QuaternionOrbitData::ci_look_vector(const ScLookVector &Sl) const
Convert to CartesianInertialLookVector. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::footprint "
std::vector< boost::shared_ptr< GroundCoordinate > > OrbitData::footprint(const Camera &C, const Dem &D, double Resolution=30, int Band=0,
double Max_height=9000) const
Return the footprint on the ground for the given camera and dem.

This just calls surface_intersect for the four corner points. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::frame_coordinate "
FrameCoordinate OrbitData::frame_coordinate(const GroundCoordinate &Gc, const Camera &C, int Band=0) const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::from_cf "
bool GeoCal::QuaternionOrbitData::from_cf() const
Was this created form the cartesian fixed version of the constructor?
This is intended for use by python when we pickle this object so we
know which constructor to call.

It probably isn't of interest to anything else. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::QuaternionOrbitData::position_cf() const
Return position as a ptr. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::QuaternionOrbitData::position_ci() const
Return position as a ptr. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::print "
void QuaternionOrbitData::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::print_to_string "
std::string GeoCal::Printable< OrbitData  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::QuaternionOrbitData "
QuaternionOrbitData::QuaternionOrbitData(Time Tm, const boost::shared_ptr< CartesianFixed > &pos_cf, const
boost::array< double, 3 > &vel_fixed, const boost::math::quaternion<
double > &sc_to_cf_q)
Construct QuaternionOrbitData.

This takes data in a CartesianFixed coordinate system (e.g., Ecr
coordinates). ";

%feature("docstring")  GeoCal::QuaternionOrbitData::QuaternionOrbitData "
QuaternionOrbitData::QuaternionOrbitData(Time Tm, const boost::shared_ptr< CartesianInertial > &pos_ci, const
boost::array< double, 3 > &vel_inertial, const
boost::math::quaternion< double > &sc_to_ci_q)
Construct QuaternionOrbitData.

This takes data in a CartesianInertial coordinate system (e.g., Eci
coordinates). ";

%feature("docstring")  GeoCal::QuaternionOrbitData::reference_surface_intersect_approximate "
boost::shared_ptr< CartesianFixed > OrbitData::reference_surface_intersect_approximate(const Camera &C, const FrameCoordinate &Fc, int Band=0, double
Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the center
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::resolution_meter "
double OrbitData::resolution_meter(const Camera &C, const FrameCoordinate &Fc, int Band=0) const
Calculate the approximate resolution on the ground of a given Camera
for this OrbitData.

This finds the intersection with the reference surface for the given
pixel of the camera, + 1 in the line and sample direction. We find the
difference in meters between these points, and select the maximum
value. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::sc_look_vector "
ScLookVector QuaternionOrbitData::sc_look_vector(const CartesianInertialLookVector &Ci) const
Convert to ScLookVector. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::sc_look_vector "
ScLookVector QuaternionOrbitData::sc_look_vector(const CartesianFixedLookVector &Cf) const
Convert to ScLookVector. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::sc_to_cf "
boost::math::quaternion<double> GeoCal::QuaternionOrbitData::sc_to_cf() const
Return the quaternion used to go from spacecraft to cartesian fixed.
";

%feature("docstring")  GeoCal::QuaternionOrbitData::sc_to_ci "
boost::math::quaternion<double> GeoCal::QuaternionOrbitData::sc_to_ci() const
Return the quaternion used to go from spacecraft to cartesian
inertial. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::surface_intersect "
boost::shared_ptr< CartesianFixed > OrbitData::surface_intersect(const Camera &C, const FrameCoordinate &Fc, const Dem &D, double
Resolution=30, int Band=0, double Max_height=9000) const
Calculate the intersection with the surface.

Resolution is the dem postings in meters, you usually don't want this
much more accurate than the intrinsic accuracy of the Dem (e.g., the
Did is 30 meter, so resolution shouldn't be better than 30). ";

%feature("docstring")  GeoCal::QuaternionOrbitData::time "
virtual Time GeoCal::QuaternionOrbitData::time() const
Return Time of OrbitData. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::velocity_cf "
boost::array< double, 3 > QuaternionOrbitData::velocity_cf() const
Velocity in CartisianFixed coordinates. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::velocity_ci "
boost::array< double, 3 > QuaternionOrbitData::velocity_ci() const
Return velocity. ";

%feature("docstring")  GeoCal::QuaternionOrbitData::~QuaternionOrbitData "
virtual GeoCal::QuaternionOrbitData::~QuaternionOrbitData()
";

// File: a01209.xml

%feature("docstring") std::queue "
STL class. ";


// File: a00145.xml

%feature("docstring") GeoCal::QuickBirdAttitude "
This is a low level class that reads a Quickbird attitude file.

You probably don't want to use this directly, but rather use the
QuickBirdOrbit.

Note a possible source of confusion. There are a few different
conventions about the ordering of the quaternion coefficients. The
boost library places the real part at the front, so we have a + b i c
j + d k and the quaternion is 4-tuple (a, b, c, d). The convention
used by quickbird data is q1 i + q2 j + q3 k + q4 with the 4-tuple is
(q1, q2, q3, q4). That means when we bring this over to the boost
library, we need to reorder this to the 4-tuple (q4, q1, q2, q3).

The code in QuickBirdOrbit accounts for these different conventions,
but if you are using this class directly you need to be aware of this
difference.

C++ includes: quickbird_orbit.h ";

%feature("docstring")  GeoCal::QuickBirdAttitude::attitude "
const std::vector<boost::array<double, 14> >& GeoCal::QuickBirdAttitude::attitude() const
Attitude data.

The first 4 parameters are the quaternion parameters (q1, q2, q3, q4).
The next 10 are the upper right elements of the attitude quaternion
covariance matrix.

Make sure to see the class notes for QuickBirdAttitude to see a
discussion about the difference between the boost and Quickbird
quaternion conventions. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::data "
const std::vector<boost::array<double, D> >& GeoCal::QuickBirdFile< D >::data() const
Data in the file. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::file_name "
const std::string& GeoCal::QuickBirdFile< D >::file_name() const
File name we are reading. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::max_time "
Time GeoCal::QuickBirdFile< D >::max_time() const
End time of data. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::min_time "
Time GeoCal::QuickBirdFile< D >::min_time() const
Start time of data. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::print "
void QuickBirdAttitude::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::print_to_string "
std::string GeoCal::Printable< QuickBirdAttitude  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::QuickBirdAttitude "
GeoCal::QuickBirdAttitude::QuickBirdAttitude(const std::string &Fname)
Read the quickbird attitude file. ";

%feature("docstring")  GeoCal::QuickBirdAttitude::time_spacing "
double GeoCal::QuickBirdFile< D >::time_spacing() const
Spacing in seconds between points. ";


// File: a00146.xml

%feature("docstring") GeoCal::QuickBirdCamera "
This class models a the QuickBird camera.

It is used to convert ScLookVector to FrameCoordinate and vice versa.

The QuickBird camera is described by metadata supplied with the
QuickBird data, a \".GEO\" file. This is potentially dynamic, and
potentially accounts for nonlinearities in the optics. However, in
practice all of the GEO file we have are exactly the same, and all the
nonlinear terms are set to 0. I'm not sure if this is because the
camera really is very stable and the optics very linear, or just that
DigitalGlobe didn't do a very good job at geometric camera
calibration. In any case, rather than reading in this file we just
hardwire the coefficients into this class. This is something we can
revisit in the future if needed.

This model is for the Panchromatic band only. We could easily extend
this to other bands, but right now we only work with the Panchromatic
data.

C++ includes: quickbird_camera.h ";

%feature("docstring")  GeoCal::QuickBirdCamera::direction "
virtual Direction GeoCal::Camera::direction() const
This gives the camera direction.

This is intended for use with steep camera angles (e.g., MISR AF
camera). For cameras that are near nadir looking, we can just
arbitrarily pick a direction for it.

The default version returns FORWARD. ";

%feature("docstring")  GeoCal::QuickBirdCamera::frame_coordinate "
FrameCoordinate QuickBirdCamera::frame_coordinate(const ScLookVector &Sl, int Band) const
This converts from ScLookVector to FrameCoordinate for a given band.

Note that the FrameCoordinate may be outside of the range (0,
number_line(band) - 1), (0, number_sample(band) - 1), this just means
the look vector is not with the active portion of the camera (i.e., it
is not seen). ";

%feature("docstring")  GeoCal::QuickBirdCamera::frame_line_coordinate "
virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::QuickBirdCamera::integration_time "
virtual double GeoCal::Camera::integration_time(int Band) const
This is the integration time in seconds.

The integration time is how long it takes the camera to collect data.
It is important to model this, so that we can include movement of the
spacecraft while the camera is in the middle of collecting data. If
the integration time is very short, or if a particular derived class
doesn't wish to model this, then the integration time can be set to 0
seconds.

The default version returns 0.0. ";

%feature("docstring")  GeoCal::QuickBirdCamera::number_band "
virtual int GeoCal::QuickBirdCamera::number_band() const
Number of bands in camera. ";

%feature("docstring")  GeoCal::QuickBirdCamera::number_line "
virtual int GeoCal::PushBroomCamera::number_line(int Band) const
Number of lines in camera for given band. ";

%feature("docstring")  GeoCal::QuickBirdCamera::number_sample "
virtual int GeoCal::QuickBirdCamera::number_sample(int Band) const
Number of samples in camera for given band. ";

%feature("docstring")  GeoCal::QuickBirdCamera::print "
void QuickBirdCamera::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::QuickBirdCamera::print_to_string "
std::string GeoCal::Printable< Camera  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::QuickBirdCamera::QuickBirdCamera "
QuickBirdCamera::QuickBirdCamera()
Constructor.

At this point, all of the camera coefficients are hardwired in this
class. We can change this in the future if needed. ";

%feature("docstring")  GeoCal::QuickBirdCamera::sc_look_vector "
ScLookVector QuickBirdCamera::sc_look_vector(const FrameCoordinate &F, int Band) const
Convert from FrameCoordinate to ScLookVector.

It is perfectly allowable for F.line to be outside the range (0,
number_line(band) 1) or for F.sample to be outside the range (0,
number_sample(band) - 1). The conversion will just act as if the
camera has infinite extent. ";

%feature("docstring")  GeoCal::QuickBirdCamera::~QuickBirdCamera "
virtual GeoCal::QuickBirdCamera::~QuickBirdCamera()
Destructor. ";


// File: a00147.xml

%feature("docstring") GeoCal::QuickBirdEphemeris "
This is a low level class that reads a Quickbird ephemeris file.

You probably don't want to use this directly, but rather use the
QuickBirdOrbit

C++ includes: quickbird_orbit.h ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::data "
const std::vector<boost::array<double, D> >& GeoCal::QuickBirdFile< D >::data() const
Data in the file. ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::ephemeris "
const std::vector<boost::array<double, 12> >& GeoCal::QuickBirdEphemeris::ephemeris() const
Ephemeris data.

This as 12 numbers in each entry. The first 3 are the X, Y, and Z
position in meters, in ECF (or ECR?) coordinate system. The next 3 are
the velocity in m/s. The final 6 are the upper right coordinates of
the position covariance matrix (so order is (1, 1), (1, 2), (1, 3),
(2, 2), (2,3), (3, 3)). ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::file_name "
const std::string& GeoCal::QuickBirdFile< D >::file_name() const
File name we are reading. ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::max_time "
Time GeoCal::QuickBirdFile< D >::max_time() const
End time of data. ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::min_time "
Time GeoCal::QuickBirdFile< D >::min_time() const
Start time of data. ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::print "
void QuickBirdEphemeris::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::print_to_string "
std::string GeoCal::Printable< QuickBirdEphemeris  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::QuickBirdEphemeris "
GeoCal::QuickBirdEphemeris::QuickBirdEphemeris(const std::string &Fname)
Read the quickbird ephemeris file. ";

%feature("docstring")  GeoCal::QuickBirdEphemeris::time_spacing "
double GeoCal::QuickBirdFile< D >::time_spacing() const
Spacing in seconds between points. ";


// File: a00148.xml

%feature("docstring") GeoCal::QuickBirdFile "
Reading QuickBird ephemeris and attitude is almost identical.

We put most of the functionality here, and then derive classes with
the few differences between attitude and ephemeris reading.

C++ includes: quickbird_orbit.h ";

%feature("docstring")  GeoCal::QuickBirdFile::data "
const std::vector<boost::array<double, D> >& GeoCal::QuickBirdFile< D >::data() const
Data in the file. ";

%feature("docstring")  GeoCal::QuickBirdFile::file_name "
const std::string& GeoCal::QuickBirdFile< D >::file_name() const
File name we are reading. ";

%feature("docstring")  GeoCal::QuickBirdFile::max_time "
Time GeoCal::QuickBirdFile< D >::max_time() const
End time of data. ";

%feature("docstring")  GeoCal::QuickBirdFile::min_time "
Time GeoCal::QuickBirdFile< D >::min_time() const
Start time of data. ";

%feature("docstring")  GeoCal::QuickBirdFile::QuickBirdFile "
GeoCal::QuickBirdFile< D >::QuickBirdFile(const std::string &Fname)
Read the given file. ";

%feature("docstring")  GeoCal::QuickBirdFile::time_spacing "
double GeoCal::QuickBirdFile< D >::time_spacing() const
Spacing in seconds between points. ";


// File: a00149.xml

%feature("docstring") GeoCal::QuickBirdOrbit "
This is a Quickbird Orbit.

This can be used for a rigorous model of Quickbird.

Note an important limitation of the rigorous model vs. RPC. We don't
currently account for atmospheric refraction, while the RPC does.
Depending on the zenith angle, this can be somewhat important. From
the approximate atmospheric model described in \"Theoretical Basis of
the SDP Toolkit Geolocation package for the ECS\", Table 6-5 the
linear displacement for a zenith angle of 10 is 0.549 meters, 20
degress is 1.223 meters, and 30 degrees is 2.221. The typical
Quickbird scene has something like 10 to 20 degree zenith angles, so
this is a correction of 1 or 2 pixels.

We will need to add atmospheric refraction in the future, but this
hasn't been done yet.

C++ includes: quickbird_orbit.h ";

%feature("docstring")  GeoCal::QuickBirdOrbit::attitude_file_name "
std::string GeoCal::QuickBirdOrbit::attitude_file_name() const
";
%feature("docstring")  GeoCal::QuickBirdOrbit::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::QuickBirdOrbit::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::QuickBirdOrbit::ephemeris_file_name "
std::string GeoCal::QuickBirdOrbit::ephemeris_file_name() const
";
%feature("docstring")  GeoCal::QuickBirdOrbit::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::QuickBirdOrbit::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::QuickBirdOrbit::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::QuickBirdOrbit::orbit_data "
boost::shared_ptr< OrbitData > QuickBirdOrbit::orbit_data(Time T) const
Return OrbitData for the given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::QuickBirdOrbit::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::QuickBirdOrbit::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::QuickBirdOrbit::print "
void QuickBirdOrbit::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::QuickBirdOrbit::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::QuickBirdOrbit::QuickBirdOrbit "
QuickBirdOrbit::QuickBirdOrbit(const std::string &Ephemeris_file, const std::string
&Attitude_file=\"\")
Constructor.

Usually the attitude and ephemeris file have names like blah.EPH and
blah.ATT. If you supply just the ephemeris name, we fill in the
attitude by replacing \".EPH\" with \".ATT\". ";

%feature("docstring")  GeoCal::QuickBirdOrbit::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::QuickBirdOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::QuickBirdOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::QuickBirdOrbit::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";

%feature("docstring")  GeoCal::QuickBirdOrbit::~QuickBirdOrbit "
virtual GeoCal::QuickBirdOrbit::~QuickBirdOrbit()
Destructor. ";


// File: a00150.xml

%feature("docstring") GeoCal::QuickBirdTimeTable "
This reads a quickbird .IMD file to get time table information.

Note that Quickbird uses the same convention that we use in our code,
an integer location is the center of a pixel. So (0, 0) is the center
of the upper left pixel.

C++ includes: quickbird_time_table.h ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::file_name "
const std::string& GeoCal::QuickBirdTimeTable::file_name() const
Underlying file name. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::image_coordinate "
ImageCoordinate QuickBirdTimeTable::image_coordinate(Time T, const FrameCoordinate &F) const
Convert from Time and FrameCoordinate to ImageCoordinate. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::max_line "
virtual int GeoCal::QuickBirdTimeTable::max_line() const
Maximum line table is valid for. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::max_time "
virtual Time GeoCal::QuickBirdTimeTable::max_time() const
Maximum time table is valid for. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::min_line "
virtual int GeoCal::QuickBirdTimeTable::min_line() const
Minimum line table is valid for. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::min_time "
virtual Time GeoCal::QuickBirdTimeTable::min_time() const
Minimum time table is valid for. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::print "
void QuickBirdTimeTable::print(std::ostream &Os) const
Print out time table to stream. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::print_to_string "
std::string GeoCal::Printable< TimeTable  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::QuickBirdTimeTable "
QuickBirdTimeTable::QuickBirdTimeTable(const std::string &Fname)
Read the given file. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::time "
void QuickBirdTimeTable::time(const ImageCoordinate &Ic, Time &T, FrameCoordinate &F) const
Convert from ImageCoordinate to Time and FrameCoordinate. ";

%feature("docstring")  GeoCal::QuickBirdTimeTable::~QuickBirdTimeTable "
virtual GeoCal::QuickBirdTimeTable::~QuickBirdTimeTable()
";

// File: a01189.xml

%feature("docstring") std::range_error "
STL class. ";


// File: a00151.xml

%feature("docstring") GeoCal::GeoCalCore::RangeException "
";
%feature("docstring")  GeoCal::GeoCalCore::RangeException::RangeException "
GeoCal::GeoCalCore::RangeException::RangeException()
 throw ()";
%feature("docstring")  GeoCal::GeoCalCore::RangeException::what "
virtual const char* GeoCal::GeoCalCore::Exception::what() const
 throw ()";
%feature("docstring")  GeoCal::GeoCalCore::RangeException::~RangeException "
virtual GeoCal::GeoCalCore::RangeException::~RangeException()
 throw ()";

// File: a00152.xml

%feature("docstring") GeoCal::RasterAveraged "
This averages a higher resolution RasterImage into a lower resolution
one.

The data is a straight average of a given number of pixels in the line
and sample direction of the full resolution data.

Typically the pixel averaging factor doesn't exactly divide the number
of lines and samples of the full resolution data. We just trim the
data to the largest low resolution pixel that completely fits in the
higher resolution data.

This class calculates the average on the fly. Sometimes this is what
you want, but if you are going to be using the resulting data a few
times, you may want to use a MemoryRasterImage to generate a copy once
and keep it in memory.

C++ includes: raster_averaged.h ";

%feature("docstring")  GeoCal::RasterAveraged::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RasterAveraged::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RasterAveraged::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RasterAveraged::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterAveraged::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterAveraged::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterAveraged::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterAveraged::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RasterAveraged::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RasterAveraged::high_resolution_image "
const RasterImage& GeoCal::RasterAveraged::high_resolution_image() const
High resolution image that this object is based on. ";

%feature("docstring")  GeoCal::RasterAveraged::high_resolution_image_ptr "
const boost::shared_ptr<RasterImage>& GeoCal::RasterAveraged::high_resolution_image_ptr() const
Pointer to high resolution image that this object is based on. ";

%feature("docstring")  GeoCal::RasterAveraged::ignore_zero "
bool GeoCal::RasterAveraged::ignore_zero() const
If true, we ignore zeros when calculating the average. ";

%feature("docstring")  GeoCal::RasterAveraged::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterAveraged::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RasterAveraged::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterAveraged::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RasterAveraged::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RasterAveraged::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RasterAveraged::number_line_per_pixel "
int GeoCal::RasterAveraged::number_line_per_pixel() const
Number of lines of high resolution data per pixel of this lower
resolution RasterImage. ";

%feature("docstring")  GeoCal::RasterAveraged::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RasterAveraged::number_sample_per_pixel "
int GeoCal::RasterAveraged::number_sample_per_pixel() const
Number of samples of high resolution data per pixel of this lower
resolution RasterImage. ";

%feature("docstring")  GeoCal::RasterAveraged::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterAveraged::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterAveraged::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RasterAveraged::print "
void RasterAveraged::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RasterAveraged::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterAveraged::RasterAveraged "
RasterAveraged::RasterAveraged(const boost::shared_ptr< RasterImage > &Data, int
Number_line_per_pixel, int Number_sample_per_pixel, bool
Ignore_zero=false)
Constructor. ";

%feature("docstring")  GeoCal::RasterAveraged::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RasterAveraged::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterAveraged::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterAveraged::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterAveraged::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RasterAveraged::read_ptr "
void RasterAveraged::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterAveraged::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::RasterAveraged::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RasterAveraged::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RasterAveraged::unchecked_read "
int RasterAveraged::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::RasterAveraged::unchecked_write "
void RasterAveraged::unchecked_write(int Line, int Sample, int Val)
Write a value.

This isn't actually implemented for this type of RasterImage. ";

%feature("docstring")  GeoCal::RasterAveraged::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterAveraged::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RasterAveraged::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::RasterAveraged::~RasterAveraged "
virtual GeoCal::RasterAveraged::~RasterAveraged()
Destructor. ";


// File: a00153.xml

%feature("docstring") GeoCal::RasterImage "
This is a general image class.

Conceptually, an image is just a two dimensional array of integers,
although it is not necessarily implemented this way. This is an
abstract class, containing behavior common to all images. It handles
keeping track of the size of the image, but all other behaviour needs
to be defined by subclasses.

It is common for Raster data to be stored on disk as a series of
tiles. It can be significantly more efficient to access and write the
data in tiles. To support this, we provide a \"preferred\" tile size
given by number_tile_line() and number_tile_sample(). Classes using a
RasterImage are free to ignore this tiling - data can be read and
written in any order desired, and the only impact of tiles is in the
performance.

It is very common to want to step through a file \"efficiently\".
There is a macro \"RASTER_FOREACH(Img, IND1, IND2)\" that steps
through a given raster image, setting the index variable IND1 and IND2
to the line and samples. This steps through by tiles, so it avoids
reading the same tile multiple times. If you need to step through by
tiles (rather than looping through all line and samples), the class
RasterImageTileIterator can be used.

The default tile size is the entire image, derived classes can supply
other values if desired.

A RasterImage may or may not have a Rpc and MapInfo associated with
it. You can query has_rpc() and has_mapinfo() to find out if it has
this metadata, and if it does you can access this by rpc() and
map_info().

C++ includes: raster_image.h ";

%feature("docstring")  GeoCal::RasterImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RasterImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RasterImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImage::has_map_info "
virtual bool GeoCal::RasterImage::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RasterImage::has_rpc "
virtual bool GeoCal::RasterImage::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RasterImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RasterImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RasterImage::map_info "
virtual const MapInfo& GeoCal::RasterImage::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RasterImage::number_line "
virtual int GeoCal::RasterImage::number_line() const =0
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RasterImage::number_sample "
virtual int GeoCal::RasterImage::number_sample() const =0
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RasterImage::number_tile_line "
virtual int GeoCal::RasterImage::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterImage::number_tile_sample "
virtual int GeoCal::RasterImage::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RasterImage::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RasterImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RasterImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImage::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RasterImage::read_ptr "
virtual void GeoCal::RasterImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const =0
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImage::rpc "
virtual Rpc GeoCal::RasterImage::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RasterImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RasterImage::unchecked_read "
virtual int GeoCal::RasterImage::unchecked_read(int Line, int Sample) const =0
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::RasterImage::unchecked_write "
virtual void GeoCal::RasterImage::unchecked_write(int Line, int Sample, int Val)=0
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::RasterImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RasterImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::RasterImage::~RasterImage "
virtual GeoCal::RasterImage::~RasterImage()
";

// File: a00154.xml

%feature("docstring") GeoCal::GeoCalCore::RasterImage "
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::interpolate "
double GeoCal::GeoCalCore::RasterImage::interpolate(double Line, double Sample) const
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::interpolate "
double GeoCal::GeoCalCore::RasterImage::interpolate(const ImageCoor &Ic) const
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::number_line "
int GeoCal::GeoCalCore::RasterImage::number_line() const
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::number_sample "
int GeoCal::GeoCalCore::RasterImage::number_sample() const
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::number_tile_line "
virtual int GeoCal::GeoCalCore::RasterImage::number_tile_line() const
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::number_tile_sample "
virtual int GeoCal::GeoCalCore::RasterImage::number_tile_sample() const
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::RasterImage "
GeoCal::GeoCalCore::RasterImage::RasterImage(int Number_line, int Number_sample)
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::RasterImage "
GeoCal::GeoCalCore::RasterImage::RasterImage()
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::subset "
virtual void GeoCal::GeoCalCore::RasterImage::subset(int Lstart, int Sstart, int Number_line, int Number_sample, Array2d<
int > &Res) const =0
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::write "
virtual void GeoCal::GeoCalCore::RasterImage::write(int Line, int Sample, int Val)=0
";
%feature("docstring")  GeoCal::GeoCalCore::RasterImage::~RasterImage "
virtual GeoCal::GeoCalCore::RasterImage::~RasterImage()
";

// File: a00155.xml

%feature("docstring") GeoCal::RasterImageMultiBand "
This is a set of RasterImages, one for each band.

C++ includes: raster_image_multi_band.h ";

%feature("docstring")  GeoCal::RasterImageMultiBand::number_band "
virtual int GeoCal::RasterImageMultiBand::number_band() const =0
Return number of bands. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::print "
virtual void GeoCal::RasterImageMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::RasterImageMultiBand::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBand::raster_image(int band) const =0
Return given band. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBand::raster_image(int band)=0
";
%feature("docstring")  GeoCal::RasterImageMultiBand::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBand::raster_image_ptr(int band) const =0
";
%feature("docstring")  GeoCal::RasterImageMultiBand::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::read_double "
Array< double, 3 > RasterImageMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";

%feature("docstring")  GeoCal::RasterImageMultiBand::~RasterImageMultiBand "
virtual GeoCal::RasterImageMultiBand::~RasterImageMultiBand()
";

// File: a00156.xml

%feature("docstring") GeoCal::RasterImageMultiBandVariable "
A common implementation of RasterImageMultiBand is to just store a
vector of RasterImage.

This class gives the common behavior.

C++ includes: raster_image_multi_band_variable.h ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::add_raster_image "
void GeoCal::RasterImageMultiBandVariable::add_raster_image(const boost::shared_ptr< RasterImage > &Ri)
";
%feature("docstring")  GeoCal::RasterImageMultiBandVariable::number_band "
virtual int GeoCal::RasterImageMultiBandVariable::number_band() const
Return number of bands. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::overview "
RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
Create a RasterImageMultiBandVariable by finding the overview (if any)
for each of the raster bands.

The bands will all be the same size, so if the overviews don't match
in size we don't use them. If we can't find overviews, then the
RasterImageMultiBandVariable will just point to the same data as this
object does. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::print "
virtual void GeoCal::RasterImageMultiBand::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::RasterImageMultiBandVariable::print_to_string "
std::string GeoCal::Printable< RasterImageMultiBand  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::raster_image "
virtual const RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band) const
Return given band. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::raster_image "
virtual RasterImage& GeoCal::RasterImageMultiBandVariable::raster_image(int band)
";
%feature("docstring")  GeoCal::RasterImageMultiBandVariable::raster_image_ptr "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBandVariable::raster_image_ptr(int band) const
";
%feature("docstring")  GeoCal::RasterImageMultiBandVariable::RasterImageMultiBandVariable "
GeoCal::RasterImageMultiBandVariable::RasterImageMultiBandVariable()
";
%feature("docstring")  GeoCal::RasterImageMultiBandVariable::RasterImageMultiBandVariable "
GeoCal::RasterImageMultiBandVariable::RasterImageMultiBandVariable(const std::vector< boost::shared_ptr< RasterImage > > &Raster_image)
";
%feature("docstring")  GeoCal::RasterImageMultiBandVariable::read "
Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::read_double "
Array< double, 3 > RasterImageMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
Read the data from the same location in each image, and put into an
array.

This will be number_band x number_line x number_sample. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::subset "
RasterImageMultiBandVariable RasterImageMultiBand::subset(int line_offset, int sample_offset, int nline, int nsamp) const
Create a RasterImageMultiBandVariable by taking the same
SubRasterImage of each band. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::write "
void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
Write the data to the same location in each image.

Data should be number_band() x nline x nsamp. ";

%feature("docstring")  GeoCal::RasterImageMultiBandVariable::~RasterImageMultiBandVariable "
virtual GeoCal::RasterImageMultiBandVariable::~RasterImageMultiBandVariable()
";

// File: a00157.xml

%feature("docstring") GeoCal::RasterImageTiledFile "
This is a RasterImage that is implemented as a TiledFile<T, 2>.

C++ includes: raster_image_tiled_file.h ";

%feature("docstring")  GeoCal::RasterImageTiledFile::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::flush "
virtual void GeoCal::RasterImageTiledFile::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::number_swap "
unsigned int GeoCal::RasterImageTiledFile::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::RasterImageTiledFile::number_tile "
int GeoCal::RasterImageTiledFile::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::RasterImageTiledFile "
GeoCal::RasterImageTiledFile::RasterImageTiledFile(const boost::shared_ptr< TiledFileBase< 2 > > &D)
Constructor. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::RasterImageTiledFile "
GeoCal::RasterImageTiledFile::RasterImageTiledFile(const boost::shared_ptr< TiledFileBase< 2 > > &D, const MapInfo &M)
";
%feature("docstring")  GeoCal::RasterImageTiledFile::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::read_double "
virtual blitz::Array<double, 2> GeoCal::RasterImageTiledFile::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::read_ptr "
virtual void GeoCal::RasterImageTiledFile::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::reset_number_swap "
void GeoCal::RasterImageTiledFile::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::tile_file_base "
const TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base() const
Underlying data. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::tile_file_base "
TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base()
Underlying data. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::tile_file_base_ptr "
const boost::shared_ptr<TiledFileBase<2> >& GeoCal::RasterImageTiledFile::tile_file_base_ptr() const
Underlying data. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::unchecked_read "
virtual int GeoCal::RasterImageTiledFile::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::unchecked_write "
virtual void GeoCal::RasterImageTiledFile::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RasterImageTiledFile::write_ptr "
virtual void GeoCal::RasterImageTiledFile::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageTiledFile::~RasterImageTiledFile "
virtual GeoCal::RasterImageTiledFile::~RasterImageTiledFile()
";

// File: a00158.xml

%feature("docstring") GeoCal::RasterImageTileIterator "
It can be useful to step through a Raster Image in tiles.

This is a helper class for RasterImage to do this.

C++ includes: raster_image.h ";

%feature("docstring")  GeoCal::RasterImageTileIterator::end "
bool GeoCal::RasterImageTileIterator::end() const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::iend "
int GeoCal::RasterImageTileIterator::iend() const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::istart "
int GeoCal::RasterImageTileIterator::istart() const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::jend "
int GeoCal::RasterImageTileIterator::jend() const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::jstart "
int GeoCal::RasterImageTileIterator::jstart() const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::number_line "
int GeoCal::RasterImageTileIterator::number_line() const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::number_sample "
int GeoCal::RasterImageTileIterator::number_sample() const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::print "
void GeoCal::RasterImageTileIterator::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::RasterImageTileIterator::print_to_string "
std::string GeoCal::Printable< RasterImageTileIterator  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterImageTileIterator::RasterImageTileIterator "
RasterImageTileIterator::RasterImageTileIterator(const RasterImage &Ri)
Constructor. ";

%feature("docstring")  GeoCal::RasterImageTileIterator::RasterImageTileIterator "
RasterImageTileIterator::RasterImageTileIterator(const RasterImage &Ri, int Number_tile_line, int Number_tile_sample)
Constructor, where we supply a tile size to use (which may be
different than Ri's tile size). ";


// File: a00159.xml

%feature("docstring") GeoCal::RasterImageVariable "
Much of the time the constructor for a RasterImage determine the size,
map_info, and tile size.

This helper class holds these values in protected member function
variables and fills in the corresponding functions.

C++ includes: raster_image_variable.h ";

%feature("docstring")  GeoCal::RasterImageVariable::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RasterImageVariable::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RasterImageVariable::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RasterImageVariable::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageVariable::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageVariable::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageVariable::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterImageVariable::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RasterImageVariable::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RasterImageVariable::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterImageVariable::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RasterImageVariable::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterImageVariable::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RasterImageVariable::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RasterImageVariable::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RasterImageVariable::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RasterImageVariable::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterImageVariable::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterImageVariable::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RasterImageVariable::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RasterImageVariable::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterImageVariable::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RasterImageVariable::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageVariable::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageVariable::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageVariable::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RasterImageVariable::read_ptr "
virtual void GeoCal::RasterImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const =0
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterImageVariable::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::RasterImageVariable::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RasterImageVariable::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RasterImageVariable::unchecked_read "
virtual int GeoCal::RasterImage::unchecked_read(int Line, int Sample) const =0
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::RasterImageVariable::unchecked_write "
virtual void GeoCal::RasterImage::unchecked_write(int Line, int Sample, int Val)=0
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::RasterImageVariable::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterImageVariable::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RasterImageVariable::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::RasterImageVariable::~RasterImageVariable "
virtual GeoCal::RasterImageVariable::~RasterImageVariable()
";

// File: a00160.xml

%feature("docstring") GeoCal::RasterImageWithOverlay "
This is a simple widget.

Because I'm not really sure what the right abstractions are yet, this
is purposely limited in what it can do. I imagine we'll completely
replace this with something more general once we figure out what we
are trying to do.

C++ includes: raster_image_with_overlay.h ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::alpha_overlay "
void RasterImageWithOverlay::alpha_overlay(double alpha)
Update alpha of overlay. Alpha should be 0 to 100.0. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::blue_band "
int GeoCal::RasterImageWithOverlay::blue_band() const
Band in the blue channel for the image. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::calculate_overlay "
virtual void GeoCal::RasterImageWithOverlay::calculate_overlay()
Calculate overlay.

The default just calls calculate_overlay_value followed by
val_to_color. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::calculate_overlay_value "
void RasterImageWithOverlay::calculate_overlay_value()
Calculate the overlay value. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::color_value "
void RasterImageWithOverlay::color_value(int v, unsigned char r, unsigned char g, unsigned char b)
Assign color to given value.

This is used by val_to_color. A value of 0 is always assigned to
transparent black, so you can't override that value with this routine.
";

%feature("docstring")  GeoCal::RasterImageWithOverlay::green_band "
int GeoCal::RasterImageWithOverlay::green_band() const
Band in the green channel for the image. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::print "
virtual void GeoCal::RasterImageWithOverlay::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::RasterImageWithOverlay::RasterImageWithOverlay "
RasterImageWithOverlay::RasterImageWithOverlay(const boost::shared_ptr< RasterImageMultiBand > &Data, const
boost::shared_ptr< DisplayStretch > &Ds, int Red_band=1, int
Green_band=2, int Blue_band=3)
Constructor. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::red_band "
int GeoCal::RasterImageWithOverlay::red_band() const
Band in the red channel for the image. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::set_band "
void RasterImageWithOverlay::set_band(int rb, int gb, int bb)
Set the bands to view. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::set_data "
void RasterImageWithOverlay::set_data(const boost::shared_ptr< RasterImageMultiBand > &Data_new, bool
do_calculate_overlay=true)
Change the raster band we are using. This recalculates everything. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::update_overlay "
void GeoCal::RasterImageWithOverlay::update_overlay()
Update the overlay image.

You need to call this after change red, blue, green, or alpha (the
change to the image isn't automatic). ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::val_to_color "
void RasterImageWithOverlay::val_to_color()
It is often convenient to calculate the overlay as one of a value
(e.g., one value for CLOUD, one value for WATER).

This uses the values found in color_map and val to assign data to the
red, green and blue channels ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::widget "
GtkWidget* GeoCal::RasterImageWithOverlay::widget()
Return underlying widget. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::zoom_factor "
double RasterImageWithOverlay::zoom_factor() const
Return zoom factor. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::zoom_factor "
void RasterImageWithOverlay::zoom_factor(double zf)
Set zoom factor. ";

%feature("docstring")  GeoCal::RasterImageWithOverlay::~RasterImageWithOverlay "
virtual GeoCal::RasterImageWithOverlay::~RasterImageWithOverlay()
";

// File: a00161.xml

%feature("docstring") RasterImageWrapClosest "
";
%feature("docstring")  RasterImageWrapClosest::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  RasterImageWrapClosest::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  RasterImageWrapClosest::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  RasterImageWrapClosest::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapClosest::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapClosest::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapClosest::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapClosest::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  RasterImageWrapClosest::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  RasterImageWrapClosest::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapClosest::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  RasterImageWrapClosest::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapClosest::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  RasterImageWrapClosest::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  RasterImageWrapClosest::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  RasterImageWrapClosest::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  RasterImageWrapClosest::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapClosest::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapClosest::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  RasterImageWrapClosest::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  RasterImageWrapClosest::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  RasterImageWrapClosest::RasterImageWrapClosest "
RasterImageWrapClosest::RasterImageWrapClosest(const MaterialDetect &M)
";
%feature("docstring")  RasterImageWrapClosest::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  RasterImageWrapClosest::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapClosest::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapClosest::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapClosest::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  RasterImageWrapClosest::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapClosest::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  RasterImageWrapClosest::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  RasterImageWrapClosest::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  RasterImageWrapClosest::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapClosest::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapClosest::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  RasterImageWrapClosest::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  RasterImageWrapClosest::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  RasterImageWrapClosest::~RasterImageWrapClosest "
virtual RasterImageWrapClosest::~RasterImageWrapClosest()
";

// File: a00162.xml

%feature("docstring") RasterImageWrapCvdNorm "
";
%feature("docstring")  RasterImageWrapCvdNorm::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  RasterImageWrapCvdNorm::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  RasterImageWrapCvdNorm::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  RasterImageWrapCvdNorm::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapCvdNorm::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapCvdNorm::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapCvdNorm::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapCvdNorm::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  RasterImageWrapCvdNorm::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  RasterImageWrapCvdNorm::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapCvdNorm::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  RasterImageWrapCvdNorm::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapCvdNorm::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  RasterImageWrapCvdNorm::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  RasterImageWrapCvdNorm::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  RasterImageWrapCvdNorm::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  RasterImageWrapCvdNorm::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapCvdNorm::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapCvdNorm::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  RasterImageWrapCvdNorm::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  RasterImageWrapCvdNorm::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  RasterImageWrapCvdNorm::RasterImageWrapCvdNorm "
RasterImageWrapCvdNorm::RasterImageWrapCvdNorm(const DoughnutAverage &Davg, int Band)
";
%feature("docstring")  RasterImageWrapCvdNorm::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  RasterImageWrapCvdNorm::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapCvdNorm::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapCvdNorm::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapCvdNorm::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  RasterImageWrapCvdNorm::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapCvdNorm::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  RasterImageWrapCvdNorm::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  RasterImageWrapCvdNorm::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  RasterImageWrapCvdNorm::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapCvdNorm::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapCvdNorm::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  RasterImageWrapCvdNorm::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  RasterImageWrapCvdNorm::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  RasterImageWrapCvdNorm::~RasterImageWrapCvdNorm "
virtual RasterImageWrapCvdNorm::~RasterImageWrapCvdNorm()
";

// File: a00163.xml

%feature("docstring") RasterImageWrapMaterial "
";
%feature("docstring")  RasterImageWrapMaterial::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  RasterImageWrapMaterial::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  RasterImageWrapMaterial::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  RasterImageWrapMaterial::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapMaterial::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapMaterial::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapMaterial::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapMaterial::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  RasterImageWrapMaterial::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  RasterImageWrapMaterial::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapMaterial::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  RasterImageWrapMaterial::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapMaterial::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  RasterImageWrapMaterial::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  RasterImageWrapMaterial::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  RasterImageWrapMaterial::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  RasterImageWrapMaterial::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapMaterial::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapMaterial::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  RasterImageWrapMaterial::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  RasterImageWrapMaterial::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  RasterImageWrapMaterial::RasterImageWrapMaterial "
RasterImageWrapMaterial::RasterImageWrapMaterial(const MaterialDetect &M, int Class_id)
";
%feature("docstring")  RasterImageWrapMaterial::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  RasterImageWrapMaterial::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapMaterial::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapMaterial::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapMaterial::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  RasterImageWrapMaterial::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapMaterial::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  RasterImageWrapMaterial::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  RasterImageWrapMaterial::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  RasterImageWrapMaterial::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapMaterial::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapMaterial::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  RasterImageWrapMaterial::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  RasterImageWrapMaterial::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  RasterImageWrapMaterial::~RasterImageWrapMaterial "
virtual RasterImageWrapMaterial::~RasterImageWrapMaterial()
";

// File: a00164.xml

%feature("docstring") RasterImageWrapPandif "
";
%feature("docstring")  RasterImageWrapPandif::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  RasterImageWrapPandif::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  RasterImageWrapPandif::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  RasterImageWrapPandif::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapPandif::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapPandif::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapPandif::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  RasterImageWrapPandif::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  RasterImageWrapPandif::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  RasterImageWrapPandif::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapPandif::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  RasterImageWrapPandif::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  RasterImageWrapPandif::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  RasterImageWrapPandif::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  RasterImageWrapPandif::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  RasterImageWrapPandif::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  RasterImageWrapPandif::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapPandif::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  RasterImageWrapPandif::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  RasterImageWrapPandif::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  RasterImageWrapPandif::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  RasterImageWrapPandif::RasterImageWrapPandif "
RasterImageWrapPandif::RasterImageWrapPandif(const DoughnutAverage &Davg)
";
%feature("docstring")  RasterImageWrapPandif::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  RasterImageWrapPandif::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapPandif::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapPandif::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapPandif::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  RasterImageWrapPandif::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  RasterImageWrapPandif::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  RasterImageWrapPandif::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  RasterImageWrapPandif::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  RasterImageWrapPandif::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapPandif::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  RasterImageWrapPandif::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  RasterImageWrapPandif::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  RasterImageWrapPandif::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  RasterImageWrapPandif::~RasterImageWrapPandif "
virtual RasterImageWrapPandif::~RasterImageWrapPandif()
";

// File: a00165.xml

%feature("docstring") GeoCal::RasterMultifile "
There are certain larger data sets that are made up of a number of
separate files all in the same map projection that cover a large area.

For example, the SRTM data available through AFIDS. This class
captures the common functionality needed to deal with those files. The
data is stitched together by either reading one file or the other,
there is no smoothing done by this class between neighboring files.

Note that if you are creating a derived class, LocationToFile may be a
useful class to use.

In general, the area covered by this RasterMultifile will not be fully
covered by the separate files (e.g., there are tiles missing in the
SRTM). Depending on the application, trying to read data where we
don't have a file will either be an error, or acceptable and we want
to return a fill value instead. To handle this, derived classes return
a RasterMultifileTile with data = 0. Then, depending on the value of
No_coverage_is_error we either let the exception be thrown or we catch
it and substitute a fill value for the attempted read.

C++ includes: raster_multifile.h ";

%feature("docstring")  GeoCal::RasterMultifile::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RasterMultifile::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RasterMultifile::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RasterMultifile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterMultifile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterMultifile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterMultifile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterMultifile::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RasterMultifile::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RasterMultifile::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterMultifile::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RasterMultifile::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterMultifile::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RasterMultifile::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RasterMultifile::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RasterMultifile::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RasterMultifile::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterMultifile::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterMultifile::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RasterMultifile::print "
virtual void GeoCal::RasterMultifile::print(std::ostream &Os) const
Write to a stream. ";

%feature("docstring")  GeoCal::RasterMultifile::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterMultifile::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RasterMultifile::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterMultifile::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterMultifile::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterMultifile::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RasterMultifile::read_ptr "
void RasterMultifile::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image.

Read a subset of the data. ";

%feature("docstring")  GeoCal::RasterMultifile::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::RasterMultifile::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RasterMultifile::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RasterMultifile::unchecked_read "
virtual int GeoCal::RasterMultifile::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::RasterMultifile::unchecked_write "
virtual void GeoCal::RasterMultifile::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterMultifile::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterMultifile::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RasterMultifile::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::RasterMultifile::~RasterMultifile "
virtual GeoCal::RasterMultifile::~RasterMultifile()
Destructor. ";


// File: a00166.xml

%feature("docstring") GeoCal::RasterMultifileTile "
This is a single file used by RasterMultifile.

C++ includes: raster_multifile.h ";

%feature("docstring")  GeoCal::RasterMultifileTile::in_tile "
bool GeoCal::RasterMultifileTile::in_tile(int Line, int Sample) const
Return true if Line and Sample is in this file, false otherwise. ";

%feature("docstring")  GeoCal::RasterMultifileTile::RasterMultifileTile "
GeoCal::RasterMultifileTile::RasterMultifileTile()
Default constructor. ";

%feature("docstring")  GeoCal::RasterMultifileTile::RasterMultifileTile "
GeoCal::RasterMultifileTile::RasterMultifileTile(const boost::shared_ptr< RasterImage > &d, int loff, int soff)
Constructor. ";

%feature("docstring")  GeoCal::RasterMultifileTile::unchecked_read "
int GeoCal::RasterMultifileTile::unchecked_read(int Line, int Sample) const
Read data. ";

%feature("docstring")  GeoCal::RasterMultifileTile::unchecked_write "
void GeoCal::RasterMultifileTile::unchecked_write(int Line, int Sample, int Val) const
Write data. ";


// File: a00167.xml

%feature("docstring") GeoCal::RasterSubSample "
This subsamples a higher resolution RasterImage into a lower
resolution one.

Typically the pixel subsampling factor doesn't exactly divide the
number of lines and samples of the full resolution data. We just trim
the data to the largest low resolution pixel that completely fits in
the higher resolution data.

C++ includes: raster_subsample.h ";

%feature("docstring")  GeoCal::RasterSubSample::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RasterSubSample::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RasterSubSample::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RasterSubSample::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterSubSample::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterSubSample::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterSubSample::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RasterSubSample::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RasterSubSample::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RasterSubSample::high_resolution_image "
const RasterImage& GeoCal::RasterSubSample::high_resolution_image() const
High resolution image that this object is based on. ";

%feature("docstring")  GeoCal::RasterSubSample::high_resolution_image_ptr "
const boost::shared_ptr<RasterImage>& GeoCal::RasterSubSample::high_resolution_image_ptr() const
Pointer to high resolution image that this object is based on. ";

%feature("docstring")  GeoCal::RasterSubSample::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterSubSample::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RasterSubSample::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RasterSubSample::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RasterSubSample::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RasterSubSample::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RasterSubSample::number_line_per_pixel "
int GeoCal::RasterSubSample::number_line_per_pixel() const
Number of lines of high resolution data per pixel of this lower
resolution RasterImage. ";

%feature("docstring")  GeoCal::RasterSubSample::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RasterSubSample::number_sample_per_pixel "
int GeoCal::RasterSubSample::number_sample_per_pixel() const
Number of samples of high resolution data per pixel of this lower
resolution RasterImage. ";

%feature("docstring")  GeoCal::RasterSubSample::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterSubSample::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RasterSubSample::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RasterSubSample::print "
void RasterSubSample::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RasterSubSample::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RasterSubSample::RasterSubSample "
RasterSubSample::RasterSubSample(const boost::shared_ptr< RasterImage > &Data, int
Number_line_per_pixel, int Number_sample_per_pixel)
Constructor. ";

%feature("docstring")  GeoCal::RasterSubSample::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RasterSubSample::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterSubSample::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterSubSample::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterSubSample::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RasterSubSample::read_ptr "
void RasterSubSample::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RasterSubSample::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::RasterSubSample::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RasterSubSample::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RasterSubSample::unchecked_read "
virtual int GeoCal::RasterSubSample::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::RasterSubSample::unchecked_write "
void RasterSubSample::unchecked_write(int Line, int Sample, int Val)
Write a value.

This isn't actually implemented for this type of RasterImage. ";

%feature("docstring")  GeoCal::RasterSubSample::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RasterSubSample::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RasterSubSample::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::RasterSubSample::~RasterSubSample "
virtual GeoCal::RasterSubSample::~RasterSubSample()
";

// File: a00168.xml

%feature("docstring") GeoCal::RawRasterImage "
This uses libraw to read RAW format.

This is the format used by some cameras, e.g., Cannon RAW format. You
can see the details about what can be read athttp://www.libraw.org.

Note that although this reads the RAW format, the data is in the RAW
space, the DN values don't mean much without translating them to RGB
color space. You may want to instead use the program dcraw which
converts to PPM or Tiff format and includes translating to color plus
making 8 bit.

C++ includes: raw_raster_image.h ";

%feature("docstring")  GeoCal::RawRasterImage::band "
int GeoCal::RawRasterImage::band() const
";
%feature("docstring")  GeoCal::RawRasterImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RawRasterImage::file_name "
const std::string& GeoCal::RawRasterImage::file_name() const
";
%feature("docstring")  GeoCal::RawRasterImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RawRasterImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RawRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RawRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RawRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RawRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RawRasterImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RawRasterImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RawRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RawRasterImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RawRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RawRasterImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RawRasterImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RawRasterImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RawRasterImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RawRasterImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RawRasterImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RawRasterImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RawRasterImage::print "
void RawRasterImage::print(std::ostream &Os) const
Print to an output stream. ";

%feature("docstring")  GeoCal::RawRasterImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RawRasterImage::RawRasterImage "
RawRasterImage::RawRasterImage(const std::string &Fname, int Band=0)
Read in the given file with the given Band (0 based). ";

%feature("docstring")  GeoCal::RawRasterImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RawRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RawRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RawRasterImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RawRasterImage::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RawRasterImage::read_ptr "
virtual void GeoCal::RawRasterImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RawRasterImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::RawRasterImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RawRasterImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RawRasterImage::unchecked_read "
virtual int GeoCal::RawRasterImage::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::RawRasterImage::unchecked_write "
virtual void GeoCal::RawRasterImage::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RawRasterImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RawRasterImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RawRasterImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::RawRasterImage::~RawRasterImage "
virtual GeoCal::RawRasterImage::~RawRasterImage()
";
%feature("docstring")  GeoCal::RawRasterImage::read_all "
std::vector< boost::shared_ptr< RasterImage > > RawRasterImage::read_all(const std::string &Fname)
These read all three bands of a file at once.

This is faster than reading each band separately. ";


// File: a00169.xml

%feature("docstring") GeoCal::RayIntersect "
This finds the point closest intersection of two look vector.

Note that in generate two look vectors don't intersect, we find the
point closest to the two look vectors.

C++ includes: ray_intersect.h ";

%feature("docstring")  GeoCal::RayIntersect::image_ground_connection1 "
boost::shared_ptr<ImageGroundConnection> GeoCal::RayIntersect::image_ground_connection1() const
";
%feature("docstring")  GeoCal::RayIntersect::image_ground_connection2 "
boost::shared_ptr<ImageGroundConnection> GeoCal::RayIntersect::image_ground_connection2() const
";
%feature("docstring")  GeoCal::RayIntersect::print "
virtual void GeoCal::RayIntersect::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RayIntersect::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RayIntersect::RayIntersect "
GeoCal::RayIntersect::RayIntersect(const boost::shared_ptr< ImageGroundConnection > Igc1, const
boost::shared_ptr< ImageGroundConnection > Igc2)
";
%feature("docstring")  GeoCal::RayIntersect::two_ray_intersect "
void GeoCal::RayIntersect::two_ray_intersect(const ImageCoordinate &Ic1, const ImageCoordinate &Ic2,
boost::shared_ptr< CartesianFixed > &P, double &Dist)
Return intersection of look vector corresponding to two image
coordinates.

This is the point closest to both look vectors, along with the
distance of that point from each look vector. ";

%feature("docstring")  GeoCal::RayIntersect::two_look_vector_intersect "
void RayIntersect::two_look_vector_intersect(const CartesianFixed &P1, const CartesianFixedLookVector &L1, const
CartesianFixed &P2, const CartesianFixedLookVector &L2,
boost::shared_ptr< CartesianFixed > &P, double &Dist)
";

// File: a00170.xml

%feature("docstring") GeoCal::Refraction "
This calculates refraction.

The algorithm used was take from the SDP toolkit. The original code
was written by Peter Noerdlinger as the SDP toolkit function
PGS_CSC_SpaceRefract. The algorithm is described in detail in
\"Theoretical Basis of the SDP Toolkit   Geolocation Package for the
ECS\".

This depends on the index of refraction of air at the surface. We
don't actually know this. There are various approximations, and the
toolkit uses one described in the \"Theoretical Basis of the SDP
Toolkit   Geolocation Package for the ECS\".

This class allows the index of refraction to simple be given. This
allows us to just fit for this unknown using something like a
simultanous bundle adjustment. Note that a reasonable range of values
is something like 1.00026 to 1.00029, so you can vary from the initial
prediction by a few 1e-5.

C++ includes: refraction.h ";

%feature("docstring")  GeoCal::Refraction::altitude "
double GeoCal::Refraction::altitude() const
Altitude of reference point, in meters. ";

%feature("docstring")  GeoCal::Refraction::displacement "
double Refraction::displacement(double Space_zenith) const
Displacement in meters for given space zenith angle in radians. ";

%feature("docstring")  GeoCal::Refraction::index_refraction_surface "
double GeoCal::Refraction::index_refraction_surface() const
Return the current index of refraction at the surface. ";

%feature("docstring")  GeoCal::Refraction::index_refraction_surface "
void GeoCal::Refraction::index_refraction_surface(double Index_refraction_surface)
Set the index of refraction. ";

%feature("docstring")  GeoCal::Refraction::latitude "
double GeoCal::Refraction::latitude() const
Latitude of reference point, in degrees. ";

%feature("docstring")  GeoCal::Refraction::print "
virtual void GeoCal::Refraction::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::Refraction::print_to_string "
std::string GeoCal::Printable< Refraction  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Refraction::Refraction "
Refraction::Refraction(double Altitude, double Latitude, double Index_refraction_surface=-1)
This sets up for a Refraction calculation.

A representative altitude (in meters) and latitude (in degrees) is
given. This doesn't depend very strongly on altitude and latitude, so
unless you using widely varying points just using the center of you
scene should be pretty much sufficient.

The index of refraction at the surface can be supplied if desired,
otherwise we calculate an approximate value. ";

%feature("docstring")  GeoCal::Refraction::refraction_apply "
boost::shared_ptr< GroundCoordinate > Refraction::refraction_apply(const GroundCoordinate &Spacecraft_pos, const GroundCoordinate
&Gc_no_refraction) const
Apply refraction.

We take the position of the spacecraft and the ground coordinates
before apply refraction. This return the ground coordinates accounting
for refraction. ";

%feature("docstring")  GeoCal::Refraction::refraction_reverse "
boost::shared_ptr< GroundCoordinate > Refraction::refraction_reverse(const GroundCoordinate &Spacecraft_pos, const GroundCoordinate
&Gc_with_refraction) const
Reverse the refraction calculation.

This returns a ground coordinate so that when we feed it to
refraction_apply we get back the original ground coordinate. This is
useful when trying to go the other way, from the ground to the
spacecraft, so we remove the effects of refraction and then continue
with a normal orbit/camera model.

Note for speed we don't calculate the exact reverse of
refraction_apply, but a close approximation. For a MISR DF camera, the
difference is only 2 cm. For most applications, the difference
shouldn't matter. But if we have an application where this does, we'll
want to replace this with a more exact calculation. ";

%feature("docstring")  GeoCal::Refraction::surface_zenith "
double GeoCal::Refraction::surface_zenith(double Space_zenith) const
Surface zenith angle in radians, given the space zenith angles. ";

%feature("docstring")  GeoCal::Refraction::~Refraction "
virtual GeoCal::Refraction::~Refraction()
";

// File: a01203.xml

%feature("docstring") std::map::reverse_iterator "
STL iterator class. ";


// File: a01210.xml

%feature("docstring") std::wstring::reverse_iterator "
STL iterator class. ";


// File: a01211.xml

%feature("docstring") std::set::reverse_iterator "
STL iterator class. ";


// File: a01157.xml

%feature("docstring") std::basic_string::reverse_iterator "
STL iterator class. ";


// File: a01180.xml

%feature("docstring") std::deque::reverse_iterator "
STL iterator class. ";


// File: a01212.xml

%feature("docstring") std::string::reverse_iterator "
STL iterator class. ";


// File: a01205.xml

%feature("docstring") std::multimap::reverse_iterator "
STL iterator class. ";


// File: a01213.xml

%feature("docstring") std::vector::reverse_iterator "
STL iterator class. ";


// File: a01207.xml

%feature("docstring") std::multiset::reverse_iterator "
STL iterator class. ";


// File: a01201.xml

%feature("docstring") std::list::reverse_iterator "
STL iterator class. ";


// File: a00171.xml

%feature("docstring") GeoCal::Rpc "
This is used to handle RPC (Rational Polynomial Coefficients).

RPC is an alternative to using a rigorous camera/orbit model to map
project satellite data.

This is a common technique, and there are numerous references. One
reference is Fraser, CS, Dial, G, Grodecki, J \"Sensor orientation via
RPCs\" ISPRS J PHOTOGRAMM 60 (3): 182-194 MAY 2006.

Note that there are two versions of the RPCs, type \"A\" and type
\"B\". The difference is the order of the terms. The military tends to
use \"A\" and the rest of the world tends to use \"B\". This class
supports both types, based on the setting of the rpc_type variable.

By definition of RPC, the leading term of the denominator coefficient
should be 1.0 - so there are 20 degrees of freedom for the numerator
but only 19 for the denominator. After some debate, I decided to do
nothing in this class to enforce this restriction, you can assign a
value other than 1 to the first coefficient of the denominator.
However, this class will complain when you actually try to use this.

The definition of line, sample and height offset and scale allow these
numbers to be floats. However file formats such as geotiff and NITF
treat these as integers. If you store a RPC into a file with one of
these formats, the data will be truncated. If you plan on saving to
one of these formats, you should be aware of this and keep these an
whole numbers. This limitation is not present in the VICAR file
format.

C++ includes: geocal_rpc.h ";

%feature("docstring")  GeoCal::Rpc::fit "
void Rpc::fit(const std::vector< boost::shared_ptr< GroundCoordinate > > &Gc, const
std::vector< ImageCoordinate > &Ic, const std::vector< double >
&Line_sigma, const std::vector< double > &Sample_sigma, std::set< int
> &Blunder, double Blunder_threshold=3, double Chisq_threshold=0.8,
bool Blunder_detect=true)
This updates the RPC parameters to better match a given set of data.

This class takes a list of GroundCoordinates, and the ImageCoordinate
that should be returned by the RPC. Each of the ImageCoordinate has
and associated line and sample sigma used to weight it (i.e., the line
and sigma returned by ImageMatcher).

We update the set of parameters given by fit_line_numerator and
fit_sample_numerator. Right now, we only do a linear fit and don't try
to fit the nonlinear denominator pieces. We can add that in the future
if it is needed.

The blunder detection used is iterative data snooping, where the point
with the largest standardized residual is removed. If the Chisq of the
match improves by the Chisq_threshold, we toss the point out,
otherwise we keep it. We then proceed to the next largest residual,
and so.

However, if we expect large numbers of blunders in otherwise well
matched data (e.g., image matching between two images with lots of
differences), it might be preferable to toss out all large outliers.
So, if Chisq_threshold is >= 1 we just toss out all points outside of
the Blunder_threshold. ";

%feature("docstring")  GeoCal::Rpc::fit_all "
void Rpc::fit_all(const std::vector< double > &Line, const std::vector< double >
&Sample, const std::vector< double > &Latitude, const std::vector<
double > &Longitude, const std::vector< double > &Height)
You may not want to use this function directly, but rather use
Rpc::generate_rpc which sets up the entire RPC.

This is a low level function used to create an RPC that matches a set
of data. The RPC should have already had the various offset and scales
filled in. This function then fills in line_denominator,
line_numerator, sample_denominator, and sample_numerator.

This fits all of the rpc parameters (the full 78 parameters) doing a
linear least squares fit. This doesn't to any blunder detection, or
allow a subset to be used.

The idea is that through some other method you can determine latitude,
longitude, height vs line, sample (e.g., using a Orbit and Camera).
You then generate a regular set of points, set up the offset and
scale, and finally call this function to fit the RPC. ";

%feature("docstring")  GeoCal::Rpc::fit_coarse "
void Rpc::fit_coarse(const std::vector< boost::shared_ptr< GroundCoordinate > > &Gc, const
std::vector< ImageCoordinate > &Ic)
This does a coarse fit to the RPC parameters to better match a given
set of data.

This class takes a list of GroundCoordinates, and the ImageCoordinate
that should be returned by the RPC.

This does an coarse fit, changing just the line_offset and
sample_offset. This is used to get the RPC roughly right so we can do
a more careful fit using fit(). ";

%feature("docstring")  GeoCal::Rpc::ground_coordinate "
Geodetic Rpc::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Invert the RPC to find the ground coordinate that maps to a give
ImageCoordinate.

This routine may fail to find a solution, in which case a
ConvergenceFailure exception will be thrown. ";

%feature("docstring")  GeoCal::Rpc::image_coordinate "
ImageCoordinate GeoCal::Rpc::image_coordinate(const GroundCoordinate &Gc) const
Use the RPC to convert from a GroundCoordinate to image space. ";

%feature("docstring")  GeoCal::Rpc::image_coordinate "
ImageCoordinate GeoCal::Rpc::image_coordinate(double Latitude, double Longitude, double Height_ellipsoid) const
Convert from object space to image space (i.e., calculate RPC).

The longitude and latitude are given in degrees (this is the geodetic
latitude). The height is in meters, relative to the reference
ellipsoid (e.g., WGS-84). ";

%feature("docstring")  GeoCal::Rpc::image_coordinate "
blitz::Array<double, N + 1> GeoCal::Rpc::image_coordinate(const blitz::Array< double, N > &Latitude, const blitz::Array<
double, N > &Longitude, const blitz::Array< double, N >
&Height_ellipsoid) const
Convert from object space to image space (i.e., calculate RPC).

The longitude and latitude are given in degrees (this is the geodetic
latitude). The height is in meters, relative to the reference
ellipsoid (e.g., WGS-84).

This converts a whole block of latitude, longitude and height points
at once. The results returned have a first dimension of 2, the first
value is line and the second sample. ";

%feature("docstring")  GeoCal::Rpc::image_coordinate_jac "
blitz::Array<double, 2> GeoCal::Rpc::image_coordinate_jac(double Latitude, double Longitude, double Height_ellipsoid) const
Jacobian of image_coordinate with respect to the Latitude, Longitude,
and Height_ellipsoid.

This is a 2 x 3 matrix, with the first row being line and the second
sample. The columns are in Latitude, Longitude and Height_ellipsoid
order. ";

%feature("docstring")  GeoCal::Rpc::image_coordinate_jac_parm "
Array< double, 2 > Rpc::image_coordinate_jac_parm(double Latitude, double Longitude, double Height_ellipsoid) const
Jacobian of image_coordinate with respect to the parameters that are
marked as ones we are fitting.

This has two rows, the first is for line and the second sample. The
columns are first the true values in fit_line_numerator, and then
fit_sample_numerator. ";

%feature("docstring")  GeoCal::Rpc::image_coordinate_jac_parm "
blitz::Array<double, 2> GeoCal::Rpc::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Jacobian of image_coordinate with respect to the parameters that are
marked as ones we are fitting.

This has two rows, the first is for line and the second sample. The
columns are first the true values in fit_line_numerator, and then
fit_sample_numerator. ";

%feature("docstring")  GeoCal::Rpc::print "
void Rpc::print(std::ostream &Os) const
Print a Rpc to a stream. ";

%feature("docstring")  GeoCal::Rpc::print_to_string "
std::string GeoCal::Printable< Rpc  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Rpc::resolution_meter "
double Rpc::resolution_meter(const Dem &D) const
Calculate the approximate resolution on the ground.

This finds the intersection for the center pixel with the surface + 1
in the line and sample direction. We find the difference in meters
between these points, and select the maximum value. We the height from
the Dem. ";

%feature("docstring")  GeoCal::Rpc::Rpc "
Rpc::Rpc()
Default constructor.

Initializes fit_line_numerator and fit_sample_numerator to all false.
";

%feature("docstring")  GeoCal::Rpc::rpc_project "
void Rpc::rpc_project(RasterImage &Res, const RasterImage &Img, const Dem &D, double
Line_scale=1.0, double Sample_scale=1.0) const
This uses the RPC to project a RasterImage to a MapInfo.

On entry, the RasterImage Res should already have the size and MapInfo
that we want to project to.

In some cases, Img might be a lower resolution image than is described
by the RPC (e.g., we are imaging matching 60 cm Quickbird data to 5 m
CIB data, so we down sample by a factor of 9). In those cases, a scale
to apply to the line and sample returned by the RPC before looking
into Img can be given. The default value is to not scale. ";

%feature("docstring")  GeoCal::Rpc::rpc_type_a "
Rpc Rpc::rpc_type_a() const
This return a Rpc that has type RPC_A.

If this rpc is already type RPC_A, then this is just a copy. Otherwise
we rearrange the coefficients to give a type RPC_A. ";

%feature("docstring")  GeoCal::Rpc::rpc_type_b "
Rpc Rpc::rpc_type_b() const
This return a Rpc that has type RPC_B.

If this rpc is already type RPC_B, then this is just a copy. Otherwise
we rearrange the coefficients to give a type RPC_B. ";

%feature("docstring")  GeoCal::Rpc::generate_rpc "
Rpc Rpc::generate_rpc(const ImageGroundConnection &Igc, double Min_height, double
Max_height, int Nlat=20, int Nlon=20, int Nheight=20)
Generate a RPC that approximates the calculation done by a
ImageGroundConnection.

We determine that latitude and longitude range to use automatically to
cover the range given by the ImageGroundConnection. ";


// File: a00172.xml

%feature("docstring") GeoCal::RpcImage "
This handles both an Rpc, and the RasterImage data the Rpc map
projects.

Some of the data we work with is high resolution (e.g., Quickbird at
60 cm). For some purposes, we may want to work with a lower resolution
version (e.g., image match with CIB data at 5 m). This class allows
the resolution of the map projection to be given, in which case we
will average the underlying RasterImage using RasterAveraged, before
doing the map projection.

We calculate imagery on demand. This means the imagery is always up to
date, so changes to the Rpc get reflected in the imagery. However, if
you are going to be accessing the same data more than once w/o
updating the Rpc, you'll want to generate a static copy so you don't
repeatedly calculate the same data over and over e.g., use a
MemoryRasterImage to copy the data into memory.

As you update the Rpc parameters, the area covered on the surface
changes. This class does not update the MapInfo describing the area
covered. However, you can call map_info_bounding_update() to update
this information if desired.

C++ includes: rpc_image.h ";

%feature("docstring")  GeoCal::RpcImage::blunder "
const std::set<int> GeoCal::RpcImage::blunder() const
Points in the last iteration of fit() that were marked as blunders. ";

%feature("docstring")  GeoCal::RpcImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::RpcImage::dem "
const Dem& GeoCal::RpcImage::dem() const
DEM. ";

%feature("docstring")  GeoCal::RpcImage::dem_ptr "
boost::shared_ptr<Dem> GeoCal::RpcImage::dem_ptr() const
";
%feature("docstring")  GeoCal::RpcImage::distance_new_and_reference "
const std::vector<double>& GeoCal::RpcImage::distance_new_and_reference() const
Return distance in meters between reference_image_ground_point and
new_image_ground_point, for diagnostic purposes.

This is a measure of how much the images disagree after the fitting
done by fit(). Note that you should also look at blunder, as some of
these points might be marked as blunders. ";

%feature("docstring")  GeoCal::RpcImage::failed_ground_point "
const std::vector<boost::shared_ptr<GroundCoordinate> >& GeoCal::RpcImage::failed_ground_point() const
Return the location in the reference image used in the final iteration
of fit, for diagnostic purposes.

This point had image matching fail on it, see image_match_diagnostic
for code as to why it failed. ";

%feature("docstring")  GeoCal::RpcImage::fit "
void RpcImage::fit(const RasterImage &Ref_img, double Max_diff=1000, const Mask
&M=CombinedMask(), const FeatureDetector
&Fd=ForstnerFeatureDetector(), const ImageMatcher
&Coarse_im=CcorrMatcher(), const ImageMatcher
&Fine_im=CcorrLsmMatcher(true), double Match_search=15, int
Feature_grid_space=100, double Blunder_threshold=3, double
Chisq_threshold=1.0, bool Blunder_detect=true)
This does image matching with a Ref_img, and adjusts the Rpc
coefficients to give a better match between this map projected imagery
and the reference imagery.

This is done in two step. First we do a coarse fit to get the map
projected roughly right, and then we do a finer adjustment to give a
better fit.

The first step is the coarse fit. Since an Rpc is usually based on
navigation data only, without and ground control points, it can give
relatively large errors. We do a coarse shift to the Rpc to reduce the
errors enough that we can do a finer fit against the Ref_img.

We first averages the image and the Ref_img. We average so that we are
coarse enough that the given image matching search window size (given
by Match_search) will contain errors up to Max_diff meters. For
example, with a 1000 m Max_diff, a search window of Match_search, and
5 m Ref_img, we would average by a factor of 14 (5 m x 14 factor x 15
search window = 1050 m error, allowing us to find 1000 m differences).

We use the FeatureDetector Fd to determine where to match, the areas
we look for a feature is in each grid square of Feature_grid_space x
Feature_grid_space. We then match at these features between the
reference and map projected images. We use the supplied Mask to mask
out areas that we should not do matching on (e.g., water, clouds). The
default Mask doesn't mask any areas.

We update the line_offset and sample_offset only of rpc() to reduce
the difference between the map projected image and the Ref_img. This
functions does not fit the other parameters given by
rpc().fit_line_numerator and rpc().fit_sample_numerator.

We repeat the coarse fit until the remaining differences between the
map projected data and the Ref_img fall within Match_search at full
resolution.

We follow this with a fine fit. We again use the FeatureDetector Fd to
determine where to match, the areas we look for a feature is in each
grid square of Feature_grid_space x Feature_grid_space. We then match
at these features between the reference and map projected images.

We fit for the Rpc parameters given by rpc().fit_line_numerator and
rpc().fit_sample_numerator.

This fine matching step is repeated until the change in average
difference from one step to the next is less than a tolerance (5%
change). ";

%feature("docstring")  GeoCal::RpcImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::RpcImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::RpcImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RpcImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RpcImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RpcImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::RpcImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::RpcImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::RpcImage::image_match_diagnostic "
const std::vector<int>& GeoCal::RpcImage::image_match_diagnostic() const
Return the code indicating why an image matching failed. ";

%feature("docstring")  GeoCal::RpcImage::image_match_succesful_diagnostic "
const std::vector<int>& GeoCal::RpcImage::image_match_succesful_diagnostic() const
Return the code indicating any diagnostics for points that matched
successfully. ";

%feature("docstring")  GeoCal::RpcImage::img_avg "
const RasterImage& GeoCal::RpcImage::img_avg() const
";
%feature("docstring")  GeoCal::RpcImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RpcImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::RpcImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::RpcImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::RpcImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::RpcImage::map_info_bounding "
MapInfo RpcImage::map_info_bounding() const
Create a MapInfo that completely covers projection of the raw_image to
the surface using the current Rpc values.

This used the same CoordinateConverter, pixel_x_resolution, and
pixel_y_resolution as the current map_info() - we just change the
corners of the MapInfo to contain the data. Note that this just
calculates the MapInfo, it doesn't actually set the map_info() to this
new value. For that, use map_info_bounding_update. ";

%feature("docstring")  GeoCal::RpcImage::map_info_bounding_update "
void RpcImage::map_info_bounding_update()
This calls map_info_bounding(), and then sets the map_info() for this
object to the results of that call. ";

%feature("docstring")  GeoCal::RpcImage::new_image_ground_point "
const std::vector<boost::shared_ptr<GroundCoordinate> >& GeoCal::RpcImage::new_image_ground_point() const
Return the location in the new image used in the final iteration of
fit, for diagnostic purposes.

Note that you should also look at blunder, as some of these points
might be marked as blunders. ";

%feature("docstring")  GeoCal::RpcImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::RpcImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::RpcImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RpcImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::RpcImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::RpcImage::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RpcImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RpcImage::raw_image "
const RasterImage& GeoCal::RpcImage::raw_image() const
Image that is being map projected. This is in the instrument space. ";

%feature("docstring")  GeoCal::RpcImage::raw_image_ptr "
boost::shared_ptr<RasterImage> GeoCal::RpcImage::raw_image_ptr() const
";
%feature("docstring")  GeoCal::RpcImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::RpcImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RpcImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RpcImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RpcImage::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::RpcImage::read_ptr "
void RpcImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::RpcImage::reference_image_ground_point "
const std::vector<boost::shared_ptr<GroundCoordinate> >& GeoCal::RpcImage::reference_image_ground_point() const
Return the location in the reference image used in the final iteration
of fit, for diagnostic purposes.

Note that you should also look at blunder, as some of these points
might be marked as blunders. ";

%feature("docstring")  GeoCal::RpcImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::RpcImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::RpcImage::RpcImage "
RpcImage::RpcImage(const boost::shared_ptr< RasterImage > &Raw_img, const Rpc &R, const
boost::shared_ptr< Dem > &D, const MapInfo &Mi, int Avg_fact=-1)
Constructor.

We either average the raw data by the given factor, or we take the
ratio of the Mi resolution and the Rpc resolution. ";

%feature("docstring")  GeoCal::RpcImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::RpcImage::unchecked_read "
virtual int GeoCal::RpcImage::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample.

It is ok if the Line, Sample results in a raw image location outside
of the image, in that case we return 0. ";

%feature("docstring")  GeoCal::RpcImage::unchecked_write "
void RpcImage::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RpcImage::value "
double RpcImage::value(int Line, int Sample) const
Return pixel value at given line and sample.

This is like operator(), but this returns a double rather than an int.
";

%feature("docstring")  GeoCal::RpcImage::value "
double RpcImage::value(int Line, int Sample, double Height) const
Variation of value that takes a height above the reference surface
rather than using the Dem associated with this class. ";

%feature("docstring")  GeoCal::RpcImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::RpcImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::RpcImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::RpcImage::~RpcImage "
virtual GeoCal::RpcImage::~RpcImage()
Destructor. ";


// File: a00173.xml

%feature("docstring") GeoCal::RpcImageGroundConnection "
This is a ImageGroundConnection where the connection is made by a Rpc.

C++ includes: rpc_image_ground_connection.h ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::cf_look_vector "
void RpcImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
boost::shared_ptr< CartesianFixed > &P) const
Return look vector for given coordinate, along with a position that
lies along the direction of the look vector (so position of satellite,
or a position on the surface. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::cover "
MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
Find a MapInfo that covers the ground coordinate of this
ImageGroundConnection.

We calculate the ground coordinate of the four corners, then find the
MapInfo that covers those corners, optionally adding a boundary in map
pixels. Depending on the actual warping involved, this may or may not
fully cover the edges of the image. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::dem "
void GeoCal::ImageGroundConnection::dem(const boost::shared_ptr< Dem > &D)
Set the DEM to use in ground_coordinate. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::dem "
const Dem& GeoCal::ImageGroundConnection::dem() const
Dem used by ground_coordinate. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::dem_ptr "
const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
DEM used by ground_coordinate. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::ground_coordinate "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
Return ground coordinate that goes with a particular image coordinate.
";

%feature("docstring")  GeoCal::RpcImageGroundConnection::ground_coordinate_dem "
virtual boost::shared_ptr<GroundCoordinate> GeoCal::RpcImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const
Return ground coordinate that goes with a particular image coordinate.

This version supplies a Dem to use. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::image "
virtual boost::shared_ptr<RasterImage> GeoCal::ImageGroundConnection::image() const
Underlying image. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::image_coordinate "
virtual ImageCoordinate GeoCal::RpcImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const
Return image coordinate that goes with a particular GroundCoordinate.

For some types of ImageGroundConnection, we might not be able to
calculate image_coordinate for all values (e.g., Ipi might fail). In
those cases, we will through a ImageGroundConnectionFailed exception.
This means that nothing is wrong, other than that we can't calculate
the image_coordinate. Callers can catch this exception if they have
some way of handling no image coordinate data. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::image_coordinate_jac_ecr "
Array< double, 2 > RpcImageGroundConnection::image_coordinate_jac_ecr(const Ecr &Gc) const
Return the Jacobian of the image coordinates with respect to the X, Y,
and Z components of the Ecr ground location. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::image_coordinate_jac_parm "
virtual blitz::Array<double, 2> GeoCal::RpcImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
Return the Jacobian of the image coordinates with respect to the
parameters. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::number_line "
virtual int GeoCal::ImageGroundConnection::number_line() const
Return number of lines in underlying image. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::number_sample "
virtual int GeoCal::ImageGroundConnection::number_sample() const
Return number of samples in underlying image. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::parameter "
Array< double, 1 > RpcImageGroundConnection::parameter() const
A image to ground connection may depend on a set of parameters, which
can by modified (e.g., during a simultaneous bundle adjustment).

This returns those parameters. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::parameter "
virtual void GeoCal::RpcImageGroundConnection::parameter(const blitz::Array< double, 1 > &Parm)
Set the value of the parameters. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::parameter_name "
std::vector< std::string > RpcImageGroundConnection::parameter_name() const
Descriptive name of each parameter. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::print "
virtual void GeoCal::RpcImageGroundConnection::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::print_to_string "
std::string GeoCal::Printable< ImageGroundConnection  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter(const ImageCoordinate &Ic) const
Calculate the approximate resolution on the ground of a given
ImageCoordinate.

This finds the intersection with the reference surface for the given
pixel, + 1 in the line and sample direction. We find the difference in
meters between these points, and select the maximum value. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::resolution_meter "
double ImageGroundConnection::resolution_meter() const
Variation of resolution_meter that find the resolution of the center
pixel. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::rpc "
const Rpc& GeoCal::RpcImageGroundConnection::rpc() const
RPC that we are using. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::rpc "
Rpc& GeoCal::RpcImageGroundConnection::rpc()
RPC that we are using. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::rpc_ptr "
boost::shared_ptr<Rpc> GeoCal::RpcImageGroundConnection::rpc_ptr() const
";
%feature("docstring")  GeoCal::RpcImageGroundConnection::RpcImageGroundConnection "
GeoCal::RpcImageGroundConnection::RpcImageGroundConnection(const Rpc &R, const boost::shared_ptr< Dem > &D, const
boost::shared_ptr< RasterImage > &Img, const std::string
&Title=\"Image\")
";
%feature("docstring")  GeoCal::RpcImageGroundConnection::title "
virtual std::string GeoCal::ImageGroundConnection::title() const
Title that we can use to describe the image.

This can be any string that is useful as a label. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::title "
virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
Set image title. ";

%feature("docstring")  GeoCal::RpcImageGroundConnection::~RpcImageGroundConnection "
virtual GeoCal::RpcImageGroundConnection::~RpcImageGroundConnection()
Destructor. ";


// File: a01184.xml

%feature("docstring") std::runtime_error "
STL class. ";


// File: a00174.xml

%feature("docstring") GeoCal::ScaleImage "
This is a simple adapter that multiples a underlying image by a scale
factor.

C++ includes: scale_image.h ";

%feature("docstring")  GeoCal::ScaleImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::ScaleImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::ScaleImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::ScaleImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ScaleImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ScaleImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ScaleImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::ScaleImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::ScaleImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::ScaleImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::ScaleImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::ScaleImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::ScaleImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::ScaleImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::ScaleImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::ScaleImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::ScaleImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::ScaleImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::ScaleImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::ScaleImage::print "
virtual void GeoCal::ScaleImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::ScaleImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ScaleImage::raw_data "
const boost::shared_ptr<RasterImage>& GeoCal::ScaleImage::raw_data() const
";
%feature("docstring")  GeoCal::ScaleImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::ScaleImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ScaleImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ScaleImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ScaleImage::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::ScaleImage::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::ScaleImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::ScaleImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::ScaleImage::scale_factor "
double GeoCal::ScaleImage::scale_factor() const
";
%feature("docstring")  GeoCal::ScaleImage::ScaleImage "
GeoCal::ScaleImage::ScaleImage(const boost::shared_ptr< RasterImage > &Data, double Scale_factor)
";
%feature("docstring")  GeoCal::ScaleImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::ScaleImage::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::ScaleImage::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::ScaleImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::ScaleImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::ScaleImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::ScaleImage::~ScaleImage "
virtual GeoCal::ScaleImage::~ScaleImage()
";

// File: a00175.xml

%feature("docstring") GeoCal::ScLookVector "
This is a look vector in an spacecraft coordinates.

C++ includes: look_vector.h ";

%feature("docstring")  GeoCal::ScLookVector::direction "
boost::array< double, 3 > LookVector::direction() const
This is the direction, as a unit vector. ";

%feature("docstring")  GeoCal::ScLookVector::length "
double LookVector::length() const
";
%feature("docstring")  GeoCal::ScLookVector::look_quaternion "
boost::math::quaternion<double> GeoCal::LookVector::look_quaternion() const
Look vector as a quaternion. ";

%feature("docstring")  GeoCal::ScLookVector::look_quaternion "
void GeoCal::LookVector::look_quaternion(const boost::math::quaternion< double > &V)
Set look vector using a quaternion. ";

%feature("docstring")  GeoCal::ScLookVector::print "
void ScLookVector::print(std::ostream &Os) const
Print to given stream. ";

%feature("docstring")  GeoCal::ScLookVector::print_to_string "
std::string GeoCal::Printable< LookVector  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::ScLookVector::ScLookVector "
GeoCal::ScLookVector::ScLookVector()
Default constructor. Does not initialize look_vector. ";

%feature("docstring")  GeoCal::ScLookVector::ScLookVector "
GeoCal::ScLookVector::ScLookVector(const boost::array< double, 3 > &Lv)
Constructor. ";

%feature("docstring")  GeoCal::ScLookVector::ScLookVector "
GeoCal::ScLookVector::ScLookVector(double x, double y, double z)
Constructor. ";

%feature("docstring")  GeoCal::ScLookVector::~ScLookVector "
virtual GeoCal::ScLookVector::~ScLookVector()
";

// File: a00176.xml

%feature("docstring") GeoCal::SdpHelper "
This class centralizes all of the SDP library calls.

This is the toolkit used by the EOS missions. This class provides one
place to modify if we ever need to replace the dependency on this
library. The library is available
fromhttp://newsroom.gsfc.nasa.gov/sdptoolkit/toolkit.html.

To use the SDP toolkit, you need to supply a Process Control File
(PCF) and set the environment variable PGS_PC_INFO_FILE. See the
toolkit documentation for details, available
athttp://edhs1.gsfc.nasa.gov/waisdata/toc/cd33360503toc.html.

C++ includes: sdp_helper.h ";

%feature("docstring")  GeoCal::SdpHelper::convert_to_eci "
void SdpHelper::convert_to_eci(const Time &T, const CartesianFixed &From, CartesianInertial &To)
This converts from Ecr to Eci. ";

%feature("docstring")  GeoCal::SdpHelper::convert_to_ecr "
void SdpHelper::convert_to_ecr(const Time &T, double Ci_to_cf[3][3])
This converts from Eci to Ecr. ";

%feature("docstring")  GeoCal::SdpHelper::convert_to_ecr "
void SdpHelper::convert_to_ecr(const Time &T, const CartesianInertial &From, CartesianFixed &To)
This converts from Eci to Ecr. ";

%feature("docstring")  GeoCal::SdpHelper::parse_time "
Time SdpHelper::parse_time(const std::string &Time_string)
This converts CCSDS ASCII time format (e.g.,
\"1996-07-03T04:13:57.987654Z\") to Time. ";

%feature("docstring")  GeoCal::SdpHelper::sdp_available "
bool SdpHelper::sdp_available()
Return true if we have SDP functionality available, false otherwise.
";

%feature("docstring")  GeoCal::SdpHelper::to_string "
std::string SdpHelper::to_string(const Time &T)
This converts Time to CCSDS ASCII time format (e.g.,
\"1996-07-03T04:13:57.987654Z\"). ";


// File: a00177.xml

%feature("docstring") GeoCal::SdpNotAvailableException "
Exception thrown if SDP Toolkit isn't available.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::SdpNotAvailableException::SdpNotAvailableException "
GeoCal::SdpNotAvailableException::SdpNotAvailableException()
Constructor. ";

%feature("docstring")  GeoCal::SdpNotAvailableException::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::SdpNotAvailableException::~SdpNotAvailableException "
virtual GeoCal::SdpNotAvailableException::~SdpNotAvailableException()
 throw ()
Destructor.
";

// File: a00178.xml

%feature("docstring") GeoCal::SdpToolkitCoordinateInterface "
";
%feature("docstring")  GeoCal::SdpToolkitCoordinateInterface::to_fixed "
void SdpToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, double Ci_to_cf[3][3])
Calculate matrix to converts from CartesianInertial to CartesianFixed
for the given body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). Note that the SDP toolkit only can do this for earth, any
other body will trigger an error. ";

%feature("docstring")  GeoCal::SdpToolkitCoordinateInterface::to_fixed "
void SdpToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, const CartesianInertial &From,
CartesianFixed &To)
This converts from CartesianInertial to CartesianFixed for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). Note that the SDP toolkit only can do this for earth, any
other body will trigger an error. ";

%feature("docstring")  GeoCal::SdpToolkitCoordinateInterface::to_inertial "
void SdpToolkitCoordinateInterface::to_inertial(int Body_id, const Time &T, const CartesianFixed &From,
CartesianInertial &To)
This converts from CartesianFixed to CartesianInertial for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). Note that the SDP toolkit only can do this for earth, any
other body will trigger an error. ";

%feature("docstring")  GeoCal::SdpToolkitCoordinateInterface::~SdpToolkitCoordinateInterface "
virtual GeoCal::SdpToolkitCoordinateInterface::~SdpToolkitCoordinateInterface()
";

// File: a00179.xml

%feature("docstring") GeoCal::SdpToolkitTimeInterface "
";
%feature("docstring")  GeoCal::SdpToolkitTimeInterface::parse_time "
Time SdpToolkitTimeInterface::parse_time(const std::string Time_string) const
Parse string to get a Time.

The specific formats that are recognized depends on the toolkit used,
but they all support CCSDS ASCII format (e.g.,
\"1996-07-03T04:13:57.987654Z\") ";

%feature("docstring")  GeoCal::SdpToolkitTimeInterface::to_string "
std::string SdpToolkitTimeInterface::to_string(const Time &T) const
Generate CCSDS format of time (e.g., \"1996-07-03T04:13:57.987654Z\")
";

%feature("docstring")  GeoCal::SdpToolkitTimeInterface::~SdpToolkitTimeInterface "
virtual GeoCal::SdpToolkitTimeInterface::~SdpToolkitTimeInterface()
";

// File: a01214.xml

%feature("docstring") std::set "
STL class. ";


// File: a00180.xml

%feature("docstring") GeoCal::SimpleDatum "
This is a Datum which returns a constant undulation everywhere.

This is useful for testing, and also in the case you are using a Dem
with height already relative to the reference surface, so no Datum
correction is needed.

C++ includes: geocal_datum.h ";

%feature("docstring")  GeoCal::SimpleDatum::print "
virtual void GeoCal::SimpleDatum::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::SimpleDatum::print_to_string "
std::string GeoCal::Printable< Datum  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SimpleDatum::SimpleDatum "
GeoCal::SimpleDatum::SimpleDatum(double U=0.0)
Constructor. ";

%feature("docstring")  GeoCal::SimpleDatum::u "
double GeoCal::SimpleDatum::u() const
Fixed value that is returned by this class. ";

%feature("docstring")  GeoCal::SimpleDatum::undulation "
virtual double GeoCal::SimpleDatum::undulation(const GroundCoordinate &Gc) const
Undulation, which is the distance form mean sea level to the reference
ellipsoid, for the given ground location.

This is in meters. This is always 0 for this class. ";

%feature("docstring")  GeoCal::SimpleDatum::~SimpleDatum "
virtual GeoCal::SimpleDatum::~SimpleDatum()
Destructor. ";


// File: a00181.xml

%feature("docstring") GeoCal::SimpleDem "
This is a simple implementation of a Dem, intended primarily for use
during testing.

It find the height of a given point above the WGS84 ellipsoid + fixed
height, in geodetic coordinates.

C++ includes: simple_dem.h ";

%feature("docstring")  GeoCal::SimpleDem::distance_to_surface "
double SimpleDem::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::SimpleDem::h "
double GeoCal::SimpleDem::h() const
Return height value used by this object. ";

%feature("docstring")  GeoCal::SimpleDem::h "
void GeoCal::SimpleDem::h(double Hnew)
Set height value used by this object. ";

%feature("docstring")  GeoCal::SimpleDem::height_reference_surface "
virtual double GeoCal::SimpleDem::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::SimpleDem::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::SimpleDem::print "
virtual void GeoCal::SimpleDem::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::SimpleDem::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SimpleDem::SimpleDem "
GeoCal::SimpleDem::SimpleDem(double H=0)
Default constructor. ";

%feature("docstring")  GeoCal::SimpleDem::surface_point "
boost::shared_ptr< GroundCoordinate > SimpleDem::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::SimpleDem::~SimpleDem "
virtual GeoCal::SimpleDem::~SimpleDem()
Destructor. ";


// File: a00182.xml

%feature("docstring") GeoCal::SimplePushBroomCamera "
This is a simple PushBroomCamera.

It is not intended as a realistic camera model, but rather as
supplying a simple class to use for testing. The defaults are for the
nominal MISR DF camera, red band.

C++ includes: camera.h ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::beta "
double GeoCal::SimplePushBroomCamera::beta() const
";
%feature("docstring")  GeoCal::SimplePushBroomCamera::delta "
double GeoCal::SimplePushBroomCamera::delta() const
";
%feature("docstring")  GeoCal::SimplePushBroomCamera::direction "
virtual Camera::Direction GeoCal::SimplePushBroomCamera::direction() const
This gives the camera direction. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::epsilon "
double GeoCal::SimplePushBroomCamera::epsilon() const
";
%feature("docstring")  GeoCal::SimplePushBroomCamera::focal_length "
double GeoCal::SimplePushBroomCamera::focal_length() const
";
%feature("docstring")  GeoCal::SimplePushBroomCamera::frame_coordinate "
FrameCoordinate SimplePushBroomCamera::frame_coordinate(const ScLookVector &Sl, int Band) const
This converts from ScLookVector to FrameCoordinate for a given band.

Note that the FrameCoordinate may be outside of the range (0,
number_line(band) - 1), (0, number_sample(band) - 1), this just means
the look vector is not with the active portion of the camera (i.e., it
is not seen). ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::frame_line_coordinate "
virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::integration_time "
virtual double GeoCal::Camera::integration_time(int Band) const
This is the integration time in seconds.

The integration time is how long it takes the camera to collect data.
It is important to model this, so that we can include movement of the
spacecraft while the camera is in the middle of collecting data. If
the integration time is very short, or if a particular derived class
doesn't wish to model this, then the integration time can be set to 0
seconds.

The default version returns 0.0. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::line_pitch "
double GeoCal::SimplePushBroomCamera::line_pitch() const
";
%feature("docstring")  GeoCal::SimplePushBroomCamera::number_band "
virtual int GeoCal::SimplePushBroomCamera::number_band() const
Number of bands in camera. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::number_line "
virtual int GeoCal::PushBroomCamera::number_line(int Band) const
Number of lines in camera for given band. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::number_sample "
virtual int GeoCal::SimplePushBroomCamera::number_sample(int Band) const
Number of samples in camera for given band. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::print "
void SimplePushBroomCamera::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::print_to_string "
std::string GeoCal::Printable< Camera  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::sample_pitch "
double GeoCal::SimplePushBroomCamera::sample_pitch() const
";
%feature("docstring")  GeoCal::SimplePushBroomCamera::sc_look_vector "
ScLookVector SimplePushBroomCamera::sc_look_vector(const FrameCoordinate &F, int Band) const
Convert from FrameCoordinate to ScLookVector.

It is perfectly allowable for F.line to be outside the range (0,
number_line(band) 1) or for F.sample to be outside the range (0,
number_sample(band) - 1). The conversion will just act as if the
camera has infinite extent. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::SimplePushBroomCamera "
SimplePushBroomCamera::SimplePushBroomCamera(double Beta=58 *Constant::deg_to_rad, double Delta=-2.7
*Constant::deg_to_rad, double Epsilon=0, double Focal=123.8e-3, double
line_pitch=18e-6, double sample_pitch=21e-6, int Number_sample=1504)
This creates SimplePushBroomCamera with the given data.

The default data is for the nominal MISR DF, red band. ";

%feature("docstring")  GeoCal::SimplePushBroomCamera::~SimplePushBroomCamera "
virtual GeoCal::SimplePushBroomCamera::~SimplePushBroomCamera()
";

// File: a01215.xml

%feature("docstring") std::smart_ptr "
STL class. ";


// File: a00183.xml

%feature("docstring") GeoCal::SmoothImage "
This creates a smoothed version of a RasterImage.

We do this by averaging a given region of the image, average (2 * nhs
+ 1) x (2 * nhs + 1) pixels centered on the resulting pixel.

Rather than doing something complicated at the edges, we simply trim
the underlying image by a border of nhs.

C++ includes: smooth_image.h ";

%feature("docstring")  GeoCal::SmoothImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::SmoothImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::SmoothImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::SmoothImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SmoothImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SmoothImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SmoothImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SmoothImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::SmoothImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::SmoothImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::SmoothImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::SmoothImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::SmoothImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::SmoothImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::SmoothImage::number_half_size "
int GeoCal::SmoothImage::number_half_size() const
";
%feature("docstring")  GeoCal::SmoothImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::SmoothImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::SmoothImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::SmoothImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::SmoothImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::SmoothImage::print "
virtual void GeoCal::SmoothImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::SmoothImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SmoothImage::raw_data_ptr "
const boost::shared_ptr<RasterImage>& GeoCal::SmoothImage::raw_data_ptr() const
";
%feature("docstring")  GeoCal::SmoothImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::SmoothImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SmoothImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SmoothImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SmoothImage::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::SmoothImage::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SmoothImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::SmoothImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::SmoothImage::SmoothImage "
SmoothImage::SmoothImage(const boost::shared_ptr< RasterImage > &Data, int Nhs=3, int
Number_tile=4)
Constructor.

We give the size of the area to average over by supplying the number
half size (Nhs), and then average over a region (2 * Nhs + 1) x (2 *
Nhs + 1) ";

%feature("docstring")  GeoCal::SmoothImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::SmoothImage::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::SmoothImage::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::SmoothImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::SmoothImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::SmoothImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::SmoothImage::~SmoothImage "
virtual GeoCal::SmoothImage::~SmoothImage()
";

// File: a00184.xml

%feature("docstring") GeoCal::SpiceException "
Exception thrown if Spice call fails.

C++ includes: spice_helper.h ";

%feature("docstring")  GeoCal::SpiceException::SpiceException "
SpiceException::SpiceException()
Spice exception.

This automatically gets the SPICE error message. As a side effect, we
also clear the error status of the SPICE library. ";

%feature("docstring")  GeoCal::SpiceException::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::SpiceException::~SpiceException "
virtual GeoCal::SpiceException::~SpiceException()
 throw ()
Destructor.
";

// File: a00185.xml

%feature("docstring") GeoCal::SpiceHelper "
This class centralizes all of the CSPICE calls from the CSPICE library
(available from NAIF athttp://naif.jpl.nasa.gov/naif/pds.html).

This class handles initializing CSPICE, and provides one place to
modify if we ever need to replace the dependency on this library.

SPICE depends on various kernels. We load the file found at
$SPICEDATA/geocal.ker. This file in turn loads a number of other
kernels, you can edit this file to change which kernels are used.

C++ includes: spice_helper.h ";

%feature("docstring")  GeoCal::SpiceHelper::cartesian_inertial_to_cartesian_fixed "
void SpiceHelper::cartesian_inertial_to_cartesian_fixed(int Body_id, const Time &T)
Return matrix that converts from CartesianInertial to CartesianFixed.

To avoid making a second copy, we return the data in SpiceHelper::m.
Since this is only called by SpiceToolkitCoordinateInterface, this
slightly awkward interface is worth the small performance advantage.
";

%feature("docstring")  GeoCal::SpiceHelper::conversion "
void SpiceHelper::conversion(const std::string &From, const std::string &To, const Time &T, const
boost::array< double, 3 > &pin, boost::array< double, 3 > &pout)
Convert from the given coordinate to the given coordinate. ";

%feature("docstring")  GeoCal::SpiceHelper::conversion_matrix "
void SpiceHelper::conversion_matrix(const std::string &From, const std::string &To, const Time &T)
Return matrix that converts between the two names coordinate system.

To avoid making a second copy, we return the data in SpiceHelper::m.
";

%feature("docstring")  GeoCal::SpiceHelper::et_to_geocal "
Time SpiceHelper::et_to_geocal(double Et)
Convert from Spice ET time to GeoCal::Time. ";

%feature("docstring")  GeoCal::SpiceHelper::geocal_to_et "
double SpiceHelper::geocal_to_et(const Time &T)
Convert from GeoCal::Time to Spice ET time. ";

%feature("docstring")  GeoCal::SpiceHelper::parse_time "
Time SpiceHelper::parse_time(const std::string &Time_string)
This is a wrapper around the SPICE call str2et_c that parses a string
to produce a time.

We convert the spice time to GeoCal::Time. See the spice documentation
for detailed information on the parse strings (seehttp://naif.jpl.nasa
.gov/pub/naif/toolkit_docs/C/cspice/str2et_c.html)

Note that str2et_c doesn't support the CCSDS ASCII time format (e.g.,
\"1996-07-03T04:13:57.987654Z\"). Because this is a format commonly
used in EOS missions, we add support for this format. This is little
more than a slight reformatting before calling str2et_c (e.g.,
\"1996-07-03 04:13:57.987654 UTC\" which the toolkit does support). ";

%feature("docstring")  GeoCal::SpiceHelper::spice_available "
bool SpiceHelper::spice_available()
Return true if we have SPICE functionality available, false otherwise.
";

%feature("docstring")  GeoCal::SpiceHelper::spice_error_check "
void SpiceHelper::spice_error_check()
Check if an error occurred in a SPICE call, and if so throw an
exception. ";

%feature("docstring")  GeoCal::SpiceHelper::spice_setup "
void SpiceHelper::spice_setup(const std::string &Kernel=\"geocal.ker\")
Set SPICE errors to just return, rather than aborting. ";

%feature("docstring")  GeoCal::SpiceHelper::to_string "
std::string SpiceHelper::to_string(const Time &T)
This converts Time to CCSDS ASCII time format (e.g.,
\"1996-07-03T04:13:57.987654Z\"). ";


// File: a00186.xml

%feature("docstring") GeoCal::SpiceNotAvailableException "
Exception thrown if Spice isn't available.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::SpiceNotAvailableException::SpiceNotAvailableException "
GeoCal::SpiceNotAvailableException::SpiceNotAvailableException()
Constructor. ";

%feature("docstring")  GeoCal::SpiceNotAvailableException::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::SpiceNotAvailableException::~SpiceNotAvailableException "
virtual GeoCal::SpiceNotAvailableException::~SpiceNotAvailableException()
 throw ()
Destructor.
";

// File: a00187.xml

%feature("docstring") GeoCal::SpiceToolkitCoordinateInterface "
";
%feature("docstring")  GeoCal::SpiceToolkitCoordinateInterface::to_fixed "
void SpiceToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, double Ci_to_cf[3][3])
This calculates a matrix from converting from CartesianInertial to
CartesianFixed for the given body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). ";

%feature("docstring")  GeoCal::SpiceToolkitCoordinateInterface::to_fixed "
void SpiceToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, const CartesianInertial &From,
CartesianFixed &To)
This converts from CartesianInertial to CartesianFixed for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). ";

%feature("docstring")  GeoCal::SpiceToolkitCoordinateInterface::to_inertial "
void SpiceToolkitCoordinateInterface::to_inertial(int Body_id, const Time &T, const CartesianFixed &From,
CartesianInertial &To)
This converts from CartesianFixed to CartesianInertial for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). ";

%feature("docstring")  GeoCal::SpiceToolkitCoordinateInterface::~SpiceToolkitCoordinateInterface "
virtual GeoCal::SpiceToolkitCoordinateInterface::~SpiceToolkitCoordinateInterface()
";

// File: a00188.xml

%feature("docstring") GeoCal::SpiceToolkitTimeInterface "
";
%feature("docstring")  GeoCal::SpiceToolkitTimeInterface::parse_time "
Time SpiceToolkitTimeInterface::parse_time(const std::string Time_string) const
Parse string to get a Time.

The specific formats that are recognized depends on the toolkit used,
but they all support CCSDS ASCII format (e.g.,
\"1996-07-03T04:13:57.987654Z\") ";

%feature("docstring")  GeoCal::SpiceToolkitTimeInterface::to_string "
std::string SpiceToolkitTimeInterface::to_string(const Time &T) const
Generate CCSDS format of time (e.g., \"1996-07-03T04:13:57.987654Z\")
";

%feature("docstring")  GeoCal::SpiceToolkitTimeInterface::~SpiceToolkitTimeInterface "
virtual GeoCal::SpiceToolkitTimeInterface::~SpiceToolkitTimeInterface()
";

// File: a00189.xml

%feature("docstring") GeoCal::SpotCamera "
This class models the SPOT camera.

The camera parameters are given by an DIMAP file. This file is an XML
file format. You can find documentation for this format
athttp://www.spot.com/web/SICORP/452-sicorp-the-dimap-format.php.

We don't actually read this file format. XML is a bit of a pain to
deal with in C++, and is much easier to process in Ruby. So we set up
an interface that just takes the PSI_X and PSI_Y values for each
pixel. The Afids class Dimap processes the XML file and can use that
to create an instance of this class.

The various angles and so forth are described in the \"SPOT Geometry
Handbook\", available athttp://www.spotimage.com/automne_modules_files
/standard/public/p229_0b
9c0d94a22e77aac09df2b360c73073SPOT_Geometry_Handbook.pdf

C++ includes: spot_camera.h ";

%feature("docstring")  GeoCal::SpotCamera::direction "
virtual Direction GeoCal::Camera::direction() const
This gives the camera direction.

This is intended for use with steep camera angles (e.g., MISR AF
camera). For cameras that are near nadir looking, we can just
arbitrarily pick a direction for it.

The default version returns FORWARD. ";

%feature("docstring")  GeoCal::SpotCamera::frame_coordinate "
FrameCoordinate SpotCamera::frame_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::SpotCamera::frame_line_coordinate "
virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
This is similar to frame_coordinate, except it only calculates the
line coordinate.

For some camera models, it is more expensive to calculate the sample
number than the line (e.g., MISR camera needs to use the pixel-theta
measurements). For functions that only need the line information, it
is wasteful to do the full calculation.

The default version of this function just calls frame_coordinate and
returns the line portion, but a derived class can override this
default with a faster way of calculation the line information. ";

%feature("docstring")  GeoCal::SpotCamera::integration_time "
virtual double GeoCal::Camera::integration_time(int Band) const
This is the integration time in seconds.

The integration time is how long it takes the camera to collect data.
It is important to model this, so that we can include movement of the
spacecraft while the camera is in the middle of collecting data. If
the integration time is very short, or if a particular derived class
doesn't wish to model this, then the integration time can be set to 0
seconds.

The default version returns 0.0. ";

%feature("docstring")  GeoCal::SpotCamera::number_band "
virtual int GeoCal::SpotCamera::number_band() const
Number of bands in camera. ";

%feature("docstring")  GeoCal::SpotCamera::number_line "
virtual int GeoCal::SpotCamera::number_line(int Band) const
Number of lines in camera for given band. ";

%feature("docstring")  GeoCal::SpotCamera::number_sample "
virtual int GeoCal::SpotCamera::number_sample(int Band) const
Number of samples in camera for given band. ";

%feature("docstring")  GeoCal::SpotCamera::print "
void SpotCamera::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::SpotCamera::print_to_string "
std::string GeoCal::Printable< Camera  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SpotCamera::psi_x "
const std::vector<double> GeoCal::SpotCamera::psi_x() const
";
%feature("docstring")  GeoCal::SpotCamera::psi_y "
const std::vector<double> GeoCal::SpotCamera::psi_y() const
";
%feature("docstring")  GeoCal::SpotCamera::sc_look_vector "
ScLookVector SpotCamera::sc_look_vector(const FrameCoordinate &F, int Band) const
Convert from FrameCoordinate to ScLookVector.

It is perfectly allowable for F.line to be outside the range (0,
number_line(band) 1) or for F.sample to be outside the range (0,
number_sample(band) - 1). The conversion will just act as if the
camera has infinite extent. ";

%feature("docstring")  GeoCal::SpotCamera::SpotCamera "
SpotCamera::SpotCamera(const std::vector< double > &Psi_x, const std::vector< double >
&Psi_y)
Create a spot camera with the given PSI angles.

Psi_x and Psi_y need to be equal in size, and have at least two
values.

The angles should be in radians.

We require that Psi_y be strictly increasing. ";

%feature("docstring")  GeoCal::SpotCamera::~SpotCamera "
virtual GeoCal::SpotCamera::~SpotCamera()
";

// File: a00190.xml

%feature("docstring") GeoCal::SpotOrbit "
This class models the SPOT orbit.

The orbit parameters are given by an DIMAP file. This file is an XML
file format. You can find documentation for this format
athttp://www.spot.com/web/SICORP/452-sicorp-the-dimap-format.php.

We don't actually read this file format. XML is a bit of a pain to
deal with in C++, and is much easier to process in Ruby. So we set up
an interface that just takes the Ephemeris and Attitude measurements.
The Afids class Dimap processes the XML file and can use that to
create an instance of this class.

The various angles and so forth are described in the \"SPOT Geometry
Handbook\", available athttp://www.spotimage.com/automne_modules_files
/standard/public/p229_0b
9c0d94a22e77aac09df2b360c73073SPOT_Geometry_Handbook.pdf

A note for a developer working with this class. The definition of
space craft and orbital coordinate used internally has a different
sign convention than we've used on other instruments such as MISR.

We use the conventions spelled out in the \"SPOT Geometry Handbook\"
consistently in this class, even though this is different than what is
used in the MISR ATB. In particular, the Z axis points from earth
center to spacecraft, not spacecraft to earth center as we've used on
MISR. This is all accounted for by this class, for the outside user
everything works as expected. But if you are working internally on
this class this might seem strange.

Note also that according to the \"SPOT Geometry Handbook\" the
reported attitudes are relative to -X, -Y, and Z axis. This means that
pitch and roll (but not yaw) have the opposite sign of what you would
expect. The Ypr given to this routine should match what is supplied by
SPOT in its DIMAP file - we account for the strange sign in this
class.

C++ includes: spot_orbit.h ";

%feature("docstring")  GeoCal::SpotOrbit::cf_look_vector "
virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianFixedLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::SpotOrbit::ci_look_vector "
virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
Convert from ScLookVector to CartesianInertialLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::SpotOrbit::frame_coordinate "
FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
const
Give the frame coordinates that a particular point on the ground is
seen. ";

%feature("docstring")  GeoCal::SpotOrbit::max_time "
Time GeoCal::Orbit::max_time() const
Maximum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::SpotOrbit::min_time "
Time GeoCal::Orbit::min_time() const
Minimum time that we can return OrbitData for. ";

%feature("docstring")  GeoCal::SpotOrbit::orbit_data "
boost::shared_ptr< OrbitData > SpotOrbit::orbit_data(Time T) const
Return orbit data for the given time. ";

%feature("docstring")  GeoCal::SpotOrbit::position_cf "
virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::SpotOrbit::position_ci "
virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
Return position at given time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::SpotOrbit::print "
void SpotOrbit::print(std::ostream &Os) const
Print to a stream. ";

%feature("docstring")  GeoCal::SpotOrbit::print_to_string "
std::string GeoCal::Printable< Orbit  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SpotOrbit::reference_surface_intersect_approximate "
boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
double Height_reference_surface=0.0) const
Return location on the reference surface that a particular frame
coordinate is seen.

This is approximate, in the same way
CartesianFixed::reference_intersect_approximate is approximate. ";

%feature("docstring")  GeoCal::SpotOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianInertialLookVector &Ci) const
Convert from CartesianInertialLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::SpotOrbit::sc_look_vector "
virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
Convert from CartesianFixedLookVector to ScLookVector for the given
time.

We should have min_time() <= T < max_time(). ";

%feature("docstring")  GeoCal::SpotOrbit::SpotOrbit "
SpotOrbit::SpotOrbit(const std::vector< Time > &Ephemeris_time, const blitz::Array<
double, 2 > &Ephemeris, const std::vector< Time > &Attitude_time,
const blitz::Array< double, 2 > &Ypr)
Constructor.

Parameters:
-----------

Ephemeris_time:   Time of each ephemeris point

Ephemeris:  Ephemeris at each time. This is a number_eph_time x 6
array. Each row has the position x, y, z and velocity x, y, z. This is
in meters and meter/second, in ECR coordinates.

Attitude_time:   Time of each attitude point.

Ypr:  Attitude. This is a number_att_time x 3 array. Each row has the
yaw, pitch and roll. This is in radians. This is YPR reported by SPOT
in the DIMAP file, and uses the conventions described in \"SPOT
Geometry Handbook\" ";

%feature("docstring")  GeoCal::SpotOrbit::velocity_ci "
virtual boost::array<double, 3> GeoCal::Orbit::velocity_ci(Time T) const
Return velocity at given time.

This is in m/s, in same coordinate system as position. ";

%feature("docstring")  GeoCal::SpotOrbit::~SpotOrbit "
virtual GeoCal::SpotOrbit::~SpotOrbit()
";

// File: a00191.xml

%feature("docstring") GeoCal::SrtmDem "
This class provides access to the SRTM.

Note comment from Tom about this data:

The srtm_filled data were prepared back in 2003. The goal was to
create a quick and dirty global data set, which means the data came
from a variety of sources, scales, datums, and accuracy. At that time,
a half pixel error was well below the threshold of concern (15-30 was
high resolution back then). So, point and area data were often mixed
in the void filling process, and some regional areas (not covered by
srtm) were carved into quadrangles and retained their individual pixel
types. I know its a mess, but the bottom line is that the data are not
accurate to half a pixel, which makes the 'point vs. area' discuss not
really relevant.

The SRTM VICAR files incorrectly label the data as \"pixel is point\",
when really this is \"pixel is area\". This matters, because it is a
1/2 pixel shift. The code overrides what the VICAR files says, and
treats it as \"area\" rather than \"pixel\".

C++ includes: srtm_dem.h ";

%feature("docstring")  GeoCal::SrtmDem::database_name "
const std::string& GeoCal::SrtmDem::database_name() const
Database name. ";

%feature("docstring")  GeoCal::SrtmDem::datum "
const Datum& GeoCal::DemMapInfo::datum() const
Datum height is relative to. ";

%feature("docstring")  GeoCal::SrtmDem::datum_ptr "
const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
Pointer to datum. ";

%feature("docstring")  GeoCal::SrtmDem::directory_base "
const std::string& GeoCal::SrtmDem::directory_base() const
Database base directory. ";

%feature("docstring")  GeoCal::SrtmDem::distance_to_surface "
double DemMapInfo::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::SrtmDem::elevation "
virtual double GeoCal::SrtmDem::elevation(int Y_index, int X_index) const
Return height in meters relative to datum(). ";

%feature("docstring")  GeoCal::SrtmDem::height_datum "
double DemMapInfo::height_datum(const GroundCoordinate &Gp) const
Height relative to datum. ";

%feature("docstring")  GeoCal::SrtmDem::height_reference_surface "
double DemMapInfo::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::SrtmDem::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::SrtmDem::map_info "
const MapInfo& GeoCal::DemMapInfo::map_info() const
MapInfo of underlying data. ";

%feature("docstring")  GeoCal::SrtmDem::outside_dem_is_error "
bool GeoCal::DemMapInfo::outside_dem_is_error() const
If true, then calling with values outside the range of area of the DEM
triggers an exception, otherwise we just return a height of 0. ";

%feature("docstring")  GeoCal::SrtmDem::print "
virtual void GeoCal::SrtmDem::print(std::ostream &Os) const
Write to a stream. ";

%feature("docstring")  GeoCal::SrtmDem::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SrtmDem::SrtmDem "
SrtmDem::SrtmDem(const std::string &Db_name=\"\", const std::string &Dirbase=\"\",
bool Outside_dem_is_error=true, const boost::shared_ptr< Datum >
&D=boost::shared_ptr< Datum >(new DatumGeoid96()))
Constructor.

You can provide the database file to use and the directory where there
data can be found. If you leave this as a blank string, we use
${ELEV_ROOT} as the directory and ${ELEV_ROOT}/L2_dem_db.int as the
file. ";

%feature("docstring")  GeoCal::SrtmDem::surface_point "
boost::shared_ptr< GroundCoordinate > DemMapInfo::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::SrtmDem::~SrtmDem "
virtual GeoCal::SrtmDem::~SrtmDem()
";

// File: a01216.xml

%feature("docstring") std::stack "
STL class. ";


// File: a00192.xml

%feature("docstring") GeoCal::Statistic "
This calculates basic statistics.

C++ includes: statistic.h ";

%feature("docstring")  GeoCal::Statistic::add "
void Statistic::add(double x)
Add a new data point. ";

%feature("docstring")  GeoCal::Statistic::count "
int GeoCal::Statistic::count() const
Number of points added. ";

%feature("docstring")  GeoCal::Statistic::max "
double GeoCal::Statistic::max() const
Maximum value encountered. ";

%feature("docstring")  GeoCal::Statistic::max_index "
int GeoCal::Statistic::max_index() const
Index number of maximum point, where the first point added is index 0,
second is 1 and so on. ";

%feature("docstring")  GeoCal::Statistic::mean "
double GeoCal::Statistic::mean() const
Mean value. ";

%feature("docstring")  GeoCal::Statistic::mean_abs "
double GeoCal::Statistic::mean_abs() const
Mean of the absolute value. ";

%feature("docstring")  GeoCal::Statistic::min "
double GeoCal::Statistic::min() const
Minimum value encountered. ";

%feature("docstring")  GeoCal::Statistic::min_index "
int GeoCal::Statistic::min_index() const
Index number of minimum point, where the first point added is index 0,
second is 1 and so on. ";

%feature("docstring")  GeoCal::Statistic::print "
void Statistic::print(std::ostream &Os) const
";
%feature("docstring")  GeoCal::Statistic::print_to_string "
std::string GeoCal::Printable< Statistic  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Statistic::sigma "
double GeoCal::Statistic::sigma() const
Standard deviation. ";

%feature("docstring")  GeoCal::Statistic::Statistic "
GeoCal::Statistic::Statistic()
Constructor. ";


// File: a00703.xml

%feature("docstring") std::string "
STL class. ";


// File: a01138.xml

%feature("docstring") std::stringstream "
STL class. ";


// File: a00193.xml

%feature("docstring") GeoCal::SubRasterImage "
This allows you to take a subset of a larger RasterImage and treat it
like a RasterImage in its own right.

C++ includes: sub_raster_image.h ";

%feature("docstring")  GeoCal::SubRasterImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::SubRasterImage::full_raster_image "
const RasterImage& GeoCal::SubRasterImage::full_raster_image() const
Underlying full raster image. ";

%feature("docstring")  GeoCal::SubRasterImage::full_raster_image "
RasterImage& GeoCal::SubRasterImage::full_raster_image()
Underlying full raster image. ";

%feature("docstring")  GeoCal::SubRasterImage::full_raster_image_ptr "
const boost::shared_ptr<RasterImage>& GeoCal::SubRasterImage::full_raster_image_ptr() const
Underlying full raster image. ";

%feature("docstring")  GeoCal::SubRasterImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::SubRasterImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::SubRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SubRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SubRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SubRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::SubRasterImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::SubRasterImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::SubRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::SubRasterImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::SubRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::SubRasterImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::SubRasterImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::SubRasterImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::SubRasterImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::SubRasterImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::SubRasterImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::SubRasterImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::SubRasterImage::print "
void SubRasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::SubRasterImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SubRasterImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::SubRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SubRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SubRasterImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SubRasterImage::read_double "
virtual blitz::Array<double, 2> GeoCal::SubRasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::SubRasterImage::read_ptr "
void SubRasterImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::SubRasterImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::SubRasterImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::SubRasterImage::start_line "
int GeoCal::SubRasterImage::start_line() const
Start line of underlying data. ";

%feature("docstring")  GeoCal::SubRasterImage::start_sample "
int GeoCal::SubRasterImage::start_sample() const
Start sample of underlying data. ";

%feature("docstring")  GeoCal::SubRasterImage::SubRasterImage "
SubRasterImage::SubRasterImage(const boost::shared_ptr< RasterImage > &Raster_image, int Start_line,
int Start_sample, int Number_line, int Number_sample)
Constructor. ";

%feature("docstring")  GeoCal::SubRasterImage::SubRasterImage "
SubRasterImage::SubRasterImage(const boost::shared_ptr< RasterImage > &Raster_image, const
std::vector< boost::shared_ptr< GroundCoordinate > > &Pt, int
boundary=0)
Create a subset of Raster_image that covers the given set of points,
plus a optional boundary.

Note that the area given by Pt may be large than the area covered by
Raster_image. In that case, we only give the portion that falls within
this image. ";

%feature("docstring")  GeoCal::SubRasterImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::SubRasterImage::unchecked_read "
int SubRasterImage::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::SubRasterImage::unchecked_write "
void SubRasterImage::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::SubRasterImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::SubRasterImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::SubRasterImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::SubRasterImage::~SubRasterImage "
virtual GeoCal::SubRasterImage::~SubRasterImage()
";

// File: a00194.xml

%feature("docstring") GeoCal::SurfaceImageToImageMatch "
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

C++ includes: surface_image_to_image_match.h ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::image_ground_connection1 "
const boost::shared_ptr<ImageGroundConnection>& GeoCal::SurfaceImageToImageMatch::image_ground_connection1() const
ImageGroundConnection for image 1. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::image_ground_connection2 "
const boost::shared_ptr<ImageGroundConnection>& GeoCal::SurfaceImageToImageMatch::image_ground_connection2() const
ImageGroundConnection for image 2. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::map_project_on_demand "
bool GeoCal::SurfaceImageToImageMatch::map_project_on_demand() const
Indicate if we are doing the map projection on demand, or if we
already have the data map projected. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::match "
void SurfaceImageToImageMatch::match(const ImageCoordinate &Ic1, ImageCoordinate &Ic2, double &Line_sigma,
double &Sample_sigma, bool &Success, int *Diagnostic=0) const
Match point Ic1 in the first image with the second image.

We return the location in the second image along with the uncertainty
if Success is true, otherwise the match failed.

Note that the line and sample sigma are only approximately calculated.
We have the line and sample sigma on the surface, and we scale this by
the approximate number of image pixels in each surface pixel.

The Diagnostic argument is used to indicate why a match failed. For
many purposes you can ignore this, because you need to know detailed
information about the specific ImageMatcher that a user passed to a
function and what the diagnostic codes mean. However, for some
purposes the failure reason can be useful (e.g., tuning parameters of
a LsmMatcher). ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::matcher "
const boost::shared_ptr<ImageMatcher>& GeoCal::SurfaceImageToImageMatch::matcher() const
Matcher we are using. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::print "
virtual void GeoCal::SurfaceImageToImageMatch::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::print_to_string "
std::string GeoCal::Printable< ImageToImageMatch  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::surface_image1 "
const boost::shared_ptr<RasterImage>& GeoCal::SurfaceImageToImageMatch::surface_image1() const
Surface projected image 1. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::surface_image2 "
const boost::shared_ptr<RasterImage>& GeoCal::SurfaceImageToImageMatch::surface_image2() const
Surface projected image 2. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::SurfaceImageToImageMatch "
SurfaceImageToImageMatch::SurfaceImageToImageMatch(const boost::shared_ptr< ImageGroundConnection > &Igc1, const
boost::shared_ptr< ImageGroundConnection > &Igc2, const MapInfo
&Map_info, const boost::shared_ptr< ImageMatcher > &Matcher)
Constructor, where we set up for doing map projection on demand.

Note that we only get the resolution and map projection from the
Map_info, we adjust the cover to work over the full images Igc1 and
Igc2. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::SurfaceImageToImageMatch "
SurfaceImageToImageMatch::SurfaceImageToImageMatch(const boost::shared_ptr< ImageGroundConnection > &Igc1, const
boost::shared_ptr< RasterImage > &Surface_image1, const
boost::shared_ptr< ImageGroundConnection > &Igc2, const
boost::shared_ptr< RasterImage > &Surface_image2, const
boost::shared_ptr< ImageMatcher > &Matcher)
Constructor, where we supply the already map projected data. ";

%feature("docstring")  GeoCal::SurfaceImageToImageMatch::~SurfaceImageToImageMatch "
virtual GeoCal::SurfaceImageToImageMatch::~SurfaceImageToImageMatch()
";

// File: a00195.xml

%feature("docstring") GeoCal::VicarOgrNsp::TempFile "
";
%feature("docstring")  GeoCal::VicarOgrNsp::TempFile::TempFile "
GeoCal::VicarOgrNsp::TempFile::TempFile()
";
%feature("docstring")  GeoCal::VicarOgrNsp::TempFile::~TempFile "
GeoCal::VicarOgrNsp::TempFile::~TempFile()
";

// File: a00196.xml

%feature("docstring") GeoCal::VicarOgrNsp::Tif "
";
%feature("docstring")  GeoCal::VicarOgrNsp::Tif::Tif "
GeoCal::VicarOgrNsp::Tif::Tif(const TempFile &Tf, const char *mode)
";
%feature("docstring")  GeoCal::VicarOgrNsp::Tif::~Tif "
GeoCal::VicarOgrNsp::Tif::~Tif()
";

// File: a00197.xml

%feature("docstring") GeoCal::Tile "
This is used by TiledFile to maintain a tile of data.

This class has two jobs: Maintain the actual tile data.

Keep track of if the data has been changed and needs to be written to
disk.

We make use of boost::multi_array ability to change the index base, so
you index data the same way you would a file (e.g., to read (100, 200)
in a file, you read data[100][200] for the appropriate tile).

C++ includes: tile.h ";

%feature("docstring")  GeoCal::Tile::in_tile "
bool GeoCal::Tile< T, D >::in_tile(const boost::array< index, D > &Index) const
Test the given index to see if it is in this tile or not. ";

%feature("docstring")  GeoCal::Tile::max_index "
boost::array<index, D> GeoCal::Tile< T, D >::max_index() const
Return maximum index. ";

%feature("docstring")  GeoCal::Tile::min_index "
boost::array<index, D> GeoCal::Tile< T, D >::min_index() const
Return minimum index. ";

%feature("docstring")  GeoCal::Tile::swap "
void GeoCal::Tile< T, D >::swap(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index)
Swap range of data. ";

%feature("docstring")  GeoCal::Tile::Tile "
GeoCal::Tile< T, D >::Tile()
Constructor. ";


// File: a00198.xml

%feature("docstring") GeoCal::TiledFile "
This class is used to handle reading and writing a file using tiles.

It is common for files to be stored as a set of tiles. This class
handles all of the book keeping needed to make these files seem like
they are all in memory at one time. We automatically handle the
swapping to and from disk as needed.

This is an abstract based class, a derived class needs to supply the
actual read and write methods used by this class.

Note that it is imperative that derived class provide a destructor
that calls flush. We can not call virtual functions of derived classes
from a destructor in the base class, because the virtual table for the
derived class has already been destroyed by the time we get to the
destructor in this class (see Stroustrup 3rd edition, 4.3 for
details). This means that the derived class needs to handle flushing
of the data to disk, by a call to flush().

Note that the tile size is not required to evenly divide the file
size, in this case we just have tiles on the edges that aren't the
full size.

C++ includes: tiled_file.h ";

%feature("docstring")  GeoCal::TiledFile::can_set_map_info "
virtual bool GeoCal::TiledFileBase< D >::can_set_map_info() const
Indicate if this file type can set MapInfo.

Default is false, derived class can override this. ";

%feature("docstring")  GeoCal::TiledFile::flush "
virtual void GeoCal::TiledFile< T, D >::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::TiledFile::get_double "
virtual double GeoCal::TiledFile< T, D >::get_double(const boost::array< index, D > &Index) const
Get data an convert to double. ";

%feature("docstring")  GeoCal::TiledFile::get_int "
virtual int GeoCal::TiledFile< T, D >::get_int(const boost::array< index, D > &Index) const
Get data an convert to integer. ";

%feature("docstring")  GeoCal::TiledFile::has_map_info "
virtual bool GeoCal::TiledFileBase< D >::has_map_info() const
Indicate if a file has MapInfo.

Default is false, derived classes can override this. ";

%feature("docstring")  GeoCal::TiledFile::map_info "
virtual MapInfo GeoCal::TiledFileBase< D >::map_info() const
Return MapInfo.

Default is to throw an exception since this isn't supported, derived
classes can override this. ";

%feature("docstring")  GeoCal::TiledFile::map_info "
virtual void GeoCal::TiledFileBase< D >::map_info(const MapInfo &M)
Set MapInfo.

Default is to throw an exception since this isn't supported, derived
classes can override this. ";

%feature("docstring")  GeoCal::TiledFile::number_swap "
unsigned int GeoCal::TiledFileBase< D >::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::TiledFile::number_tile "
int GeoCal::TiledFile< T, D >::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::TiledFile::read "
boost::multi_array<T, D> GeoCal::TiledFile< T, D >::read(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index) const
Read a subset of the data. ";

%feature("docstring")  GeoCal::TiledFile::read_double "
virtual void GeoCal::TiledFile< T, D >::read_double(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, double *Res) const
Read a subset of the data, and convert to double. ";

%feature("docstring")  GeoCal::TiledFile::read_int "
virtual void GeoCal::TiledFile< T, D >::read_int(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, int *Res) const
Read a subset of the data, and convert to int. ";

%feature("docstring")  GeoCal::TiledFile::read_ptr "
virtual void GeoCal::TiledFile< T, D >::read_ptr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, T *Res) const
Read a subset of the data.

Check first to see if entire requested data is in a single tile. If
not, the we read from disk (rather than trying to stitch together
multiple tiles). ";

%feature("docstring")  GeoCal::TiledFile::reset_number_swap "
void GeoCal::TiledFileBase< D >::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::TiledFile::size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::size() const
File size. ";

%feature("docstring")  GeoCal::TiledFile::tile_size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::tile_size() const
Tile size. ";

%feature("docstring")  GeoCal::TiledFile::TiledFile "
GeoCal::TiledFile< T, D >::TiledFile(const boost::array< index, D > &File_size, const boost::array< index,
D > &Tile_size, unsigned int Number_tile=4)
Constructor. ";

%feature("docstring")  GeoCal::TiledFile::write "
void GeoCal::TiledFile< T, D >::write(const boost::array< index, D > &Min_index, const boost::multi_array<
T, D > &V) const
Write a subset of the data. ";

%feature("docstring")  GeoCal::TiledFile::write_double "
virtual void GeoCal::TiledFile< T, D >::write_double(const boost::array< index, D > &Index, double Val)
Convert a double to type T, and write to file. ";

%feature("docstring")  GeoCal::TiledFile::write_double_arr "
virtual void GeoCal::TiledFile< T, D >::write_double_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const double *V)
Convert a double to type T, and write to file. ";

%feature("docstring")  GeoCal::TiledFile::write_int "
virtual void GeoCal::TiledFile< T, D >::write_int(const boost::array< index, D > &Index, int Val)
Convert an int to type T, and write to file. ";

%feature("docstring")  GeoCal::TiledFile::write_int_arr "
virtual void GeoCal::TiledFile< T, D >::write_int_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const int *V)
Convert an int to type T, and write to file. ";

%feature("docstring")  GeoCal::TiledFile::write_ptr "
virtual void GeoCal::TiledFile< T, D >::write_ptr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const T *V) const
Write a subset of the data. ";

%feature("docstring")  GeoCal::TiledFile::~TiledFile "
virtual GeoCal::TiledFile< T, D >::~TiledFile()
Destructor. ";


// File: a00199.xml

%feature("docstring") GeoCal::TiledFileBase "
This class is the base class of TiledFile<T, D>.

It turns out to be useful to factor out the part of TiledFile that
doesn't depend on the type T. For most purposes though, you can ignore
this class and use TiledFile<T, D> directly.

For use with some classes, it is useful to be able to change a generic
type T to some common type. For instance, RasterImage converts the
types to and from int, and Dem converts to double. We supply
conversion routines in this base class for a few common types, as
get_int, put_double, read_int etc.

C++ includes: tiled_file.h ";

%feature("docstring")  GeoCal::TiledFileBase::can_set_map_info "
virtual bool GeoCal::TiledFileBase< D >::can_set_map_info() const
Indicate if this file type can set MapInfo.

Default is false, derived class can override this. ";

%feature("docstring")  GeoCal::TiledFileBase::flush "
virtual void GeoCal::TiledFileBase< D >::flush() const =0
Flush data to disk. ";

%feature("docstring")  GeoCal::TiledFileBase::get_double "
virtual double GeoCal::TiledFileBase< D >::get_double(const boost::array< index, D > &Index) const =0
Get data an convert to double. ";

%feature("docstring")  GeoCal::TiledFileBase::get_int "
virtual int GeoCal::TiledFileBase< D >::get_int(const boost::array< index, D > &Index) const =0
Get data an convert to integer. ";

%feature("docstring")  GeoCal::TiledFileBase::has_map_info "
virtual bool GeoCal::TiledFileBase< D >::has_map_info() const
Indicate if a file has MapInfo.

Default is false, derived classes can override this. ";

%feature("docstring")  GeoCal::TiledFileBase::map_info "
virtual MapInfo GeoCal::TiledFileBase< D >::map_info() const
Return MapInfo.

Default is to throw an exception since this isn't supported, derived
classes can override this. ";

%feature("docstring")  GeoCal::TiledFileBase::map_info "
virtual void GeoCal::TiledFileBase< D >::map_info(const MapInfo &M)
Set MapInfo.

Default is to throw an exception since this isn't supported, derived
classes can override this. ";

%feature("docstring")  GeoCal::TiledFileBase::number_swap "
unsigned int GeoCal::TiledFileBase< D >::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::TiledFileBase::number_tile "
unsigned int GeoCal::TiledFileBase< D >::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::TiledFileBase::read_double "
virtual void GeoCal::TiledFileBase< D >::read_double(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, double *Res) const =0
Read a subset of the data, and convert to double. ";

%feature("docstring")  GeoCal::TiledFileBase::read_int "
virtual void GeoCal::TiledFileBase< D >::read_int(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, int *Res) const =0
Read a subset of the data, and convert to int. ";

%feature("docstring")  GeoCal::TiledFileBase::reset_number_swap "
void GeoCal::TiledFileBase< D >::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::TiledFileBase::size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::size() const
File size. ";

%feature("docstring")  GeoCal::TiledFileBase::tile_size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::tile_size() const
Tile size. ";

%feature("docstring")  GeoCal::TiledFileBase::write_double "
virtual void GeoCal::TiledFileBase< D >::write_double(const boost::array< index, D > &Index, double Val)=0
Convert a double to type T, and write to file. ";

%feature("docstring")  GeoCal::TiledFileBase::write_double_arr "
virtual void GeoCal::TiledFileBase< D >::write_double_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const double *V)=0
Read a subset of the data, and convert to double. ";

%feature("docstring")  GeoCal::TiledFileBase::write_int "
virtual void GeoCal::TiledFileBase< D >::write_int(const boost::array< index, D > &Index, int Val)=0
Convert an int to type T, and write to file. ";

%feature("docstring")  GeoCal::TiledFileBase::write_int_arr "
virtual void GeoCal::TiledFileBase< D >::write_int_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const int *V)=0
Write a subset of the data, converted from int. ";

%feature("docstring")  GeoCal::TiledFileBase::~TiledFileBase "
virtual GeoCal::TiledFileBase< D >::~TiledFileBase()
Destructor. ";


// File: a00200.xml

%feature("docstring") GeoCal::TiledFileReference "
This is a helper class for TiledFile.

This allows access to a value, which can optionally be updated. If it
is updated, we make sure that we correctly mark the affected tile as
changed.

C++ includes: tiled_file.h ";

%feature("docstring")  GeoCal::TiledFileReference::TiledFileReference "
GeoCal::TiledFileReference< T >::TiledFileReference(T &Val, bool &Changed)
";

// File: a00201.xml

%feature("docstring") GeoCal::Time "
There are a few reasonable choices for expressing time information.

We could use TAI, GPS, the PGS toolkit. Each of these time system can
be related to the other by a constant, since the only difference is
the Epoch that time is measure against.

Note that for accurate work we do not want to use something like Unix
time, because this does not account for leapseconds (POSIX unix time
is the number of seconds since January 1, 1970 not including
leapseconds).

Most code doesn't care what the underlying time representation is, we
just need to be able to do functions such as comparing two times to
determine what is later, adding or subtracting a given number of
seconds to a time, or give the duration between two times.

This class abstracts out the representation we use for time. We supply
conversions to the specific time systems for use in the cases that a
specific system is needed (e.g., calling a PGS toolkit routine).

We also supply methods for converting to and from a string
representation of the time. The supported formats of the string
parsing depends on the underlying toolkit used, but all of them
support CCSDS format (e.g., \"1996-07-03T04:13:57.987654Z\").

If either SPICE or SDP is available, then that toolkit is the one used
for the conversion. If both are available, we default to SPICE. In
each case, you can change the default by updating the variable
Time::toolkit_time_interface.

If we don't have either SPICE or SDP, then we default to using unix
time (through the boost date_time library). This isn't ideal, but it
is better than not supporting time at all. For a number of purposes
the unix time is fine (e.g., indexing data in a orbit file).

As an implementation detail, we use PGS time, which has an epoch of
1993-01-01.

Note there is a subtle difference between time used for Terrestrial
uses (such as UTC) and for planetary use. There are two ways used for
measuring time - International atomic time (TAI) and Barycentric
Dynamic Time (TDB). The first is the time measured by an atomic clock
on the earth vs. the second which measures time at the barycenter of
the solar system. Due to relativistic effects, the two clocks vary by
something close to a periodic function with a size of about 1 ms over
the coarse of a year. We measure time durations (e.g., the difference
between 2 times) in TAI. For most purposes, you can ignore the
difference between the two systems.

When calling SPICE routines, we internally convert to and from TAI
time as needed. This is a bit less efficient than simply keeping
everything in TDB time, but it gives a cleaner interface. We can
revisit this if the computer time it takes to do the conversions start
becoming prohibitive.

C++ includes: geocal_time.h ";

%feature("docstring")  GeoCal::Time::parse_time "
static Time GeoCal::Time::parse_time(const std::string Time_string)
Parse string to get a Time.

Uses interface supplied by toolkit_time_interface. ";

%feature("docstring")  GeoCal::Time::time_acs "
Time Time::time_acs(double acs_time)
Return time from ACS time.

ACS time is an odd time system. It is measured in UTC seconds from a
particular epoch. The choice of UTC seconds means that this cannot
correctly handle times that occur during a leapsecond, by definition
the UTC time before and after a leapsecond is the same. The epoch is
noon January, 1 2000 in UTC. Note that this is 64.184 seconds
different from terrestrial time J2000. ";

%feature("docstring")  GeoCal::Time::time_et "
Time Time::time_et(double et)
Return time from given SPICE ET time. ";

%feature("docstring")  GeoCal::Time::time_gps "
static Time GeoCal::Time::time_gps(double gps)
Return time from given GPS time (epoch of 1980-01-06). ";

%feature("docstring")  GeoCal::Time::time_j2000 "
static Time GeoCal::Time::time_j2000(double j2000)
Return time from given J2000 time (epoch of 2000-01-01 12:00:00 TT).

Note that TT is different than UTC noon by about 64.184 seconds ";

%feature("docstring")  GeoCal::Time::time_pgs "
static Time GeoCal::Time::time_pgs(double pgs)
Return time from given PGS toolkit time (epoch of 1993-01-01). ";

%feature("docstring")  GeoCal::Time::time_unix "
static Time GeoCal::Time::time_unix(std::time_t unix_time)
Return time from given Unix time (epoch of 1970-01-01). ";

%feature("docstring")  GeoCal::Time::time_unix "
static Time GeoCal::Time::time_unix(double unix_time)
Return time from given Unix time (epoch of 1970-01-01). ";

%feature("docstring")  GeoCal::Time::acs "
double Time::acs() const
Give ACS time. ";

%feature("docstring")  GeoCal::Time::et "
double Time::et() const
Give time as SPICE ET time. ";

%feature("docstring")  GeoCal::Time::gps "
double GeoCal::Time::gps() const
Give time in GPS. ";

%feature("docstring")  GeoCal::Time::j2000 "
double GeoCal::Time::j2000() const
Give time in j2000. ";

%feature("docstring")  GeoCal::Time::pgs "
double GeoCal::Time::pgs() const
Give time in PGS toolkit time (epoch 1993-01-01). ";

%feature("docstring")  GeoCal::Time::print "
void GeoCal::Time::print(std::ostream &os) const
Print to stream. ";

%feature("docstring")  GeoCal::Time::print_to_string "
std::string GeoCal::Printable< Time  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::Time::to_string "
std::string GeoCal::Time::to_string() const
Generate CCSDS format of time (e.g., \"1996-07-03T04:13:57.987654Z\").

Uses interface supplied by toolkit_time_interface. ";

%feature("docstring")  GeoCal::Time::unix_time "
std::time_t GeoCal::Time::unix_time() const
Give time in unix time.

Note that this is only accurate to the nearest second. ";

%feature("docstring")  GeoCal::Time::unix_time_double "
double GeoCal::Time::unix_time_double() const
Give time in unix time, as a double. ";


// File: a00202.xml

%feature("docstring") GeoCal::TimeAcsCreator "
Small helper class to wrap creation of time in a common interface.

This is useful for templates.

C++ includes: geocal_time.h ";


// File: a00203.xml

%feature("docstring") GeoCal::TimeEtCreator "
Small helper class to wrap creation of time in a common interface.

This is useful for templates.

C++ includes: geocal_time.h ";


// File: a00204.xml

%feature("docstring") GeoCal::TimeGpsCreator "
Small helper class to wrap creation of time in a common interface.

This is useful for templates.

C++ includes: geocal_time.h ";


// File: a00205.xml

%feature("docstring") GeoCal::TimeJ2000Creator "
Small helper class to wrap creation of time in a common interface.

This is useful for templates.

C++ includes: geocal_time.h ";


// File: a00206.xml

%feature("docstring") GeoCal::TimePgsCreator "
Small helper class to wrap creation of time in a common interface.

This is useful for templates.

C++ includes: geocal_time.h ";


// File: a00207.xml

%feature("docstring") GeoCal::TimeTable "
This class is used to relate time to image line number and vice versa.

Often the relationship is pretty simply, there is just a fixed time
interval between one line and the next. However this class can be used
to model any instrument complications that need to be modelled (e.g.,
missing lines, drift in spacing).

C++ includes: time_table.h ";

%feature("docstring")  GeoCal::TimeTable::image_coordinate "
virtual ImageCoordinate GeoCal::TimeTable::image_coordinate(Time T, const FrameCoordinate &F) const =0
Convert from Time and FrameCoordinate to ImageCoordinate. ";

%feature("docstring")  GeoCal::TimeTable::max_line "
virtual int GeoCal::TimeTable::max_line() const =0
Maximum line table is valid for. ";

%feature("docstring")  GeoCal::TimeTable::max_time "
virtual Time GeoCal::TimeTable::max_time() const =0
Maximum time table is valid for. ";

%feature("docstring")  GeoCal::TimeTable::min_line "
virtual int GeoCal::TimeTable::min_line() const =0
Minimum line table is valid for. ";

%feature("docstring")  GeoCal::TimeTable::min_time "
virtual Time GeoCal::TimeTable::min_time() const =0
Minimum time table is valid for. ";

%feature("docstring")  GeoCal::TimeTable::print "
virtual void GeoCal::TimeTable::print(std::ostream &Os) const =0
";
%feature("docstring")  GeoCal::TimeTable::print_to_string "
std::string GeoCal::Printable< TimeTable  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::TimeTable::time "
virtual void GeoCal::TimeTable::time(const ImageCoordinate &Ic, Time &T, FrameCoordinate &F) const =0
Convert from ImageCoordinate to Time and FrameCoordinate. ";

%feature("docstring")  GeoCal::TimeTable::TimeTable "
GeoCal::TimeTable::TimeTable()
Default constructor. ";

%feature("docstring")  GeoCal::TimeTable::~TimeTable "
virtual GeoCal::TimeTable::~TimeTable()
";

// File: a00208.xml

%feature("docstring") TokenizerFunction "
Function to doing tokens. ";

%feature("docstring")  TokenizerFunction::reset "
void TokenizerFunction::reset()
";

// File: a00209.xml

%feature("docstring") GeoCal::ToolkitCoordinateInterface "
Each toolkit supplies methods to convert to and from
CartesianInertial.

C++ includes: toolkit_coordinate_interface.h ";

%feature("docstring")  GeoCal::ToolkitCoordinateInterface::to_fixed "
virtual void GeoCal::ToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, const CartesianInertial &From,
CartesianFixed &To)=0
This converts from CartesianInertial to CartesianFixed for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). We use this because it is a unique coding, the underlying
toolkit doesn't need to be SPICE. ";

%feature("docstring")  GeoCal::ToolkitCoordinateInterface::to_fixed "
virtual void GeoCal::ToolkitCoordinateInterface::to_fixed(int Body_id, const Time &T, double Ci_to_cf[3][3])=0
Return a matrix for converting from CartesianInertial to
CartesianFixed. ";

%feature("docstring")  GeoCal::ToolkitCoordinateInterface::to_inertial "
virtual void GeoCal::ToolkitCoordinateInterface::to_inertial(int Body_id, const Time &T, const CartesianFixed &From,
CartesianInertial &To)=0
This converts from CartesianFixed to CartesianInertial for the given
body.

We use the NAIF coding for the bodies (see the SPICE documentation for
details). We use this because it is a unique coding, the underlying
toolkit doesn't need to be SPICE. ";

%feature("docstring")  GeoCal::ToolkitCoordinateInterface::~ToolkitCoordinateInterface "
virtual GeoCal::ToolkitCoordinateInterface::~ToolkitCoordinateInterface()
";

// File: a00210.xml

%feature("docstring") GeoCal::ToolkitTimeInterface "
Each toolkit supplies methods to convert to and from strings.

C++ includes: toolkit_time_interface.h ";

%feature("docstring")  GeoCal::ToolkitTimeInterface::parse_time "
virtual Time GeoCal::ToolkitTimeInterface::parse_time(const std::string Time_string) const =0
Parse string to get a Time.

The specific formats that are recognized depends on the toolkit used,
but they all support CCSDS ASCII format (e.g.,
\"1996-07-03T04:13:57.987654Z\") ";

%feature("docstring")  GeoCal::ToolkitTimeInterface::to_string "
virtual std::string GeoCal::ToolkitTimeInterface::to_string(const Time &T) const =0
Generate CCSDS format of time (e.g., \"1996-07-03T04:13:57.987654Z\")
";

%feature("docstring")  GeoCal::ToolkitTimeInterface::~ToolkitTimeInterface "
virtual GeoCal::ToolkitTimeInterface::~ToolkitTimeInterface()
";

// File: a01190.xml

%feature("docstring") std::underflow_error "
STL class. ";


// File: a01217.xml

%feature("docstring") std::unique_ptr "
STL class. ";


// File: a00211.xml

%feature("docstring") UnixToolkitTimeInterface "
Unix version. ";

%feature("docstring")  UnixToolkitTimeInterface::parse_time "
virtual Time UnixToolkitTimeInterface::parse_time(const std::string Time_string) const
Parse string to get a Time.

The specific formats that are recognized depends on the toolkit used,
but they all support CCSDS ASCII format (e.g.,
\"1996-07-03T04:13:57.987654Z\") ";

%feature("docstring")  UnixToolkitTimeInterface::to_string "
virtual std::string UnixToolkitTimeInterface::to_string(const Time &T) const
Generate CCSDS format of time (e.g., \"1996-07-03T04:13:57.987654Z\")
";

%feature("docstring")  UnixToolkitTimeInterface::~UnixToolkitTimeInterface "
virtual UnixToolkitTimeInterface::~UnixToolkitTimeInterface()
";

// File: a01218.xml

%feature("docstring") std::valarray "
STL class. ";


// File: a01219.xml

%feature("docstring") std::vector "
STL class. ";


// File: a00212.xml

%feature("docstring") GeoCal::VFunctorWithDerivative "
";
%feature("docstring")  GeoCal::VFunctorWithDerivative::df "
virtual blitz::Array<double, 2> GeoCal::VFunctorWithDerivative::df(const blitz::Array< double, 1 > &X) const =0
Return jacobian. J_ij = Df_i / dx_j. ";

%feature("docstring")  GeoCal::VFunctorWithDerivative::f_and_df "
virtual void GeoCal::VFunctorWithDerivative::f_and_df(const blitz::Array< double, 1 > &X, blitz::Array< double, 1 > &F,
blitz::Array< double, 2 > &Df) const
Return both the value of the function and the Jacobian.

Often this can be calculated more quickly. The default implementation
here just calls opererator() and df separately. ";


// File: a00213.xml

%feature("docstring") GeoCal::VicarArgument "
This provides access to arguments passed by VICAR.

Note an important limitation of VICAR, you can only initialize the
argument handling once. This means that if you try to create multiple
VicarArgument objects, you'll trigger a TAE error when you try to
create the second object.

VICAR uses a somewhat unusual mechanism to pass arguments to a program
called from the TAE. Rather than putting the arguments on the command
line to appear in Argv, the arguments get passed down a pipe (usually
file logical 4) in a compressed format. I'm not sure why exactly it
does this, probably some historical artifact from when it was
developed on VMS. In any case, in order to fit into VICAR we need to
parse arguments the same way.

This class handles the arguments.

C++ includes: vicar_argument.h ";

%feature("docstring")  GeoCal::VicarArgument::print "
void VicarArgument::print(std::ostream &Os) const
Print an VicarArgument to a stream. ";

%feature("docstring")  GeoCal::VicarArgument::print_to_string "
std::string GeoCal::Printable< VicarArgument  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarArgument::VicarArgument "
VicarArgument::VicarArgument(int Argc, char **Argv, bool copy_primary_input_label=false)
Set up to parse VICAR arguments.

VICAR arguments are accessed by keywords. This isn't a bad interface,
but because we also want to support straight unix command line style
arguments, we have the VicarArgument interface in terms of index
number (first argument, second argument, etc.). Keyword_list gives the
list of keywords in order that we want.

VICAR has the convention that when creating a new file it
automatically copies over all of the labels from the \"primary input\"
- usually the first input file. The idea is that with old VICAR files
most of the time the output image would have same size, map
projection, etc. However, this doesn't fit well with the geocal usage
at all. Much of the time the output file doesn't have any relationship
to the input file. Also the label reading and writing happens in the
background anyways, so there isn't a lot gained by the defaults. This
can often cause problems, such as copying map projection data from an
input file to an output file that isn't map projected.

By default, we turn this copying off. You can get the old VICAR
behavior by setting Copy_primary_input_label to true. ";

%feature("docstring")  GeoCal::VicarArgument::write_out "
void VicarArgument::write_out(const std::string &Keyword, int Val)
Write a value to a TCL variable. ";

%feature("docstring")  GeoCal::VicarArgument::write_out "
void VicarArgument::write_out(const std::string &Keyword, double Val)
";
%feature("docstring")  GeoCal::VicarArgument::write_out "
void VicarArgument::write_out(const std::string &Keyword, const std::string &Val)
";
%feature("docstring")  GeoCal::VicarArgument::arg "
std::vector< std::string > GeoCal::VicarArgument::arg(const std::string &Keyword)
Stand alone version of argument parsing.

This is just a typesafe version of the Vicar RTL zvp call. ";

%feature("docstring")  GeoCal::VicarArgument::type "
void VicarArgument::type(const std::string &Keyword, std::string &Type, int &Count)
Return the type and count for the given keyword. ";

%feature("docstring")  GeoCal::VicarArgument::type "
void VicarArgument::type(const std::string &Keyword, std::string &Type, int &Count, int
&Maxlen)
Return the type, count and maxlen for the given keyword. ";

%feature("docstring")  GeoCal::VicarArgument::zvparmdw "
int VicarArgument::zvparmdw(char *name, void *value, int *count, int *def, int maxcnt, int
length)
This is a thin wrapper around the RTL function zvparmd.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarArgument::zvparmw "
int VicarArgument::zvparmw(char *name, void *value, int *count, int *def, int maxcnt, int
length)
This is a thin wrapper around the RTL function zvparm.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarArgument::zvpw "
int VicarArgument::zvpw(char *name, void *value, int *count)
This is a thin wrapper around the RTL function zvp.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";


// File: a00214.xml

%feature("docstring") GeoCal::VicarDem "
This uses a VicarTiledFile<T> as a DEM.

The type of T should be one we can convert to a double, and should
give the height in meters above sea level. We do a bilinear
interpolation to get values between points, and we optionally return a
height of 0 outside of the range of where we have data (or
alternatively, throw an exception).

The default datum used is NoDatum, e.g., the heights in the VICAR file
are relative to WGS-84. If desired, a different Datum can be given.

C++ includes: vicar_dem.h ";

%feature("docstring")  GeoCal::VicarDem::datum "
const Datum& GeoCal::DemMapInfo::datum() const
Datum height is relative to. ";

%feature("docstring")  GeoCal::VicarDem::datum_ptr "
const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
Pointer to datum. ";

%feature("docstring")  GeoCal::VicarDem::distance_to_surface "
double DemMapInfo::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::VicarDem::elevation "
virtual double GeoCal::DemTiledFile::elevation(int Y_index, int X_index) const
Return height in meters relative to datum().

Note that the call is in line, sample order, which means Y and then X.
";

%feature("docstring")  GeoCal::VicarDem::height_datum "
double DemMapInfo::height_datum(const GroundCoordinate &Gp) const
Height relative to datum. ";

%feature("docstring")  GeoCal::VicarDem::height_reference_surface "
double DemMapInfo::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::VicarDem::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::VicarDem::map_info "
const MapInfo& GeoCal::DemMapInfo::map_info() const
MapInfo of underlying data. ";

%feature("docstring")  GeoCal::VicarDem::number_line_per_tile "
int GeoCal::DemTiledFile::number_line_per_tile() const
";
%feature("docstring")  GeoCal::VicarDem::number_sample_per_tile "
int GeoCal::DemTiledFile::number_sample_per_tile() const
";
%feature("docstring")  GeoCal::VicarDem::number_swap "
int GeoCal::DemTiledFile::number_swap() const
";
%feature("docstring")  GeoCal::VicarDem::number_tile "
unsigned int GeoCal::DemTiledFile::number_tile() const
";
%feature("docstring")  GeoCal::VicarDem::outside_dem_is_error "
bool GeoCal::DemMapInfo::outside_dem_is_error() const
If true, then calling with values outside the range of area of the DEM
triggers an exception, otherwise we just return a height of 0. ";

%feature("docstring")  GeoCal::VicarDem::print "
virtual void GeoCal::VicarDem::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::VicarDem::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarDem::reset_number_swap "
void GeoCal::DemTiledFile::reset_number_swap()
";
%feature("docstring")  GeoCal::VicarDem::surface_point "
boost::shared_ptr< GroundCoordinate > DemMapInfo::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::VicarDem::vicar_file "
const VicarFile& GeoCal::VicarDem::vicar_file() const
Underlying file. ";

%feature("docstring")  GeoCal::VicarDem::vicar_file "
VicarFile& GeoCal::VicarDem::vicar_file()
Underlying file. ";

%feature("docstring")  GeoCal::VicarDem::vicar_file_ptr "
boost::shared_ptr<VicarFile> GeoCal::VicarDem::vicar_file_ptr() const
";
%feature("docstring")  GeoCal::VicarDem::VicarDem "
GeoCal::VicarDem::VicarDem(const std::string &Fname, bool Outside_dem_is_error=false, int
Number_line_per_tile=100, int Number_tile=4, const boost::shared_ptr<
Datum > &D=boost::shared_ptr< Datum >(new NoDatum()))
Open VICAR file as a DEM. ";

%feature("docstring")  GeoCal::VicarDem::VicarDem "
GeoCal::VicarDem::VicarDem(int Instance, bool Outside_dem_is_error=false, int
Number_line_per_tile=100, int Number_tile=4, const std::string
&Name=\"INP\", const boost::shared_ptr< Datum > &D=boost::shared_ptr<
Datum >(new NoDatum()))
Open VICAR file as a DEM. ";

%feature("docstring")  GeoCal::VicarDem::~VicarDem "
virtual GeoCal::VicarDem::~VicarDem()
Destructor. ";


// File: a00215.xml

%feature("docstring") GeoCal::VicarException "
Exception thrown if Vicar call fails.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::VicarException::VicarException "
GeoCal::VicarException::VicarException(int status, const std::string &W=\"\")
Default constructor.

Can give an optional string describing the error. ";

%feature("docstring")  GeoCal::VicarException::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::VicarException::~VicarException "
virtual GeoCal::VicarException::~VicarException()
 throw ()
Destructor.
";

// File: a00216.xml

%feature("docstring") GeoCal::VicarFile "
This handles opening and closing a Vicar file.

This class doesn't actually read and write data, but is used by other
classes which do.

This uses the VICAR RTL, which is documented athttp://www-
mipl.jpl.nasa.gov/RTL

If the VICAR RTL library wasn't available when this geocal was built,
then this class will just throw an exception when we try to do
anything with a VICAR file.

As an implementation detail, the class VicarFile and IbisFile need to
coordinate in translating file names to unit numbers since the same
underlying Vicar routine zvunit is used by both. We arbitrarily have
VicarFile handle this, and IbisFile accesses VicarFile when it needs
to do this.

Also a note for developers. The Vicar RTL library uses various
variable argument functions. You need to end the list of arguments
with \"NULL\". Note that a cast to a point type is mandatory, you
can't just say \"0\". If say \"0\" then you will get fairly difficult
to track down errors. Without the cast, this gets passed as an int,
which the RTL library code will then try to interpret as a char *.
va_args has undefined behavior when called with the wrong type,
ranging from seeming to work to core dumping.

Because of the complication involved, we have separated out the
functionality of reading and writing MapInfo metadata. This is done by
the class VicarOgr. This is handled behind the scenes, so for a user
of this class this separation makes no difference. But if you are
looking for the code that does this, you'll need to look in VicarOgr.

The current implementation of the MapInfo metadata requires the GDAL
library to be available. If this isn't you can still build the GeoCal
system but any attempt to read or write MapInfo metadata will trigger
an exception.

C++ includes: vicar_file.h ";

%feature("docstring")  GeoCal::VicarFile::access "
access_type GeoCal::VicarFile::access() const
Access type of file. ";

%feature("docstring")  GeoCal::VicarFile::close "
void VicarFile::close()
Close file. ";

%feature("docstring")  GeoCal::VicarFile::file_name "
const std::string& GeoCal::VicarFile::file_name() const
File name. ";

%feature("docstring")  GeoCal::VicarFile::flush "
void GeoCal::VicarFile::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::VicarFile::force_area_pixel "
bool GeoCal::VicarFile::force_area_pixel() const
If true, then force the file to be treated as \"pixel is area\".

This is really just meant as a work around for the SRTM data, which
incorrectly labels the data as \"point\" rather than \"area\". Since
this is a 15 meter difference, it matters for many applications. Most
users should just ignore this value. ";

%feature("docstring")  GeoCal::VicarFile::has_label "
bool GeoCal::VicarFile::has_label(const std::string &Lbl) const
Test if a label is found in a file, and if so return true.

Otherwise return false. ";

%feature("docstring")  GeoCal::VicarFile::has_map_info "
bool VicarFile::has_map_info() const
Return true if the file has a GEOTIFF label in it, indicating it has
map information. ";

%feature("docstring")  GeoCal::VicarFile::has_rpc "
bool VicarFile::has_rpc() const
Return true if the file has a NITF_CETAG label in it, indicating it
has RPC information. ";

%feature("docstring")  GeoCal::VicarFile::label "
float GeoCal::VicarFile::label< float >(const std::string &F, const std::string &Property=\"\") const
Return value for the given label.

Optionally also supply a property for labels that are part of one
(e.g., GEOTIFF) ";

%feature("docstring")  GeoCal::VicarFile::label_delete "
void VicarFile::label_delete(const std::string &F, const std::string &Property=\"\")
Delete a label from a file. ";

%feature("docstring")  GeoCal::VicarFile::label_nelement "
const std::map<std::string, int>& GeoCal::VicarFile::label_nelement() const
Map between Label names and the number of elements.

As a convention, we store a label that is part of a property as the
string \"property label\". We can break this out in the future if
needed, but for now this seems sufficient. ";

%feature("docstring")  GeoCal::VicarFile::label_set "
void VicarFile::label_set(const std::string &F, int Val, const std::string &Property=\"\")
Set the value of a label.

If the label is already in the file, is is deleted and replaced with
this new value. Otherwise, it is simply added. Optionally the label
can be part of a Property. ";

%feature("docstring")  GeoCal::VicarFile::label_set "
void VicarFile::label_set(const std::string &F, float Val, const std::string &Property=\"\")
Set the value of a label.

If the label is already in the file, is is deleted and replaced with
this new value. Otherwise, it is simply added. Optionally the label
can be part of a Property. ";

%feature("docstring")  GeoCal::VicarFile::label_set "
void VicarFile::label_set(const std::string &F, const std::string &Val, const std::string
&Property=\"\")
Set the value of a label.

If the label is already in the file, is is deleted and replaced with
this new value. Otherwise, it is simply added. Optionally the label
can be part of a Property. ";

%feature("docstring")  GeoCal::VicarFile::label_string "
std::string GeoCal::VicarFile::label_string(const std::string &F, const std::string &Property=\"\") const
Non template form of label, useful in some contexts. ";

%feature("docstring")  GeoCal::VicarFile::label_type "
const std::map<std::string, int>& GeoCal::VicarFile::label_type() const
Map between Label names and the type of the label.

As a convention, we store a label that is part of a property as the
string \"property label\". We can break this out in the future if
needed, but for now this seems sufficient. ";

%feature("docstring")  GeoCal::VicarFile::map_info "
MapInfo VicarFile::map_info() const
Read metadata for MapInfo. ";

%feature("docstring")  GeoCal::VicarFile::map_info "
void VicarFile::map_info(const MapInfo &M)
Set metadata for MapInfo. ";

%feature("docstring")  GeoCal::VicarFile::number_line "
int GeoCal::VicarFile::number_line() const
Number of lines in file. ";

%feature("docstring")  GeoCal::VicarFile::number_sample "
int GeoCal::VicarFile::number_sample() const
Number of samples in file. ";

%feature("docstring")  GeoCal::VicarFile::print "
virtual void GeoCal::VicarFile::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::VicarFile::print_to_string "
std::string GeoCal::Printable< VicarFile  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarFile::property "
const std::set<std::string>& GeoCal::VicarFile::property() const
List of properties. ";

%feature("docstring")  GeoCal::VicarFile::reopen_file "
void VicarFile::reopen_file() const
Close and reopen the file.

Vicar is odd about reading to the end of the file, and we sometimes
need to reopen it to clear end of file status. ";

%feature("docstring")  GeoCal::VicarFile::rpc "
Rpc VicarFile::rpc() const
Read metadata for Rpc. ";

%feature("docstring")  GeoCal::VicarFile::rpc "
void VicarFile::rpc(const Rpc &R)
Set metadata for Rpc. ";

%feature("docstring")  GeoCal::VicarFile::type "
data_type GeoCal::VicarFile::type() const
Type of data in file. ";

%feature("docstring")  GeoCal::VicarFile::unit "
int GeoCal::VicarFile::unit() const
Unit number for VicarFile. ";

%feature("docstring")  GeoCal::VicarFile::VicarFile "
VicarFile::VicarFile(const std::string &Fname, access_type Access=READ, bool
Force_area_pixel=false)
Open an existing VICAR file for reading or update.

The Force_area_pixel forces the file to be treated as \"pixel as
area\" rather than \"pixel as point\". This is really just meant as a
work around for the SRTM data, which incorrectly labels the data as
\"point\" rather than \"area\". Since this is a 15 meter difference,
it matters for may applications. Most users should just ignore this
value. ";

%feature("docstring")  GeoCal::VicarFile::VicarFile "
VicarFile::VicarFile(const std::string &Fname, int Number_line, int Number_sample, const
std::string &Type=\"BYTE\", compression C=NONE)
Create a new VICAR file with the given size. ";

%feature("docstring")  GeoCal::VicarFile::VicarFile "
VicarFile::VicarFile(int Instance, access_type Access=READ, const std::string
&Name=\"INP\")
Open a file, using the VICAR Name and Instance input (so for example,
\"INP\" and 2 is the second INP file passed to a VICAR program. ";

%feature("docstring")  GeoCal::VicarFile::VicarFile "
VicarFile::VicarFile(int Instance, int Number_line, int Number_sample, const std::string
&Type=\"BYTE\", const std::string &Name=\"OUT\", compression C=NONE)
Create a new VICAR file with the given size.

Use the VICAR Name and Instance input (so for example, \"INP\" and 2
is the second INP file passed to a VICAR program. ";

%feature("docstring")  GeoCal::VicarFile::zvreadw "
int VicarFile::zvreadw(void *buffer, int Line) const
This is a thin wrapper around the RTL function zvread.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zvreadw "
int VicarFile::zvreadw(void *buffer) const
This is a thin wrapper around the RTL function zvread.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::~VicarFile "
VicarFile::~VicarFile()
Destructor, closes file. ";

%feature("docstring")  GeoCal::VicarFile::file_name_to_unit "
int VicarFile::file_name_to_unit(const std::string &Fname)
Return a VICAR unit number that is tied to the given file name. ";

%feature("docstring")  GeoCal::VicarFile::is_vicar_file "
bool VicarFile::is_vicar_file(const std::string &Fname)
Determine if a given file is a VICAR file or not.

This looks for the first few characters of the file being \"LBLSIZE=\"
";

%feature("docstring")  GeoCal::VicarFile::vicar_available "
bool VicarFile::vicar_available()
Return true if vicar functionality is available, otherwise return
false.

We have vicar functionality of the library was configured to use the
VICAR library. ";

%feature("docstring")  GeoCal::VicarFile::zlgetsh "
int VicarFile::zlgetsh(int unit, const char *key, char *value)
This is a thin wrapper around the RTL function zlget.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zlgetsh "
int VicarFile::zlgetsh(int unit, const char *key, char *value, int ulen)
This is a thin wrapper around the RTL function zlget.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zlgetw "
int VicarFile::zlgetw(int unit, const char *type, const char *key, char *value)
This is a thin wrapper around the RTL function zlget.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zlgetw "
int VicarFile::zlgetw(int unit, const char *type, const char *key, char *value, const char
*prop)
This is a thin wrapper around the RTL function zlget.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zlgetw "
int VicarFile::zlgetw(int unit, const char *type, const char *key, char *value, int ulen)
This is a thin wrapper around the RTL function zlget.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zlgetw "
int VicarFile::zlgetw(int unit, const char *type, const char *key, char *value, int ulen,
const char *prop)
This is a thin wrapper around the RTL function zlget.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zvwritw "
int VicarFile::zvwritw(int unit, void *buffer)
This is a thin wrapper around the RTL function zvwrit.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zvwritw "
int VicarFile::zvwritw(int unit, void *buffer, int Line)
This is a thin wrapper around the RTL function zvwrit.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";

%feature("docstring")  GeoCal::VicarFile::zvzinitw "
int VicarFile::zvzinitw(int argc, char *argv[])
This is a thin wrapper around the RTL function zvzinit.

If the VICAR library is installed then we just forward to that
library, otherwise we throw an exception saying that it isn't
available. ";


// File: a00217.xml

%feature("docstring") GeoCal::VicarImageCoordinate "
VICAR programs use a different convention than our ImageCoordinate.

Instead of the upper left hand corner of the bounding box being
(-0.5,-0.5), VICAR uses a 1-based convention where the upper left
corner is (0.5, 0.5). This is just a difference of 1-pixel. To avoid
any confusion, we define a VicarImageCoordinate class, and allow
conversions to and from ImageCoordinate.

C++ includes: image_coordinate.h ";

%feature("docstring")  GeoCal::VicarImageCoordinate::print "
void VicarImageCoordinate::print(std::ostream &Os) const
Print an VicarImageCoordinate to a stream. ";

%feature("docstring")  GeoCal::VicarImageCoordinate::print_to_string "
std::string GeoCal::Printable< VicarImageCoordinate  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarImageCoordinate::VicarImageCoordinate "
GeoCal::VicarImageCoordinate::VicarImageCoordinate(double Line, double Sample)
Constructor. ";

%feature("docstring")  GeoCal::VicarImageCoordinate::VicarImageCoordinate "
GeoCal::VicarImageCoordinate::VicarImageCoordinate(const ImageCoordinate &Ic)
Convert from ImageCoordinate. ";

%feature("docstring")  GeoCal::VicarImageCoordinate::VicarImageCoordinate "
GeoCal::VicarImageCoordinate::VicarImageCoordinate()
Default constructor, line and sample aren't initialized. ";

%feature("docstring")  GeoCal::VicarImageCoordinate::~VicarImageCoordinate "
virtual GeoCal::VicarImageCoordinate::~VicarImageCoordinate()
";

// File: a00218.xml

%feature("docstring") GeoCal::VicarLiteDem "
This uses VicarLiteFile to implement a Dem.

We use VicarLiteFile to implement a Dem

C++ includes: vicar_lite_file.h ";

%feature("docstring")  GeoCal::VicarLiteDem::band "
int GeoCal::VicarLiteDem::band() const
Return band number. ";

%feature("docstring")  GeoCal::VicarLiteDem::datum "
const Datum& GeoCal::DemMapInfo::datum() const
Datum height is relative to. ";

%feature("docstring")  GeoCal::VicarLiteDem::datum_ptr "
const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
Pointer to datum. ";

%feature("docstring")  GeoCal::VicarLiteDem::distance_to_surface "
double DemMapInfo::distance_to_surface(const GroundCoordinate &Gp) const
Return distance to surface directly above/below the given point.

Distance is in meters. Positive means Gp is above the surface,
negative means below. ";

%feature("docstring")  GeoCal::VicarLiteDem::elevation "
virtual double GeoCal::VicarLiteDem::elevation(int Y_index, int X_index) const
Return height in meters relative to datum(). ";

%feature("docstring")  GeoCal::VicarLiteDem::file "
const VicarLiteFile& GeoCal::VicarLiteDem::file() const
Underlying file. ";

%feature("docstring")  GeoCal::VicarLiteDem::file "
VicarLiteFile& GeoCal::VicarLiteDem::file()
Underlying file. ";

%feature("docstring")  GeoCal::VicarLiteDem::file_ptr "
boost::shared_ptr<VicarLiteFile> GeoCal::VicarLiteDem::file_ptr() const
";
%feature("docstring")  GeoCal::VicarLiteDem::height_datum "
double DemMapInfo::height_datum(const GroundCoordinate &Gp) const
Height relative to datum. ";

%feature("docstring")  GeoCal::VicarLiteDem::height_reference_surface "
double DemMapInfo::height_reference_surface(const GroundCoordinate &Gp) const
Return height of surface above/below the reference surface (e.g.,
WGS-84 for the earth).

Positive means above, negative below. This is in meters. ";

%feature("docstring")  GeoCal::VicarLiteDem::intersect "
boost::shared_ptr< CartesianFixed > Dem::intersect(const CartesianFixed &Cf, const CartesianFixedLookVector &Lv, double
Resolution, double Max_height=9000) const
Determine the intersection along a look vector from a starting
position with the Dem (e.g., what point on the ground does a satellite
see).

This is brute force, so it won't mistakenly pick a plain behind a
mountain.

Resolution is the desired resolution in meters. Max_height is the
maximum height we will encounter in the Dem - the default of 9 km is
higher than the highest point in the world. ";

%feature("docstring")  GeoCal::VicarLiteDem::map_info "
const MapInfo& GeoCal::DemMapInfo::map_info() const
MapInfo of underlying data. ";

%feature("docstring")  GeoCal::VicarLiteDem::outside_dem_is_error "
bool GeoCal::DemMapInfo::outside_dem_is_error() const
If true, then calling with values outside the range of area of the DEM
triggers an exception, otherwise we just return a height of 0. ";

%feature("docstring")  GeoCal::VicarLiteDem::print "
virtual void GeoCal::VicarLiteDem::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::VicarLiteDem::print_to_string "
std::string GeoCal::Printable< Dem  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarLiteDem::surface_point "
boost::shared_ptr< GroundCoordinate > DemMapInfo::surface_point(const GroundCoordinate &Gp) const
Return a GroundCoordinate on the surface directly above or below the
given point. ";

%feature("docstring")  GeoCal::VicarLiteDem::VicarLiteDem "
GeoCal::VicarLiteDem::VicarLiteDem(const std::string &Fname, bool Outside_dem_is_error=false, const
boost::shared_ptr< Datum > &D=boost::shared_ptr< Datum >(new
NoDatum()), int Band=0)
Constructor.

If Outside_dem_is_error is true, then calls for Dem data outside of
the range of the Dem cause an exception to be thrown, otherwise we
just return a height of 0. ";

%feature("docstring")  GeoCal::VicarLiteDem::~VicarLiteDem "
virtual GeoCal::VicarLiteDem::~VicarLiteDem()
";

// File: a00219.xml

%feature("docstring") GeoCal::VicarLiteFile "
This handles opening and closing a Vicar file.

This class doesn't actually read and write data, but is used by other
classes which do.

This class is a simple implementation of Vicar. It doesn't use the
Vicar RTL library, which is the primary attraction of it. We can use
this to read Vicar files in a context where we don't want to use the
RTL library, or don't want to be constrained to running in the Vicar
environment.

This class does not handle new VICAR features such as compression.

C++ includes: vicar_lite_file.h ";

%feature("docstring")  GeoCal::VicarLiteFile::access "
access_type GeoCal::VicarLiteFile::access() const
Access type of file. ";

%feature("docstring")  GeoCal::VicarLiteFile::data_offset "
int GeoCal::VicarLiteFile::data_offset() const
Offset to where the data starts. ";

%feature("docstring")  GeoCal::VicarLiteFile::file_name "
const std::string& GeoCal::VicarLiteFile::file_name() const
File name. ";

%feature("docstring")  GeoCal::VicarLiteFile::force_area_pixel "
bool GeoCal::VicarLiteFile::force_area_pixel() const
If true, then force the file to be treated as \"pixel is area\".

This is really just meant as a work around for the SRTM data, which
incorrectly labels the data as \"point\" rather than \"area\". Since
this is a 15 meter difference, it matters for many applications. Most
users should just ignore this value. ";

%feature("docstring")  GeoCal::VicarLiteFile::has_label "
bool GeoCal::VicarLiteFile::has_label(const std::string &Lbl) const
Test if a label is found in a file, and if so return true.

Otherwise return false. ";

%feature("docstring")  GeoCal::VicarLiteFile::has_map_info "
bool VicarLiteFile::has_map_info() const
Return true if the file has a GEOTIFF label in it, indicating it has
map information. ";

%feature("docstring")  GeoCal::VicarLiteFile::has_rpc "
bool VicarLiteFile::has_rpc() const
Return true if the file has a NITF_CETAG label in it, indicating it
has RPC information. ";

%feature("docstring")  GeoCal::VicarLiteFile::is_compressed "
bool GeoCal::VicarLiteFile::is_compressed() const
Indicate if the file is compressed.

If it is, we can't actually read the data but we can still read all of
the labels. ";

%feature("docstring")  GeoCal::VicarLiteFile::label "
double GeoCal::VicarLiteFile::label< double >(const std::string &F, const std::string &Property=\"\") const
Return value for the given label.

Optionally also supply a property for labels that are part of one
(e.g., GEOTIFF) ";

%feature("docstring")  GeoCal::VicarLiteFile::label_map "
const std::map<std::string, std::string>& GeoCal::VicarLiteFile::label_map() const
Sometimes it is more convenient to work directly with a map. ";

%feature("docstring")  GeoCal::VicarLiteFile::label_string "
std::string GeoCal::VicarLiteFile::label_string(const std::string &F, const std::string &Property=\"\") const
Non template form of label, useful in some contexts. ";

%feature("docstring")  GeoCal::VicarLiteFile::map_info "
MapInfo VicarLiteFile::map_info() const
Read metadata for MapInfo. ";

%feature("docstring")  GeoCal::VicarLiteFile::map_info "
void VicarLiteFile::map_info(const MapInfo &M)
Set metadata for MapInfo. ";

%feature("docstring")  GeoCal::VicarLiteFile::number_band "
int GeoCal::VicarLiteFile::number_band() const
Number of bands in file. ";

%feature("docstring")  GeoCal::VicarLiteFile::number_line "
int GeoCal::VicarLiteFile::number_line() const
Number of lines in file. ";

%feature("docstring")  GeoCal::VicarLiteFile::number_sample "
int GeoCal::VicarLiteFile::number_sample() const
Number of samples in file. ";

%feature("docstring")  GeoCal::VicarLiteFile::print "
virtual void GeoCal::VicarLiteFile::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::VicarLiteFile::print_to_string "
std::string GeoCal::Printable< VicarLiteFile  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarLiteFile::property "
const std::set<std::string>& GeoCal::VicarLiteFile::property() const
List of properties. ";

%feature("docstring")  GeoCal::VicarLiteFile::read_double "
double GeoCal::VicarLiteFile::read_double(int B, int L, int S) const
Read data as an double. ";

%feature("docstring")  GeoCal::VicarLiteFile::read_double "
void GeoCal::VicarLiteFile::read_double(int B, int L, int S, int Nb, int Nl, int Ns, double *Res) const
Read data as a double. ";

%feature("docstring")  GeoCal::VicarLiteFile::read_int "
int GeoCal::VicarLiteFile::read_int(int B, int L, int S) const
Read data as an int. ";

%feature("docstring")  GeoCal::VicarLiteFile::read_int "
void GeoCal::VicarLiteFile::read_int(int B, int L, int S, int Nb, int Nl, int Ns, int *Res) const
Read data as an int. ";

%feature("docstring")  GeoCal::VicarLiteFile::rpc "
Rpc VicarLiteFile::rpc() const
Read metadata for Rpc. ";

%feature("docstring")  GeoCal::VicarLiteFile::rpc "
void GeoCal::VicarLiteFile::rpc(const Rpc &R)
";
%feature("docstring")  GeoCal::VicarLiteFile::type "
data_type GeoCal::VicarLiteFile::type() const
Type of data in file. ";

%feature("docstring")  GeoCal::VicarLiteFile::VicarLiteFile "
VicarLiteFile::VicarLiteFile(const std::string &Fname, access_type Access=READ, bool
Force_area_pixel=false)
Open an existing VICAR file for reading or update.

The Force_area_pixel forces the file to be treated as \"pixel as
area\" rather than \"pixel as point\". This is really just meant as a
work around for the SRTM data, which incorrectly labels the data as
\"point\" rather than \"area\". Since this is a 15 meter difference,
it matters for many applications. Most users should just ignore this
value. ";

%feature("docstring")  GeoCal::VicarLiteFile::VicarLiteFile "
VicarLiteFile::VicarLiteFile(const std::string &Fname, int Number_line, int Number_sample, const
std::string &Type=\"BYTE\")
Create a new VICAR file with the given size. ";

%feature("docstring")  GeoCal::VicarLiteFile::write_int "
void GeoCal::VicarLiteFile::write_int(int B, int L, int S, int V) const
Write data as an int. ";

%feature("docstring")  GeoCal::VicarLiteFile::~VicarLiteFile "
virtual GeoCal::VicarLiteFile::~VicarLiteFile()
";
%feature("docstring")  GeoCal::VicarLiteFile::is_vicar_file "
bool VicarLiteFile::is_vicar_file(const std::string &Fname)
Determine if a given file is a VICAR file or not.

This looks for the first few characters of the file being \"LBLSIZE=\"
";


// File: a00220.xml

%feature("docstring") GeoCal::VicarLiteRasterImage "
This uses VicarLiteFile to implement a RasterImage.

We use a VicarLiteFile to implement a RasterImage.

C++ includes: vicar_lite_file.h ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::band "
int GeoCal::VicarLiteRasterImage::band() const
Return band number. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::file "
const VicarLiteFile& GeoCal::VicarLiteRasterImage::file() const
Underlying file. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::file "
VicarLiteFile& GeoCal::VicarLiteRasterImage::file()
Underlying file. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::file_ptr "
boost::shared_ptr<VicarLiteFile> GeoCal::VicarLiteRasterImage::file_ptr() const
";
%feature("docstring")  GeoCal::VicarLiteRasterImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::is_compressed "
bool GeoCal::VicarLiteRasterImage::is_compressed() const
Indicate if the file is compressed.

If it is, we can't actually read the data but we can still read all of
the labels. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::print "
virtual void GeoCal::VicarLiteRasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::read_double "
virtual blitz::Array<double, 2> GeoCal::VicarLiteRasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::read_ptr "
virtual void GeoCal::VicarLiteRasterImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::unchecked_read "
virtual int GeoCal::VicarLiteRasterImage::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::unchecked_write "
virtual void GeoCal::VicarLiteRasterImage::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::VicarLiteRasterImage "
GeoCal::VicarLiteRasterImage::VicarLiteRasterImage(const std::string &Fname, access_type Access=VicarLiteFile::READ, int
Band=0, int Number_tile_line=-1, int Number_tile_sample=-1, bool
Force_area_pixel=false)
Constructor.

The Force_area_pixel forces the file to be treated as \"pixel as
area\" rather than \"pixel as point\". This is really just meant as a
work around for the SRTM data, which incorrectly labels the data as
\"point\" rather than \"area\". Since this is a 15 meter difference,
it matters for many applications. Most users should just ignore this
value. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::VicarLiteRasterImage::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::VicarLiteRasterImage::~VicarLiteRasterImage "
virtual GeoCal::VicarLiteRasterImage::~VicarLiteRasterImage()
";

// File: a00221.xml

%feature("docstring") GeoCal::VicarMultiFile "
This gives read access to some of the larger AFIDS datasets, such as
CIB and SRTM.

These larger datasets split into a number of VICAR files. Each one of
these is a some extent in latitude/longitude, for example 1 x 1 degree
tiles for the SRTM. There is an index IBIS file that gives a listing
of all the input datasets and where they are located.

This class gives an interface to this set of VICAR files that looks
like one really big RasterImage.

Note that the files might not cover the full extent of area, for
example one of the 1x1 degree SRTM tiles might be missing. If we
attempt to access the area covered by a missing file then an error is
thrown. We may want to come up with a more robust interface, but for
now this interface seems to be sufficient.

C++ includes: vicar_multi_file.h ";

%feature("docstring")  GeoCal::VicarMultiFile::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::VicarMultiFile::get_file "
RasterMultifileTile VicarMultiFile::get_file(int Line, int Sample) const
Get a file that covers the given location. ";

%feature("docstring")  GeoCal::VicarMultiFile::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::VicarMultiFile::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::VicarMultiFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarMultiFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarMultiFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarMultiFile::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarMultiFile::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::VicarMultiFile::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::VicarMultiFile::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::VicarMultiFile::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::VicarMultiFile::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::VicarMultiFile::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::VicarMultiFile::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::VicarMultiFile::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::VicarMultiFile::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::VicarMultiFile::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::VicarMultiFile::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::VicarMultiFile::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::VicarMultiFile::print "
virtual void GeoCal::VicarMultiFile::print(std::ostream &Os) const
Write to a stream. ";

%feature("docstring")  GeoCal::VicarMultiFile::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarMultiFile::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::VicarMultiFile::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarMultiFile::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarMultiFile::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarMultiFile::read_double "
blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::VicarMultiFile::read_ptr "
void RasterMultifile::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image.

Read a subset of the data. ";

%feature("docstring")  GeoCal::VicarMultiFile::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::VicarMultiFile::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::VicarMultiFile::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::VicarMultiFile::unchecked_read "
virtual int GeoCal::RasterMultifile::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::VicarMultiFile::unchecked_write "
virtual void GeoCal::RasterMultifile::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::VicarMultiFile::VicarMultiFile "
VicarMultiFile::VicarMultiFile(const std::string &Db_name, const std::string &Dirbase, const
std::string &Extension=\"\", int Number_tile_line=10000, int
Number_tile_sample=10000, int Number_line_per_tile=100000, int
Number_tile_each_file=4, int Number_file=4, bool
Favor_memory_mapped=true, bool No_coverage_is_error=true, int
No_coverage_fill_value=0, bool Force_area_pixel=false)
Constructor.

This takes the database file (an IBIS file) and the directory that the
files pointed to by the database file are located. The directory is
needed because although the files are often in the same directory as
the database file they aren't always. The database file doesn't
contain directory information, just the file name, so we need to pass
the directory in.

For some odd reason, the file names also don't always have the
extension on them. For some databases, you need to add something like
\".img\" to get the actual file name. You can pass any extension that
is needed.

There are two kinds of tiling going on. At the top level, we have a
number of files open at one time, given by Number_file. For each file,
we read that it tiles with the given Number_line_per_tile and
Number_tile_each_file tiles.

There are two strategies that can be used to reading a Vicar file. One
reads a tile of a given number of lines and store it in memory. The
other uses memory mapped file io to read data on demand. Often but not
always the memory mapped file is faster, particularly for small files
or for files with a large number of samples where only a portion is
used. You can control which is used by setting Favor_memory_mapped. If
the underlying data uses VICAR compression, then we always use the
VICAR IO routines since memory mapping won't work. This class handles
this case - it checks if the file is compressed before trying to do
memory mapping.

The Force_area_pixel forces the file to be treated as \"pixel as
area\" rather than \"pixel as point\". This is really just meant as a
work around for the SRTM data, which incorrectly labels the data as
\"point\" rather than \"area\". Since this is a 15 meter difference,
it matters for may applications. Most users should just ignore this
value. ";

%feature("docstring")  GeoCal::VicarMultiFile::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::VicarMultiFile::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::VicarMultiFile::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::VicarMultiFile::~VicarMultiFile "
virtual GeoCal::VicarMultiFile::~VicarMultiFile()
";

// File: a00222.xml

%feature("docstring") GeoCal::VicarNotAvailableException "
Exception thrown if Vicar isn't available.

C++ includes: geocal_exception.h ";

%feature("docstring")  GeoCal::VicarNotAvailableException::VicarNotAvailableException "
GeoCal::VicarNotAvailableException::VicarNotAvailableException()
Constructor. ";

%feature("docstring")  GeoCal::VicarNotAvailableException::what "
virtual const char* GeoCal::Exception::what() const
 throw ()
Description of what the error is.
";
%feature("docstring")  GeoCal::VicarNotAvailableException::~VicarNotAvailableException "
virtual GeoCal::VicarNotAvailableException::~VicarNotAvailableException()
 throw ()
Destructor.
";

// File: a00223.xml

%feature("docstring") GeoCal::VicarOgr "
This class is really part of VicarFile, but because of the
complication in this software we separate this out into its own class.

This class handles the reading and writing of the GeoTIFF map
projection and coordinate transformation information, going to and
from a MapInfo.

AFIDS stores map projection information as text labels in a VICAR
file. The text is GeoTIFF tags stored as text. We can't directly work
with these to supply map projection information. Instead, we want to
use the GDAL library to handle this (through the OgrCoordinate class).
However, there is no easy mapping between GDAL which use the Well
Known Text (WKT) to express its coordinate information and GeoTIFF
which uses about 40 different tags for this information.

The two systems contain similar information, so one possible approach
would be to create a mapping between the two systems - e.g., Tag X
corresponds to WKT node Y. While possible, this would result in a
large amount of code.

As an alternative, we take advantage of the ability of GDAL to create
and read GeoTIFF files. The GDAL library contains all of the code
connecting the two, which we don't want to duplicate.

This class creates a temporary GeoTIFF file, and either writes map
projection information using GDAL and a MapInfo, or from the VICAR
GeoTIFF information. We then read the file and go the other way,
creating a MapInfo or the metadata for a VICAR file. The temporary
file is then removed.

This is a bit awkward, but this is the best approach I could come up
with to map VICAR and GDAL together.

C++ includes: vicar_ogr.h ";

%feature("docstring")  GeoCal::VicarOgr::from_vicar "
MapInfo VicarOgr::from_vicar(const VicarFile &F)
Read the metadata from a Vicar File, and use to create a MapInfo based
on GDAL. ";

%feature("docstring")  GeoCal::VicarOgr::from_vicar "
MapInfo VicarOgr::from_vicar(const VicarLiteFile &F)
Read the metadata from a VicarLiteFile, and use to create a MapInfo
based on GDAL. ";

%feature("docstring")  GeoCal::VicarOgr::to_vicar "
void VicarOgr::to_vicar(const MapInfo &Mi, VicarFile &F)
Use a MapInfo to write the GeoTIFF metadata for a Vicar File. ";

%feature("docstring")  GeoCal::VicarOgr::VicarOgr "
VicarOgr::VicarOgr()
Constructor.

Normally we only have one of these objects around, although nothing is
hurt if you create multiple copies. ";


// File: a00224.xml

%feature("docstring") GeoCal::VicarRasterImage "
This wraps a VicarTiledFile<T> to make a RasterImage.

The type of T should be one we can convert to and from a int, since
this is what a RasterImage works in.

C++ includes: vicar_raster_image.h ";

%feature("docstring")  GeoCal::VicarRasterImage::close "
void GeoCal::VicarRasterImage::close()
Close a file.

You don't normally need to call this directly, it is done by the
destructor. But it is useful to have for use by Ruby. ";

%feature("docstring")  GeoCal::VicarRasterImage::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::VicarRasterImage::flush "
virtual void GeoCal::VicarRasterImage::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::VicarRasterImage::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::VicarRasterImage::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::VicarRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarRasterImage::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::VicarRasterImage::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::VicarRasterImage::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::VicarRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::VicarRasterImage::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::VicarRasterImage::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::VicarRasterImage::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::VicarRasterImage::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::VicarRasterImage::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::VicarRasterImage::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::VicarRasterImage::number_swap "
unsigned int GeoCal::RasterImageTiledFile::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::VicarRasterImage::number_tile "
int GeoCal::RasterImageTiledFile::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::VicarRasterImage::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::VicarRasterImage::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::VicarRasterImage::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::VicarRasterImage::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::VicarRasterImage::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::VicarRasterImage::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::VicarRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarRasterImage::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarRasterImage::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarRasterImage::read_double "
virtual blitz::Array<double, 2> GeoCal::RasterImageTiledFile::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Some raster images actually are floating point values (e.g., a
CalcRaster).

This function gives access to the underlying floating point numbers.
The default function here just returns the result of read as a double,
but derived classes can override this. ";

%feature("docstring")  GeoCal::VicarRasterImage::read_ptr "
virtual void GeoCal::RasterImageTiledFile::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::VicarRasterImage::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::VicarRasterImage::reset_number_swap "
void GeoCal::RasterImageTiledFile::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::VicarRasterImage::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::VicarRasterImage::set_map_info "
void GeoCal::VicarRasterImage::set_map_info(const MapInfo &Mi)
Set the Map Info. ";

%feature("docstring")  GeoCal::VicarRasterImage::set_rpc "
void GeoCal::VicarRasterImage::set_rpc(const Rpc &R)
Set the RPC. ";

%feature("docstring")  GeoCal::VicarRasterImage::tile_file_base "
const TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base() const
Underlying data. ";

%feature("docstring")  GeoCal::VicarRasterImage::tile_file_base "
TiledFileBase<2>& GeoCal::RasterImageTiledFile::tile_file_base()
Underlying data. ";

%feature("docstring")  GeoCal::VicarRasterImage::tile_file_base_ptr "
const boost::shared_ptr<TiledFileBase<2> >& GeoCal::RasterImageTiledFile::tile_file_base_ptr() const
Underlying data. ";

%feature("docstring")  GeoCal::VicarRasterImage::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::VicarRasterImage::unchecked_read "
virtual int GeoCal::RasterImageTiledFile::unchecked_read(int Line, int Sample) const
Return pixel value at given line and sample. ";

%feature("docstring")  GeoCal::VicarRasterImage::unchecked_write "
virtual void GeoCal::RasterImageTiledFile::unchecked_write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::VicarRasterImage::vicar_file "
const VicarFile& GeoCal::VicarRasterImage::vicar_file() const
Underlying file. ";

%feature("docstring")  GeoCal::VicarRasterImage::vicar_file "
VicarFile& GeoCal::VicarRasterImage::vicar_file()
Underlying file. ";

%feature("docstring")  GeoCal::VicarRasterImage::vicar_file_ptr "
boost::shared_ptr<VicarFile> GeoCal::VicarRasterImage::vicar_file_ptr() const
";
%feature("docstring")  GeoCal::VicarRasterImage::VicarRasterImage "
GeoCal::VicarRasterImage::VicarRasterImage(const std::string &Fname, access_type Access=VicarFile::READ, int
Number_line_per_tile=100, int Number_tile=4, bool
Force_area_pixel=false)
Open an existing VICAR file for reading or update.

The Force_area_pixel forces the file to be treated as \"pixel as
area\" rather than \"pixel as point\". This is really just meant as a
work around for the SRTM data, which incorrectly labels the data as
\"point\" rather than \"area\". Since this is a 15 meter difference,
it matters for may applications. Most users should just ignore this
value. ";

%feature("docstring")  GeoCal::VicarRasterImage::VicarRasterImage "
GeoCal::VicarRasterImage::VicarRasterImage(const std::string &Fname, const std::string &Type, int Number_line,
int Number_sample, int Number_line_per_tile=100, compression
C=VicarFile::NONE)
Create a new VICAR file with the given size. ";

%feature("docstring")  GeoCal::VicarRasterImage::VicarRasterImage "
GeoCal::VicarRasterImage::VicarRasterImage(const std::string &Fname, const MapInfo &M, const std::string
&Type=\"BYTE\", int Number_line_per_tile=100, compression
C=VicarFile::NONE)
Create a new VICAR file with the given size. ";

%feature("docstring")  GeoCal::VicarRasterImage::VicarRasterImage "
GeoCal::VicarRasterImage::VicarRasterImage(int Instance, access_type Access=VicarFile::READ, const std::string
&Name=\"INP\", int Number_line_per_tile=100, int Number_tile=4)
Open a file, using the VICAR Name and Instance input (so for example,
\"INP\" and 2 is the second INP file passed to a VICAR program. ";

%feature("docstring")  GeoCal::VicarRasterImage::VicarRasterImage "
GeoCal::VicarRasterImage::VicarRasterImage(int Instance, const std::string &Type, int Number_line, int
Number_sample, const std::string &Name=\"OUT\", int
Number_line_per_tile=100, compression C=VicarFile::NONE)
Create a new VICAR file with the given size.

Use the VICAR Name and Instance input (so for example, \"INP\" and 2
is the second INP file passed to a VICAR program. ";

%feature("docstring")  GeoCal::VicarRasterImage::VicarRasterImage "
GeoCal::VicarRasterImage::VicarRasterImage(int Instance, const MapInfo &M, const std::string &Type=\"BYTE\",
const std::string &Name=\"OUT\", int Number_line_per_tile=100,
compression C=VicarFile::NONE)
Create a new VICAR file with the given size.

Use the VICAR Name and Instance input (so for example, \"INP\" and 2
is the second INP file passed to a VICAR program. ";

%feature("docstring")  GeoCal::VicarRasterImage::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::VicarRasterImage::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::VicarRasterImage::write_ptr "
virtual void GeoCal::RasterImageTiledFile::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the image. ";

%feature("docstring")  GeoCal::VicarRasterImage::~VicarRasterImage "
virtual GeoCal::VicarRasterImage::~VicarRasterImage()
Destructor. ";


// File: a00225.xml

%feature("docstring") GeoCal::VicarTiledFile "
This reads and writes a VICAR file, as a TiledFile.

C++ includes: vicar_tiled_file.h ";

%feature("docstring")  GeoCal::VicarTiledFile::can_set_map_info "
virtual bool GeoCal::VicarTiledFile< T >::can_set_map_info() const
We can set MapInfo for this kind of file. ";

%feature("docstring")  GeoCal::VicarTiledFile::flush "
virtual void GeoCal::TiledFile< T, D >::flush() const
Flush data to disk. ";

%feature("docstring")  GeoCal::VicarTiledFile::get_double "
virtual double GeoCal::TiledFile< T, D >::get_double(const boost::array< index, D > &Index) const
Get data an convert to double. ";

%feature("docstring")  GeoCal::VicarTiledFile::get_int "
virtual int GeoCal::TiledFile< T, D >::get_int(const boost::array< index, D > &Index) const
Get data an convert to integer. ";

%feature("docstring")  GeoCal::VicarTiledFile::has_map_info "
virtual bool GeoCal::VicarTiledFile< T >::has_map_info() const
True if we have map info. ";

%feature("docstring")  GeoCal::VicarTiledFile::map_info "
virtual MapInfo GeoCal::VicarTiledFile< T >::map_info() const
Return MapInfo. ";

%feature("docstring")  GeoCal::VicarTiledFile::map_info "
virtual void GeoCal::VicarTiledFile< T >::map_info(const MapInfo &M)
Set MapInfo. ";

%feature("docstring")  GeoCal::VicarTiledFile::number_swap "
unsigned int GeoCal::TiledFileBase< D >::number_swap() const
Number of times we have swapped a tile since reset_number_swap called.
";

%feature("docstring")  GeoCal::VicarTiledFile::number_tile "
int GeoCal::TiledFile< T, D >::number_tile() const
Number of tiles. ";

%feature("docstring")  GeoCal::VicarTiledFile::read "
boost::multi_array<T, D> GeoCal::TiledFile< T, D >::read(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index) const
Read a subset of the data. ";

%feature("docstring")  GeoCal::VicarTiledFile::read_double "
virtual void GeoCal::TiledFile< T, D >::read_double(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, double *Res) const
Read a subset of the data, and convert to double. ";

%feature("docstring")  GeoCal::VicarTiledFile::read_int "
virtual void GeoCal::TiledFile< T, D >::read_int(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, int *Res) const
Read a subset of the data, and convert to int. ";

%feature("docstring")  GeoCal::VicarTiledFile::read_ptr "
virtual void GeoCal::TiledFile< T, D >::read_ptr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, T *Res) const
Read a subset of the data.

Check first to see if entire requested data is in a single tile. If
not, the we read from disk (rather than trying to stitch together
multiple tiles). ";

%feature("docstring")  GeoCal::VicarTiledFile::reset_number_swap "
void GeoCal::TiledFileBase< D >::reset_number_swap()
Reset number of swap counter to 0. ";

%feature("docstring")  GeoCal::VicarTiledFile::size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::size() const
File size. ";

%feature("docstring")  GeoCal::VicarTiledFile::tile_size "
const boost::array<index, D>& GeoCal::TiledFileBase< D >::tile_size() const
Tile size. ";

%feature("docstring")  GeoCal::VicarTiledFile::vicar_file "
const VicarFile& GeoCal::VicarTiledFile< T >::vicar_file() const
VicarFile associated with this class. ";

%feature("docstring")  GeoCal::VicarTiledFile::vicar_file "
VicarFile& GeoCal::VicarTiledFile< T >::vicar_file()
VicarFile associated with this class. ";

%feature("docstring")  GeoCal::VicarTiledFile::VicarTiledFile "
GeoCal::VicarTiledFile< T >::VicarTiledFile(const boost::shared_ptr< VicarFile > &F, int
Number_line_per_tile=100, int Number_tile=4)
Constructor. ";

%feature("docstring")  GeoCal::VicarTiledFile::write "
void GeoCal::TiledFile< T, D >::write(const boost::array< index, D > &Min_index, const boost::multi_array<
T, D > &V) const
Write a subset of the data. ";

%feature("docstring")  GeoCal::VicarTiledFile::write_double "
virtual void GeoCal::TiledFile< T, D >::write_double(const boost::array< index, D > &Index, double Val)
Convert a double to type T, and write to file. ";

%feature("docstring")  GeoCal::VicarTiledFile::write_double_arr "
virtual void GeoCal::TiledFile< T, D >::write_double_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const double *V)
Convert a double to type T, and write to file. ";

%feature("docstring")  GeoCal::VicarTiledFile::write_int "
virtual void GeoCal::TiledFile< T, D >::write_int(const boost::array< index, D > &Index, int Val)
Convert an int to type T, and write to file. ";

%feature("docstring")  GeoCal::VicarTiledFile::write_int_arr "
virtual void GeoCal::TiledFile< T, D >::write_int_arr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const int *V)
Convert an int to type T, and write to file. ";

%feature("docstring")  GeoCal::VicarTiledFile::write_ptr "
virtual void GeoCal::TiledFile< T, D >::write_ptr(const boost::array< index, D > &Min_index, const boost::array< index,
D > &Max_index, const T *V) const
Write a subset of the data. ";

%feature("docstring")  GeoCal::VicarTiledFile::~VicarTiledFile "
virtual GeoCal::VicarTiledFile< T >::~VicarTiledFile()
Destructor. ";


// File: a01220.xml

%feature("docstring") std::weak_ptr "
STL class. ";


// File: a01126.xml

%feature("docstring") std::wfstream "
STL class. ";


// File: a01134.xml

%feature("docstring") std::wifstream "
STL class. ";


// File: a01136.xml

%feature("docstring") std::wios "
STL class. ";


// File: a01144.xml

%feature("docstring") std::wistream "
STL class. ";


// File: a01142.xml

%feature("docstring") std::wistringstream "
STL class. ";


// File: a01147.xml

%feature("docstring") std::wofstream "
STL class. ";


// File: a00226.xml

%feature("docstring") GeoCal::WorldView2CloudMask "
This create a cloud mask using WorldView2 mulispectral data.

This cloud mask has two parts:

A simple threshold on the coastal blue band to detect clouds. A ratio
comparision of (band 8 - band 1) / band 1 < threshold to detect cloud
shadows.

To help with false positives, we have a minimum cloud size. This is
used to average the data, and only if the lower resolution data shows
a cloud or a shadow do we mark a pixel as cloudy or in shadow.

We use reflectance data. It can be convenient to scale the reflectance
data (e.g., go from 0 to 32767 to give half word VICAR data). This
class doesn't care, as long as the Cloud_threshold is scaled the same
way.

C++ includes: worldview2_cloudmask.h ";

%feature("docstring")  GeoCal::WorldView2CloudMask::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::WorldView2CloudMask "
WorldView2CloudMask::WorldView2CloudMask(const RasterImageMultiBand &Mul_spec_data, int Min_cloud_size, double
Cloud_threshold, double Shadow_threshold)
Constructor. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  GeoCal::WorldView2CloudMask::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  GeoCal::WorldView2CloudMask::~WorldView2CloudMask "
virtual GeoCal::WorldView2CloudMask::~WorldView2CloudMask()
";

// File: a00227.xml

%feature("docstring") WorldView2CloudMaskHelper "
";
%feature("docstring")  WorldView2CloudMaskHelper::calc "
virtual void WorldView2CloudMaskHelper::calc(int Lstart, int Sstart) const
Calculate data for given array.

Data will be written in data member variable, which has already been
resize to the correct dimension. ";

%feature("docstring")  WorldView2CloudMaskHelper::coordinate "
ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
Shortcut to calling mapinfo().coordinate. ";

%feature("docstring")  WorldView2CloudMaskHelper::grid_center_line_resolution "
double RasterImage::grid_center_line_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the line direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2 + 1,
number_sample() / 2. ";

%feature("docstring")  WorldView2CloudMaskHelper::grid_center_sample_resolution "
double RasterImage::grid_center_sample_resolution() const
This calculates the grid resolution in meters for the center of the
grid in the sample direction.

This is just the distance between The center pixel number_line() / 2,
number_sample() / 2 and the pixel number_line() / 2, number_sample() /
2 + 1. ";

%feature("docstring")  WorldView2CloudMaskHelper::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  WorldView2CloudMaskHelper::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(const ImageCoordinate &Ic, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  WorldView2CloudMaskHelper::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  WorldView2CloudMaskHelper::ground_coordinate "
boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
Shortcut to calling mapinfo().ground_coordinate. ";

%feature("docstring")  WorldView2CloudMaskHelper::has_map_info "
virtual bool GeoCal::RasterImageVariable::has_map_info() const
Functions available if we have MapInfo data.

Indicate if we have MapInfo. The default is false, but derived classes
can override this. ";

%feature("docstring")  WorldView2CloudMaskHelper::has_rpc "
virtual bool GeoCal::RasterImageVariable::has_rpc() const
Functions available if we have RPC

Indicate if we have Rpc. The default is false, but derived classes can
override this. ";

%feature("docstring")  WorldView2CloudMaskHelper::interpolate "
double GeoCal::RasterImage::interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  WorldView2CloudMaskHelper::interpolate "
blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample) const
Interpolate a region, starting with the given fractional line and
sample.

This is a bilinear interpolation. ";

%feature("docstring")  WorldView2CloudMaskHelper::interpolate "
double GeoCal::RasterImage::interpolate(const ImageCoordinate &Ic) const
This does a bilinear interpolation of the data for fractional Line and
Sample. ";

%feature("docstring")  WorldView2CloudMaskHelper::interpolate_derivative "
boost::array<double, 2> GeoCal::RasterImage::interpolate_derivative(double Line, double Sample) const
This calculates the derivative of interpolate with respect to Line and
Sample.

First entry in results is derivative with respect to Line, the second
is with respect to the Sample. ";

%feature("docstring")  WorldView2CloudMaskHelper::map_info "
virtual const MapInfo& GeoCal::RasterImageVariable::map_info() const
MapInfo for image. ";

%feature("docstring")  WorldView2CloudMaskHelper::number_line "
virtual int GeoCal::RasterImageVariable::number_line() const
Return number of lines in the image. ";

%feature("docstring")  WorldView2CloudMaskHelper::number_sample "
virtual int GeoCal::RasterImageVariable::number_sample() const
Return number of samples in the image. ";

%feature("docstring")  WorldView2CloudMaskHelper::number_tile_line "
virtual int GeoCal::RasterImageVariable::number_tile_line() const
Number of lines in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  WorldView2CloudMaskHelper::number_tile_sample "
virtual int GeoCal::RasterImageVariable::number_tile_sample() const
Number of samples in the preferred tile size.

Default is the entire image. ";

%feature("docstring")  WorldView2CloudMaskHelper::overview "
virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
Some RasterImage may have overviews which are lower resolution images.

If we do have these, then this will return the lower resolution
RasterImage that has at least the given number of samples. If we don't
have an overview, this returns a null - which the calling routine
needs to be ready to handle. ";

%feature("docstring")  WorldView2CloudMaskHelper::print "
virtual void GeoCal::RasterImage::print(std::ostream &Os) const
Print to stream. ";

%feature("docstring")  WorldView2CloudMaskHelper::print_to_string "
std::string GeoCal::Printable< RasterImage  >::print_to_string() const
Print to string.

This is primarily useful for SWIG wrappers to this class, e.g. a to_s
method in ruby. ";

%feature("docstring")  WorldView2CloudMaskHelper::read "
int GeoCal::RasterImage::read(int Line, int Sample) const
Alternate name for operator().

Languages that wrap this class such as Ruby don't support operator(),
so we give another name for the same function. ";

%feature("docstring")  WorldView2CloudMaskHelper::read "
blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  WorldView2CloudMaskHelper::read "
blitz::Array< int, 2 > RasterImage::read(const RasterImageTileIterator &Ti) const
Return a subset of the image. ";

%feature("docstring")  WorldView2CloudMaskHelper::read_array "
boost::multi_array< int, 2 > RasterImage::read_array(int Lstart, int Sstart, int Number_line, int Number_sample) const
Return a subset of the image. ";

%feature("docstring")  WorldView2CloudMaskHelper::read_double "
blitz::Array< double, 2 > CalcRaster::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
Calculate the results as a blitz::Array of doubles. ";

%feature("docstring")  WorldView2CloudMaskHelper::read_ptr "
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
const
Return a subset of the image. ";

%feature("docstring")  WorldView2CloudMaskHelper::remove_rpc "
void GeoCal::RasterImageVariable::remove_rpc()
Remove an RPC. ";

%feature("docstring")  WorldView2CloudMaskHelper::rpc "
virtual Rpc GeoCal::RasterImageVariable::rpc() const
Rpc for image. ";

%feature("docstring")  WorldView2CloudMaskHelper::unchecked_interpolate "
double GeoCal::RasterImage::unchecked_interpolate(double Line, double Sample) const
This does a bilinear interpolation of the data for fractional Line and
Sample.

This does not do range checking. ";

%feature("docstring")  WorldView2CloudMaskHelper::unchecked_read "
virtual int GeoCal::CalcRaster::unchecked_read(int Line, int Sample) const
Most of the time you should use the range checked operator().

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  WorldView2CloudMaskHelper::unchecked_write "
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
Most of the time you should use the range checked write.

But for some tight loops the range checking may be prohibitive. In
those cases you can use this unchecked version. ";

%feature("docstring")  WorldView2CloudMaskHelper::WorldView2CloudMaskHelper "
WorldView2CloudMaskHelper::WorldView2CloudMaskHelper(const boost::shared_ptr< RasterImage > &Band_1, const
boost::shared_ptr< RasterImage > &Band_8, double Cloud_threshold,
double Shadow_threshold)
";
%feature("docstring")  WorldView2CloudMaskHelper::write "
virtual void GeoCal::RasterImage::write(int Line, int Sample, int Val)
Write the pixel value to the given location. ";

%feature("docstring")  WorldView2CloudMaskHelper::write "
void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)
";
%feature("docstring")  WorldView2CloudMaskHelper::write_ptr "
virtual void GeoCal::RasterImage::write_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, const int
*V)
Write a subset of the data.

The default is just to call write for each pixel, but derived classes
can implement more efficient versions of this. ";

%feature("docstring")  WorldView2CloudMaskHelper::~WorldView2CloudMaskHelper "
virtual WorldView2CloudMaskHelper::~WorldView2CloudMaskHelper()
";

// File: a01152.xml

%feature("docstring") std::wostream "
STL class. ";


// File: a01150.xml

%feature("docstring") std::wostringstream "
STL class. ";


// File: a01153.xml

%feature("docstring") std::wstring "
STL class. ";


// File: a01139.xml

%feature("docstring") std::wstringstream "
STL class. ";


// File: a00510.xml


// File: a00511.xml


// File: a00512.xml


// File: a00513.xml


// File: a00514.xml
%feature("docstring")  GeoCal::Constant::const_pointer "
boost::shared_ptr<T> GeoCal::const_pointer(const T &t)
Create a boost pointer to a reference, where we don't actually take
ownership of the object.

The pointed to object must continue to exist. ";

%feature("docstring")  GeoCal::Constant::copy "
void GeoCal::copy(const RasterImageMultiBand &Img_in, RasterImageMultiBand &Img_out,
bool Log_progress=false)
This copies one multiband image to another.

The images should be the same size. We also assume that all the bands
are the same size.

Setting Diagnostic to true causes messages to be printed as we do the
copying. ";

%feature("docstring")  GeoCal::Constant::copy "
void GeoCal::copy(const RasterImage &Img_in, RasterImage &Img_out, bool
diagnostic=false, int Tile_nline=-1, int Tile_nsamp=-1)
This copies one image to another.

The images should be the same size. Setting Diagnostic to true causes
messages to be printed as we do the copying. ";

%feature("docstring")  GeoCal::Constant::copy_no_fill "
void GeoCal::copy_no_fill(const RasterImage &Img_in, RasterImage &Img_out, int Fill_value=0,
bool diagnostic=false)
This copies one image to another.

The images should be the same size. Setting Diagnostic to true causes
messages to be printed as we do the copying.

This differs from copy by skipping copying all values that are equal
to the Fill_value.

This can be used to create a simple mosaic by copying multiple images
to a common image. ";

%feature("docstring")  GeoCal::Constant::copy_only_to_fill "
void GeoCal::copy_only_to_fill(const RasterImage &Img_in, RasterImage &Img_out, int Fill_value=0,
bool diagnostic=false)
This copies one image to another.

The images should be the same size. Setting Diagnostic to true causes
messages to be printed as we do the copying.

This differs from copy by skipping copying all values that are equal
to the Fill_value, and only writing to areas in the new image that
aren't fill value. This is useful to do a fast mosaic without any
feathering, which just lay image on top of an existing mosaic and crop
wherever there is already data. ";

%feature("docstring")  GeoCal::Constant::cross "
void GeoCal::cross(const boost::array< double, 3 > &x, const boost::array< double, 3 >
&y, boost::array< double, 3 > &res)
Cross product. ";

%feature("docstring")  GeoCal::Constant::dfunctor_adapter "
double GeoCal::dfunctor_adapter(double x, void *params)
";
%feature("docstring")  GeoCal::Constant::distance "
double GeoCal::distance(const GroundCoordinate &G1, const GroundCoordinate &G2)
Distance in meters between ground coordinates.

This is the distance of the points in 3 space, i.e. it is not distance
along a geodesic or anything like that. ";

%feature("docstring")  GeoCal::Constant::dot "
double GeoCal::dot(const boost::array< double, D > &A, const boost::array< double, D >
&B)
Dot product of two arrays. ";

%feature("docstring")  GeoCal::Constant::gdal_band_type "
GDALDataType GeoCal::gdal_band_type(const std::string &Fname, int Band_id=1)
Utility function to return the type of a raster band. ";

%feature("docstring")  GeoCal::Constant::gdal_create_copy "
boost::shared_ptr< GDALDataset > GeoCal::gdal_create_copy(const std::string &Fname, const std::string &Driver_name, const
GDALDataset &Source_dataset, const std::string &Options, bool
Log_progress=false)
Utility to create a new file.

This is a thin wrapper around the GDAL library that gets the requested
driver and creates the file with the given options.

This uses the \"CreateCopy\" command rather than \"Create\", and takes
an existing GDALDataset. There are some drivers that only have certain
features available when using \"CreateCopy\" vs. \"Create\", e.g., the
statistics calculation for a Erdas HFA driver. Other write once
drivers only implement a \"CreateCopy\" without implementing
\"Create\", e.g., the JPEG driver. If you don't already have an
existing GDAL file it is often useful to use the \"MEM\" driver that
creates a file in memory only, this can then be used to do a
CreateCopy.

Because the option interface of GDAL is a bit awkward to use, we
supply an interface that takes a single string, with the options
separated by \" \", so for example for a TIFF file we might have
\"TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024\" ";

%feature("docstring")  GeoCal::Constant::gdal_create_dataset "
boost::shared_ptr< GDALDataset > GeoCal::gdal_create_dataset(const std::string &Fname, const std::string &Driver_name, int
Number_line, int Number_sample, int Number_band, GDALDataType Dt,
const std::string &Options)
Utility to create a new file.

This is a thin wrapper around the GDAL library that gets the requested
driver and creates the file with the given options.

Because the option interface of GDAL is a bit awkward to use, we
supply an interface that takes a single string, with the options
separated by \" \", so for example for a TIFF file we might have
\"TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024\" ";

%feature("docstring")  GeoCal::Constant::gdal_create_erdas "
boost::shared_ptr< GDALDataset > GeoCal::gdal_create_erdas(const std::string &Fname, const GDALDataset &Source_dataset, bool
Log_progress=false)
Utility to create a new Erdas file.

This is similar to gdal_create_copy, except we also create the
statistics and image pyramid used by Erdas to view a file. Note that
often there is a \"fill value\" such as 0 which should be excluded
from the statistics calculation. If that is the case, then the input
Source_dataset should have had SetNoDataValue called on each of the
bands to set this value.

This function is more expensive to call than just to do a
gdal_create_copy to an erdas file. We 1) calculate statistics, 2)
calculate an image pyramid, 3) store the data compressed. If you don't
want these additional features, you shouldn't call this function -
instead just call gdal_create_dataset or gdal_create_copy. ";

%feature("docstring")  GeoCal::Constant::gdal_has_map_info "
bool GeoCal::gdal_has_map_info(const std::string &Fname)
Utility function that returns true if the file contains projection
information, and false otherwise. ";

%feature("docstring")  GeoCal::Constant::gdal_has_rpc "
bool GeoCal::gdal_has_rpc(const GDALDataset &D)
Indicates if we have RPC metadata. ";

%feature("docstring")  GeoCal::Constant::gdal_map_info "
void GeoCal::gdal_map_info(GDALDataset &D, const MapInfo &M)
Set MapInfo for the file. ";

%feature("docstring")  GeoCal::Constant::gdal_metadata "
S GeoCal::gdal_metadata(const GDALDataset &D, const std::string &M, const std::string
&Domain=\"\")
This is a thin wrapper around GDALGetMetadataItem().

We read the requested metadata, and cast to the desired type. A
MetadataMissing is thrown if we don't find the requested metadata. ";

%feature("docstring")  GeoCal::Constant::gdal_rpc "
Rpc GeoCal::gdal_rpc(const std::string &Fname)
This reads in the Rpc for the given file. ";

%feature("docstring")  GeoCal::Constant::gdal_rpc "
Rpc GeoCal::gdal_rpc(const GDALDataset &D)
This reads in the Rpc for the given data. ";

%feature("docstring")  GeoCal::Constant::gdal_rpc "
void GeoCal::gdal_rpc(GDALDataset &D, const Rpc &R)
This writes the Rpc for the given data. ";

%feature("docstring")  GeoCal::Constant::gsl_fit "
void GeoCal::gsl_fit(const GslMatrix &X, const GslVector &W, const GslVector &Y, GslVector
&C, GslMatrix &Cov, double &Chisq)
This does a linear fit to solve the system Y = X * C (in the least
squares sense).

The data Y is given the weight W (i.e., W = 1 / (Y uncertainty)). We
return the Cov and chisqr in addition to the solution. ";

%feature("docstring")  GeoCal::Constant::gsl_fit "
void GeoCal::gsl_fit(const GslMatrix &X, const GslVector &Y, GslVector &C, GslMatrix &Cov,
double &Chisq)
This does a linear fit to solve the system Y = X * C (in the least
squares sense).

The data Y is given equal weight for each point. We return the Cov and
chisqr in addition to the solution. ";

%feature("docstring")  GeoCal::Constant::gsl_root "
blitz::Array<double, 1> GeoCal::gsl_root(const VFunctor &F, const blitz::Array< double, 1 > &Initial, double
Residual=1e-6)
";
%feature("docstring")  GeoCal::Constant::gsl_root "
blitz::Array<double, 1> GeoCal::gsl_root(const VFunctorWithDerivative &F, const blitz::Array< double, 1 >
&Initial, double Residual=1e-6)
";
%feature("docstring")  GeoCal::Constant::gsl_root "
double GeoCal::gsl_root(const DFunctor &F, double Xmin, double Xmax, double Eps=1e-6)
This finds the root of a Double -> Double function, without a
derivative available.

The solution found is in the bracketed range Xmin <= X <= Xmax. We
find a solution when we have bracketed it within a range xlow, xhigh,
with xhigh - xlow < Eps.

If we can't find a solution, we throw a ConvergenceFailure exception.
";

%feature("docstring")  GeoCal::Constant::HdfFile::read_attribute< std::string > "
std::string GeoCal::HdfFile::read_attribute< std::string >(const std::string &Aname) const
Read the given attribute attached to a group or dataset. ";

%feature("docstring")  GeoCal::Constant::HdfFile::read_attribute< std::vector< std::string > > "
std::vector<std::string> GeoCal::HdfFile::read_attribute< std::vector< std::string > >(const std::string &Aname) const
Read the given attribute attached to a group or dataset. ";

%feature("docstring")  GeoCal::Constant::interpolate "
boost::shared_ptr< QuaternionOrbitData > GeoCal::interpolate(const QuaternionOrbitData &t1, const QuaternionOrbitData &t2, Time
tm)
Interpolate between two QuaternionOrbitData for the given time. ";

%feature("docstring")  GeoCal::Constant::mat_copy "
void GeoCal::mat_copy(const double m[D][D], double mcopy[D][D])
Copy a matrix. On exit, mcopy = m. ";

%feature("docstring")  GeoCal::Constant::mat_t_copy "
void GeoCal::mat_t_copy(const double m[D][D], double mcopy[D][D])
Copy a matrix transpose. On exit, mcopy = m^t. ";

%feature("docstring")  GeoCal::Constant::matrix_to_quaternion "
boost::math::quaternion<double> GeoCal::matrix_to_quaternion(const double m[3][3])
Convert a rotation matrix to a quaternion.

http://www.j3d.org/matrix_faq/matrfaq_latest.html for this algorithm.
";

%feature("docstring")  GeoCal::Constant::mul "
void GeoCal::mul(const double m[D][D], const boost::array< double, D > &x,
boost::array< double, D > &res)
Multiply a fixed size matrix with a vector. On exit res = m * x. ";

%feature("docstring")  GeoCal::Constant::mul_t "
void GeoCal::mul_t(const double m[D][D], const boost::array< double, D > &x,
boost::array< double, D > &res)
Multiply a transpose of a fixed size matrix with a vector.

On exit res = m^t * x ";

%feature("docstring")  GeoCal::Constant::norm "
double GeoCal::norm(const boost::array< double, D > &A)
Norm of a array. ";

%feature("docstring")  GeoCal::Constant::pixbuf "
GdkPixbuf * GeoCal::pixbuf(const RasterImageMultiBand &Rband, const DisplayStretch &Stretch, int
red_band=1, int blue_band=2, int green_band=3)
Create a GdkPixbuf to display the given raster data. ";

%feature("docstring")  GeoCal::Constant::pointer "
boost::shared_ptr<T> GeoCal::pointer(T &t)
Create a boost pointer to a reference, where we don't actually take
ownership of the object.

The pointed to object must continue to exist. ";

%feature("docstring")  GeoCal::Constant::quaternion_to_matrix "
void GeoCal::quaternion_to_matrix(const boost::math::quaternion< double > &q, double m[3][3])
Convert a quaternion to a rotation matrix. ";

%feature("docstring")  GeoCal::Constant::range_check_template "
void GeoCal::range_check_template(const T &Val, const T &Min, const T &Max, const char *File, int Line)
Range check. ";

%feature("docstring")  GeoCal::Constant::range_max_check_template "
void GeoCal::range_max_check_template(const T &Val, const T &Max, const char *File, int Line)
Range check. ";

%feature("docstring")  GeoCal::Constant::range_min_check_template "
void GeoCal::range_min_check_template(const T &Val, const T &Min, const char *File, int Line)
Range check. ";

%feature("docstring")  GeoCal::Constant::root_list "
std::vector< double > GeoCal::root_list(const DFunctor &F, double Xmin, double Xmax, double
Root_minimum_spacing, double Eps=1e-6)
This will find a (possible empty) list of roots of a function, where
the roots have a seperation of at least the supplied minimum
separation.

This function is useful for finding roots when you don't know how many
solutions there are in the given range. It will find all roots,
provided that they have a seperation larger then Root_minimum_spacing,
and return the list of solutions. This list is ordered from smallest
to greatest.

This function works by sampling the Functor with a spacing of
Root_minimum_spacing. If the function changes sign between one spacing
and the next, the routine root is called between those spacings and
the results is added to the root list.

This will not finds roots that are closer together then the supplied
minimum spacing. (Limitation) ";

%feature("docstring")  GeoCal::Constant::vdffunctor_adapter "
int GeoCal::vdffunctor_adapter(const gsl_vector *x, void *params, gsl_matrix *j)
";
%feature("docstring")  GeoCal::Constant::vdffunctor_adapter2 "
int GeoCal::vdffunctor_adapter2(const gsl_vector *x, void *params, gsl_vector *f, gsl_matrix *j)
";
%feature("docstring")  GeoCal::Constant::vfunctor_adapter "
int GeoCal::vfunctor_adapter(const gsl_vector *x, void *params, gsl_vector *f)
";
%feature("docstring")  GeoCal::Constant::VicarFile::label< std::string > "
std::string GeoCal::VicarFile::label< std::string >(const std::string &K, const std::string &P) const
Return value for the given label. ";

%feature("docstring")  GeoCal::Constant::VicarFile::label< std::vector< float > > "
std::vector<float> GeoCal::VicarFile::label< std::vector< float > >(const std::string &K, const std::string &P) const
Return value for the given label. ";

%feature("docstring")  GeoCal::Constant::VicarFile::label< std::vector< int > > "
std::vector<int> GeoCal::VicarFile::label< std::vector< int > >(const std::string &K, const std::string &P) const
Return value for the given label. ";

%feature("docstring")  GeoCal::Constant::VicarFile::label< std::vector< std::string > > "
std::vector<std::string> GeoCal::VicarFile::label< std::vector< std::string > >(const std::string &K, const std::string &P) const
";
%feature("docstring")  GeoCal::Constant::VicarLiteFile::label< std::string > "
std::string GeoCal::VicarLiteFile::label< std::string >(const std::string &K, const std::string &Property) const
Return value for the given label. ";


// File: a00515.xml


// File: a00516.xml
%feature("docstring")  GeoCal::GdalRegister::gdal_register "
void GeoCal::GdalRegister::gdal_register()
Register GDAL drivers once.

Not normally directly called by the user, this is an internal
function. ";


// File: a00517.xml
%feature("docstring")  GeoCal::GdalType::gdal_data_type "
int GeoCal::GdalType::gdal_data_type()
";
%feature("docstring")  GeoCal::GdalType::gdal_data_type< double > "
int GeoCal::GdalType::gdal_data_type< double >()
";
%feature("docstring")  GeoCal::GdalType::gdal_data_type< float > "
int GeoCal::GdalType::gdal_data_type< float >()
";
%feature("docstring")  GeoCal::GdalType::gdal_data_type< GByte > "
int GeoCal::GdalType::gdal_data_type< GByte >()
";
%feature("docstring")  GeoCal::GdalType::gdal_data_type< GInt16 > "
int GeoCal::GdalType::gdal_data_type< GInt16 >()
";
%feature("docstring")  GeoCal::GdalType::gdal_data_type< GInt32 > "
int GeoCal::GdalType::gdal_data_type< GInt32 >()
";
%feature("docstring")  GeoCal::GdalType::gdal_data_type< GUInt16 > "
int GeoCal::GdalType::gdal_data_type< GUInt16 >()
";
%feature("docstring")  GeoCal::GdalType::gdal_data_type< GUInt32 > "
int GeoCal::GdalType::gdal_data_type< GUInt32 >()
";

// File: a00518.xml
%feature("docstring")  GeoCal::GeoCalCore::range_check_template "
void GeoCal::GeoCalCore::range_check_template(const T &Val, const T &Min, const T &Max, const CodeLocation &Cl)
";
%feature("docstring")  GeoCal::GeoCalCore::range_inclusive_check_template "
void GeoCal::GeoCalCore::range_inclusive_check_template(const T &Val, const T &Min, const T &Max, const CodeLocation &Cl)
";
%feature("docstring")  GeoCal::GeoCalCore::range_max_check_template "
void GeoCal::GeoCalCore::range_max_check_template(const T &Val, const T &Max, const CodeLocation &Cl)
";
%feature("docstring")  GeoCal::GeoCalCore::range_min_check_template "
void GeoCal::GeoCalCore::range_min_check_template(const T &Val, const T &Min, const CodeLocation &Cl)
";

// File: a00519.xml


// File: a00520.xml
%feature("docstring")  GeoCal::VicarType::vicar_type "
std::string GeoCal::VicarType::vicar_type()
";
%feature("docstring")  GeoCal::VicarType::vicar_type< VicarByte > "
std::string GeoCal::VicarType::vicar_type< VicarByte >()
";
%feature("docstring")  GeoCal::VicarType::vicar_type< VicarDouble > "
std::string GeoCal::VicarType::vicar_type< VicarDouble >()
";
%feature("docstring")  GeoCal::VicarType::vicar_type< VicarFloat > "
std::string GeoCal::VicarType::vicar_type< VicarFloat >()
";
%feature("docstring")  GeoCal::VicarType::vicar_type< VicarFull > "
std::string GeoCal::VicarType::vicar_type< VicarFull >()
";
%feature("docstring")  GeoCal::VicarType::vicar_type< VicarHalf > "
std::string GeoCal::VicarType::vicar_type< VicarHalf >()
";

// File: a00521.xml


// File: a00522.xml


// File: a00523.xml
%feature("docstring")  mmap_file::mmap_file "
def mmap_file.mmap_file
Open a VICAR file as a memmap. This either opens for reading or
creates a new Vicar file with the given map info and data type.
Usage:   mmap_file(fname) - Open an existing file for reading. If the
dtype is supplied that is used, otherwise we read the VICAR      label
and use either np.float64, np.float32, np.int8, np.int16,      or
np.int32. VICAR doesn't differentiate between signed and
unsigned integers, so if you want the unsigned version you need
to specify that.   mmap_file(fname, mi, dtype=value) - Create a new
file for      reading and writing. If dtype isn't supplied, we default
to      np.int8.  The data type should be one of np.float64,
np.float32, np.int8, np.int16, np.int32, np.uint8, np.uint16,
np.uint32
";

// File: a00524.xml
%feature("docstring")  mmap_file_test::test_read_write "
def mmap_file_test.test_read_write";

// File: a00525.xml


// File: a00526.xml
%feature("docstring")  raster_tile::read_iterator "
def raster_tile.read_iterator
This is similar to tile_iterator, but it is used as a short cut when
all you want to do is read the tile given by tile_iterator. Instead of
return a RasterImageTileIterator, this returns a numpy array of the
data given by the tile. This can be use either with a RasterImage or a
RasterImageMultiBand. For the RasterImageMultiBand the iterator is
generated for raster_image(0), and then we read all bands at the same
location.  Example usage:  for i in read_iterator(file):    print
i.shape
";
%feature("docstring")  raster_tile::tile_iterator "
def raster_tile.tile_iterator
This gives a tile_iterator. This returns a RasterImageTileIterator
that can be used to step through a file. See also read_iterator for an
alternative. This can either be used with a RasterImage or a
RasterImageMultiBand. For the RasterImageMultiBand we call generate a
iterator for raster_image(0).  Example usage:  for i in
tile_iterator(file):    print i
";

// File: a00527.xml
%feature("docstring")  raster_tile_test::setUp "
def raster_tile_test.setUp";
%feature("docstring")  raster_tile_test::test_read_iterator "
def raster_tile_test.test_read_iterator";
%feature("docstring")  raster_tile_test::test_tile_iterator "
def raster_tile_test.test_tile_iterator";

// File: a00528.xml


// File: a00529.xml


// File: a00228.xml


// File: a00229.xml


// File: a00230.xml


// File: a00231.xml


// File: a00232.xml


// File: a00233.xml


// File: a00234.xml


// File: a00235.xml


// File: a00236.xml
%feature("docstring")  v2_deactivate_a_unit "
int v2_deactivate_a_unit(int unit)
";

// File: a00237.xml


// File: a00238.xml
%feature("docstring")  material_class_sort "
bool material_class_sort(const MaterialDetect::MaterialClass &M1, const
MaterialDetect::MaterialClass &M2)
";

// File: a00239.xml


// File: a00240.xml


// File: a00241.xml


// File: a00242.xml


// File: a00243.xml


// File: a00244.xml


// File: a00245.xml


// File: a00246.xml


// File: a00247.xml


// File: a00248.xml


// File: a00249.xml


// File: a00250.xml


// File: a00251.xml


// File: a00252.xml


// File: a00253.xml


// File: a00254.xml


// File: a00255.xml


// File: a00256.xml


// File: a00257.xml


// File: a00258.xml


// File: a00259.xml


// File: a00260.xml
%feature("docstring")  to_s1 "
std::string to_s1(double x)
";
%feature("docstring")  to_s2 "
std::string to_s2(double x)
";

// File: a00261.xml


// File: a00262.xml


// File: a00263.xml


// File: a00264.xml


// File: a00265.xml


// File: a00266.xml
%feature("docstring")  GeoCal::GTIFFree "
void GTIFFree(GTIF *gtif)
";
%feature("docstring")  GeoCal::GTIFKeyGet "
int GTIFKeyGet(GTIF *gtif, geokey_t key, void *val, int index, int count)
";
%feature("docstring")  GeoCal::GTIFKeyInfo "
int GTIFKeyInfo(GTIF *gtif, geokey_t key, int *size, tagtype_t *type)
";
%feature("docstring")  GeoCal::GTIFKeyName "
char* GTIFKeyName(geokey_t key)
";
%feature("docstring")  GeoCal::GTIFKeySet "
int GTIFKeySet(GTIF *gtif, geokey_t keyID, tagtype_t type, int count,...)
";
%feature("docstring")  GeoCal::GTIFNew "
GTIF* GTIFNew(void *tif)
";
%feature("docstring")  GeoCal::GTIFValueName "
char* GTIFValueName(geokey_t key, int value)
";
%feature("docstring")  GeoCal::GTIFWriteKeys "
int GTIFWriteKeys(GTIF *gtif)
";
%feature("docstring")  GeoCal::TIFFGetField "
int TIFFGetField(TIFF *, ttag_t,...)
";
%feature("docstring")  GeoCal::TIFFSetField "
int TIFFSetField(TIFF *, ttag_t,...)
";
%feature("docstring")  GeoCal::TIFFWriteEncodedStrip "
int32_t TIFFWriteEncodedStrip(TIFF *, uint32_t, void *, int32_t)
";
%feature("docstring")  GeoCal::XTIFFClose "
void XTIFFClose(TIFF *tif)
";
%feature("docstring")  GeoCal::XTIFFOpen "
TIFF* XTIFFOpen(const char *name, const char *mode)
";

// File: a00267.xml


// File: a00268.xml


// File: a00269.xml


// File: a00270.xml


// File: a00271.xml


// File: a00272.xml


// File: a00273.xml


// File: a00274.xml


// File: a00275.xml


// File: a00276.xml


// File: a00277.xml


// File: a00278.xml


// File: a00279.xml
%feature("docstring")  gmm::main "
int main(int Argc, char **Argv)
";

// File: a00280.xml
%feature("docstring")  main "
int main(int Argc, char **Argv)
";
%feature("docstring")  sqr "
double sqr(double x)
";

// File: a00281.xml


// File: a00282.xml


// File: a00283.xml


// File: a00284.xml


// File: a00285.xml


// File: a00286.xml


// File: a00287.xml


// File: a00288.xml


// File: a00289.xml


// File: a00290.xml


// File: a00291.xml


// File: a00292.xml


// File: a00293.xml


// File: a00294.xml


// File: a00295.xml


// File: a00296.xml


// File: a00297.xml


// File: a00298.xml


// File: a00299.xml


// File: a00300.xml


// File: a00301.xml


// File: a00302.xml


// File: a00303.xml


// File: a00304.xml


// File: a00305.xml


// File: a00306.xml


// File: a00307.xml
%feature("docstring")  do_read_all "
std::vector<boost::shared_ptr<RasterImage> > do_read_all(const GdalRasterImage &d)
";

// File: a00308.xml


// File: a00309.xml
%feature("docstring")  to_s1 "
std::string to_s1(double x)
";
%feature("docstring")  to_s2 "
std::string to_s2(double x)
";

// File: a00310.xml


// File: a00311.xml


// File: a00312.xml


// File: a00313.xml


// File: a00314.xml


// File: a00315.xml


// File: a00316.xml


// File: a00317.xml


// File: a00318.xml


// File: a00319.xml


// File: a00320.xml


// File: a00321.xml


// File: a00322.xml


// File: a00323.xml


// File: a00324.xml


// File: a00325.xml


// File: a00326.xml


// File: a00327.xml


// File: a00328.xml


// File: a00329.xml


// File: a00330.xml


// File: a00331.xml


// File: a00332.xml


// File: a00333.xml


// File: a00334.xml


// File: a00335.xml


// File: a00336.xml


// File: a00337.xml


// File: a00338.xml


// File: a00339.xml


// File: a00340.xml


// File: a00341.xml


// File: a00342.xml


// File: a00343.xml


// File: a00344.xml


// File: a00345.xml


// File: a00346.xml


// File: a00347.xml


// File: a00348.xml


// File: a00349.xml


// File: a00350.xml


// File: a00351.xml


// File: a00352.xml


// File: a00353.xml


// File: a00354.xml


// File: a00355.xml


// File: a00356.xml


// File: a00357.xml


// File: a00358.xml


// File: a00359.xml


// File: a00360.xml


// File: a00361.xml


// File: a00362.xml


// File: a00363.xml


// File: a00364.xml


// File: a00365.xml


// File: a00366.xml


// File: a00367.xml


// File: a00368.xml


// File: a00369.xml


// File: a00370.xml


// File: a00371.xml


// File: a00372.xml


// File: a00373.xml


// File: a00374.xml


// File: a00375.xml


// File: a00376.xml


// File: a00377.xml


// File: a00378.xml


// File: a00379.xml


// File: a00380.xml
%feature("docstring")  sqr "
double sqr(double x)
";

// File: a00381.xml


// File: a00382.xml


// File: a00383.xml


// File: a00384.xml


// File: a00385.xml


// File: a00386.xml


// File: a00387.xml


// File: a00388.xml


// File: a00389.xml


// File: a00390.xml


// File: a00391.xml
%feature("docstring")  sqr "
double sqr(double x)
";

// File: a00392.xml


// File: a00393.xml
%feature("docstring")  sqr "
double sqr(double x)
";

// File: a00394.xml


// File: a00395.xml


// File: a00396.xml


// File: a00397.xml


// File: a00398.xml


// File: a00399.xml


// File: a00400.xml


// File: a00401.xml


// File: a00402.xml


// File: a00403.xml
%feature("docstring")  sqr "
double sqr(double x)
Length of look vector, in meters. ";


// File: a00404.xml


// File: a00405.xml


// File: a00406.xml


// File: a00407.xml


// File: a00408.xml


// File: a00409.xml


// File: a00410.xml


// File: a00411.xml


// File: a00412.xml


// File: a00413.xml


// File: a00414.xml
%feature("docstring")  cross2 "
blitz::Array<double, 1> cross2(const blitz::Array< double, 1 > &x, const blitz::Array< double, 1 >
&y)
";
%feature("docstring")  dotb "
double dotb(const blitz::Array< double, 1 > &x, const blitz::Array< double, 1 >
&y)
Calculate OrbitData for KeplerOrbit. ";

%feature("docstring")  interpolate "
void interpolate(const boost::array< double, 3 > &P1, const boost::array< double, 3 >
&V1, const boost::array< double, 3 > &P2, const boost::array< double,
3 > &V2, double toffset, double tspace, boost::array< double, 3 >
&Pres, boost::array< double, 3 > &Vres)
This is a utility function for use by derived classes.

A common way of getting orbit data is to have discrete measurements of
the position and velocity. This function does a third order polynomial
interpolation to find the position and velocity at a time in between
the discrete measurements. For a time t between t1 and t2, we have P1
and V1 the position and velocity at time t1, P2 and V2 the position
and velocity at t2, tspace = t2 - t1, toffset = t - t1. This function
then returns Pres and Vres. ";

%feature("docstring")  interpolate "
boost::math::quaternion<double> interpolate(const boost::math::quaternion< double > &Q1, const
boost::math::quaternion< double > &Q2, double toffset, double tspace)
This is a utility function for use by derived classes.

A common way of getting orbit data is to have discrete measurements of
the quaternion describing the rotation of the spacecraft. For a time t
between t1 and t2, we have Q1 as the quaternion at time t1, Q2 the
quaternion at time t2, tspace = t2 - t1, toffset = t - t1. This
function then returns Qres. We calculate this by determining the axis
and angle rotation that takes use from Q1 to Q2, and then do a linear
interpolation of that angle for the given time. ";

%feature("docstring")  normb "
double normb(const blitz::Array< double, 1 > &x)
";

// File: a00415.xml


// File: a00416.xml


// File: a00417.xml


// File: a00418.xml


// File: a00419.xml
%feature("docstring")  to_c_order_const "
blitz::Array<T, D> to_c_order_const(const blitz::Array< T, D > &In)
";

// File: a00420.xml


// File: a00421.xml


// File: a00422.xml


// File: a00423.xml


// File: a00424.xml


// File: a00425.xml


// File: a00426.xml


// File: a00427.xml


// File: a00428.xml


// File: a00429.xml


// File: a00430.xml


// File: a00431.xml


// File: a00432.xml


// File: a00433.xml


// File: a00434.xml


// File: a00435.xml


// File: a00436.xml


// File: a00437.xml


// File: a00438.xml


// File: a00439.xml


// File: a00440.xml


// File: a00441.xml


// File: a00442.xml


// File: a00443.xml


// File: a00444.xml


// File: a00445.xml


// File: a00446.xml


// File: a00447.xml


// File: a00448.xml


// File: a00449.xml


// File: a00450.xml


// File: a00451.xml


// File: a00452.xml


// File: a00453.xml


// File: a00454.xml


// File: a00455.xml


// File: a00456.xml


// File: a00457.xml


// File: a00458.xml


// File: a00459.xml


// File: a00460.xml


// File: a00461.xml


// File: a00462.xml


// File: a00463.xml


// File: a00464.xml


// File: a00465.xml


// File: a00466.xml


// File: a00467.xml


// File: a00468.xml
%feature("docstring")  BZ_DECLARE_FUNCTION_RET "
BZ_DECLARE_FUNCTION_RET(in_range, unsigned char)
";
%feature("docstring")  in_range "
unsigned char in_range(double x)
";

// File: a00469.xml


// File: a00470.xml


// File: a00471.xml


// File: a00472.xml
%feature("docstring")  cb_key_press_event "
static gint cb_key_press_event(GtkWidget *widget, GdkEventKey *event)
";
%feature("docstring")  main "
int main(int argc, char **argv)
";

// File: a00473.xml


// File: a00474.xml


// File: a00475.xml
%feature("docstring")  main "
int main(int Argc, char **Argv)
";

// File: a00476.xml


// File: a00477.xml
%feature("docstring")  main "
int main(int Argc, char **Argv)
";

// File: a00478.xml
%feature("docstring")  main "
int main(int Argc, char **Argv)
";
%feature("docstring")  sqr "
double sqr(double x)
";

// File: a00479.xml


// File: a00480.xml


// File: a00481.xml


// File: a00482.xml


// File: a00483.xml


// File: a00484.xml


// File: a00485.xml
%feature("docstring")  findBelowThres "
void findBelowThres(cv::Mat &matrix, std::vector< cv::Point2f > &belThres, double thresh)
";
%feature("docstring")  getAmountNonZero "
double getAmountNonZero(cv::Mat &img)
";
%feature("docstring")  getgrid "
void getgrid(GeoCal::GeoCalCore::RasterImage &img, float *chip, int narray, int
ndim, double trans[12], float centrx, float centry, float rmag[2], int
*chop, float pcl, float pcs, int zerothr)
";
%feature("docstring")  imageCoorToPoint "
void imageCoorToPoint(GeoCal::GeoCalCore::ImageCoor &coor, cv::Point2f &pt)
";
%feature("docstring")  imageCoorToPoint "
void imageCoorToPoint(std::vector< GeoCal::GeoCalCore::ImageCoor > &coors, std::vector<
cv::Point2f > &pts)
";
%feature("docstring")  makeGrid "
bool makeGrid(GeoCal::GeoCalCore::RasterImage &img, int grid, std::vector<
GeoCal::GeoCalCore::ImageCoor > &center)
";
%feature("docstring")  matToRast "
void matToRast(cv::Mat &src, GeoCal::GeoCalCore::RasterImage &dst)
";
%feature("docstring")  rastToMat "
cv::Mat rastToMat(GeoCal::GeoCalCore::RasterImage &src)
";
%feature("docstring")  subImage "
cv::Mat subImage(cv::Mat &img, cv::Point2f pt, cv::Size size)
";

// File: a00486.xml
%feature("docstring")  findBelowThres "
void findBelowThres(cv::Mat &matrix, std::vector< cv::Point2f > &belThres, double
thresh=0)
";
%feature("docstring")  getAmountNonZero "
double getAmountNonZero(cv::Mat &img)
";
%feature("docstring")  getgrid "
void getgrid(GeoCal::GeoCalCore::RasterImage &img, float *chip, int narray, int
ndim, double trans[12], float centrx, float centry, float rmag[2], int
*chop, float pcl, float pcs, int zerothr)
";
%feature("docstring")  imageCoorToPoint "
void imageCoorToPoint(GeoCal::GeoCalCore::ImageCoor &coor, cv::Point2f &pt)
";
%feature("docstring")  imageCoorToPoint "
void imageCoorToPoint(std::vector< GeoCal::GeoCalCore::ImageCoor > &coors, std::vector<
cv::Point2f > &pts)
";
%feature("docstring")  makeGrid "
bool makeGrid(GeoCal::GeoCalCore::RasterImage &img, int grid, std::vector<
GeoCal::GeoCalCore::ImageCoor > &center)
";
%feature("docstring")  matToRast "
void matToRast(cv::Mat &src, GeoCal::GeoCalCore::RasterImage &dst)
";
%feature("docstring")  rastToMat "
cv::Mat rastToMat(GeoCal::GeoCalCore::RasterImage &src)
";
%feature("docstring")  subImage "
cv::Mat subImage(cv::Mat &img, cv::Point2f pt, cv::Size size)
";

// File: a00487.xml


// File: a00488.xml


// File: a00489.xml


// File: a00490.xml


// File: a00491.xml


// File: a00492.xml


// File: a00493.xml


// File: a00494.xml


// File: a00495.xml


// File: a00496.xml


// File: a00497.xml


// File: a00498.xml
%feature("docstring")  convertGeoTIFFTiePoints "
void convertGeoTIFFTiePoints(float **fstImgCoeff, float **sndImgCoeff, float **c_data, int
geocord1, double itie[6], double otie[6], int geocord2, int
RefImageLocCount, int labnl1, int labns1, int labnl2, int labns2, bool
rotated)
";
%feature("docstring")  convertPoints "
void convertPoints(int geocord1, double tiePoints[6], float **lineSamp, int dim, int
labnl, int labns, float **c_data, int RefImageLocCount)
";
%feature("docstring")  convertTiePoints "
void convertTiePoints(float **fstImgCoeff, float **sndImgCoeff, float **c_data, int
geocord1, double itie[6], double otie[6], int geocord2, int
RefImageLocCount, int labnl1, int labns1, int labnl2, int labns2)
";
%feature("docstring")  getGeoTIFFLabels "
void getGeoTIFFLabels(double t[6], double tinv[6], const char *labelstr, int labnl, int
labns, int gtgetlabval)
";
%feature("docstring")  getzvl "
float getzvl(float *imgCoeff, int n, float coord[2], int nw, int nr)
";

// File: a00499.xml
%feature("docstring")  convertGeoTIFFTiePoints "
void convertGeoTIFFTiePoints(float **fstImgCoeff, float **sndImgCoeff, float **c_data, int
geocord1, double itie[6], double otie[6], int geocord2, int
RefImageLocCount, int labnl1, int labns1, int labnl2, int labns2, bool
rotated=false)
";
%feature("docstring")  convertPoints "
void convertPoints(int geocord1, double tiePoints[6], float **lineSamp, int dim, int
labnl, int labns, float **c_data=NULL, int RefImageLocCount=0)
";
%feature("docstring")  convertTiePoints "
void convertTiePoints(float **fstImgCoeff, float **sndImgCoeff, float **c_data, int
geocord1, double itie[6], double otie[6], int geocord2, int
RefImageLocCount, int labnl1, int labns1, int labnl2, int labns2)
";
%feature("docstring")  getGeoTIFFLabels "
void getGeoTIFFLabels(double t[6], double tinv[6], const char *labelstr, int labnl, int
labns, int gtgetlabval)
";
%feature("docstring")  getzvl "
float getzvl(float *imgCoeff, int n, float coord[2], int nw, int nr)
";

// File: a00500.xml
%feature("docstring")  calcLeastSquaresFit "
void calcLeastSquaresFit(double soln[12], bool predfunc, int nEquationsMax, std::vector<
GeoCal::GeoCalCore::ImageCoor > &fstImgCoors, std::vector<
GeoCal::GeoCalCore::ImageCoor > &sndImgCoors)
";
%feature("docstring")  initialMatch "
void initialMatch(double New_guess[12], GeoCal::GeoCalCore::ImageCoor
RefImageTiePoints[3], GeoCal::GeoCalCore::ImageCoor
NewImageTiePoints[3], int nEquationsMax, bool predfunc,
GeoCal::GeoCalCore::RasterImage *New, double angoff)
";
%feature("docstring")  refine "
void refine(float corr[3][3], float *vloff, float *vsoff, bool *ireferr)
";
%feature("docstring")  throwout "
void throwout(int *throwcount, int nEquationsMax, std::vector<
GeoCal::GeoCalCore::ImageCoor > &fstImgCoors, std::vector<
GeoCal::GeoCalCore::ImageCoor > &sndImgCoors)
";
%feature("docstring")  tiltCoor "
void tiltCoor(GeoCal::GeoCalCore::RasterImage *Img, GeoCal::GeoCalCore::ImageCoor
TiePoints[3], double angoff)
";

// File: a00501.xml
%feature("docstring")  calcLeastSquaresFit "
void calcLeastSquaresFit(double soln[12], bool predfunc, int nEquationsMax, std::vector<
GeoCal::GeoCalCore::ImageCoor > &fstImgCoors, std::vector<
GeoCal::GeoCalCore::ImageCoor > &sndImgCoors)
";
%feature("docstring")  getgrid "
void getgrid(GeoCal::GeoCalCore::RasterImage &img, float *chip, int narray, int
ndim, double trans[12], float centrx, float centry, float rmag[2], int
*chop, float pcl, float pcs, int zerothr)
";
%feature("docstring")  initialMatch "
void initialMatch(double New_guess[12], GeoCal::GeoCalCore::ImageCoor
RefImageTiePoints[3], GeoCal::GeoCalCore::ImageCoor
NewImageTiePoints[3], int nEquationsMax, bool predfunc=false,
GeoCal::GeoCalCore::RasterImage *New=NULL, double angoff=0)
";
%feature("docstring")  refine "
void refine(float corr[3][3], float *vloff, float *vsoff, bool *ireferr)
";
%feature("docstring")  throwout "
void throwout(int *throwcount, int nEquationsMax, std::vector<
GeoCal::GeoCalCore::ImageCoor > &fstImgCoors, std::vector<
GeoCal::GeoCalCore::ImageCoor > &sndImgCoors)
";
%feature("docstring")  tiltCoor "
void tiltCoor(GeoCal::GeoCalCore::RasterImage *Img, GeoCal::GeoCalCore::ImageCoor
TiePoints[3], double angoff)
";

// File: a00502.xml
%feature("docstring")  main "
int main(int argc, char **argv)
";

// File: a00503.xml
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(calcLeastSquaresFit_test)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(throwout_OneDiffIndex_test)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(throwout_twoDiffIndex_test)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match1_Perfect_Predictor)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match2_Predictor_OffBy6PlusPixels)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match3_OffByScale)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match4_PerfectPredictor_NewPoint)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match5_MagnifyCase)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match6_64x64FFT)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match7_DiffTiePoints)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match8_NewTiePoints64x64FFT)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match9_RotatedImage)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match10_RotatedImageWithRedo)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match11_FourEdgeCases_For_SubImages)
";
%feature("docstring")  BOOST_AUTO_TEST_CASE "
BOOST_AUTO_TEST_CASE(Match12_AutoThresh_Case)
";

// File: a00504.xml


// File: a00505.xml


// File: a00506.xml


// File: a00507.xml


// File: a00508.xml


// File: a00509.xml


// File: a00532.xml


// File: a00530.xml


// File: a00534.xml


// File: a00531.xml


// File: a00533.xml


// File: dir_a6b25effbb55561dae9229c79c93c63e.xml


// File: dir_7fabc8d5070d50884683d36eb053c403.xml


// File: dir_76ba507cc6a93ebb72deddd6ba40c7e3.xml


// File: dir_d45a3f9edce04836d13f7ef2a9102345.xml


// File: dir_60040d7d2cf9b2bc7a600298af4645df.xml


// File: dir_c1d3ed369cad375590d2eec6d31c4e9d.xml


// File: dir_17b4e6a7d1a48f7833ad4df3da9d97c1.xml


// File: dir_baeee0ec13135f84838c512a2d5d914b.xml


// File: dir_4a71c87774650963f5de8ca88e60aa6a.xml


// File: dir_1a6f470301e258b799b06938ccba281a.xml


// File: dir_a6c12f8f1af1f2b3deecee3f933aa002.xml


// File: dir_534339b2083d3042109d34871b65c676.xml


// File: dir_ee259ff6bfea30afc6b7c1d3b3c37616.xml


// File: dir_60b44e9a0c43c2a9f6d443aed2802640.xml


// File: dir_f164c57b43fe6fabe78875aa197ef072.xml


// File: dir_8fce7f51ecd623c3f4945b28e6d42e12.xml


// File: dir_279223300cf58ec0c1ef6de27b4010fa.xml


// File: indexpage.xml

