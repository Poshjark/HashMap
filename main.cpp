#include "hashtable.h"
#include <iostream>
#include <unordered_map>


int main() {
    HashTable<int,int> dict;
    dict[0] = 0;
    dict[1] = 1;
    dict[100] = 100;
    dict[120] = 120;

    auto it_begin = dict.begin();
    int a = 1;
    std:: cout << (*it_begin).second << "\n";
    it_begin++;
    std:: cout << (*it_begin).second << "\n";
    it_begin++;
    std:: cout << (*it_begin).second << "\n";
    it_begin++;
    std:: cout << (*it_begin).second << "\n";


    std::forward_list<int> s;
    s.emplace_front(std::move(int(1)));
    return 0;
}
