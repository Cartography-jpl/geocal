#include "dem_fit.h"
#include "vicar_lite_file.h"
#include "gdal_map_projected_image.h"

using namespace GeoCal;
using namespace gmm;

int main(int Argc, char** Argv)
{
  // All the data and numbers for this comes from a Test case
  // developed in Ruby. The MapInfo is just a subset of the full map.
  // The RPCs come from fitting to the CIB.
  boost::shared_ptr<Dem> d(new VicarLiteDem("/Users/smyth/AlStuff/china_aoi_wgs84.hlf"));
  boost::shared_ptr<RasterImage> img1(new GdalRasterImage("/Users/smyth/AlStuff/05NOV23034641-P1BS-005545406170_01_P001.NTF"));
  boost::shared_ptr<RasterImage> img2(new GdalRasterImage("/Users/smyth/AlStuff/05SEP12034327-P1BS-005545406060_01_P001.NTF"));
  MapInfo mi(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     110.990669578398, 34.5095662901431,
	     110.996618182458, 34.5050706060031, 1000 / 3, 1000 / 3);
  MapInfo mi2(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     110.990669578398, 34.5095662901431,
	     110.996618182458, 34.5050706060031, 1000 * 2, 1000 * 2);
  MapInfo midem(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     110.990669578398, 34.5095662901431,
	     110.996618182458, 34.5050706060031, 1000 / 30, 1000 / 30);
  MapInfo midem2(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     110.990669578398, 34.5095662901431,
	     110.996618182458, 34.5050706060031, 1000 / 10, 1000 / 10);
  Rpc r1, r2;
  r1.rpc_type = Rpc::RPC_B;
  r1.height_offset = 939;
  r1.height_scale = 500;
  r1.latitude_offset = 34.4442;
  r1.latitude_scale = 0.0866;
  boost::array<double, 20> t1 = {1.0, -0.001611708, -0.0004374615, 0.0003213522, -4.969936e-06, 1.03912e-06, 1.67282e-06, 1.018175e-05, -5.624895e-05, 2.214139e-05, 4.692714e-07, -3.949979e-08, -1.321447e-05, -1.536869e-07, -2.34092e-06, -1.54714e-05, -1.40093e-07, 0.0, 1.169986e-06, 0.0};
  r1.line_denominator = t1;
  boost::array<double, 20> t2 = {-0.00122425293791307, -0.0779579162255224, -1.06679000658298, 0.0108687592137241, -0.001378518, 3.666944e-05, 0.0002887064, -0.0008135215, 0.0007201723, -3.681922e-06, 8.528696e-07, -2.913232e-06, 5.602734e-06, -1.649238e-06, -2.242186e-05, -8.891333e-05, -2.399012e-05, 5.665745e-07, 2.849807e-06, 2.601614e-07};
  r1.line_numerator = t2;
  r1.line_offset = 13309.4220427037;
  r1.line_scale = 13482;
  r1.longitude_offset = 111.0688;
  r1.longitude_scale = 0.1045;
  boost::array<double, 20> t3 = {1.0, 0.001417272, -0.0008010268, -0.000489322, -4.065933e-05, 3.601403e-06, -1.981981e-06, -1.716611e-05, 3.528571e-05, -9.542906e-06, -4.286949e-08, -2.799477e-08, -5.081929e-07, -6.04217e-08, 9.638801e-07, -7.107457e-07, 1.416549e-08, -6.287279e-08, -1.204538e-08, 0.0};
  r1.sample_denominator = t3;
  boost::array<double, 20> t4 = {0.00361262176991768, 0.986212004679851, -0.00357086221371153, 0.0177491743707752, 0.0008379322, 0.0004213121, -0.0003771973, -0.005385674, 0.0002700672, 1.286316e-05, 5.071675e-06, 1.526115e-05, -5.049506e-05, -9.036717e-06, 7.684332e-05, 8.929494e-05, 1.169447e-07, -6.739701e-06, -4.724112e-06, -7.879736e-08};
  r1.sample_numerator = t4;
  r1.sample_offset = 13754.5475353233;
  r1.sample_scale = 14038;

  r2.rpc_type = Rpc::RPC_B;
  r2.height_offset = 832;
  r2.height_scale = 500;
  r2.latitude_offset = 34.4548;
  r2.latitude_scale = 0.0826;
  boost::array<double, 20> t5 = {1.0, -0.002002715, -0.001371389, 0.0003327338, 1.097566e-05, 1.310541e-06, 0.0, -3.695272e-05, 8.403069e-05, -3.719642e-05, -5.417193e-08, 4.250471e-08, -4.565548e-08, 1.848491e-07, 5.061996e-07, 3.593512e-06, 7.690634e-08, 1.080982e-08, -7.81733e-07, -2.072384e-08};
  r2.line_denominator = t5;
  boost::array<double, 20> t6 = {-0.00282268987525862, -0.00703709491353175, -1.0055632804472, 0.00129293456267302, -0.001964089, 4.482139e-06, 0.0003269453, -0.0005151566, 0.001830678, -3.396918e-07, 1.279497e-06, -8.262934e-07, 2.211756e-05, 2.798134e-07, 2.166861e-05, 0.0001389663, 3.715647e-05, 1.600936e-07, -9.85684e-07, -2.012578e-08};
  r2.line_numerator = t6;
  r2.line_offset = 14284.3060569805;
  r2.line_scale = 14324.0;
  r2.longitude_offset = 110.9801;
  r2.longitude_scale = 0.0989;
  boost::array<double, 20> t7 = {1.0, 6.924541e-06, -0.001428286, -0.0005426935, -4.926334e-05, 3.01361e-06, -3.713436e-07, -1.134919e-05, 4.986903e-05, -1.40634e-05, -5.734918e-08, -8.921996e-08, -1.069673e-07, -6.735712e-08, 8.176282e-07, 1.235345e-07, 4.788291e-08, -3.497643e-08, -6.660312e-08, 1.621442e-08};
  r2.sample_denominator = t7;
  boost::array<double, 20> t8 = {0.00583152853744995, 0.987962353682714, 0.0019958960069814, 0.0155717190188491, 0.001444162, 0.0004582328, -0.0003930035, -0.005643121, 0.0005492247, 8.228209e-06, 3.528878e-06, 9.370667e-06, -7.116927e-05, -1.34449e-05, 9.132583e-05, 7.115967e-05, 7.997803e-08, -6.410922e-06, -2.985705e-06, -1.968281e-07};
  r2.sample_numerator = t8;
  r2.sample_offset = 13775.7243639308;
  r2.sample_scale = 14007.0;

  boost::shared_ptr<MemoryDem> md(new MemoryDem(boost::shared_ptr<Datum>(new SimpleDatum()), midem));
  for(int i = 0; i < md->elevation_data.shape()[0]; ++i)
    for(int j = 0; j < md->elevation_data.shape()[1]; ++j) 
      md->elevation_data[i][j] = d->height_reference_surface(*md->map_info().ground_coordinate(j + 0.5, i + 0.5));

  DemFit dem_fit(img1, r1, img2, r2, md, mi, 10.0);
//   dem_fit.write("start_img1.img", "start_img2.img", "start_dem.img", 
// 		"start_corr.img");
  std::vector<double> r;
  for(int iter = 0; iter < 5; ++iter) {
    dem_fit.value(r);
    std::cerr << "norm(r): " << vect_norm2(r) << "\n";
    DemFit::sparse_matrix j;
    dem_fit.jacobian(j);
    std::vector<double> jtr(mat_ncols(j));
    mult(transposed(j), r, jtr);
    DemFit::sparse_matrix jtj(mat_ncols(j), mat_ncols(j));
    mult(transposed(j), j, jtj);
    std::vector<double> x(mat_ncols(j));
    double condj;
    SuperLU_solve(jtj, x, jtr, condj);
    if(condj != condj) {		// Not really portable, need to come
				// up with another way of doing this.
      std::cerr << "Condition is nan, indicates problem with matrix\n";
      exit(1);
    }
    std::cerr << "Condition: " << condj << "\n";
    int k = 0;
    // Scale so largest correction is 40 m. This is empirical, we'll
  // want to come up with something more real.
    double mx = std::max(*(max_element(x.begin(), x.end())),
			 -*(min_element(x.begin(), x.end())));
    std::cerr << "max: " << mx << "\n";
    double scale = 1.0;
    if(mx > 40)
      scale = 40.0 / mx;
    for(int i = 0; i < md->elevation_data.shape()[0]; ++i)
      for(int j = 0; j < md->elevation_data.shape()[1]; ++j, ++k)
	md->elevation_data[i][j] -= scale * x[k];
  }
  dem_fit.value(r);
  std::cerr << "norm(r): " << vect_norm2(r);

  boost::shared_ptr<MemoryDem> md2(new MemoryDem(boost::shared_ptr<Datum>(new SimpleDatum()), midem2));
  for(int i = 0; i < md2->elevation_data.shape()[0]; ++i)
    for(int j = 0; j < md2->elevation_data.shape()[1]; ++j) 
      md2->elevation_data[i][j] = d->height_reference_surface(*md2->map_info().ground_coordinate(j + 0.5, i + 0.5));

  DemFit dem_fit2(img1, r1, img2, r2, md2, mi2, 100.0);
  dem_fit2.write("start_img1.img", "start_img2.img", "start_dem.img", 
		"start_corr.img");
  for(int i = 0; i < md2->elevation_data.shape()[0]; ++i)
    for(int j = 0; j < md2->elevation_data.shape()[1]; ++j) 
      md2->elevation_data[i][j] = md->height_reference_surface(*md2->map_info().ground_coordinate(j + 0.5, i + 0.5));
  dem_fit2.write("step1_img1.img", "step1_img2.img", "step1_dem.img",
		 "step1_corr.img");


  for(int iter = 0; iter < 5; ++iter) {
    dem_fit2.value(r);
    std::cerr << "norm(r): " << vect_norm2(r) << "\n";
    DemFit::sparse_matrix j;
    dem_fit2.jacobian(j);
    std::vector<double> jtr(mat_ncols(j));
    mult(transposed(j), r, jtr);
    DemFit::sparse_matrix jtj(mat_ncols(j), mat_ncols(j));
    mult(transposed(j), j, jtj);
    std::vector<double> x(mat_ncols(j));
    double condj;
    SuperLU_solve(jtj, x, jtr, condj);
    if(condj != condj) {		// Not really portable, need to come
				// up with another way of doing this.
      std::cerr << "Condition is nan, indicates problem with matrix\n";
      exit(1);
    }
    std::cerr << "Condition: " << condj << "\n";
    int k = 0;
    // Scale so largest correction is 40 m. This is empirical, we'll
  // want to come up with something more real.
    double mx = std::max(*(max_element(x.begin(), x.end())),
			 -*(min_element(x.begin(), x.end())));
    std::cerr << "max: " << mx << "\n";
    double scale = 1.0;
    if(mx > 40)
      scale = 40.0 / mx;
    for(int i = 0; i < md2->elevation_data.shape()[0]; ++i)
      for(int j = 0; j < md2->elevation_data.shape()[1]; ++j, ++k)
	md2->elevation_data[i][j] -= scale * x[k];
  }
  dem_fit2.value(r);
  std::cerr << "norm(r): " << vect_norm2(r);
  dem_fit2.write("step2_img1.img", "step2_img2.img", "step2_dem.img",
		 "step2_corr.img");
  
}
