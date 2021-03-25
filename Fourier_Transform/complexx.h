#include<iostream>
#pragma once
class Complexx{
private:
  double re, im;
public:
  Complexx();
  Complexx(double x, double y);
  double modul();
  Complexx operator+ (const Complexx & x);
  Complexx operator- (const Complexx & x);
  Complexx operator* (const Complexx & x);
  Complexx operator/ (const Complexx & x);
  Complexx operator= (const Complexx & x);
  friend std::istream & operator>>(std::istream & is, Complexx & ob);
  friend std::ostream & operator<<(std::ostream & is, const Complexx & ob);
};
