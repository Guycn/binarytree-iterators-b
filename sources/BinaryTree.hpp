#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Node.hpp"
#include "inorder.hpp"
#include "postorder.hpp"
#include "preorder.hpp"
using namespace std;

namespace ariel{
    template<typename T> class BinaryTree{
        
        public:
            BinaryTree(){
                root = nullptr;
            }

            //Node* getNode(T data){
            //}
            //template<typename T>
            friend ostream& operator<<(ostream& out, const BinaryTree<T>& bin_tree);

            BinaryTree& add_root(T data){
                if(root!= nullptr){
                    root->data = data;
                    return *this;
                }
                root = new Node<T>(data);
                root->data = data;
                return *this;
            }

            //Destructor
            ~BinaryTree(){
                //delete_tree();
            }

            BinaryTree(BinaryTree&& other)noexcept{
		        root = other.root;
		        other.root = nullptr;
            }

            BinaryTree(const BinaryTree& other){
                tree_copy(other);
            }

            BinaryTree& operator=(const BinaryTree other){
            if (this!=&other)
            {
                delete_tree();
            }
            tree_copy(other);
            return *this;
            }

            BinaryTree& operator=(BinaryTree&& other) noexcept{
            root = other.root;
            other.root = nullptr;
            return *this;
            }

            BinaryTree& add_left(T parent, T child){
                for(auto iter = begin_preorder(); iter!=end_preorder();++iter){
                    Node<T>* tmp = iter.getNode();
                    if(tmp->data == parent){
                        if(!tmp->left){
                            tmp->left = new Node<T>(child);
                            //tmp->left->data = child;
                            return *this;
                        }
                        tmp->left->data = child;
                        return *this;
                    }
                }
                        throw invalid_argument("Parent not exist");
            }

            BinaryTree& add_right(T parent, T child){
                for(auto iter = begin_preorder(); iter!=end_preorder();++iter){
                    Node<T>* tmp = iter.getNode();
                    if(tmp->data == parent){
                        if(!tmp->right){
                            tmp->right = new Node<T>(child);
                            //tmp->right->data = child;
                            return *this;
                        }
                        tmp->right->data = child;
                        return *this;
                    }
                }
                throw invalid_argument("Parent not exist");
            }

            preorder<T> begin_preorder(){
                return preorder<T>{root};
            }

            inorder<T> begin_inorder(){
                return inorder<T>{root};
            }

            postorder<T> begin_postorder(){
                return postorder<T>{root};
            }

            preorder<T> end_preorder(){
                return preorder<T>{};
            }

            inorder<T> end_inorder(){
                return inorder<T>{};
            }

            postorder<T> end_postorder(){
                return postorder<T>{};
            }

            inorder<T> begin(){
                return inorder<T>{root};
            }

            inorder<T> end(){
                return inorder<T>{};
            }

        private:
            Node<T>* root;


            //this print should work like inorder
            friend ostream& operator<<(ostream& out, BinaryTree<T>& bin_tree){
                for(auto var: bin_tree){
                    out << var << " ";
                }
                out << endl;
                return out;
            }

            void recursive_copy(Node<T>* head, Node<T>* copy){
                if(head->right!=nullptr){
                    copy->right = new Node<T>{head->right->data};
                    //copy->right->data = head->right->data;
                    recursive_copy(head->right, copy->right);
                }
                
                if(head->left!=nullptr){
                    copy->left = new Node<T>{head->left->data};
                    //copy->left->data = head->left->data;
                    recursive_copy(head->left, copy->left);
                }
            }

            void tree_copy(const BinaryTree& temp){
                if(!temp.root){
                    return;
                }
                root = new Node<T>{temp.root->data};
                //root->data = temp.root->data;
                recursive_copy(temp.root,root);

            }

            void delete_tree(){
                for(auto iter = begin_preorder(); iter!= end_preorder();++iter){
                    Node<T>* tmp = iter.getNode();
                    delete tmp;
                }
            }

    };
}
