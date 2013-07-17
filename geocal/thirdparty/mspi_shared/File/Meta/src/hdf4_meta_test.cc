//===========================================================================
//                                                                          
//                               Hdf4Meta                                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "hdf4_meta.h"		// Definition of Hdf4Meta
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.
#include <cstring>		// Definition of strcmp
#include "MSPI-Shared/UnitTest/src/float_comparator.h"
				// Definition of FloatComparator
#include <cmath> 		// Definition std::acos
#include "mfhdf.h"		// Definition of SDstart
#include "MSPI-Shared/File/FileHandle/src/hdfeos2_file_handle.h"
				// Definition of Hdfeos2FileHandle

using std::cerr;
using MSPI::Shared::Meta;
using MSPI::Shared::Hdf4Meta;
using MSPI::Shared::Exception;
using MSPI::Shared::FloatComparator;
using MSPI::Shared::Hdfeos2FileHandle;

//////////////////////////////////////////////////////////////////////////
// Generate unit-test input.
//////////////////////////////////////////////////////////////////////////

Hdf4Meta meta_init()
{
  Hdf4Meta meta;
  meta.add<std::string>("name","Kanzi");
  meta.add<unsigned>("number_banana",5);
  meta.add<int>("score",-2);
  meta.add<bool>("hungry",true);
  meta.add<bool>("sleepy",false);
  meta.add<float>("temperature",98.6);
  meta.add<double>("alpha_score",std::acos(-1));
  return meta;
}

