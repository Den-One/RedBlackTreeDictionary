#pragma once

#include "RedBlackTree.hpp"

namespace dwt { // doctor web task

    template<typename Key, typename Value>
    class Dictionary {
    public:
        virtual ~Dictionary() = default;

        virtual const Value& get(const Key& key) const = 0;
        virtual void set(const Key& key, const Value& value) = 0;
        virtual bool isSet(const Key& key) const = 0;
    };

    template<typename Key, typename Value>
    class Map : public Dictionary<Key, Value>, public RedBlackTree<Key, Value> {
    public:
        Map() : Dictionary<Key, Value>(), RedBlackTree<Key, Value>() {}

        const Value& get(const Key& key) const override {
            try {
                auto res = this->find(key);
                return res->value();
            }
            catch (NotFoundKeyException<int>& e) {
                std::cout << "Key " << e.getKey() << " isn't found" << std::endl;
                //throw e;
            }
        }

        const Value& operator[](const Key& key) {
            return get(std::move(key));
        }

        void set(const Key& key, const Value& value) override {
            if (isSet(key)) {
                this->deleteNode(key);
                this->insert(key, value);
            }
            else {
                this->insert(key, value);
            }
        }

        bool isSet(const Key& key) const override {
            return (this->find(key) == this->_null) ? false : true;
        }

        ~Map() {

        }
    };

} // namespace dwt