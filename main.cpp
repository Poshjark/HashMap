#include <vector>
#include <utility>
#include <functional>
#include <forward_list>
#include <bitset>
#include <iostream>

using namespace std;

template <typename Key,
          typename Value>
class HashTable final {
private:
    static constexpr size_t default_size = 1000;
    static constexpr size_t current_size = default_size;
    size_t number_of_elements = 0;

    std::vector<std::forward_list<std::pair<Key, Value>>> buckets;
    unsigned long hash_function(string key, const int size=current_size){
        // Полиномиальное хеширование.
        unsigned long long h=0;
        static constexpr int k = 31;
        for (auto character:key){
            int x = int(character - ' ' + 1);
            h = h * k + x;
            h %= size;
        }
        return h;
    }

public:
    void add(const Key key, Value value){
        size_t index = hash_function(key, current_size);
        buckets[index].push_front(pair<Key,Value>(key,value));

//        if(!buckets[index].empty()){
//            for(auto key_value:buckets[index]){
//                if (key_value.first==key){
//                    key_value.second = value;
//                    break;
//                }
//            }
//            buckets[index].push_front(pair<Key,Value>(key,value));
//        }
//        else{
//            buckets[index].push_front(pair<Key,Value>(key,value));
//        }
    }
    pair<const Key, Value>* find(Key key){
        constexpr size_t index = hash_function(key);
        if (!buckets[index].empty()){
            for (auto key_value:buckets[index]){
                if (key_value.first == key){
                    return *key_value;
                }
            }
        }
        return nullptr;
    }
    void delete_(const Key key){
        const size_t index = hash_function(key);
        if (!buckets[index].empty()){
           auto it_before = buckets[index].before_begin();
           auto it = buckets[index].begin();
           while(it!=buckets[index].end()){
               if (it->first == key){
                   buckets[index].erase_after(it_before);
                   break;
               }
               it++;
               it_before++;
           }
        }
    }

};



int main(){
    HashTable<string,int> a;
    a.add("asd",2);
    return 0;
}
