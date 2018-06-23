#ifndef IINDEX_TREE_H
#define IINDEX_TREE_H

namespace itree_impl{
    
template <typename T>
struct TreeNode{
    TreeNode *left,*right;
    int wt, hl, hr;
    T val;
    TreeNode(T x, int w, int l, int r): left(nullptr),right(nullptr),wt(w),hl(l),hr(r),val(x){}
};

template <typename T> using ptr_type = TreeNode<T>*;
template <typename T> using node_type = TreeNode<T>;
template <typename T> using value_type = T;
using size_type = int;

}

template<typename T>
class ordered_tree
{
    using node_type = itree_impl::node_type<T>;
    using   ptr_type =  itree_impl::ptr_type<T>;
    using  size_type = itree_impl::size_type;
    using value_type = itree_impl::value_type<T>;
    typedef node_type node;
   
    ptr_type root;
    size_type sz;
    
    void updateWt(ptr_type );
    void updateBal(ptr_type);
    void update(ptr_type node);
    ptr_type LL(ptr_type);
    ptr_type RR(ptr_type );
    ptr_type RL(ptr_type );
    ptr_type LR(ptr_type );
    ptr_type updateRot(ptr_type);
    size_type _height(ptr_type ) const;
    ptr_type _insert(ptr_type, const value_type&, size_type);
    ptr_type leftmostParent(ptr_type ) const;
    ptr_type _delete(ptr_type , size_type );
    ptr_type get(ptr_type, int );
    
    //    exposed interfaces
    public:
        ordered_tree();
        ptr_type begin();
        ptr_type end();
        void insert(size_type, value_type);
        void remove(size_type);
        size_type height() const;
        size_type size() const;
        value_type& operator[](size_type);
};


#endif /* IINDEX_TREE_H */
