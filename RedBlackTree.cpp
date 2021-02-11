#include <bits/stdc++.h>
using namespace std;
#define BLACK 1
#define RED 0
class RBTree{
    struct Node{
        double data;
        Node *left;
        Node *right;
        Node *parent;
        int color;
    };
private:
    Node *root;
    Node *nil;
public:
    RBTree(){
        nil = new Node;
        nil -> left = NULL;
        nil -> right = NULL;
        nil -> parent = NULL;
        nil -> data = 0.0;
        nil -> color = BLACK;
        root = nil;
        root -> parent = nil;
    }
    void DeleteTree(Node *r){
        if(r == nil){
            return;
        }
        DeleteTree(r->left);
        DeleteTree(r->right);
        delete r;
    }
    ~RBTree(){
        cout << "Destructor" << endl;
        DeleteTree(root);
    }
    Node *Find(const double & val){
        Node *z = root;
        while(z != nil && z-> data != val){
          if(val < z -> data){
            z = z -> left;
          }
          else{
            z = z -> right;
          }
        }
        return z;
      }
    Node *Min(Node *z){
        while(z -> left != nil){
            z = z -> left;
        }
        return z;
    }
    Node *Max(Node *z){
        while(z -> right != nil){
            z = z -> right;
        }
        return z;
    }
    void Left_Rotate(Node *x){
        if(x -> right == nil){
          return;
        }
        Node *y = x -> right;
        x -> right = y -> left;
        if(y -> left != nil){
            y -> left -> parent = x;
        }
        y -> parent = x -> parent;
        if(x -> parent == nil){
            root = y;
        }
        else if(x == x -> parent -> left){
          x -> parent -> left = y;
        }
        else{
          x -> parent -> right = y;
        }
        y -> left = x;
        x -> parent = y;
    }
    void Right_Rotate(Node *x){
        if(x -> left == nil){
          return;
        }
        Node *y = x -> left;
        x -> left = y -> right;
        if(y -> right != nil){
            y -> right -> parent = x;
        }
        y -> parent = x -> parent;
        if(x -> parent == nil){
            root = y;
        }
        else if(x == x -> parent -> left){
          x -> parent -> left = y;
        }
        else{
          x -> parent -> right = y;
        }
        y -> right = x;
        x -> parent = y;
    }
    void Insert(const double &val){
      Node *z = new Node;
      z -> data = val;
      z -> left = nil;
      z -> right = nil;
      z -> color = RED;
      Node *y = nil;
      Node *x = root;
      while(x != nil)
      {
        y = x;
        if(z -> data < x -> data)
          x = x -> left;
        else
          x = x -> right;
      }
      z -> parent = y;
      if(y == nil){
          root = z;
      }
      else if(z -> data < y -> data)
        y -> left = z;
      else
      y -> right = z;
      Insert_Fixup(z);
    //  cout << root -> data << endl;
    }
    void Insert_Fixup(Node *z){
      Node *y = nil;
      while(z -> parent -> color == RED){
        y = z -> parent -> parent -> left;
        if(y == z -> parent){
    //      cout << "left";
          y = z -> parent -> parent -> right;
          if(y -> color == RED){
            z -> parent -> color =  BLACK;
            y -> color = BLACK;
            z -> parent -> parent -> color = RED;
            z = z -> parent -> parent;
          }
          else{
            if(z == z -> parent -> right){
              z = z -> parent;
              Left_Rotate(z);
            }
            z -> parent -> color = BLACK;
            z -> parent -> parent -> color = RED;
            Right_Rotate(z -> parent -> parent);
          }
        }
        else{
      //    cout << "right ";
          if(y -> color == RED){
        //    cout << "case 1" << endl;
            z -> parent -> color =  BLACK;
            y -> color = BLACK;
            z -> parent -> parent -> color = RED;
            z = z -> parent -> parent;
          }
          else{
             if(z == z -> parent -> left){
          //     cout << "case 2" << endl;
               z = z -> parent;
               Right_Rotate(z);
             }
            // cout << "case 3" << endl;
             z -> parent -> color = BLACK;
             z -> parent -> parent -> color = RED;
            // cout << "check1 " << root -> data << " " << z -> parent -> parent -> parent -> data << endl;
             Left_Rotate(z -> parent -> parent);
             //cout << "check2 " << root -> data << endl;
          }
        }
        if(z == root){
          break;
        }
      }
      root -> color = BLACK;
    }
    void Tree_Print(Node *r){
        if(r != nil){
          Tree_Print(r -> left);
          cout << r -> data << " - " << r -> color << " ";
          Tree_Print(r -> right);
      }
    }
    void Print(){
        Tree_Print(root);
        cout << endl;
    }
    Node *Succesor(Node * x){
        if(x->right != nil){
          return Min(x->right);
        }
        Node *y = x -> parent;
        while(y != nil and x == y -> right){
          x = y;
          y = y -> parent;
        }
        return y;
    }
    Node *Predecessor(Node * x){
        if(x -> left != nil){
          return Max(x->left);
        }
        Node *y = x->parent;
        while(y != nil and x == y -> left)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }
    void Delete(Node * x){
        Node *y = nil;
        if(x -> left == nil || x -> right == nil){
            y = x;
        }
        else{
            y = Succesor(x);
        }
        Node *z = nil;
        if(y -> left != nil){
            z = y -> left;
        }
        else{
            z = y -> right;
        }
        z -> parent = y -> parent;
        if(y -> parent == nil){
            root = z;
        }
        else{
            if(y == (y -> parent) -> left){
                (y -> parent) -> left = z;
            }
            else{
                (y -> parent) -> right = z;
            }
        }
        if(y != x){
            x -> data = y -> data;
        }
        if(y -> color == BLACK)
          Delete_FixUp(z);
        delete y;
    }
    void Delete_FixUp(Node* n){
      if(n -> color == RED){
        n -> color = BLACK;
        return;
      }
      delete_case1(n);
    }
    void delete_case1(Node *n)
    {
	     if (n->parent != nil)
		     delete_case2(n);
    }
    void delete_case2(Node *n)
    {
    	Node *s = n -> parent -> left;
      if(n == s){
        s = n -> parent -> right;
      }
    	if (s -> color == RED) {
    		n -> parent -> color = RED;
    		s -> color = BLACK;
    		if (n == n -> parent -> left)
    			Left_Rotate(n->parent);
    		else
    			Right_Rotate(n->parent);
    	}
    	delete_case3(n);
    }
    void delete_case3(Node *n)
    {
    	Node *s = n -> parent -> left;
      if(s == n){
        s = n -> parent -> right;
      }
    	if ((n->parent->color == BLACK) &&
    	   (s->color == BLACK) &&
    	   (s->left->color == BLACK) &&
    	   (s->right->color == BLACK)) {
    		      s->color = RED;
    		delete_case1(n->parent);
    	}
      else
    		delete_case4(n);
    }
    void delete_case4(Node *n)
    {
    	Node *s = n -> parent -> left;
      if(s == n){
        s = n -> parent -> right;
      }
    	if ((n->parent->color == RED) &&
    	    (s->color == BLACK) &&
    	    (s->left->color == BLACK) &&
    	    (s->right->color == BLACK)) {
    		s->color = RED;
    		n->parent->color = BLACK;
    	} else
    		delete_case5(n);
    }
    void delete_case5(Node *n)
    {
	     Node *s = n -> parent -> left;
       if(s == n){
         s = n -> parent -> right;
       }
    	 if  (s->color == BLACK) {
    	  if ((n == n->parent->left) &&
    	      (s->right->color == BLACK) &&
    		    (s->left->color == RED)) {
    			s->color = RED;
    			s->left->color = BLACK;
    		  Right_Rotate(s);
    		} else if ((n == n->parent->right) &&
    		           (s->left->color == BLACK) &&
    		           (s->right->color == RED)) {
    	    s->color = RED;
    			s->right->color = BLACK;
    		  Left_Rotate(s);
        }
	    }
	 delete_case6(n);
  }
  void delete_case6(Node *n)
  {
    Node *s = n -> parent -> left;
    if(s == n){
      s = n -> parent -> right;
    }
    s -> color = n -> parent -> color;
    n -> parent -> color = BLACK;
	  if (n == n -> parent -> left) {
      s -> right -> color = BLACK;
      Left_Rotate(n -> parent);
	  } else {
		  s -> left -> color = BLACK;
		  Right_Rotate(n -> parent);
	  }
  }
  void Print_Node(Node * x){
        cout << x -> data << endl;
  }
  Node *Root(){
    return root;
  }
  void Print_with_levels(){
       queue<pair<Node*, int>> Q;
       Q.push({root, 0});
       int last = 0;
       while(!Q.empty()){
           pair<Node*, int> x = Q.front();
           Q.pop();
           if(last != x.second){
               last++;
               cout << endl;
           }
           if(x.first != nil)
               cout << x.first -> data << "-" << x.first -> color << " ";
           else
               cout << "nil ";
           if(x.first -> left != NULL)
               Q.push({x.first -> left, x.second + 1});
           if(x.first -> right != NULL)
               Q.push({x.first -> right, x.second + 1});
       }
       cout << endl;
   }
   bool Empty(){
    if(root == nil)
      return true;
    return false;
   }
};


int main(){
  RBTree a;
  int n;
  cin >> n;
  while(n--){
    double x;
    cin >> x;
    a.Insert(x);
  }
  a.Print_with_levels();
  while(!a.Empty()){
    double x;
    cin >> x;
    a.Delete(a.Find(x));
    a.Print_with_levels();
  }
  return 0;
}
