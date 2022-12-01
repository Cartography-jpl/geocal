// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%module msp_wrap
%{
  #include <iostream>
  class Test {
  public:
    Test() { std::cerr << "Hi there"; }
  };
%}

class Test {
public:
  Test();
};

