#include <vector>
#include <utility>
#include <forward_list>
#include <string>

using namespace std;

template <typename Key,
    typename Value>
    class HashTable final {
    private:
        static constexpr size_t default_size = 1000;
        size_t current_size = default_size;
        size_t number_of_elements = 0;
        size_t length_of_forward_list(size_t hash){
            size_t num = 0;
            for(auto elem:*buckets[hash]){
                num++;
            }
            return num;
        }

        unsigned long long hash_function(string key) {
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
                    }
                }

                else {
                    delete buckets[hash];
                    number_of_elements--;
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


