#include <vector>
#include <utility>
#include <forward_list>
#include <string>
#include <concepts>
#include <type_traits>

template<typename T>
concept String_type = std::convertible_to<T,std::string>;


using namespace std;

template <typename Key,
    typename Value>
    class HashTable final {
    private:
        static constexpr size_t default_size = 1000;
        size_t current_size = default_size;
        size_t number_of_elements = 0;
        static constexpr double overload_factor = 0.75;
        double load_factor = 0;

        size_t length_of_forward_list(size_t hash){
            size_t num = 0;
            for(auto elem:*buckets[hash]){
                num++;
            }
            return num;
        }

        void resize(){
            this->current_size*=2;
            for(size_t i = buckets.size();i<current_size;i++){
                forward_list<std::pair<Key, Value>>* p = new forward_list<pair<Key,Value>>;
                buckets.push_back(p);
            }
        };

        void rehash();

        template<String_type Key_type>
        unsigned long long hash_function(Key_type key) {
            size_t size = (default_size == current_size) ? default_size : current_size ;
            // Полиномиальное хеширование.
            unsigned long long h = 0;
            static constexpr int k = 31;
            for (auto character : key) {
                int x = int(character - ' ' + 1);
                h = h * k + x;
                h %= size;
            }
            return h;
        }

        template<integral T>
        unsigned long long hash_function(T key){
            unsigned long long hash = key*key;
            const int i = 10;
            do{
               hash+=(key%i)*(key%i);
               hash%=current_size;
               key%=i;
            }while(key >= 10);
            return hash;
        };

        template<floating_point T>
        unsigned long long hash_function(T key){
            return (unsigned)key%this->current_size;
        };




        bool key_exists(size_t hash) {
            return !buckets[hash]->empty();
        }

    public:

        std::vector< std::forward_list< std::pair<Key, Value> >* > buckets;

        HashTable() {
            for (size_t i = 0; i <= default_size; i++) {
                forward_list<std::pair<Key, Value>>* p = new forward_list<pair<Key,Value>>;
                buckets.push_back(p);
            }
        }

        void add(const Key key, Value value) {
            size_t hash = hash_function(key);
            pair<Key, Value> pair_to_insert = make_pair(key, value);
            if (!key_exists(hash)) {
                buckets[hash]->insert_after(buckets[hash]->before_begin(),pair_to_insert);
            }
            else {

                buckets[hash]->insert_after(buckets[hash]->begin(), pair_to_insert);
            }
            number_of_elements++;
            this->load_factor = this->number_of_elements / this->current_size;
        }

        pair<Key, Value>* find(Key key) {
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

        void delete_(const Key key) {
            const size_t hash = hash_function(key);
            if (key_exists(hash)) {
                if (length_of_forward_list(hash) > 1) {
                    forward_list<pair<Key, Value>>* current_list = buckets[hash];
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

        Value&  operator[](Key key){
            pair<Key,Value> *pair = find(key);
            if (pair == nullptr){
                this->add(key,Value());
                pair = find(key);
                return pair->second;
            }
            else{
                return pair->second;
            }
        }

        size_t size(){
            return number_of_elements;
        }


    };


