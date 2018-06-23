# indexed_tree
A sequence container that supports O(log n) indexing, O(log n) insertion, O(logn n) deletion, O(log n) search and O(n) iteration,

USAGE :

initialization-

ordered_tree<int> ot;


Insertion -

ot.insert(pos,val)

ot.push_back(val) is equivalent to ot.insert(ot.size(),val)

ot.push_front(val) is equivalent to ot.insert(1,val)


Deletion-

ot.remove(pos)


Getter and setter (indexing)-

x = ot[pos]

ot[pos] = new_val


Height of tree-

h = ot.height()


size of container-

s = ot.size()


iteration -

don't use getter/setter for iteration, instead use iterator

ordered_tree<int>:: iterator it = ot.begin();

for(;it!=ot.end();it++) cout<<*it<<" ";
