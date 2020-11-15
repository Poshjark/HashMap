#include "hashtable.h"
#include <iostream>
#include <unordered_map>


int main() {
    HashTable<string, string> myhash = HashTable<string, string>();
    auto sec = HashTable<int,int>();
    sec.add(12,45);
    cout << sec[12] << endl;
    cout << sec[1] << endl;
    auto elem = sec[0];
    cout << elem << endl;
    for (auto i=0; i < 1000; i++){
        sec[i] = (i-10)*i;
        if (i >990){
            cout << i << endl;
        }
    }

    return 0;
}
