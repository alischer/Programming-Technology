#include<iostream>
#include<math.h>
#include<string.h>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;

ifstream fin;
ofstream fout;
struct Node{
  char symbol;
  int frequency;
  Node * left;
  Node * right;
};
class Priority_Queue_Min {
private:
  Node ** heap;
  int heap_size;
  int max_size;
  vector<pair<list<bool>, int>> cipher;
  void Sift_Up(int index){
    int p = index / 2;
    if(p == 0){
      return;
    }
    if(heap[index] -> frequency < heap[p] -> frequency){
      struct Node* temp = heap[p];
      heap[p] = heap[index];
      heap[index] = temp;
      Sift_Up(p);
    }
    return;
  }
  void Sift_Down(int index){
    int large = index, l = 2 * index, r = 2 * index + 1;
    if(l <= heap_size){
      if(heap[large] -> frequency > heap[l] -> frequency){
        large = l;
      }
    }
    if(r <= heap_size){
      if(heap[large] -> frequency > heap[r] -> frequency){
        large = r;
      }
    }
    if(large != index)
    {
      struct Node* temp = heap[large];
      heap[large] = heap[index];
      heap[index] = temp;
      Sift_Down(large);
    }
    return;
  }
public:
  void Insert(Node * new_elem);
  Priority_Queue_Min (unsigned long long* freq);
  Node *Extract_Min() ;
  Node *Build_Huffman_Tree();
  void Huffman_rec_code(Node *x, list<bool> last);
  pair<list<bool>, int> Cipher(int c);
  void print_cipher(int c);
  Node* Decipher(Node* x, list<bool>* deciph);
  void Tree_Delete(Node *r);
  ~Priority_Queue_Min();
};

void Priority_Queue_Min::Insert(Node * new_elem){
  if(heap_size == max_size) {
       cout << "You have exceed max size of your queue!" << endl;
       return;
   }
   heap_size++;
   heap[heap_size] = new_elem;
   Sift_Up(heap_size);
}

Priority_Queue_Min::Priority_Queue_Min(unsigned long long* freq){
  heap_size = 0;
  max_size = 1000000;
  heap = new Node*[max_size];
  cipher = vector<pair<list<bool>, int>>(257);
  for(int i = 0; i < 256; i++){
    if(freq[i] != 0){
        Node * new_elem = new Node;
        new_elem -> symbol = char(i - 128);
        new_elem -> frequency = freq[i];
        new_elem -> left = NULL;
        new_elem -> right = NULL;
        Insert(new_elem);
    }
  }
}

Node* Priority_Queue_Min:: Extract_Min(){
  if(heap_size == 0){
    cout << 0 << " ";
    return NULL;
  }
  struct Node* temp = heap[heap_size];
  heap[heap_size] = heap[1];
  heap[1] = temp;
  heap_size--;
  if(heap_size == 0){
    return heap[heap_size + 1];
  }
  else{
    Sift_Down(1);
  }
  return heap[heap_size + 1];
}

void Priority_Queue_Min::Huffman_rec_code(Node *x, list<bool> last){
  if(x -> left == NULL && x -> right == NULL){
    int symbol = int(x -> symbol) + 128;
    cipher[symbol].first = last;
    cipher[symbol].second = last.size();
    return;
  }
  if(x -> left != NULL){
    last.push_back(1);
    Huffman_rec_code(x -> left, last);
    last.pop_back();
  }
  if(x -> right != NULL)
  {
    last.push_back(0);
    Huffman_rec_code(x -> right, last);
    last.pop_back();
  }
  return;
}


Node* Priority_Queue_Min::Build_Huffman_Tree(){
  if(heap_size != 1){
    while(heap_size != 1){
      Node *ver1 = new Node;
      ver1 -> right = Extract_Min();
      ver1 -> left = Extract_Min();
      ver1 -> frequency = ver1 -> left -> frequency + ver1 -> right -> frequency;
      Insert(ver1);
    }
    list<bool> last;
    Huffman_rec_code(heap[1], last);
    return heap[1];
  }
  return heap[1];
}

pair<list<bool>, int>  Priority_Queue_Min::Cipher(int c){
  return cipher[int(c)];
}

Node* Priority_Queue_Min:: Decipher(Node* x, list<bool>* deciph){
  if(x -> left == NULL && x -> right == NULL){
    fout << x -> symbol;
    return heap[1];
  }
  if(deciph -> size() == 0){
    return x;
  }
  bool flag = deciph -> back();
  deciph -> pop_back();
  if(flag){
    x = x -> left;
    return Decipher(x, deciph);
  }
  x = x -> right;
  return Decipher(x, deciph);
}

void Priority_Queue_Min:: Tree_Delete(Node * r){
  if(r == NULL){
      return;
  }
  Tree_Delete(r->left);
  Tree_Delete(r->right);
  delete r;
}
Priority_Queue_Min::~Priority_Queue_Min(){
  Tree_Delete(heap[1]);
  delete [] heap;
}

int main(int argc, char** argv){
  fin.open(argv[1]);
  fout.open(argv[2]);
  char c;
  unsigned long long freq[257] = {0};
  while(fin.get(c)){
      freq[int(c) + 128]++;
  }
  Priority_Queue_Min a(freq);
  a.Build_Huffman_Tree();
  fin.close();
  fin.open(argv[1]);
  unsigned int c1 = 0;
  pair<list<bool>, int> ciph;
  int counter = 0;
  while(fin.get(c)){
    ciph = a.Cipher(int(c) + 128);
    int l = 0;
    list<bool>:: iterator it = ciph.first.begin();
    for(; it != ciph.first.end(); it++){
      c1 = c1 | *it;
      counter++;
      if(counter == 8){
        counter = 0;
        unsigned char s = c1;
        fout << s;
        c1 = 0;
      }
      else
        c1 = c1 << 1;
    }
  }
  if(counter != 0){
    while(counter < 8){
      c1 = c1 << 1;
      counter++;
    }
    c1 = c1 >> 1;
    unsigned char s = c1;
    fout << s;
  }
  fin.close();
  fout.close();
  fin.open(argv[2]);
  fout.open(argv[3]);
  list<bool>* decipher = new list<bool>;
  Node *ver = a.Build_Huffman_Tree();
  while(fin.get(c)){
    bool ext = 1;
    unsigned char s = c;
    int c1 = int(s);
    int count = 0;
    while(count < 8){
      decipher -> push_back(c1 & ext);
      c1 = c1 >> 1;
      count++;
    }
    while(decipher -> size() != 0){
      ver = a.Decipher(ver, decipher);
    }
  }
  while(decipher -> size() != 0){
    ver = a.Decipher(ver, decipher);
  }
  return 0;
}




0000
