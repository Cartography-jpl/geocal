#ifndef GEOCAL_STATIC_SORT_H
#define GEOCAL_STATIC_SORT_H
#include "static_sort.h"
#include "geocal_exception.h"
#include <vector>

namespace GeoCal {

//-----------------------------------------------------------------------
/// Simple wrapper around StaticSort to work with data of size 0 to
/// 30. Data is sorted in place.
//-----------------------------------------------------------------------
template<class T> void static_sort(std::vector<T>& d) {
  switch(d.size()) {
  case 0:
    return;
  case 1:
    return;
  case 2:
    StaticSort<2> t2;
    t2(d);
    return;
  case 3:
    StaticSort<3> t3;
    t3(d);
    return;
  case 4:
    StaticSort<4> t4;
    t4(d);
    return;
  case 5:
    StaticSort<5> t5;
    t5(d);
    return;
  case 6:
    StaticSort<6> t6;
    t6(d);
    return;
  case 7:
    StaticSort<7> t7;
    t7(d);
    return;
  case 8:
    StaticSort<8> t8;
    t8(d);
    return;
  case 9:
    StaticSort<9> t9;
    t9(d);
    return;
  case 10:
    StaticSort<10> t10;
    t10(d);
    return;
  case 11:
    StaticSort<11> t11;
    t11(d);
    return;
  case 12:
    StaticSort<12> t12;
    t12(d);
    return;
  case 13:
    StaticSort<13> t13;
    t13(d);
    return;
  case 14:
    StaticSort<14> t14;
    t14(d);
    return;
  case 15:
    StaticSort<15> t15;
    t15(d);
    return;
  case 16:
    StaticSort<16> t16;
    t16(d);
    return;
  case 17:
    StaticSort<17> t17;
    t17(d);
    return;
  case 18:
    StaticSort<18> t18;
    t18(d);
    return;
  case 19:
    StaticSort<19> t19;
    t19(d);
    return;
  case 20:
    StaticSort<20> t20;
    t20(d);
    return;
  case 21:
    StaticSort<21> t21;
    t21(d);
    return;
  case 22:
    StaticSort<22> t22;
    t22(d);
    return;
  case 23:
    StaticSort<23> t23;
    t23(d);
    return;
  case 24:
    StaticSort<24> t24;
    t24(d);
    return;
  case 25:
    StaticSort<25> t25;
    t25(d);
    return;
  case 26:
    StaticSort<26> t26;
    t26(d);
    return;
  case 27:
    StaticSort<27> t27;
    t27(d);
    return;
  case 28:
    StaticSort<28> t28;
    t28(d);
    return;
  case 29:
    StaticSort<29> t29;
    t29(d);
    return;
  case 30:
    StaticSort<30> t30;
    t30(d);
    return;
  default:
    Exception e;
    e << "Size of vector needs to be <= 30. Size found: " << d.size();
    throw e;
  }
}
}
#endif

