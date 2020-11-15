#include <vector>
#include <utility>
#include <forward_list>
#include <string>
#include <concepts>
#include <type_traits>

template<typename T>
concept String_type = std::convertible_to<T,std::string>;




using namespace std;
/**
 * @class HashTable
 * @brief STL-compatible HashMap
 */
template <typename Key,
    typename Value,
          size_t size = 1000>
    class HashTable final {

        using container_type = std::vector< std::forward_list< std::pair<const Key, Value> >* >;
        using bucket_type = std::forward_list< std::pair<const Key, Value> >* ;
        using mapped_type = Value;
        using key_type = const Key;

    private:
        static constexpr size_t default_size = 1000;
        static constexpr size_t current_size = default_size;
        size_t number_of_elements = 0;
        static constexpr double overload_factor = 0.75;
        double load_factor = 0;

        container_type buckets;

        constexpr size_t length_of_forward_list(size_t hash){
            size_t num = 0;
            for(auto elem:*buckets[hash]){
                num++;
            }
            return num;
        }

        void resize(){
            this->current_size*=2;
            for(size_t i = buckets.size();i<current_size;i++){
                forward_list<std::pair<key_type, mapped_type>>* p = new forward_list<pair<key_type,Value>>;
                buckets.push_back(p);
            }
        };

        void rehash();

        constexpr size_t hash_function(key_type key){
            return std::hash<int>::_Do_hash(key)%current_size;
        }

//        template<String_type Key_type>
//        constexpr size_t hash_function(Key_type key) {
//            size_t size = (default_size == current_size) ? default_size : current_size ;
//            // Полиномиальное хеширование.
//            unsigned long long h = 0;
//            constexpr int k = 31;
//            for (auto character : key) {
//                int x = int(character - ' ' + 1);
//                h = h * k + x;
//                h %= size;
//            }
//            return h;
//        }

//        template<integral T>
//        constexpr size_t hash_function(T key){
//            unsigned long long hash = key*key;
//            const int i = 10;
//            do{
//               hash+=(key%i)*(key%i);
//               hash%=current_size;
//               key%=i;
//            }while(key >= 10);
//            return hash;
//        };

//        template<floating_point T>
//        constexpr size_t hash_function(T key){
//            return (unsigned)key%this->current_size;
//        };


        constexpr bool key_exists(size_t hash) {
            return !buckets[hash]->empty();
        }


        class iterator {
            pair<key_type, mapped_type>* ptr;
            iterator()=default;
            size_t pos;
            auto constexpr operator*(){
                return *ptr;
            }
            auto constexpr operator++(){
                this->next();
                return *this;
            };
            auto constexpr operator--(){
                this->prev();
            };

            constexpr bool operator==(iterator&& it){
                return this->ptr == it->ptr;
            }
            void first();
            void end();
            void constexpr next();
            void constexpr prev();
            };

    public:


        HashTable() {
            for (size_t i = 0; i <= default_size; i++) {
                forward_list<std::pair<key_type, Value>>* p = new forward_list<pair<key_type,Value>>;
                buckets.push_back(p);
            }
        }

        void add(key_type key, mapped_type value) {
            size_t hash = hash_function(key);
            pair<key_type, mapped_type> pair_to_insert = make_pair(key, value);
            if (!key_exists(hash)) {
                buckets[hash]->insert_after(buckets[hash]->before_begin(),pair_to_insert);
            }
            else {

                buckets[hash]->insert_after(buckets[hash]->begin(), pair_to_insert);
            }
            number_of_elements++;
            this->load_factor = this->number_of_elements / this->current_size;
        }

        pair<key_type, mapped_type>* find(key_type key) {
            const size_t index = hash_function(key);
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
            const size_t hash = hash_function(key);
            if (key_exists(hash)) {
                if (length_of_forward_list(hash) > 1) {
                    forward_list<pair<key_type, mapped_type>>* current_list = buckets[hash];
                    auto before = current_list->before_begin();
                    auto now = current_list->begin();
                    if (now->first == key) {
                        current_list->erase_after(before);
                        number_of_elements--;
                        this->load_factor = this->number_of_elements / this->current_size;

                    }
                }

                else {
                    delete buckets[hash];
                    number_of_elements--;
                    this->load_factor = this->number_of_elements / this->current_size;

                }
            }
        }

        Value&  operator[](key_type key){
            pair<key_type,Value> *pair = find(key);
            if (pair == nullptr){
                this->add(key,Value());
                pair = find(key);
            }
            return pair->second;
        }

        size_t size(){
            return number_of_elements;
        }


    };
