#pragma once

#include <exception>

namespace dwt { // doctor web task

    template<class Key>
    class NotFoundException : public std::exception {
    public:
        virtual const Key& getKey() const noexcept = 0;
    };

    template<class Key>
    class NotFoundKeyException : public NotFoundException<Key> {
        Key _key;

    public:
        NotFoundKeyException(const Key& key) : NotFoundException<Key>(), _key{ key } {}

        const Key& getKey() const noexcept {
            return _key;
        }
    };

} // namespace dwt