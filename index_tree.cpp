#ifndef IINDEX_TREE_CPP
#define IINDEX_TREE_CPP

#include "index_tree.h"
#include <algorithm>
#include <cassert>

template class ordered_tree<int>;

template<typename T>
void ordered_tree<T>::updateWt(ordered_tree<T>::ptr_type tn){
        int w1 = 0, w2 = 0;
        if(tn->left) w1 = tn->left->wt;
        if(tn->right) w2 = tn->right->wt;
        tn->wt = w1 + w2 + 1;
}
template<typename T>
void ordered_tree<T>::updateBal(ordered_tree<T>::ptr_type tn){
       if(tn->left)  tn->hl = std::max(tn->left->hl, tn->left->hr)+1;
       else tn->hl = 0;
       if(tn->right) tn->hr = std::max(tn->right->hl,tn->right->hr)+1;
       else tn->hr = 0;  
}
template<typename T>
void ordered_tree<T>::update(ordered_tree<T>::ptr_type node){
        updateWt(node);
        updateBal(node);
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::LL(ordered_tree<T>::ptr_type parent){
    ptr_type tn = parent->left;
    parent->left = tn->right;
    tn->right = parent;
    update(parent);
    update(tn);
    return tn;
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::RR(ordered_tree<T>::ptr_type parent){
    ptr_type tn = parent->right;
    parent->right = parent->right->left;
    tn->left = parent;
    update(parent);
    update(tn);
    return tn;
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::RL(ordered_tree<T>::ptr_type parent){
    parent->right = LL(parent->right);
    return RR(parent);
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>:: LR(ordered_tree<T>::ptr_type parent){
    parent->left = RR(parent->left);
    return   LL(parent);
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::updateRot(ordered_tree<T>::ptr_type node){
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
template<typename T>
typename ordered_tree<T>::size_type ordered_tree<T>::_height(ordered_tree<T>::ptr_type root) const{
    if(root==nullptr)  return -1;
    return std::max(_height(root->left),_height(root->right))+1;
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::_insert(ordered_tree<T>::ptr_type node, const ordered_tree<T>::value_type& val, ordered_tree<T>::size_type pos){
    if(node==nullptr) {
        node = new ordered_tree<T>::node(val,1,0,0);
        return node;
    }
    int leftwt = 0;
    if(node->left)  leftwt = node->left->wt;
    if(leftwt + 1 == pos) {
        ptr_type tmp = node->left;
        int w=1, hl = 0;
        if(tmp){
            w = tmp->wt+1;
            hl = tmp->hl+1;
        }
        node->left = new ordered_tree<T>::node(val,w,hl,0);
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
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::leftmostParent(ordered_tree<T>::ptr_type node) const{
    if(node->left->left == nullptr) return node;
    leftmostParent(node->left);
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::_delete(ordered_tree<T>::ptr_type node, ordered_tree<T>::size_type pos){
    assert(node!=nullptr);
    int lft = 0;
    if(node->left) lft = node->left->wt;
    if(pos == lft+1){
        if(!node->right){  // no right child
           ptr_type t = node->left;
            delete node;
            return t;
        }
        else if(node->right->left == nullptr){  //right with no left child 
            node->right->left = node->left;
            ptr_type t = node->right;
            delete node;
            update(t);
            t = updateRot(t);
            return t;
        }
        else{    // right child with left child
            ptr_type t = leftmostParent(node->right);
            ptr_type tt = t->left;
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
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::get(ordered_tree<T>::ptr_type node,ordered_tree<T>::size_type i){
    assert(node!=nullptr);
    int lft = 0;
    if(node->left) lft = node->left->wt;
    if(i == lft+1) return node;
    else if(i < lft+ 1) return get(node->left,i);
    else return get(node->right,i-lft-1);
}
template<typename T>
ordered_tree<T>::ordered_tree(){
    root = nullptr;
    sz = 0;
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::begin(){
    return root;
}
template<typename T>
typename ordered_tree<T>::ptr_type ordered_tree<T>::end(){
    return nullptr;
}
template<typename T>
void ordered_tree<T>::insert(ordered_tree<T>::size_type pos, ordered_tree<T>::value_type val){
    pos += 1;
    size_type size = sz;
    assert(pos>0 && pos <= size+1 );
    root = _insert(root,val,pos);
    sz++;
}
template<typename T>
void ordered_tree<T>::remove(ordered_tree<T>::size_type pos){
    pos += 1;
    size_type size = sz;
    assert(pos>0 && pos <=size );
    root = _delete(root,pos);
    sz -- ;
}
template<typename T>
typename ordered_tree<T>::size_type ordered_tree<T>::height() const{
    return _height(root);
}
template<typename T>
typename ordered_tree<T>::size_type ordered_tree<T>::size() const{
    return sz;
}
template<typename T>
typename ordered_tree<T>::value_type& ordered_tree<T>::operator[](ordered_tree<T>::size_type pos){
    pos += 1;
    size_type size = sz;
    assert(pos>0 && pos <=size );
    return get(root,pos)->val;
}

#endif
