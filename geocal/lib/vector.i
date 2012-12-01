%module geocal

// Allow access to std::vector
%include <std_vector.i>

namespace std {
%template(Vector_double) vector<double>;
%template(Vector_string) vector<std::string>;
%template(Vector_unsigned_char) vector<unsigned char>;
%template(Vector_short_int) vector<short int>;
%template(Vector_int) vector<int>;
%template(Vector_float) vector<float>;
};

