#include <vector>
#include <utility>
#include <forward_list>
#include <string>
#include <iostream>

#define DEBUG false

void debug_text(std::string_view str){
    if(DEBUG){
        std::cout << str <<"\n";
    }
}

struct CanNotAccessNullptr{};

/**
 * @class HashTable
 * @brief STL-compatible HashMap
 */
template <typename Key,
    typename Value>
    class HashTable final {

        friend class iterator;
        using container_type = std::vector< std::forward_list< std::pair<const Key, Value> >* >;
        using bucket_type = std::forward_list<std::pair<const Key, Value>>;
        using bucket_type_ptr = std::forward_list< std::pair<const Key, Value> >* ;
        using mapped_type = Value;
        using key_type = Key;
        using const_key_type = const Key;
        static constexpr size_t default_size = 8;
        size_t current_size = default_size;
        size_t number_of_elements = 0;
        static constexpr double overload_factor = 0.75;
        double load_factor = 0;
        size_t first_pos = 0;
        size_t last_pos = 0;

        container_type buckets;

        constexpr size_t length_of_forward_list(bucket_type list){
            size_t num = 0;
            for(auto elem:list){
                num++;
            }
            return num;
        }

        void resize();

        void rehash();

        constexpr size_t hash_function(key_type key){
            return std::hash<key_type>::_Do_hash(key)%current_size;
        }

        constexpr bool key_exists(size_t hash) {
            return !(buckets[hash] == nullptr);
        }


        void last_first(size_t index){
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
        }

    public:

        HashTable():first_pos(0),last_pos(0) {
            for (size_t i = 0; i <= default_size; i++) {
                std::forward_list<std::pair<const_key_type, Value>>* p = nullptr;
                buckets.push_back(p);
            }
            if (DEBUG){
                std::cout << "HashMap is created. Size of table is " << current_size << "\n";
            }

        }
        HashTable(std::initializer_list<std::pair<key_type,mapped_type>> args){

        }

        ~HashTable(){
            for(auto elem:buckets){
                if(elem == nullptr){
                    delete elem;
                }
            }
            if (DEBUG){
                std::cout << "HashMap is destructed\n";
            }
        }

        void add(key_type key, mapped_type value) {
            size_t hash = hash_function(key);
            if(DEBUG){
                std::cout << std::setw(10);
                std::cout << "Adding a key...\n";
                std::cout << "key = " << key << "\tvalue = " << value << "\thash = " << hash << "\n";
            }
            std::pair<const_key_type, mapped_type> pair_to_insert = std::make_pair(key, value);
            if (!key_exists(hash)) {
                buckets[hash] = new bucket_type;
                buckets[hash]->insert_after(buckets[hash]->before_begin(),pair_to_insert);
            }
            else {
                buckets[hash]->insert_after(buckets[hash]->before_begin(), pair_to_insert);
            }
            last_first(hash);
            number_of_elements++;

            this->load_factor = double(this->number_of_elements) / double(this->current_size);
        }

        std::pair<const_key_type, mapped_type>* find(key_type key) {
            size_t index = hash_function(key);
            if (key_exists(index)) {
                auto it = buckets[index]->begin();
                do{
                    if (it->first == key){
                        return &(*it);
                    }
                    it++;
                }while(it != buckets[index]->end());
            }
            return nullptr;
        }

        void delete_(key_type key) {
            constexpr size_t hash = hash_function(key);
            if (key_exists(hash)) {
                if (length_of_forward_list(hash) > 1) {
                    bucket_type_ptr current_list = buckets[hash];
                    auto before = current_list->before_begin();
                    auto now = current_list->begin();
                    if (now->first == key) {
                        current_list->erase_after(before);
                        number_of_elements--;
                        this->load_factor = double(this->number_of_elements) / double(this->current_size);

                    }
                }

                else {
                    delete buckets[hash];
                    number_of_elements--;
                    this->load_factor = double(this->number_of_elements) / double(this->current_size);

                }
            }
        }

        Value&  operator[](key_type key){
            std::pair<const_key_type,Value> *pair = find(key);
            if (pair == nullptr){
                this->add(key,Value());
                pair = find(key);
            }
            return pair->second;
        }



        class iterator {
            using base_container_type = const HashTable<key_type,mapped_type>;
            using base_container_type_ptr = HashTable<key_type,mapped_type>*;
            using const_base_container_type_ref = const HashTable<key_type,mapped_type>&;
            using KeyVal_ptr = std::pair<const_key_type, mapped_type>*;



        public:
            const_base_container_type_ref parent;
            KeyVal_ptr ptr;
            size_t pos;
            iterator(base_container_type_ptr _parent, bool points_to_the_end = false):parent(*_parent),pos(parent.first_pos){
                if(DEBUG){
                    std::cout << "HashMap iterator created in bucket #" << pos << "\n";
                }
                if(!points_to_the_end){
                    auto it = parent.buckets[pos]->begin();
                    ptr = &*it;
                }
                else{
                    auto it = parent.buckets[pos]->end();
                    ptr = &*it;
                }



            }

            iterator(iterator& it, bool points_to_the_end = false)
                :
                  parent(it.parent),
                  pos(points_to_the_end
                      ? parent.last_pos
                      : it.pos),
                  ptr(it.ptr){
            }

            ~iterator(){
                debug_text("Iterator destructed\n");
            }
            auto constexpr operator*(){
                return *ptr;
            }
            auto constexpr operator++(){
                this->next();
                return this;
            };

            iterator  operator++(int){
                iterator temp = iterator(*this);
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

            void first(){
                ptr = &*parent.buckets[parent.first_pos]->begin();
            };

            void end(){
                ptr = &(*(parent.buckets[parent.last_pos])->end());
            };

            void constexpr next(){
                if(ptr==nullptr){
                    return;
                }
                for (std::size_t i =  pos; i <= parent.last_pos; i++){
                    if(parent.buckets[i] == nullptr){
                        continue;
                    }

                    if (i==pos){
                        auto it = parent.buckets[i]->begin();
                        auto end = parent.buckets[i]->end();
                        while (it!=end){
                            if ((*it == *ptr) && (it._Ptr->_Next != nullptr)){
                                pos = i;
                                ptr = &(it._Ptr->_Next->_Myval);
                                debug_text("Iterated in the same #"+std::to_string(pos) + " bucket(collision happened)");
                                return;
                            }
                            it++;
                        }
                    }

                    else{
                        auto it = parent.buckets[i]->begin();
                        ptr = &*it;
                        pos = i;
                        debug_text("Iterated to bucket #"+std::to_string(pos));
                        return;
                    }
                }
                pos++;
                this->end();
            }

            void constexpr prev();
            };


        iterator begin(){
            return iterator(this);
        }
        iterator end(){
            return iterator(this,true);
        }


        const size_t number(){
            return  size_t(number_of_elements);
        }

        const bucket_type& const_buckets(){
            return &buckets;
        }

    };
