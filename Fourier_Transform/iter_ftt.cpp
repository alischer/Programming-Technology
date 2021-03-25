#include<vector>
#include<math.h>
#include"complexx.h"
#include"iter_ftt.h"
using namespace std;

vector<Complexx> iter_ftt(const vector<Complexx>& a, int n){
  int ext = 1;
  int pow = 0;
  while(ext < n){
    ext = ext << 1;
    pow++;
  }
  vector<Complexx> A(n);
  bit_rev_copy(a, A, n, pow);
  for(int s = 1; s <= pow; s++)
  {
    int m = 1 << s;
    Complexx w_m(cos(2.0 * (M_PI / (m * 1.0)) * M_PI / 180.0), sin(2.0 * (M_PI / (m * 1.0)) * M_PI / 180.0));
    for(int k = 0; k <= n - 1; k += m){
      Complexx w(1.0, 0.0);
      for(int j = 0; j <= m/2 - 1; j++){
        Complexx t = w * A[k + j + m / 2];
        Complexx u = A[k + j];
        A[k + j] = u + t;
        A[k + j + m / 2] = u - t;
        w = w * w_m;
      }
    }
  }
  return A;
}

void bit_rev_copy(const vector<Complexx>& a, vector<Complexx>& A, int n, int pow){
  int rev[n];
  for(int i = 0; i < n; i++)
    rev[i] = i;
  int higher = -1;
  for(int i = 1; i <= n - 1; i++){
    if((i & (i - 1)) == 0)
      higher++;
    rev[i] = rev[i ^ (1 << higher)];
    rev[i] |= (1 << (pow - higher - 1));
  }
  cout << endl;
  for(int i = 0; i < n; i++){
    A[rev[i]] = a[i];
  }
}


/*vector<Complexx> multiply(const vector<Complexx>& a1, const vector<Complex>& a2, int size_a1, int size_a2){
    int size = 1;
    vector<Complex> y1(a1), y2(a2);
    while (size < max(size_a1, size_a2))
        size = size << 1;
    size = size << 1;
    y1.resize(n);
    y2.resize(n);
    y1 = rec_ftt(y1);
    y2 = rec_ftt(y2);
    vector<Complexx> ans(size);
    for (int i = 0; i < size; i++)
        ans[i] = b1[i] * b2[i];
    ans = rev_rec_ftt(ans);
    for(int i = 0; i < n; i++){
      if(modul(ans[i]) < 0.000001){
        ans[i] = 0;
      }
    }
    return ans;
}*/
