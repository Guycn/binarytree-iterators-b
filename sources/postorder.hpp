#pragma once
#include <stack>
#include "Node.hpp"
using namespace std;

namespace ariel{
    template<typename T>
    class postorder{
        private:
            stack<Node<T>*> nodes;
        
        public:
        postorder(){};
        
        postorder(Node<T>* root){
            if(root){
                Node<T>* temp = root;
                fill_the_stack(temp);
            }
        };

        postorder<T>& operator++(){
            Node<T>* temp = nodes.top();
            nodes.pop();
            return *this;
        }

        postorder<T> operator++(int){
            postorder<T> temp = *this;
            operator++();
            return temp;
        }


        Node<T>* getNode(){
            return nodes.top();
        }

        void fill_the_stack(Node<T>* temp){
            if(temp){
                nodes.push(temp);
            }
            else{
                return;
            }
            fill_the_stack(temp->right);
            fill_the_stack(temp->left);
        }

        T &operator*()
        {
            return nodes.top()->data;
        }
        T *operator->()
        {
            return &(nodes.top()->data);
        }
        bool operator!=(const postorder &other) const
        {
            return other.nodes.empty() != nodes.empty() ||
                   (!nodes.empty() && other.nodes.top() != nodes.top());
        }

        bool operator==(const postorder &other){
            return !(other!=*this);
        }


    };
}