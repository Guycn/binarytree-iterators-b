#pragma once
#include <stack>
#include "Node.hpp"
using namespace std;

namespace ariel{
    template<typename T>
    class preorder{
        private:
            stack<Node<T>*> nodes;
        
        public:
        preorder(){};  //end
        
        preorder(Node<T>* root){
            if(root){
                nodes.push(root);
            }
        };

        preorder<T>& operator++(){
            Node<T>* temp = nodes.top();
            nodes.pop();
            if(temp->right)nodes.push(temp->right);
            if(temp->left)nodes.push(temp->left);
            return *this;
        }
        preorder<T> operator++(int){
            preorder<T> temp = *this;
            operator++();
            return temp;
        }

        bool operator!=(const preorder &other) const
        {
            return other.nodes.empty() != nodes.empty() ||
                   (!nodes.empty() && other.nodes.top() != nodes.top());
        }

        bool operator==(const preorder &other){
            return !(other!=*this);
        }

        T &operator*()
        {
            return nodes.top()->data;
        }
        T *operator->()
        {
            return &(nodes.top()->data);
        }

        Node<T>* getNode(){
            return nodes.top();
        }


    };
}