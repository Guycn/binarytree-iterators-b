#pragma once
#include <iostream>

namespace ariel{
    template<typename T>
    class Node{
        //private:
        public:
        T data;
        Node* right;
        Node* left;

        Node(T d) : data(d), left(nullptr), right(nullptr) {};
    };
}
//Node(T d) : data(d), left(nullptr), right(nullptr) {};