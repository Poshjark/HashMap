#pragma once

#include <vector>
#include <utility>
#include <forward_list>
#include <string>
#include <string_view>
#include <iostream>

#define DEBUG false


/**
 * @class HashTable
 * @brief STL-compatible HashMap
 */
template <typename Key,
    typename Value>
    class HashTable final {

        friend class iterator;
        using container_type = std::vector< std::forward_list< std::pair<const Key, Value> > >;
        using bucket_type = std::forward_list<std::pair<const Key, Value>>;
        using mapped_type = Value;
        using key_type = Key;
        using const_key_type = const Key;
        static constexpr size_t default_size = 100;
        size_t current_size = default_size;
        size_t number_of_elements = 0;
        static constexpr double overload_factor = 0.75;
        double load_factor = 0;
        size_t first_pos = 0;
        size_t last_pos = 0;
        size_t buckets_initiliazed  = 0;

        container_type buckets;

        constexpr size_t length_of_forward_list(const bucket_type& list){
            size_t num = 0;
            for(auto elem:list){
                num++;
            }
            return num;
        }

        void resize();

        void rehash();

        constexpr size_t hash_function(key_type key){
            return std::hash<key_type>()(key)%current_size;
        }

        bool hash_exists(size_t hash) {
            auto size = buckets.size();
            if(size==0){
                return false;
            }
            if(size - 1 >= hash){
                return !(buckets[hash].empty());
            }
            return false;
        }


        constexpr size_t find_pre_last_bucket(){
            for(size_t index = last_pos;index>=0;index--){
                if(!buckets[index].empty()){
                    return index;
                }
            }
            return 0;
        }

        void last_first(size_t index, bool deleting = false){
            if (this->number_of_elements==0){
                this->first_pos = index;
                this->last_pos = index;
                return;
            }
            if(index<first_pos){
                this->first_pos = index;
            }
            else if(index>last_pos){
                this->last_pos = index;
            }

            if(deleting && (index==last_pos)){
                last_pos = find_pre_last_bucket();
            }

        }


    public:

        class iterator {
            using base_container_type =  HashTable<key_type,mapped_type>;
            using base_container_type_ptr =  HashTable<key_type,mapped_type>*;
            using base_container_type_ref = HashTable<key_type,mapped_type>&;
            using KeyVal_ptr = std::pair<const_key_type, mapped_type>*;
            using key_val_pair = std::pair<const_key_type,mapped_type>;



        public:
            base_container_type_ref parent;
            KeyVal_ptr ptr;
            size_t pos;
            size_t pos_in_bucket;
            iterator(base_container_type_ptr _parent, bool points_to_the_end = false)
                :
                  parent(*_parent),
                  pos(points_to_the_end
                      ? parent.first_pos
                      : parent.last_pos){
                if(!points_to_the_end){
                    auto it = parent.buckets[pos].begin();
                    ptr = &(*it);
                }
                else{

                    auto it = parent.buckets[pos].end();
                    ptr = &*it;
#ifdef DEBUG
                    std::cout << "Iterator to the END created\n";
#endif
                }
            }

            iterator(base_container_type_ptr _parent,KeyVal_ptr _ptr, size_t hash, size_t _pos_in_bucket):
                  parent(*_parent),
                  pos(hash),
                  pos_in_bucket(_pos_in_bucket),
                  ptr(_ptr){}

            iterator(iterator& it, bool points_to_the_end = false):
                  parent(it.parent),
                  pos(points_to_the_end
                      ? parent.last_pos
                      : it.pos),
                  ptr(it.ptr){}


            iterator& operator=(iterator& right){
                this->ptr = right.ptr;
                this->pos = right.pos;
                this->pos_in_bucket = right.pos_in_bucket;
                return *this;
            }

            std::pair<const_key_type,mapped_type>& operator*(){
                return *ptr;
            }

            auto constexpr operator++(){
                this->next();
                return this;
            };

            iterator  operator++(int){
                iterator temp = iterator(std::move(*this));
                this->next();
                return temp;
            };

            auto constexpr operator--(){
                this->prev();
                return *this;
            };




            constexpr bool operator==(iterator second){
                return ptr == second.ptr;
            }


            auto constexpr operator->(){
                return this;
            }



            void constexpr next(){
                for(size_t i = pos;i<=parent.last_pos;i++){
                    if(!parent.buckets[i].empty()){
                        auto it = parent.buckets[i].begin();
                        auto end = parent.buckets[i].end();
                        while(it!=end){
                            if(*it == *ptr){
                                it++;
                                if(it!=parent.buckets[i].end()){
                                    pos = i;
                                    ptr = &*it;
                                    return;
                                }
                                else{
                                    break;
                                }
                            }
                            it++;
                        }
                    }
                }
            pos = parent.last_pos;
            ptr = nullptr;
            }


            void constexpr prev();
            };


        HashTable():first_pos(0),last_pos(0) {
            buckets.push_back(std::forward_list<std::pair<const_key_type,mapped_type>>());

#ifndef NDEBUG

#endif
        }



        void add(key_type key, mapped_type value) {
            size_t hash = hash_function(key);
            for (size_t _ = buckets.size();_<= hash;_++){
                buckets.push_back(std::forward_list<std::pair<const key_type, mapped_type>>());
            }
            buckets[hash].insert_after(buckets[hash].before_begin(),std::pair<const_key_type,mapped_type>(key,value));
            last_first(hash);
            number_of_elements++;

            this->load_factor = double(this->number_of_elements) / double(this->current_size);
        }



        bool delete_(key_type key){
            size_t index = hash_function(key);
            if(hash_exists(index)){
                auto before_it = buckets[index].before_begin();
                auto it = buckets[index].begin();
                while(it!=buckets[index].end()){
                    if((*it).first == key){
                        buckets[index].erase_after(before_it);
                        number_of_elements--;
                        if(buckets[index].empty()){
                            last_first(index,true);
                        }
                        break;
                    }
                    before_it++;
                    it++;
                }
            }
            return false;
        }

        mapped_type&  operator[](key_type key){
            iterator it = find(key);
            if(it == this->end()){
                this->add(key,mapped_type());
                it = iterator(this->find(key));
            }
            mapped_type& lref = (*it).second;
            return lref;
        }


        iterator find(key_type key) {
           size_t index = hash_function(key);
           if (hash_exists(index)){
               auto it = buckets[index].begin();
               while(it!=buckets[index].end())
                   if((*it).first == key){
                       return iterator(this,&(*it),index,0);
                   }
               it++;
           }
           return this->end();
       }



        iterator begin(){
            return iterator(this);
        }
        iterator end(){
            return iterator(this,true);
        }


        const size_t number(){
            return  size_t(number_of_elements);
        }

        const container_type const_buckets(){
            return buckets;
        }

    };
