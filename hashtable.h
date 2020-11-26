#include <vector>
#include <utility>
#include <forward_list>
#include <functional>
#include <string>
#include <iostream>
#include <unordered_map>

/**
 * @class HashTable
 * @brief STL-compatible HashMap
 */



template <typename Key,
          typename Value>
    class HashTable final {
        friend class iterator;
        using Hash = std::hash<Key>;
        using container_type = std::vector< std::forward_list< std::pair<const Key, Value> > >;
        using bucket_type = std::forward_list<std::pair<const Key, Value>>;
        using key_value_pair = std::pair<const Key, Value>;
        using mapped_type = Value;
        using key_type = Key;
        using const_key_type = const Key;
        static constexpr size_t default_size = 100;
        size_t current_size = default_size;
        size_t number_of_elements = 0;
        size_t first_pos;
        size_t last_pos;

        container_type buckets = container_type(default_size);

        constexpr size_t hash_function(key_type key) const{
            return (Hash{}(key))%current_size;
        }

        bool hash_exists(size_t hash) const{
            return !buckets[hash].empty();
        }

        constexpr size_t find_pre_last_bucket() const{
            for(size_t index = last_pos;index>0;index--){
                if(!buckets[index].empty()){
                    return index;
                }
            }
            return 0;
        }

        constexpr size_t find_second_bucket() const{
            for(size_t index =first_pos+1;index <= last_pos;index++){
                if(!buckets[index].empty()){
                    return index;
                }
            }
            return last_pos;
        }

        void last_first(size_t index, bool deleting = false) noexcept{
            if (number_of_elements==0){
                first_pos = index;
                last_pos = index;
                return;
            }
            if(index<first_pos){
                this->first_pos = index;
            }
            else if(index>last_pos){

                this->last_pos = index;
            }
            if(!deleting){
                return;
            }

            if(deleting && (index==last_pos)){
                last_pos = find_pre_last_bucket();
            }

            else if(deleting && (index == first_pos)){
                first_pos = find_second_bucket();
            }

        }

          template<typename T_k, typename T_v>
        bool insert_pair(T_k&& key, T_v&& value) {
            size_t hash = hash_function(key);
            if(hash_exists(hash)){
                for(auto pair:buckets[hash]){
                    if (pair.first == key){
                        return false;
                    }
                    else{
                        buckets[hash].push_front(std::pair<const Key, Value>(key,value));
                        last_first(hash);
                        number_of_elements++;
                        return true;
                    }
                }
            }
            else{
                buckets[hash].push_front(std::pair<const Key, Value>(key,value));

                last_first(hash);
                number_of_elements++;
                return true;
            }
            return false;
        }


    public:

        class iterator {
            using base_container_type =  HashTable<key_type,mapped_type>;
            using base_container_type_ptr =  HashTable<key_type,mapped_type>*;
            using base_container_type_ref = HashTable<key_type,mapped_type>&;
            using KeyVal_ptr = std::pair<const_key_type, mapped_type>*;
            using key_val_pair = std::pair<const_key_type,mapped_type>;

            base_container_type_ref parent;
            KeyVal_ptr ptr;
            size_t pos;

            iterator()=default;

        public:

            iterator(base_container_type_ptr _parent, bool points_to_the_end = false)
                :
                  parent(*_parent),
                  pos(0){
                if(points_to_the_end){
                    pos = parent.last_pos;
                    auto it = parent.buckets[pos].end();
                    ptr = &(*it);

                }
                else{
                    pos = parent.first_pos;
                    auto it = parent.buckets[pos].begin();
                    ptr = &*it;
                }
            }

            iterator(base_container_type_ptr _parent,KeyVal_ptr _ptr, size_t hash, size_t _pos_in_bucket=0):
                  parent(*_parent),
                  pos(hash),
                  ptr(_ptr){}

            iterator(const iterator& it, bool points_to_the_end = false):
                  parent(it.parent),
                  pos(points_to_the_end
                      ? parent.last_pos
                      : it.pos),
                  ptr(it.ptr){}


            iterator(iterator&& rv_iter):parent(rv_iter.parent),pos(rv_iter.pos),ptr(rv_iter.ptr){}

            template<typename iter_type>
            iterator& operator=(iter_type&& right){
                this->ptr = right.ptr;
                this->pos = right.pos;
                this->pos_in_bucket = right.pos_in_bucket;
                return *this;
            }

            std::pair<const_key_type,mapped_type>& operator*() const{
                return *ptr;
            }

            iterator constexpr operator++(){
                this->next();
                return *this;
            };

            iterator  operator++(int){
                iterator temp = iterator(*this);
                this->next();
                return temp;
            };

            constexpr bool operator==(iterator&& second) const noexcept{
                return ptr == second.ptr;
            }

            constexpr bool operator!=(iterator second)const noexcept{
                return ptr != second.ptr;
            }


            KeyVal_ptr operator->() const{
                return &(this->operator*());;
            }

            void constexpr next(){
                for(size_t i = pos;i<=parent.last_pos;i++){
                    if(i==pos){
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
                    else{
                        if(!parent.buckets[i].empty()){
                            pos = i;
                            ptr = &*(parent.buckets[i].begin());
                            return;
                        }
                    }
                }
                pos = parent.last_pos;
                ptr = &(*(parent.buckets[parent.last_pos].end()));
            }
        };


        HashTable(size_t n = default_size) {
            buckets.resize(n);
        }

        HashTable(const HashTable<key_type,mapped_type>& right)
            :first_pos(right.first_pos),
              last_pos(right.last_pos),
              buckets(right.buckets),
              number_of_elements(right.number_of_elements){};

        HashTable(HashTable<key_type,mapped_type>&& right)
            :first_pos(right.first_pos),
              last_pos(right.last_pos),
              buckets(right.buckets),
              number_of_elements(right.number_of_elements){};



        template <typename T_k, typename T_v>
        std::pair<iterator,bool> insert_or_assign(T_k&& key, T_v&& value){
            bool was_inserted;
            size_t hash = hash_function(key);
            if(!insert_pair(key,value)){
                was_inserted = false;
                auto it = buckets[hash].begin();
                while(  it != buckets[hash].end() ){
                    if((*it).first  == key){
                        (*it).second = value;
                        key_value_pair* now_pair = &(*it);
                        iterator result_iter(this,now_pair,hash);
                        std::pair<iterator,bool> result_pair = std::make_pair(result_iter,was_inserted);
                        return result_pair;

                    }
                    it++;
                }
            }
            was_inserted = true;
            return std::make_pair(find(std::move(key)),was_inserted);
        }

        template<typename T_k>
        size_t erase(T_k&& key){
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
                        return 1;
                    }
                    before_it++;
                    it++;
                }
            }
            return 0;
        }
        template<typename T_k>
        mapped_type&  operator[](T_k&& key){
            iterator it = insert_or_assign(std::move(key),mapped_type()).first;
            mapped_type& lref = (*it).second;
            return lref;
        }


        template<typename T_k>
        iterator find(T_k&& key) {
           size_t index = hash_function(key);
           if (hash_exists(index)){
               auto it = buckets[index].begin();
               while(it!=buckets[index].end()){
                   if((*it).first == key){
                       return iterator(this,&(*it),index,0);
                   }
               it++;
               }
           }
           return this->end();
        }

        iterator begin() {
            return iterator(this,false);
        }

        iterator end() {
            return iterator(this,true);
        }

        const size_t size() noexcept{
            return  number_of_elements;
        }



        void rehash(size_t new_number_of_nodes=0){
            if(!new_number_of_nodes){
                new_number_of_nodes = this->buckets.size() * 2;
            }
            const container_type new_buckets = buckets;
            buckets.clear();
            buckets.resize(new_number_of_nodes);
            current_size = new_number_of_nodes;
            number_of_elements = 0;
            for(auto list:new_buckets){
                if(!list.empty()){
                    for(auto pair:list){
                        this->insert_pair(pair.first,pair.second);
                    }
                }
            }
        }

    };



