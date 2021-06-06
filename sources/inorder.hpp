#pragma once
#include <stack>
#include "Node.hpp"
using namespace std;

namespace ariel{
    template<typename T>
    class inorder{
        private:
            stack<Node<T>*> nodes;
        
        public:
        inorder(){};
        ~inorder(){};
        
        inorder(Node<T>* root){
            if(root){
                Node<T>* temp = root;
                fill_the_stack(temp);
            }
        };

        inorder<T>& operator++(){
            Node<T>* temp = nodes.top();
            nodes.pop();
            return *this;
        }
        inorder<T> operator++(int){
            inorder<T> temp = *this;
            operator++();
            return temp;
        }


        Node<T>* getNode(){
            return nodes.top();
        }

        void fill_the_stack(Node<T>* temp){
            if(temp){
                fill_the_stack(temp->right);
                nodes.push(temp);
                fill_the_stack(temp->left);
            }
            else{
                return;
            }
        }

        bool operator!=(const inorder &other) const
        {
            return other.nodes.empty() != nodes.empty() ||
                   (!nodes.empty() && other.nodes.top() != nodes.top());
        }

        bool operator==(const inorder &other){
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


    };
}