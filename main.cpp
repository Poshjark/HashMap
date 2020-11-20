#include "hashtable.h"
#include <iostream>
#include <unordered_map>
#include <iomanip>



int main() {
    HashTable<int,int> dict;
    dict[0] = 0;
    dict[1] = 1;
    dict[100] = 100;
    dict[120] = 120;
    std:: cout <<"\nManual iterating through the dict with .next() method: \n";

    auto it_begin = dict.begin();
    std::cout << "key = " << (*it_begin).first << ", value = " << (*it_begin).second  << "\n";
    for(size_t i = 1;i<dict.number();i++){
        it_begin.next();
        std::cout << "key = " << (*it_begin).first << ", value = " << (*it_begin).second  << "\n";
    }

    std:: cout <<"\nIterating through the dict with while() loop: \n";
    auto it = dict.begin();
    auto end = dict.end();
    while(it!=dict.end()){
        std::cout << "key = " << (*it).first << ", value = " << (*it).second  << "\n";
        it++;
    }

    std:: cout <<"\nIterating through the dict with range-based for() loop: \n";
    for(auto p:dict){
        std::cout << "key = " << p.first << ", value = " << p.second  << "\n";
    }

    HashTable<const int, int> ss;
    std::forward_list<int> s;
    s.emplace_front(std::move(int(1)));

    return 0;
}
