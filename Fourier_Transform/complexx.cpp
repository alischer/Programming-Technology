#include<iostream>
#include<math.h>
#include"complexx.h"
using namespace std;

Complexx::Complexx(){
  re = 0.0;
  im = 0.0;
}
Complexx::Complexx(double x, double y){
  re = x;
  im = y;
}
double Complexx::modul(){
  return sqrt(re * re + im * im);
}
Complexx Complexx::operator+ (const Complexx & x){
  return Complexx(x.re + re, x.im + im);
}
Complexx Complexx::operator- (const Complexx & x){
  return Complexx(re - x.re, im - x.im);
}
Complexx Complexx::operator* (const Complexx & x){
  return Complexx(re * x.re - im * x.im, re * x.im + im * x.re);
}
Complexx Complexx::operator/ (const Complexx & x){
  if ((x.re == 0) & (x.im == 0)) {
    cout << "Division by Zero";
    return Complexx(-1, 0);
  }
  double dnom = x.re * x.re + x.im * x.im;
  return Complexx((re * x.re - im * x.im) / dnom, (re * x.im + im * x.re) / dnom);
}
Complexx Complexx::operator= (const Complexx & x){
    re = x.re;
    im = x.im;
    return * this;
}
istream & operator>>(istream & is, Complexx & ob){
    is >> ob.re >> ob.im;
    return is;
}
ostream & operator<<(ostream & is, const Complexx & ob){
  is << "(" << ob.re << ", " << ob.im << ")";
  return is;
}
