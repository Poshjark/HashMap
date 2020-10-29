#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <forward_list>

template <typename Value>
struct Allocator
{
    [[nodiscard]] std::vector<std::forward_list<Value>*>* allocate(const size_t size){
        return new std::vector<std::forward_list<Value>*>[size];
    };
    void deallocate(std::vector<std::forward_list<Value>*>* data){
        delete[] data;
    };
};


template <typename Key,
          typename Value>
class HashTable final : private Allocator<Value> {
private:
    static const std::size_t default_size = 1000;
    std::vector<std::forward_list<Value>*>* buckets;
public:
//    HashTable(size_t def_size = default_size);
//    ~HashTable<Key,Value>(){
//        deallocate(this->buckets);
//    };
    void add(Key key, Value value);
    void find(Key key);
    void delete_(Key key,[[maybe_unused]] Value value);
};

template <typename Key> size_t hash_function(Key key);


