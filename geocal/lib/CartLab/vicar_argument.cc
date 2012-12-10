#include "vicar_argument.h"
#include "geocal_exception.h"
#ifdef HAVE_VICAR_RTL
#include <zvproto.h>
extern "C" {
#include <taeconf.inp>
#include <parblk.inc>
// We can't include taeintproto.h because it contains structures with 
// the reserved name "class".
void q_init(struct PARBLK *p, FUNINT pool_size, FUNINT mode);
FUNCTION CODE q_intg(struct PARBLK* p, const TEXT name[], FUNINT count, 
		     TAEINT intg[], FUNINT mode);
FUNCTION CODE q_real(struct PARBLK* p, const TEXT name[], FUNINT count, 
		     TAEFLOAT real[], FUNINT mode);
FUNCTION CODE q_string(struct PARBLK *p, const TEXT name[],
		       FUNINT count, const TEXT *vector[], FUNINT mode);
}
#endif
#include <iostream>

using namespace GeoCal;
//-----------------------------------------------------------------------
/// Set up to parse VICAR arguments. VICAR arguments are accessed by
/// keywords. This isn't a bad interface, but because we also want to
/// support straight unix command line style arguments, we have the
/// VicarArgument interface in terms of index number (first argument, 
/// second argument, etc.). Keyword_list gives the list of keywords in
/// order that we want.
///
/// VICAR has the convention that when creating a new file it
/// automatically copies over all of the labels from the "primary
/// input" - usually the first input file. The idea is that with old
/// VICAR files most of the time the output image would have same
/// size, map projection, etc. However, this doesn't fit well with the
/// geocal usage at all. Much of the time the output file doesn't have
/// any relationship to the input file. Also the label reading and
/// writing happens in the background anyways, so there isn't a lot
/// gained by the defaults. This can often cause problems, such as
/// copying map projection data from an input file to an output file
/// that isn't map projected.
///
/// By default, we turn this copying off. You can get the old VICAR
/// behavior by setting Copy_primary_input_label to true.
//-----------------------------------------------------------------------

VicarArgument::VicarArgument(int Argc, char** Argv, 
			     bool Copy_primary_input_label)
{
#ifdef HAVE_VICAR_RTL
  if(!zvzinit(Argc, Argv))
    throw Exception("Call to zvzinit failed");
  if(!Copy_primary_input_label)
    zvselpi(0);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Return the type and count for the given keyword.
//-----------------------------------------------------------------------

void VicarArgument::type(const std::string& Keyword, std::string& Type, 
			 int& Count)
{
#ifdef HAVE_VICAR_RTL
  int def, maxlen;
  char tp[100];
  int status = zvpstat(const_cast<char*>(Keyword.c_str()), &Count, 
		       &def, &maxlen, tp);
  if(status != 1)
    throw Exception("Call to zvpstat failed");
  Type = tp;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Return the type, count and maxlen for the given keyword.
//-----------------------------------------------------------------------

void VicarArgument::type(const std::string& Keyword, std::string& Type, 
			 int& Count, int& Maxlen)
{
#ifdef HAVE_VICAR_RTL
  int def;
  char tp[100];
  int status = zvpstat(const_cast<char*>(Keyword.c_str()), &Count, 
		       &def, &Maxlen, tp);
  if(status != 1)
    throw Exception("Call to zvpstat failed");
  Type = tp;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
///  Print an VicarArgument to a stream
//-----------------------------------------------------------------------

void VicarArgument::print(std::ostream& Os) const
{
  Os << "VicarArgument";
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvp. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarArgument::zvpw(char *name, void *value, int *count)
{
#ifdef HAVE_VICAR_RTL
  return zvp(name, value, count);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvparm. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarArgument::zvparmw(char *name, void *value, int *count, int *def,
			   int maxcnt, int length)
{
#ifdef HAVE_VICAR_RTL
  return zvparm(name, value, count, def, maxcnt, length);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvparmd. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarArgument::zvparmdw(char *name, void *value, int *count, int *def,
			    int maxcnt, int length)
{
#ifdef HAVE_VICAR_RTL
  return zvparmd(name, value, count, def, maxcnt, length);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Write a value to a TCL variable.
//-----------------------------------------------------------------------

void VicarArgument::write_out(const std::string& Keyword, int Val)
{
#ifdef HAVE_VICAR_RTL
  // This is copied directly from cartoTaeUtils.c. We have a copy here so
  // we don't need to link to that library.
  struct PARBLK parblk; 

  q_init(&parblk, 500, P_ABORT);
  q_intg(&parblk, Keyword.c_str(), 1, &Val, P_ADD);
  zvq_out(&parblk);
#else
  throw VicarNotAvailableException();
#endif
}

void VicarArgument::write_out(const std::string& Keyword, double Val)
{
#ifdef HAVE_VICAR_RTL
  // This is copied directly from cartoTaeUtils.c. We have a copy here so
  // we don't need to link to that library.
  struct PARBLK parblk; 

  q_init(&parblk, 500, P_ABORT);
  q_real(&parblk, Keyword.c_str(), 1, &Val, P_ADD);
  zvq_out(&parblk);
#else
  throw VicarNotAvailableException();
#endif
}

void VicarArgument::write_out(const std::string& Keyword, 
			      const std::string& Val)
{
#ifdef HAVE_VICAR_RTL
  // This is copied directly from cartoTaeUtils.c. We have a copy here so
  // we don't need to link to that library.
  struct PARBLK parblk; 

  q_init(&parblk, 500, P_ABORT);
  const char* v = Val.c_str();
  q_string(&parblk, Keyword.c_str(), 1, &v, P_ADD);
  zvq_out(&parblk);
#else
  throw VicarNotAvailableException();
#endif
}

