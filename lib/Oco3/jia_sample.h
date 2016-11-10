#ifndef JIA_SAMPLE_H
#define JIA_SAMPLE_H
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  Dummy class for Jia.
*******************************************************************/

class JiaSample : public Printable<JiaSample> {
public:
  JiaSample(int Blah) : blah_(Blah) {}
  virtual ~JiaSample() {}
  void jia_func() const;

//-----------------------------------------------------------------------
/// Sample "accessor" function, we'll put this in as a python
/// attribute in swig.
//-----------------------------------------------------------------------

  int blah() const {return blah_;}

//-----------------------------------------------------------------------
/// Print to stream (all classes have this)
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "JiaSample"; }
private:
  int blah_;

//-----------------------------------------------------------------------
/// Cookie cutter code for adding "serialization". This allows objects
/// of this class to be stored to a file. Useful for pickle
/// serialization in python, and also just for saving from C++.
//-----------------------------------------------------------------------
  JiaSample() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

// More cookie cutter for serialization.
GEOCAL_EXPORT_KEY(JiaSample);
#endif
