#include<iostream>
#include<vector>
#include<fstream>
#include"rec_ftt.h"
#include"complexx.h"
#include"iter_ftt.h"
#include"rev_rec_ftt.h"
using namespace std;

vector<Complexx> multiply(const vector<Complexx>& a1, const vector<Complexx>& a2, int size_a1, int size_a2){
    int size = 1;
    vector<Complexx> b1(a1), b2(a2);
    while (size < max(size_a1, size_a2))
        size = size << 1;
		size = size << 1;
		cout << size << endl;
    b1.resize(size);
    b2.resize(size);
    vector <Complexx> y1 = rec_ftt(b1, size);
    vector <Complexx> y2 = rec_ftt(b2, size);
    vector<Complexx> ans(size);
    for (int i = 0; i < size; i++)
        ans[i] = y1[i] * y2[i];
    vector <Complexx> ans1 = rev_rec_ftt(ans, size);
    for(int i = 0; i < size; i++){
      if(ans1[i].modul() < 0.000001){
        ans1[i] = Complexx(0, 0);
      }
    }
    return ans1;
}

int main(int argc, char** argv){
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);
	vector<Complexx> y1, y2;
  int size_a, size_b;
  fin >> size_a;
  vector<Complexx> a, b;
  for(int j = 0; j < size_a; j++){
    double d;
    fin >> d;
    a.push_back(Complexx(d, 0));
  }
  //fout << endl;
	fin >> size_b;
	for(int j = 0; j < size_b; j++){
    double d;
    fin >> d;
    b.push_back(Complexx(d, 0));
  }
	//fout << endl;
	vector<Complexx> ans = multiply(a, b, size_a, size_b);
	for(int i = 0; i < ans.size(); i++){
		fout << ans[i] << " ";
	}
	fin.close();
	fout.close();
	return 0;
  }
