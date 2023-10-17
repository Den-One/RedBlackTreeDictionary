#pragma once

#ifdef DEBUG
#include <iostream>
#endif

#include <utility>

#include "NotFoundException.hpp"

namespace dwt { // doctor web task

    enum class Color : unsigned char {
        BLACK,
        RED,
    };

    template<typename Key, typename Value>
    struct TreeNode {
        std::pair<const Key, Value> _data;
        Color _color = Color::BLACK;

        TreeNode<const Key, Value>* _parent = nullptr;
        TreeNode<const Key, Value>* _left = nullptr;
        TreeNode<const Key, Value>* _right = nullptr;

        TreeNode() = default;

        TreeNode(const Key key, Value value) : _data(key, value) {}

        TreeNode(const TreeNode<const Key, Value>& rhs) = delete;

        TreeNode(TreeNode<const Key, Value>&& rhs) noexcept :
            _data{ std::move(rhs._data) },
            _color{ rhs._color },
            _parent{ rhs._parent },
            _left{ rhs._left },
            _right{ rhs._right }
        {
            rhs._left = nullptr;
            rhs._right = nullptr;
            rhs._parent = nullptr;
        }
        
        TreeNode<const Key, Value>& operator=(const TreeNode<const Key, Value>& rhs) = delete;
        TreeNode<const Key, Value>& operator=(TreeNode<const Key, Value>&& rhs) = delete;

        const Key& key() const {
            return _data.first;
        }

        const Value& value() const {
            return _data.second;
        }
    };

    template<typename Key, typename Value>
    class RedBlackTree {
    protected:
        TreeNode<const Key, Value>* _null = nullptr;
        TreeNode<const Key, Value>* _root = nullptr;

    private:
        TreeNode<const Key, Value>* searchTreeHelper(TreeNode<const Key, Value>* node, const Key& key) const {
            if (node == _null || key == node->_data.first) {
                return node;
            }

            if (key < node->_data.first) {
                return searchTreeHelper(node->_left, key);
            }
            return searchTreeHelper(node->_right, key);
        }

        void fixTreeInvariant(TreeNode<const Key, Value>* node) {
            TreeNode<const Key, Value>* brother;
            while (node != _root && node->_color == Color::BLACK) {
                if (node == node->_parent->_left) {
                    brother = node->_parent->_right;
                    if (brother->_color == Color::RED) {
                        brother->_color = Color::BLACK;
                        node->_parent->_color = Color::RED;
                        leftRotate(node->_parent);
                        brother = node->_parent->_right;
                    }

                    if (brother->_left->_color == Color::BLACK && brother->_right->_color == Color::BLACK) {
                        brother->_color = Color::RED;
                        node = node->_parent;
                    }
                    else {
                        if (brother->_right->_color == Color::BLACK) {
                            brother->_left->_color = Color::BLACK;
                            brother->_color = Color::RED;
                            rightRotate(brother);
                            brother = node->_parent->_right;
                        }

                        brother->_color = node->_parent->_color;
                        node->_parent->_color = Color::BLACK;
                        brother->_right->_color = Color::BLACK;
                        leftRotate(node->_parent);
                        node = _root;
                    }
                }
                else {
                    brother = node->_parent->_left;
                    if (brother->_color == Color::RED) {
                        brother->_color = Color::BLACK;
                        node->_parent->_color = Color::RED;
                        rightRotate(node->_parent);
                        brother = node->_parent->_left;
                    }

                    if (brother->_right->_color == Color::BLACK && brother->_right->_color == Color::BLACK) {
                        brother->_color = Color::RED;
                        node = node->_parent;
                    }
                    else {
                        if (brother->_left->_color == Color::BLACK) {
                            brother->_right->_color = Color::BLACK;
                            brother->_color = Color::RED;
                            leftRotate(brother);
                            brother = node->_parent->_left;
                        }

                        brother->_color = node->_parent->_color;
                        node->_parent->_color = Color::BLACK;
                        brother->_left->_color = Color::BLACK;
                        rightRotate(node->_parent);
                        node = _root;
                    }
                }
            }
            node->_color = Color::BLACK;
        }

        void transplantNode(TreeNode<const Key, Value>* toNode, TreeNode<const Key, Value>* fromNode) {
            if (toNode->_parent == nullptr) {
                _root = fromNode;
            }
            else if (toNode == toNode->_parent->_left) {
                toNode->_parent->_left = fromNode;
            }
            else {
                toNode->_parent->_right = fromNode;
            }
            fromNode->_parent = toNode->_parent;
        }

        void deleteNodeHelper(TreeNode<const Key, Value>* node, const Key& key) {
            TreeNode<const Key, Value>* z = _null;
            TreeNode<const Key, Value>* x;
            TreeNode<const Key, Value>* y;

            while (node != _null) {
                if (node->_data.first == key) {
                    z = node;
                }

                if (node->_data.first <= key) {
                    node = node->_right;
                }
                else {
                    node = node->_left;
                }
            }

            if (z == _null) {
                throw NotFoundKeyException<Key>(key);
                return;
            }

            y = z;
            Color y_original_color = y->_color;
            if (z->_left == _null) {
                x = z->_right;
                transplantNode(z, z->_right);
            }
            else if (z->_right == _null) {
                x = z->_left;
                transplantNode(z, z->_left);
            }
            else {
                y = minimum(z->_right);
                y_original_color = y->_color;
                x = y->_right;
                if (y->_parent == z) {
                    x->_parent = y;
                }
                else {
                    transplantNode(y, y->_right);
                    y->_right = z->_right;
                    y->_right->_parent = y;
                }

                transplantNode(z, y);
                y->_left = z->_left;
                y->_left->_parent = y;
                y->_color = z->_color;
            }
            delete z;
            if (y_original_color == Color::BLACK) {
                fixTreeInvariant(x);
            }
        }

