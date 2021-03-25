#include<vector>
#include<math.h>
#include"complexx.h"
#include"rec_ftt.h"
using namespace std;

vector<Complexx> rev_rec_ftt(const vector<Complexx> &a, int n){
  if(n == 1){
    return a;
  }
  Complexx w_n(cos(2.0 * (M_PI / (n * 1.0)) * M_PI / 180.0), -sin(2.0 * (M_PI / (n * 1.0)) * M_PI / 180.0));
  Complexx w(1.0, 0.0);
  vector<Complexx> a_0, a_1;
  for(int i = 0; i < n; i += 2){
    a_0.push_back(a[i]);
  }
  for(int i = 1; i < n; i += 2){
    a_1.push_back(a[i]);
  }
  vector<Complexx>y_0, y_1, y(2*n);
  y_0 = rec_ftt(a_0, n / 2);
  y_1 = rec_ftt(a_1, n / 2);
  for(int k = 0; k < n / 2; k++){
    y[k] = y_0[k] + w * y_1[k];
    y[k + n / 2] = y_0[k] - w * y_1[k];
    w = w * w_n;
    y[k] = y[k] / Complexx(2, 0);
    y[k + n / 2] = y[k + n / 2]  / Complexx(2, 0);
  }
  return y;
}
