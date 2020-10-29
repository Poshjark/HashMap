#include <iostream>
#include "hashtable.h"
#include <cstddef>
#include <bitset>
#include <functional>

using namespace std;

int main(){

    std::string s = "asdasda123fgsdhgdfhsgfhsdfhsdhsdfsdfhd";
    auto h = hash<string>{}(s);
    cout << h << endl;
}