        void balance(TreeNode<const Key, Value>* newNode) {
            TreeNode<const Key, Value>* uncle;
            while (newNode->_parent->_color == Color::RED) {
                if (newNode->_parent == newNode->_parent->_parent->_right) {
                    uncle = newNode->_parent->_parent->_left;
                    if (uncle->_color == Color::RED) {
                        uncle->_color = Color::BLACK;
                        newNode->_parent->_color = Color::BLACK;
                        newNode->_parent->_parent->_color = Color::RED;
                        newNode = newNode->_parent->_parent;
                    }
                    else {
                        if (newNode == newNode->_parent->_left) {
                            newNode = newNode->_parent;
                            rightRotate(newNode);
                        }
                        newNode->_parent->_color = Color::BLACK;
                        newNode->_parent->_parent->_color = Color::RED;
                        leftRotate(newNode->_parent->_parent);
                    }
                }
                else {
                    uncle = newNode->_parent->_parent->_right;

                    if (uncle->_color == Color::RED) {
                        uncle->_color = Color::BLACK;
                        newNode->_parent->_color = Color::BLACK;
                        newNode->_parent->_parent->_color = Color::RED;
                        newNode = newNode->_parent->_parent;
                    }
                    else {
                        if (newNode == newNode->_parent->_right) {
                            newNode = newNode->_parent;
                            leftRotate(newNode);
                        }
                        newNode->_parent->_color = Color::BLACK;
                        newNode->_parent->_parent->_color = Color::RED;
                        rightRotate(newNode->_parent->_parent);
                    }
                }
                if (newNode == _root) {
                    break;
                }
            }
            _root->_color = Color::BLACK;
        }

#ifdef DEBUG
        void printHelper(TreeNode<const Key, Value>* _root, std::string indent, bool last) {
            if (_root != _null) {
                std::cout << indent;
                if (last) {
                    std::cout << "R----";
                    indent += "   ";
                }
                else {
                    std::cout << "L----";
                    indent += "|  ";
                }

                std::string sColor = _root->_color == Color::RED ? "RED" : "BLACK";
                std::cout << _root->_data.first << "(" << sColor << ")" << std::endl;
                printHelper(_root->_left, indent, false);
                printHelper(_root->_right, indent, true);
            }
        }
#endif

        void leftRotate(TreeNode<const Key, Value>* x) {
            TreeNode<const Key, Value>* y = x->_right;
            x->_right = y->_left;
            if (y->_left != _null) {
                y->_left->_parent = x;
            }
            y->_parent = x->_parent;
            if (x->_parent == nullptr) {
                _root = y;
            }
            else if (x == x->_parent->_left) {
                x->_parent->_left = y;
            }
            else {
                x->_parent->_right = y;
            }
            y->_left = x;
            x->_parent = y;
        }

        void rightRotate(TreeNode<const Key, Value>* x) {
            TreeNode<const Key, Value>* y = x->_left;
            x->_left = y->_right;
            if (y->_right != _null) {
                y->_right->_parent = x;
            }
            y->_parent = x->_parent;
            if (x->_parent == nullptr) {
                _root = y;
            }
            else if (x == x->_parent->_right) {
                x->_parent->_right = y;
            }
            else {
                x->_parent->_left = y;
            }
            y->_right = x;
            x->_parent = y;
        }

    public:
        RedBlackTree() {
            _null = new TreeNode<const Key, Value>();
            _null->_color = Color::BLACK;
            _null->_left = nullptr;
            _null->_right = nullptr;
            _root = _null;
        }

        ~RedBlackTree() {
            while (_root->_left != _null) {
                TreeNode<const Key, Value>* pair = minimum(_root);
                deleteNode(pair->key());
            }
            while (_root->_right != _null) {
                TreeNode<const Key, Value>* pair = maximum(_root);
                deleteNode(pair->key());
            }
            deleteNode(_root->key());
            delete _null;
        }

        TreeNode<const Key, Value>* find(const Key& k) const {
            return searchTreeHelper(_root, k);
        }

        TreeNode<const Key, Value>* minimum(TreeNode<const Key, Value>* node) {
            while (node->_left != _null) {
                node = node->_left;
            }
            return node;
        }

        TreeNode<const Key, Value>* maximum(TreeNode<const Key, Value>* node) {
            while (node->_right != _null) {
                node = node->_right;
            }
            return node;
        }

        void insert(const Key& key, const Value& value) {
            TreeNode<const Key, Value>* node = new TreeNode<const Key, Value>(key, value);
            node->_parent = nullptr;
            node->_left = _null;
            node->_right = _null;
            node->_color = Color::RED;

            TreeNode<const Key, Value>* y = nullptr;
            TreeNode<const Key, Value>* x = _root;

            while (x != _null) {
                y = x;
                if (node->_data.first < x->_data.first) {
                    x = x->_left;
                }
                else {
                    x = x->_right;
                }
            }

            node->_parent = y;
            if (y == nullptr) {
                _root = node;
            }
            else if (node->_data.first < y->_data.first) {
                y->_left = node;
            }
            else {
                y->_right = node;
            }

            if (node->_parent == nullptr) {
                node->_color = Color::BLACK;
                return;
            }

            if (node->_parent->_parent == nullptr) {
                return;
            }

            balance(node);
        }

        void deleteNode(const Key& kay) {
            deleteNodeHelper(_root, kay);
        }

#ifdef DEBUG
        void print() {
            if (_root) {
                printHelper(_root, "", true);
            }
        }
#endif
    };

} // namespace dwt