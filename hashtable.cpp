#include "hashtable.h"
#include <vector>
#include <forward_list>
#include <cstddef>

//template <typename Key, typename Value>
//HashTable<Key, Value>::HashTable(size_t){
//    this->buckets = allocate(def_size);
//}

//template <typename Key, typename Value> HashTable<Key, Value>::~HashTable(){
//    Allocator<Value>::deallocate(this->buckets);
//}


template <typename Key, typename Value>
void HashTable <Key, Value>::add(Key key, Value value){

}

template <typename Key, typename Value>
void HashTable <Key, Value>::find(Key key){

}


template <typename Key, typename Value>
void HashTable <Key, Value>::delete_(Key key, [[maybe_unused]] Value value){

}

template <typename Key> size_t hash_function(Key key){
    std::byte a;
}
