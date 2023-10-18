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

    template<typename Key, typename Value, typename Comp = std::less<Key>>
    class Map final : public Dictionary<Key, Value>, public RedBlackTree<Key, Value, Comp> {
    public:
        Map() : Dictionary<Key, Value>(), RedBlackTree<Key, Value>() {}

        const Value& get(const Key& key) const override {
            auto res = this->find(key);
            if (res == this->_null) {
                throw NotFoundKeyException<Key>(key);
            }
            return res->value();
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