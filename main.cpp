#include "hashtable.h"
#include <iostream>


int main() {
    HashTable<string, string> myhash = HashTable<string, string>();
    myhash.add("ad"s, "ad"s);
    auto b = myhash["ad"s];
    cout << b << endl;
    myhash.delete_("ad"s);
    auto c = myhash["q"];
    myhash["q"] = "asd";
    cout << myhash["q"] << endl;
    auto sec = HashTable<int,char>();
    sec.add(12,'c');
    cout << sec[12] << endl;
    return 0;
}
