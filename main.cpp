#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct TreeNode{
    TreeNode *parent,*left,*right;
    int wt, hl, hr;
    T val;
    TreeNode(T x, int w, int l, int r): parent(NULL),left(NULL),right(NULL),wt(w),hl(l),hr(r),val(x){}
};
template <typename T>
class ordered_tree
{
    TreeNode<T> *root;
    int sz;
    void updateWt(TreeNode<T>* tn){
        int w1 = 0, w2 = 0;
        if(tn->left) w1 = tn->left->wt;
        if(tn->right) w2 = tn->right->wt;
        tn->wt = w1 + w2 + 1;
    }
    void updateBal(TreeNode<T>* tn){
       if(tn->left)  tn->hl = max(tn->left->hl, tn->left->hr)+1;
       else tn->hl = 0;
       if(tn->right) tn->hr = max(tn->right->hl,tn->right->hr)+1;
       else tn->hr = 0;
    }
    void update(TreeNode<T>* node){
        updateWt(node);
        updateBal(node);
    }
    TreeNode<T>* LL(TreeNode<T>* parent){
        TreeNode<T>* tn = parent->left;
        parent->left = tn->right;
        tn->right = parent;
        update(parent);
        update(tn);
        return tn;
    }
    TreeNode<T>* RR(TreeNode<T>* parent){
        TreeNode<T>* tn = parent->right;
        parent->right = parent->right->left;
        tn->left = parent;
        update(parent);
        update(tn);
        return tn;
    }
    TreeNode<T>* RL(TreeNode<T>* parent){
        parent->right = LL(parent->right);
        return RR(parent);
    }
    TreeNode<T>* LR(TreeNode<T>* parent){
        parent->left = RR(parent->left);
        return   LL(parent);
    }
    TreeNode<T>* updateRot(TreeNode<T>* node){
        if(node->hl > node->hr+1){
             if(node->left->hr > node->left->hl) node = LR(node);
             else node = LL(node);
        }
        else if(node->hr > node->hl+1){
            if(node->right->hl > node->right->hr) node = RL(node);
            else node = RR(node);
        }
        return node;
    }
    int _height(TreeNode<T>* root,int h=0){
        if(root==NULL)  return h-1;
        return max(_height(root->left),_height(root->right))+1;
    }
    TreeNode<T>* _insert(TreeNode<T>* node, const T& val, int pos){
        if(node==NULL) {
            node = new TreeNode<T>(val,1,0,0);
            return node;
        }
        int leftwt = 0;
        if(node->left)  leftwt = node->left->wt;
        if(leftwt + 1 == pos) {
            TreeNode<T>* tmp = node->left;
            int w=1, hl = 0;
            if(tmp){
                w = tmp->wt+1;
                hl = tmp->hl+1;
            }
            node->left = new TreeNode<T>(val,w,hl,0);
            node->left->left = tmp;
        }
        else if( pos > leftwt + 1){
            node->right = _insert(node->right,val,pos-leftwt-1);
        }
        else{
           node->left =  _insert(node->left,val,pos);
        }
          update(node);
          node = updateRot(node);
          return node;
    }
    TreeNode<T>* leftmostParent(TreeNode<T>* node){
        if(node->left->left == NULL) return node;
        leftmostParent(node->left);
    }
    TreeNode<T>* _delete(TreeNode<T>* node, int pos){
        assert(node!=NULL);
        int lft = 0;
        if(node->left) lft = node->left->wt;
        if(pos == lft+1){
//            no right child
            if(!node->right){
                TreeNode<T>* t = node->left;
                delete node;
                return t;
            }
//             right with no left child 
            else if(node->right->left == NULL){
                node->right->left = node->left;
                TreeNode<T>* t = node->right;
                delete node;
                update(t);
                t = updateRot(t);
                return t;
            }
            // right child with left child
            else{
                TreeNode<T>* t = leftmostParent(node->right);
                TreeNode<T>* tt = t->left;
                t->left = tt->right;
                tt->left = node->left;
                tt->right = node->right;
                delete node;
                
                update(t);
                update(tt);
                t = updateRot(t);
                tt = updateRot(tt);
                return tt;
            }
        }
        else if(pos < lft+1)  node->left = _delete(node->left,pos);
        else node->right = _delete(node->right,pos-lft-1); 
        
        update(node);
        node = updateRot(node);
        return node;
    }
    TreeNode<T>* get(TreeNode<T>* node, int i){
        assert(node!=NULL);
        int lft = 0;
        if(node->left) lft = node->left->wt;
        if(i == lft+1) return node;
        else if(i < lft+ 1) return get(node->left,i);
        else return get(node->right,i-lft-1);
    }
    
//    exposed interfaces
    public:
        class iterator{
        TreeNode<T>* cur;
        TreeNode<T>* nxt;
        stack<TreeNode<T>* > stk;
        void left_iter(){
            while(cur){
                stk.push(cur);
                cur = cur->left;
            }
        }
    public:
        iterator(){
            cur = NULL;
            nxt = NULL;
        }
        iterator(TreeNode<T>* t){
            cur = t;
            next();
        }
        void operator=(TreeNode<T>* t){
            cur = t;
            next();
        }
        void next(){
            left_iter();
            TreeNode<T>* t = stk.top();
            stk.pop();
            cur = t->right;
            nxt = t;
        }
        T& operator*(void){
            return nxt->val;
        }
        TreeNode<T>* operator++(int){
            if(stk.empty() && !cur) { nxt = NULL; return NULL; }
            next();
            return nxt;
        }
        bool operator == (TreeNode<T>* t){
            return nxt == t;
        }
        bool operator != (TreeNode<T>* t){
            return nxt != t;
        }
    };
        ordered_tree(){
            root = NULL;
            sz = 0;
        }
        TreeNode<T>* begin(){
            return root;
        }
        TreeNode<T>* end(){
            return NULL;
        }
        void insert(int pos, T val){
            int size = sz;
            assert(pos>0 && pos <= size+1 );
            root = _insert(root,val,pos);
            sz++;
        }
        void remove(int pos){
            int size = sz;
            assert(pos>0 && pos <=size );
            root = _delete(root,pos);
            sz -- ;
        }
        int height(){
            return _height(root);
        }
        int size(){
            return sz;
        }
        T& operator[](int pos){
            int size = sz;
            assert(pos>0 && pos <=size );
            return get(root,pos)->val;
        }
};

int main() {
     int val,pos,c=1;
    int i=100;
    ordered_tree<pair<int,int>> is;
    pair<int,int> pr;
    srand(clock());
    while(i--){
        val = rand()%200;
        pos = 1+rand()%c;
         is.insert(pos,make_pair(0,0));
         is[pos] = make_pair(val-1,val);
        c++;
    }    
    ordered_tree<pair<int,int>>::iterator it ;
    for(it=is.begin();it!=is.end(); it++) cout<<(*it).first<<" "; 
    cout<<endl;
    for(int i=1; i<=is.size(); ++i) cout<<is[i].first<<" ";
    cout<<"\nheight of the tree = "<<is.height()<<endl;
    return 0;
}

