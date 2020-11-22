#include "hashtable.h"
#include <iostream>
#include <unordered_map>
#include <iomanip>



int main() {

    std::cout << '!';
    HashTable<int,int> a;
    for(int i = 0;i<90;i++){
        a[i] = i*i;
        std::cout << a[i] << "\n";
    }

    return 0;
}
