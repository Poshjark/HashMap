#include "hashtable.h"
#include <iostream>
#include <string>



int main() {
    HashTable<std::string,int> a;
    a.insert_or_assign("TWO",2);
    a["ONE"] = 1;
    a["FIVE"] = 5;
    const auto it = a.find("THREE");
    if(it != a.end()){
        std:: cout << "Key = " << it->first << " Value = " << (*it).second << "\n";
    }
    else{
        std::cerr << "No such key!\n";
    }
    a["zero"];
    auto iter = a.begin();
    while(iter!=a.end()){
        std:: cout << "Key = " << iter->first << " Value = " << (*iter).second << "\n";
        iter++;
    }
    std::cout << "\n";
    a.insert_or_assign("TWO", 100);
    for (auto pair:a){
        std:: cout << "Key = " << pair.first << " Value = " << pair.second << "\n";
    }
    return 0;
}
