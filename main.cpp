#include <iostream>
#include "index_tree.h"

using namespace std;

int main() {
    int val,pos,c=1;
    int i=1024;
    ordered_tree<int> is;
    srand(clock());
    while(i--){
        val = rand()%200;
        pos = rand()%c;
         is.insert(pos,val);
        c++;
    }    
//    for(int i=0; i<is.size(); ++i) cout<<is[i]<<" ";
    cout<<"\nheight of the tree = "<<is.height()<<endl;
    return 0;
}