//////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  FloatComparator cmpf;
  FloatComparator cmpd;

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test default constructor.
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdf4Meta meta;

      if (meta.number_element() != 0) {
	cerr << "number_element = " <<meta.number_element() << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add(string,...)
// Test number_element()
//-----------------------------------------------------------------------

    test_number++;

    {
      Hdf4Meta meta;
      meta.add<std::string>("name","Bobo");
      meta.add<int>("IQ",130);
      meta.add<int>("weight",175);

      if (meta.number_element() != 3) {
	cerr << "number_element = " <<meta.number_element() << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test get_value()
//-----------------------------------------------------------------------

    test_number++;

    {
      Hdf4Meta meta(meta_init());
      std::string name = meta.get_value<std::string>("name");
      int number_banana = meta.get_value<unsigned>("number_banana");
      bool hungry = meta.get_value<bool>("hungry");
      bool sleepy = meta.get_value<bool>("sleepy");
      float temperature = meta.get_value<float>("temperature");
      double alpha_score = meta.get_value<double>("alpha_score");
      
      if (number_banana != 5 ||
	  name != "Kanzi" ||
	  hungry != true ||
	  sleepy != false ||
	  cmpf.neq(temperature,98.6) ||
	  cmpd.neq(alpha_score,std::acos(-1))) {
	cerr << "number_banana = " << number_banana << "\n";;
	cerr << "name = " << name << "\n";
	cerr << "hungry = " << hungry << "\n";
	cerr << "sleepy = " << sleepy << "\n";
	cerr << "temperature = " << temperature << "\n";
	cerr << "alpha_score = " << alpha_score << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test match_prefix(std::string&)
//-----------------------------------------------------------------------

    test_number++;

    {
      Hdf4Meta meta(meta_init());
      Meta result = meta.match_prefix("na");

      if (result.number_element() != 1) {
	cerr << "number_element = " << result.number_element() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      Hdf4Meta meta(meta_init());
      Meta result = meta.match_prefix("n");

      if (result.number_element() != 2) {
	cerr << "number_element = " << result.number_element() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      Hdf4Meta meta(meta_init());
      Meta result = meta.match_prefix("k");

      if (result.number_element() != 0) {
	cerr << "number_element = " << result.number_element() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add_prefix(std::string&)
//-----------------------------------------------------------------------

    test_number++;

    {
      Hdf4Meta meta(meta_init());
      Meta result = meta.add_prefix("Kanzi.");

      std::ostringstream out;
      std::string expect = 
	"3.14159265358979 : Kanzi.alpha_score\n"
	"1 : Kanzi.hungry\n"
	"Kanzi : Kanzi.name\n"
	"5 : Kanzi.number_banana\n"
	"-2 : Kanzi.score\n"
	"0 : Kanzi.sleepy\n"
	"98.6 : Kanzi.temperature\n";

      result.stream_export(out);

      if (out.str() != expect) {
	cerr << "metadata:\n" << out.str() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add(Meta&) with a duplicate of the existing metadata.
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdf4Meta meta(meta_init());
      meta.add(meta_init());

      if (meta.number_element() != 7) {
	cerr << "number_element = " << meta.number_element() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add(Meta&) with new metadata.
// Test stream_export()
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdf4Meta meta(meta_init());

      Hdf4Meta meta2;
      meta2.add<std::string>("parent","Koko");
      meta2.add<int>("parent.weight",310);
      meta.add(meta2);

      std::ostringstream out;
      std::string expect = 
	"3.14159265358979 : alpha_score\n"
	"1 : hungry\n"
	"Kanzi : name\n"
	"5 : number_banana\n"
	"Koko : parent\n"
	"310 : parent.weight\n"
	"-2 : score\n"
	"0 : sleepy\n"
	"98.6 : temperature\n";

      meta.stream_export(out);

      if (out.str() != expect) {
	cerr << "metadata:\n" << out.str() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test hdf4_export(int32)
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdf4Meta meta(meta_init());
      std::string filename = "File/Meta/tmp/hdf4_meta_test.hdf4";
      meta.add<std::string>("filename",filename);
      
      Hdfeos2FileHandle file(filename,DFACC_CREATE);
      meta.hdf4_export(file.file_attr_group_id());
    }

//-----------------------------------------------------------------------
// Test hdf4_import(int32)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::string filename = "File/Meta/tmp/hdf4_meta_test.hdf4";
      Hdfeos2FileHandle file(filename,DFACC_READ);
      Meta meta = Hdf4Meta::hdf4_import(file.file_attr_group_id());

      
      std::string expect = 
	"3.14159265358979 : alpha_score\n"
	"File/Meta/tmp/hdf4_meta_test.hdf4 : filename\n"
	"1 : hungry\n"
	"Kanzi : name\n"
	"5 : number_banana\n"
	"-2 : score\n"
	"0 : sleepy\n"
	"98.6 : temperature\n";

      std::ostringstream out;
      meta.stream_export(out);
      if (out.str() != expect) {
	cerr << "metadata:\n" << out.str() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test re-writing metadata in an existing file.
//-----------------------------------------------------------------------

    test_number++;
    {
      std::string filename = "File/Meta/tmp/hdf4_meta_test.hdf4";
      Hdf4Meta meta(meta_init());
      meta.add<std::string>("filename","This is replaced");
      Hdfeos2FileHandle file(filename,DFACC_RDWR);
      file.clear_meta();
      meta.hdf4_export(file.file_attr_group_id());

      std::string expect = 
	"3.14159265358979 : alpha_score\n"
	"This is replaced : filename\n"
	"1 : hungry\n"
	"Kanzi : name\n"
	"5 : number_banana\n"
	"-2 : score\n"
	"0 : sleepy\n"
	"98.6 : temperature\n";

      std::ostringstream out;
      meta.stream_export(out);
      
      if (out.str() != expect) {
	cerr << "metadata:\n" << out.str() << "\n";;
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add(std::string,...) with different values for the same name.
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;

      Hdf4Meta meta(meta_init());

      try {
	meta.add<unsigned>("number_banana",6);
      } catch (Exception &e) {
	caught_exception = true;
	if (strcmp(e.what(),
		   "Update not allowed for metadata: number_banana=5 -> number_banana=6")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add(std::string,...) with incompatible types for the same name.
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;

      Hdf4Meta meta(meta_init());

      try {
	meta.add<int>("number_banana",5);
      } catch (std::exception &e) {
	caught_exception = true;
	if (strcmp(e.what(),
		   "boost::bad_get: failed value get using boost::get")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add(Meta) with different values for the same name.
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;

      Hdf4Meta meta(meta_init());
      Hdf4Meta meta2;
      meta2.add<std::string>("name","Koko");
      meta2.add<int>("number_banana",5);

      try {
	meta.add(meta2);
      } catch (Exception &e) {
	caught_exception = true;
	if (strcmp(e.what(),
		   "Update not allowed for metadata: name=Kanzi -> name=Koko")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test get_value() with invalid type conversion
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      Hdf4Meta meta(meta_init());

      try {
	meta.get_value<int>("name");
      } catch (std::exception &e) {
	caught_exception = true;
	if (strcmp(e.what(),
		   "boost::bad_get: failed value get using boost::get")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Check for test failures.
//-----------------------------------------------------------------------

    if (error) {
      cerr << "Error! One or more tests failed.\n";
    } else {
      cerr << "All tests succeeded.\n";
    }

//-----------------------------------------------------------------------
// End try block for main.
//-----------------------------------------------------------------------
    
  }

//-----------------------------------------------------------------------
// Catch exceptions.
//-----------------------------------------------------------------------

  catch(const Exception& e) {
    cerr << "Caught Exception: " << e.what() << "\n"
	 << "test_number = " << test_number <<"\n";
  }
}
