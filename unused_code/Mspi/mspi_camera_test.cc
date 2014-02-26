/*===========================================================================
=                                                                           =
=                                MspiCamera                                  =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                               L1B1 Equation

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: mspi_camera_test.cc

!ABSTRACT:

   This does unit testing for the class MspiCamera.

!DESCRIPTION:

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "unit_test_support.h"
#include "mspi_camera.h"	// Definition of MspiCamera
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <fstream>  		// Definition of std::ofstream
#include <sstream>  		// Definition of std::ostringstream
#include "MSPI-Shared/UnitTest/src/double_comparator.h"
				// Definition of DoubleComparator
#include <string.h> 		// Definition of strcmp
#include <iomanip>		// Definition of setprecision
#include <set> 			// Definition of std::set
#include "MSPI-Shared/UnitTest/src/check.h"
				// Definition of array_check
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::remove

using std::cerr;
using namespace GeoCal;
using MSPI::Shared::Exception;
using MSPI::Shared::DoubleComparator;
using MSPI::Shared::ConfigFile;
using MSPI::Shared::array_check;
using MSPI::Shared::vector_check;

/////////////////////////////////////////////////////////////////////////
// Generate unit-test input and expected results.
/////////////////////////////////////////////////////////////////////////

const std::string expect_filename = "GeoCal/MspiCamera/src/mspi_camera_test.expect";
const std::string new_expect_filename = "GeoCal/MspiCamera/src/mspi_camera_test.expect.new";

/////////////////////////////////////////////////////////////////////////////
/// Convert NEWMAT::Matrix to boost::multi_array<double,2>
/////////////////////////////////////////////////////////////////////////////

boost::multi_array<double,2> multi_array(const blitz::Array<double, 2>& Matrix)
{
  int number_row = Matrix.rows();
  int number_col = Matrix.cols();
  boost::multi_array<double,2> result(boost::extents[number_row][number_col]);
  for (int i = 0 ; i < number_row ; i++) {
    for (int j = 0 ; j < number_col ; j++) {
      result[i][j] = Matrix(i,j);
    }
  }
  return result;
}

//----------------------------------------------------------------------
boost::multi_array<double,2> expect_init(
  int Band,
  const std::string& Name
)
{
  const size_t number_row = 2;
  const size_t number_col = 3;
  boost::multi_array<double,2> result(boost::extents[number_row][number_col]);
  ConfigFile config(expect_filename);
  char buffer[100];
  sprintf(buffer, "%d-%s",Band, Name.c_str());
  std::string varname(buffer);
  if (config.is_defined(varname)) {
    result = config.get<double>(varname, number_row, number_col);
  } else {
    cerr << "Trouble with unit-test expect_init : " + varname + "\n";
  }
  return result;
}

//-----------------------------------------------------------------------
void save_expect(
  const boost::multi_array<double,2>& Param,
  int Band,
  const std::string& Name
)
{
  static std::set<std::string> saved; // Save each variable only once.
    
  char buffer[100];
  sprintf(buffer, "%d-%s",Band, Name.c_str());
  std::string varname(buffer);
  if (saved.find(varname) != saved.end()) {
    return;
  }
  saved.insert(varname);
  
  FILE *fp = fopen(new_expect_filename.c_str(),"a");
  for (size_t i = 0 ; i < Param.shape()[0] ; i++) {
    for (size_t j = 0 ; j < Param.shape()[1] ; j++) {
      fprintf(fp, "%20.20e ",Param[i][j]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp,"         : %s\n################\n", varname.c_str());
  fclose(fp);
}

/////////////////////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE(mspi_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  DoubleComparator cmp(100.0);	// Double comparator

//-----------------------------------------------------------------------
// Delete new expected result file.
//-----------------------------------------------------------------------

    boost::filesystem::remove(new_expect_filename.c_str());

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    std::string config_filename = mspi_test_data_dir() + 
      "mspi_camera_test.config";
    ConfigFile config(config_filename);
    MspiCamera camera(config);
    const int number_band = 7;

//-----------------------------------------------------------------------
// Test granule_id()
//-----------------------------------------------------------------------

    test_number++;
    {
      if (camera.granule_id() != 
	  "granule_MSPI-Ground/GeoCal/MspiCamera/src/mspi_camera_test.config") {
	cerr << "granule_id = "<< camera.granule_id() << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test parameter_step_size()
//-----------------------------------------------------------------------

    test_number++;
    {
      DoubleComparator cmp;
      double deg2rad = std::acos(-1)/180;
      std::vector<double> expect(3, 0.1 * deg2rad);
      
      vector_check(camera.parameter_step_size(), expect, cmp, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test row_number(int)
// Test band_number(int)
//-----------------------------------------------------------------------

    test_number++;

    {
      int row_number_expect[number_band] = {32,27,22,34,29,24,64};

      for (int iband = 0 ; iband < camera.number_band() ; iband++) {
	int row_number = camera.row_number(iband);
	int band_number = camera.band_number(row_number);

	if (row_number != row_number_expect[iband]  ||
	    band_number != iband) {
	  cerr << "row_number("<<iband<<") = " << row_number
	       << " (expected " << row_number_expect[iband] << ")\n";
	  cerr << "band_number("<<iband<<") = " << band_number
	       << " (expected " << iband << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test frame_coor(), frame_line_coor()
//-----------------------------------------------------------------------

    test_number++;

    {
      ScLookVector sc(27.0, -0.3, 0.4);

      double line_expect[number_band] = {4.02840527542328388222e+01,
                                         4.82840527542328388222e+01,
					 5.62840527542328388222e+01,
                                         3.70840527542328430854e+01,
                                         4.50840527542328359800e+01,
					 5.30840527542328430854e+01,
                                         -1.07776474074074037901e+01};
      double sample_expect[number_band] = {8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
                                           8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
                                           7.98416764444444424953e+02};
      
      for (int iband = 0 ; iband < number_band ; iband++) {
	
	FrameCoordinate fc = camera.frame_coordinate(sc, iband);


	if (cmp.neq(fc.line,line_expect[iband]) ||
	    cmp.neq(fc.sample,sample_expect[iband])) {
	  cerr << iband << ": fc.line = " << fc.line
	       << " (expected " << line_expect[iband] << ")\n";
	  cerr << iband << ": fc.sample = " << fc.sample
	       << " (expected " << sample_expect[iband] << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test frame_coor(), frame_line_coor() with reversed direction flag.
//-----------------------------------------------------------------------

    test_number++;

    {
      ConfigFile config(mspi_test_data_dir() + 
			"mspi_camera_test.config2");
      MspiCamera camera(config);
      ScLookVector sc(27.0, -0.3, 0.4);

      double line_expect[number_band] = {-4.02840527542328388222e+01,
                                         -4.82840527542328388222e+01,
					 -5.62840527542328388222e+01,
                                         -3.70840527542328430854e+01,
                                         -4.50840527542328359800e+01,
					 -5.30840527542328430854e+01,
                                         1.07776474074074037901e+01};
      double sample_expect[number_band] = {8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
                                           8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
					   8.00175853880662771189e+02,
                                           7.98416764444444424953e+02};
      
      for (int iband = 0 ; iband < number_band ; iband++) {
	
	FrameCoordinate fc = camera.frame_coordinate(sc, iband);

	if (cmp.neq(fc.line,line_expect[iband]) ||
	    cmp.neq(fc.sample,sample_expect[iband])) {
	  cerr << iband << ": fc.line = " << fc.line
	       << " (expected " << line_expect[iband] << ")\n";
	  cerr << iband << ": fc.sample = " << fc.sample
	       << " (expected " << sample_expect[iband] << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test frame_coor(), frame_line_coor()
//-----------------------------------------------------------------------

    test_number++;

    {
      ScLookVector sc(27.0, 0.3, -0.4);
      
      double line_expect[number_band] = {-4.17447096000012081163e+01,
					 -3.37447096000012081163e+01,
					 -2.57447096000012152217e+01,
					 -4.49447096000012109585e+01,
					 -3.69447096000012109585e+01,
					 -2.89447096000012109585e+01,
                                         -9.32223525925925855518e+01};
      double sample_expect[number_band] = {7.34639362781665909097e+02,
					   7.34639362781665909097e+02,
					   7.34639362781665909097e+02,
					   7.34639362781665909097e+02,
					   7.34639362781665909097e+02,
					   7.34639362781665909097e+02,
                                           7.36583235555555575047e+02};
      
      for (int iband = 0 ; iband < number_band ; iband++) {
	
	FrameCoordinate fc = camera.frame_coordinate(sc, iband);
	if (cmp.neq(fc.line,line_expect[iband]) ||
	    cmp.neq(fc.sample,sample_expect[iband])) {
	  cerr << iband << ": fc.line = " << fc.line
	       << " (expected " << line_expect[iband] << ")\n";
	  cerr << iband << ": fc.sample = " << fc.sample
	       << " (expected " << sample_expect[iband] << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test sc_look_vector()
// Compare with next test: decreasing frame line coor increases yf, 
// and decreases Xscs.
//-----------------------------------------------------------------------

    test_number++;
    
    {
      double direction_expect[number_band][3] = {
	{9.66619417237047029445e-01,
	 2.55774135797655954860e-01,
	 -1.50497068833303302565e-02},
	{9.66607740287202310547e-01,
	 2.55634495509085957199e-01,
	 -1.79019865568946813517e-02},
	{9.66588016103036462567e-01,
	 2.55492897390171169736e-01,
	 -2.07602145743232650854e-02},
	{9.66634991935328646839e-01,
	 2.55778256969776296437e-01,
	 -1.39382792227167474214e-02},
	{9.66626379606913443965e-01,
	 2.55639424967947448408e-01,
	 -1.67906715198743762552e-02},
	{9.66609725960589138971e-01,
	 2.55498635837488607958e-01,
	 -1.96490397622165365032e-02},
	{9.63872717052457916864e-01,
	 2.65865218588980323222e-01,
	 1.62809970992600361317e-02}
      };
      
      for (int iband = 0 ; iband < number_band ; iband++) {
	FrameCoordinate fc(-5,0);
	
	ScLookVector sc = camera.sc_look_vector(fc, iband);
	
	double length_expect = 1.0;
	
	if (cmp.neq(sc.direction()[0],direction_expect[iband][0]) ||
	    cmp.neq(sc.direction()[1],direction_expect[iband][1]) ||
	    cmp.neq(sc.direction()[2],direction_expect[iband][2]) ||
	    cmp.neq(sc.length(),length_expect)) {
	  cerr << "sc.direction()[0] = " << sc.direction()[0] 
	       << " (expected " << direction_expect[iband][0] << ")\n";
	  cerr << "sc.direction()[1] = " << sc.direction()[1] 
	       << " (expected " << direction_expect[iband][1] << ")\n";
	  cerr << "sc.direction()[2] = " << sc.direction()[2] 
	       << " (expected " << direction_expect[iband][2] << ")\n";
	  cerr << "sc_length() = " << sc.length()
	       << " (expected " << length_expect << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }
      
//-----------------------------------------------------------------------
// Test sc_look_vector() with direction reversal
// Compare with next test: decreasing frame line coor increases yf, 
// and decreases Xscs.
//-----------------------------------------------------------------------

    test_number++;
    
    {
      ConfigFile config(mspi_test_data_dir() + "mspi_camera_test.config2");
      MspiCamera camera(config);

      double direction_expect[number_band][3] = {
	{9.66664123914725226250e-01,
	 2.55785965491580558062e-01,
	 -1.15763290259476552685e-02},
	{9.66662025013343084723e-01,
	 2.55648851951728040000e-01,
	 -1.44289255617417611771e-02},
	{9.66651897661589987187e-01,
	 2.55509782851415012672e-01,
	 -1.72875566367786809296e-02},
	{9.66675967222485499342e-01,
	 2.55789099312144663223e-01,
	 -1.04647535830482783098e-02},
	{9.66676933910773805714e-01,
	 2.55652794842224806438e-01,
	 -1.33174297154035146257e-02},
	{9.66669878342800381610e-01,
	 2.55514535586047475402e-01,
	 -1.61761679319347151385e-02},
	{9.63812579095920662908e-01,
	 2.65848630723510781504e-01,
	 1.97438071025929848257e-02}
      };
      
      for (int iband = 0 ; iband < number_band ; iband++) {
	FrameCoordinate fc(-5,0);
	
	ScLookVector sc = camera.sc_look_vector(fc, iband);
	
	double length_expect = 1.0;
	
	if (cmp.neq(sc.direction()[0],direction_expect[iband][0]) ||
	    cmp.neq(sc.direction()[1],direction_expect[iband][1]) ||
	    cmp.neq(sc.direction()[2],direction_expect[iband][2]) ||
	    cmp.neq(sc.length(),length_expect)) {
	  cerr << "sc.direction()[0] = " << sc.direction()[0]
	       << " (expected " << direction_expect[iband][0] << ")\n";
	  cerr << "sc.direction()[1] = " << sc.direction()[1]
	       << " (expected " << direction_expect[iband][1] << ")\n";
	  cerr << "sc.direction()[2] = " << sc.direction()[2]
	       << " (expected " << direction_expect[iband][2] << ")\n";
	  cerr << "sc_length() = " << sc.length()
	       << " (expected " << length_expect << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }
      
//-----------------------------------------------------------------------
// Test sc_look_vector()
// Compare with previous test: increasing frame line coor decreases yf, 
// and increases Xscs.
//-----------------------------------------------------------------------

    test_number++;
    {
      double direction_expect[number_band][3] = {
	{9.66664123914725004205e-01,
	 2.55785965491580502551e-01,
	 -1.15763290259476535338e-02},
	{9.66662025013343084723e-01,
	 2.55648851951728040000e-01,
	 -1.44289255617417611771e-02},
	{9.66651897661589987187e-01,
	 2.55509782851415012672e-01,
	 -1.72875566367786809296e-02},
	{9.66675967222485499342e-01,
	 2.55789099312144663223e-01,
	 -1.04647535830482783098e-02},
	{9.66676933910773805714e-01,
	 2.55652794842224806438e-01,
	 -1.33174297154035146257e-02},
	{9.66669878342800381610e-01,
	 2.55514535586047475402e-01,
	 -1.61761679319347151385e-02},
	{9.63812579095920662908e-01,
	 2.65848630723510781504e-01,
	 1.97438071025929848257e-02}
      };

      for (int iband = 0 ; iband < number_band ; iband++) {
	FrameCoordinate fc(5,0);

	ScLookVector sc = camera.sc_look_vector(fc, iband);

	double length_expect = 1.0;
    
	if (cmp.neq(sc.direction()[0],direction_expect[iband][0]) ||
	    cmp.neq(sc.direction()[1],direction_expect[iband][1]) ||
	    cmp.neq(sc.direction()[2],direction_expect[iband][2]) ||
	    cmp.neq(sc.length(),length_expect)) {
	  cerr << "sc.direction()[0] = " << sc.direction()[0]
	       << " (expected " << direction_expect[iband][0] << ")\n";
	  cerr << "sc.direction()[1] = " << sc.direction()[1] 
	       << " (expected " << direction_expect[iband][1] << ")\n";
	  cerr << "sc.direction()[2] = " << sc.direction()[2] 
	       << " (expected " << direction_expect[iband][2] << ")\n";
	  cerr << "sc_length() = " << sc.length()
	       << " (expected " << length_expect << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test sc_look_vector()
// Compare with previous test: increasing frame sample coor increases xf, 
// increases Ydcs, and decreases Yscs.
//-----------------------------------------------------------------------

    test_number++;
    {
      double direction_expect[number_band][3] = {
	{9.66664123914725004205e-01,
	 -2.55785965491580502551e-01,
	 -1.15763290259476535338e-02},
	{9.66662025013343084723e-01,
	 -2.55648851951728040000e-01,
	 -1.44289255617417611771e-02},
	{9.66651897661589987187e-01,
	 -2.55509782851415012672e-01,
	 -1.72875566367786809296e-02},
	{9.66675967222485499342e-01,
	 -2.55789099312144663223e-01,
	 -1.04647535830482783098e-02},
	{9.66676933910773805714e-01,
	 -2.55652794842224806438e-01,
	 -1.33174297154035146257e-02},
	{9.66669878342800381610e-01,
	 -2.55514535586047475402e-01,
	 -1.61761679319347151385e-02},
	{9.63812579095920662908e-01,
	 -2.65848630723510781504e-01,
	 1.97438071025929848257e-02}
      };

      for (int iband = 0 ; iband < number_band ; iband++) {
	FrameCoordinate fc(5,1535);

	ScLookVector sc = camera.sc_look_vector(fc, iband);

	double length_expect = 1.0;
    
	if (cmp.neq(sc.direction()[0],direction_expect[iband][0]) ||
	    cmp.neq(sc.direction()[1],direction_expect[iband][1]) ||
	    cmp.neq(sc.direction()[2],direction_expect[iband][2]) ||
	    cmp.neq(sc.length(),length_expect)) {
	  cerr << iband << ":sc.direction()[0] = " << sc.direction()[0]
	       << " (expected " << direction_expect[iband][0] << ")\n";
	  cerr << iband << ":sc.direction()[1] = " << sc.direction()[1] 
	       << " (expected " << direction_expect[iband][1] << ")\n";
	  cerr << iband << ":sc.direction()[2] = " << sc.direction()[2] 
	       << " (expected " << direction_expect[iband][2] << ")\n";
	  cerr << "sc_length() = " << sc.length()
	       << " (expected " << length_expect << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test sc_look_vector()
// Compare with previous test: decreasing frame sample coor decreases xf, 
// decreases Ydcs, and increases Yscs.
//-----------------------------------------------------------------------

    test_number++;
    {
      double direction_expect[number_band][3] = {
	{9.66664123914725004205e-01,
	 2.55785965491580502551e-01,
	 -1.15763290259476535338e-02},
	{9.66662025013343084723e-01,
	 2.55648851951728040000e-01,
	 -1.44289255617417611771e-02},
	{9.66651897661589987187e-01,
	 2.55509782851415012672e-01,
	 -1.72875566367786809296e-02},
	{9.66675967222485499342e-01,
	 2.55789099312144663223e-01,
	 -1.04647535830482783098e-02},
	{9.66676933910773805714e-01,
	 2.55652794842224806438e-01,
	 -1.33174297154035146257e-02},
	{9.66669878342800381610e-01,
	 2.55514535586047475402e-01,
	 -1.61761679319347151385e-02},
	{9.63812579095920662908e-01,
	 2.65848630723510781504e-01,
	 1.97438071025929848257e-02}
      };

      for (int iband = 0 ; iband < number_band ; iband++) {
	FrameCoordinate fc(5,0);

	ScLookVector sc = camera.sc_look_vector(fc, iband);

	double length_expect = 1.0;
    
	if (cmp.neq(sc.direction()[0],direction_expect[iband][0]) ||
	    cmp.neq(sc.direction()[1],direction_expect[iband][1]) ||
	    cmp.neq(sc.direction()[2],direction_expect[iband][2]) ||
	    cmp.neq(sc.length(),length_expect)) {
	  cerr << "sc.direction()[0] = " << sc.direction()[0] 
	       << " (expected " << direction_expect[iband][0] << ")\n";
	  cerr << "sc.direction()[1] = " << sc.direction()[1] 
	       << " (expected " << direction_expect[iband][1] << ")\n";
	  cerr << "sc.direction()[2] = " << sc.direction()[2] 
	       << " (expected " << direction_expect[iband][2] << ")\n";
	  cerr << "sc_length() = " << sc.length()
	       << " (expected " << length_expect << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test detector_look(FrameCoor,int,ColumnVector)
//-----------------------------------------------------------------------

    test_number++;
    {
      double v_dcs_expect[number_band][3] = {
	{-1.15763290259476535338e-02,
	 -2.55785965491580502551e-01,
	 9.66664123914725004205e-01},
	{-1.44289255617417611771e-02,
	 -2.55648851951728040000e-01,
	 9.66662025013343084723e-01},
	{-1.72875566367786809296e-02,
	 -2.55509782851415012672e-01,
	 9.66651897661589987187e-01},
	{-1.04647535830482783098e-02,
	 -2.55789099312144663223e-01,
	 9.66675967222485499342e-01},
	{-1.33174297154035146257e-02,
	 -2.55652794842224806438e-01,
	 9.66676933910773805714e-01},
	{-1.61761679319347151385e-02,
	 -2.55514535586047475402e-01,
	 9.66669878342800381610e-01},
	{1.97438071025929813562e-02,
	 -2.65848630723510725993e-01,
	 9.63812579095920440864e-01}
      };

      for (int iband = 0 ; iband < number_band ; iband++) {
	FrameCoordinate fc(5,0);

	blitz::Array<double, 1> v_dcs(3);
	camera.detector_look(fc,iband,v_dcs);

	if (cmp.neq(v_dcs(0),v_dcs_expect[iband][0]) ||
	    cmp.neq(v_dcs(1),v_dcs_expect[iband][1]) ||
	    cmp.neq(v_dcs(2),v_dcs_expect[iband][2])) {
	  cerr << "v_dcs()(1) = " << v_dcs(0) 
	       << " (expected " << v_dcs_expect[iband][0] << ")\n";
	  cerr << "v_dcs()(2) = " << v_dcs(1) 
	       << " (expected " << v_dcs_expect[iband][1] << ")\n";
	  cerr << "v_dcs()(3) = " << v_dcs(2) 
	       << " (expected " << v_dcs_expect[iband][2] << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test detector_look(ScLookVector,ColumnVector)
//-----------------------------------------------------------------------

    test_number++;

    {
      ScLookVector sc(27.0, -0.3, 0.4);

      double v_dcs_expect[3] = {
	1.48122752048167587863e-02,
	1.11092064036125686560e-02,
	9.99828576325131113123e-01
      };
	
      blitz::Array<double, 1> v_dcs(3);
      camera.detector_look(sc,v_dcs);

      if (cmp.neq(v_dcs(0),v_dcs_expect[0]) ||
	  cmp.neq(v_dcs(1),v_dcs_expect[1]) ||
	  cmp.neq(v_dcs(2),v_dcs_expect[2])) {
	cerr << "v_dcs()(1) = " << v_dcs(0) 
	     << " (expected " << v_dcs_expect[0] << ")\n";
	cerr << "v_dcs()(2) = " << v_dcs(1) 
	     << " (expected " << v_dcs_expect[1] << ")\n";
	cerr << "v_dcs()(3) = " << v_dcs(2) 
	     << " (expected " << v_dcs_expect[2] << ")\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test angular_separation(int,int)
//-----------------------------------------------------------------------

    test_number++;

    {
      int reference_band = 1;

      double angular_separation_expect[number_band] = 
	{-2.87509743311235464389e-03, 
	 0.0, 
	 2.87505465482229246102e-03,
	 -4.02513773725820221683e-03, 
	 -1.15003650156956832747e-03, 
	 1.72504096818234002250e-03,
	 -2.12735828800964844876e-02};

      for (int iband = 0 ; iband < number_band ; iband++) {

	double angular_separation = 
	  camera.angular_separation(reference_band, iband);

	if (cmp.neq(angular_separation, 
			     angular_separation_expect[iband])) {
	  cerr << iband << ":angular_separation = " << angular_separation
	       << " (expected " << angular_separation_expect[iband] << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test paraxial_offset()
//-----------------------------------------------------------------------

    test_number++;

    {
      double line_offset_expect[number_band] = 
	{3.91114046925584588621e+01,
	 3.93225488022550777600e+01,
	 3.95515012995958485931e+01,
	 3.91114046925584588621e+01,
	 3.93225488022550777600e+01,
	 3.95515012995958485931e+01,
	 0.0};
      double sample_offset_expect[number_band] = 
	{3.12289783792453867761e+01,
	 3.16220033730039595810e+01,
	 3.20145509437610442660e+01,
	 3.12289783792453867761e+01,
	 3.16220033730039595810e+01,
	 3.20145509437610442660e+01,
	 0.0};

      FrameCoordinate f(-0.3, 0.1);

      for (int iband = 0 ; iband < number_band ; iband++) {

	double line_offset;
	double sample_offset;

	camera.paraxial_offset(iband, f, line_offset, sample_offset);

	if (cmp.neq(line_offset, 
			     line_offset_expect[iband]),
	    cmp.neq(sample_offset, 
			     sample_offset_expect[iband])) {
	  cerr << iband << ":line_offset = " << line_offset
	       << " (expected " << line_offset_expect[iband] << ")\n";
	  cerr << iband << ":sample_offset = " << sample_offset
	       << " (expected " << sample_offset_expect[iband] << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test parameter()
//-----------------------------------------------------------------------

    test_number++;

    {
      const int size_expect = 3;
      blitz::Array<double, 1> parameter_expect(size_expect);

      double deg_to_rad = acos(-1.0) / 180.0;
      parameter_expect(0) = 180.0 * deg_to_rad;
      parameter_expect(1) = 90.0 * deg_to_rad;
      parameter_expect(2) = 0.0 * deg_to_rad;
      
      blitz::Array<double, 1> parameter = camera.parameter();
      
      if (parameter.rows() != size_expect) {
	cerr << "parameter.Nrows() = " << parameter.rows() 
	     << " (expected " << size_expect << ")\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
	
      }

      for (int i = 0 ; i < size_expect ; i++) {
	if (parameter(i) != parameter_expect(i)) {
	  cerr << "parameter("<<i<<") = " << parameter(i) 
	       << " (expected " << parameter_expect(i) << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test parameter(NEWMAT::ColumnVector)
//-----------------------------------------------------------------------

    test_number++;

    {
      MspiCamera camera(config);

      const int size_expect = 3;
      blitz::Array<double, 1> parameter_expect(size_expect);
      parameter_expect(0) = 10.0;
      parameter_expect(1) = 20.0;
      parameter_expect(2) = 30.0;
      

      camera.parameter(parameter_expect);

      blitz::Array<double, 1> parameter = camera.parameter();
      
      if (parameter.rows() != size_expect) {
	cerr << "parameter.Nrows() = " << parameter.rows() 
	     << " (expected " << size_expect << ")\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
	
      }

      for (int i = 0 ; i < size_expect ; i++) {
	if (parameter(i) != parameter_expect(i)) {
	  cerr << "parameter("<<i<<") = " << parameter(i) 
	       << " (expected " << parameter_expect(i) << ")\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test jacobian_look()
//-----------------------------------------------------------------------

    test_number++;

    {
      ScLookVector sc(27.0, 0.3, -0.4);

      std::string name = "jacobian_look";
      for (int iband = 0 ; iband < number_band ; iband++) {
	boost::multi_array<double,2> result = 
	  multi_array(camera.jacobian_look(sc, iband));
	save_expect(result, iband, name);

	if (array_check(result, expect_init(iband, name), cmp, __LINE__, error)) {
	  fprintf(stderr,"Trouble with %d-%s\n", iband, name.c_str());
	}
      }
    }

//-----------------------------------------------------------------------
// Test jacobian()
//-----------------------------------------------------------------------

    test_number++;

    {
      ScLookVector sc(27.0, 0.3, -0.4);

      std::string name = "jacobian";
      for (int iband = 0 ; iband < number_band ; iband++) {
	boost::multi_array<double,2> result = camera.jacobian(sc, iband);
	save_expect(result, iband, name);

	if (array_check(result, expect_init(iband, name), cmp, __LINE__, error)) {
	  fprintf(stderr,"Trouble with %d-%s\n", iband, name.c_str());
	}
      }
    }

//-----------------------------------------------------------------------
// Test jacobian() with AirMSPI camera model.  Note that
// the relationship between camera and station coordinates is different
// than for GroundMSPI.
//-----------------------------------------------------------------------

    test_number++;

    {
      ConfigFile config("GeoCal/MspiCamera/src/AIRMSPI_CONFIG_CAMERA_MODEL_0001.config");
      MspiCamera camera(config);

      ScLookVector sc(0,0,1);

      std::string name = "jacobian-AirMSPI";
      for (int iband = 0 ; iband < camera.number_band() ; iband++) {
	boost::multi_array<double,2> result = camera.jacobian(sc, iband);
	save_expect(result, iband, name);

	if (array_check(result, expect_init(iband, name), cmp, __LINE__, error)) {
	  fprintf(stderr,"Trouble with %d-%s\n", iband, name.c_str());
	}
      }
    }

//-----------------------------------------------------------------------
// Output changes in frame coordinates as a function of change in 
// camera_yaw.
//-----------------------------------------------------------------------

//    test_number++;
//
//    {
//      MspiCamera camera(config);
//
//      ColumnVector v(3);
//      v(1) = 27.0; // (Xscs, Zccs, Zdcs)   
//      v(2) = -0.3;   // (Yscs, -Yccs, -Ydcs, +xf)
//      v(3) = 0.4;   // (Zscs, Xccs, Xdcs, +yf)
//      double length = 2.0;
//      ScLookVector sc(v,length);
//      FrameCoor fc0 = camera.frame_coor(sc, 0);
//      ColumnVector p0 = camera.parameter();
//      ColumnVector p = p0;
//      for (double yaw = -2e-1 ; yaw < 2e-1 ; yaw += 1e-6) {
//	p(1) = p0(1) + yaw;
//	camera.parameter(p);
//	FrameCoor fc = camera.frame_coor(sc, 0);
//	cerr << yaw << " " << fc.line - fc0.line() << " (yaw diff line)\n";
//      }
//    }

//-----------------------------------------------------------------------
// Write paraxial offsets to external file for visual inspection using 
// gnuplot. 
//   gnuplot GeoCal/MspiCamera/src/mspi_camera_test.gnuplot
//-----------------------------------------------------------------------

    test_number++;

    {
      for (int iband = 0 ; iband < number_band ; iband++) {
	std::ostringstream filename;
	filename << "GeoCal/MspiCamera/tmp/mspi_camera_test.band" << iband << ".dat";
	std::ofstream dat(filename.str().c_str());
	
	for (int isample = 0 ; isample < camera.number_sample(0) ; isample++) {
	  FrameCoordinate f(0, isample);

	  double line_offset;
	  double sample_offset;
	  camera.paraxial_offset(iband, f, line_offset, sample_offset);
	  
	  dat << isample << " " << line_offset << " " << sample_offset << "\n";
	}
      }
    }

//-----------------------------------------------------------------------
// Argument check: Band < 0 
//                 Band > number_band()-1
// (frame_coor)
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      int band = -1;
      FrameCoordinate fc(0,0);
      ScLookVector sc = camera.sc_look_vector(fc, 0);

      try {
	FrameCoordinate fc = camera.frame_coordinate(sc, band);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Band < 0")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      bool caught_exception = false;
      int band = 50;
      FrameCoordinate fc(0,0);
      ScLookVector sc = camera.sc_look_vector(fc, 0);

      try {
	FrameCoordinate fc = camera.frame_coordinate(sc, band);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Band > number_band() - 1")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Argument check: Band < 0 
//                 Band > number_band()-1
// (sc_look_vector)
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      int band = -1;
      FrameCoordinate fc(0,0);

      try {
	ScLookVector sc = camera.sc_look_vector(fc, band);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Band < 0")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      bool caught_exception = false;
      int band = 50;
      FrameCoordinate fc(0,0);

      try {
	ScLookVector sc = camera.sc_look_vector(fc, band);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Band > number_band() - 1")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test band_number(int) with out of range row number.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      int row_number = 100;

      try {
	camera.band_number(row_number);
      } catch (std::exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Row number not assigned to camera model band: 100")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Check for test failures.
//-----------------------------------------------------------------------

    if (error) {
      cerr << "Error! One or more tests failed.\n";
      cerr << "kdiff3 " << new_expect_filename << " " << expect_filename << "\n";
    } else {
      cerr << "All tests succeeded.\n";
    }
}
BOOST_AUTO_TEST_SUITE_END()

