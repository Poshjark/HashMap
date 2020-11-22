#include "hashtable.h"
#include <iostream>



int main() {
    HashTable<int,int> a;
    for(int i = 0;i<10;i++){
        a[i] = i*i;
    }


    std::cout << "\nIterating with while()\n";
    auto it = a.begin();
    while(it != a.end()){
        auto elem = *it;
        std::cout << "Key = " << elem.first << "  Value = " << elem.second << "\n";
        it++;
    }

    std::cout << "\nIterating with range based for\n";
    for(auto elem:a){
        std::cout << "Key = " << elem.first << "  Value = " << elem.second << "\n";
    }

    if (a.delete_(1)){
        std::cout << "Pair with key 1 deleted\n";
    }


    return 0;
}
