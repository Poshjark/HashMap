#include "hashtable.h"
#include <iostream>
#include <unordered_map>
#include <forward_list>


int main() {
    HashTable<int,int> dict;
    dict[0] = 0;
    dict[1] = 1;
    dict[100] = 100;
    dict[120] = 120;

    auto it_begin = dict.begin();
    std:: cout << (*it_begin).second << "\n";
    it_begin++;
    std:: cout << (*it_begin).second << "\n";
    it_begin++;
    std:: cout << (*it_begin).second << "\n";
    it_begin++;
    std:: cout << (*it_begin).second << "\n";



    return 0;
}
