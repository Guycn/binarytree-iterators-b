#include "doctest.h"
#include "BinaryTree.hpp"
#include <array>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>
#include <random>
using namespace std;
using namespace ariel;
using namespace std::placeholders;
//constants
const size_t NUM_OF_NODES = 50;
const double MAX_DOUBLE = 100;
const int FIRST_CHAR = (int)'0';
const int LAST_CHAR = (int)'z';
const int MAX_OPTIONS = 1;

//---------------------Helpers--------------------
/*
generate random values helpers
rand_double & randInt => [max,min] range
randChar => ['0','z'] (number, letters, and special charcters(!,. etc))
*/
double rand_double(double max = MAX_DOUBLE, double min = 0.0)
{
    uniform_real_distribution<double> unif(max, min);
    default_random_engine re;
    return unif(re);
}
const int randInt(int max, int min = 0)
{
    int range = max - min + 1;
    return (const int)(rand() % range + min);
}
char randChar()
{
    return (char)(randInt(FIRST_CHAR, LAST_CHAR));
}
//classic and simple recursion traversal
/*
get empty vector of element of type T
where T is also the data type of the tree
and root node(use tree.getNode())
*/
template <typename T>
void inorder(vector<T> &elements, Node<T> *curr)
{
    if (curr)
    {
        inorder(elements, curr->left);
        elements.push_back(curr->data);
        inorder(elements, curr->right);
    }
}
template <typename T>
void preorder(vector<T> &elements, Node<T> *curr)
{
    if (curr)
    {
        elements.push_back(curr->data);
        preorder(elements, curr->left);
        preorder(elements, curr->right);
    }
}
template <typename T>
void postorder(vector<T> &elements, Node<T> *curr)
{
    if (curr)
    {
        postorder(elements, curr->left);
        postorder(elements, curr->right);
        elements.push_back(curr->data);
    }
}
/*
T - node name type, Function - get T and return T (T generate_node_name(T){})
helper method to create random shape binary tree in size NUM_OF_NODES
tree - empty tree to change
generate_node_name - method that get the last node name and create a new node name
assume this method need to create a distinct nodes names
root_name - init name for the root node
return the created tree nodes names in vector shape
*/
template <typename T, typename Function>
vector<T> buildRandomTree(BinaryTree<T> &tree, Function generate_node_name, T root_name, size_t num_of_nodes = NUM_OF_NODES)
{
    vector<T> nodes_names;
    nodes_names.reserve(num_of_nodes);
    T last_node_name = root_name;
    nodes_names.push_back(last_node_name);
    tree.add_root(last_node_name);
    for (int i = 0; i < num_of_nodes; i++)
    {
        const int option = randInt(MAX_OPTIONS);
        T new_node_name = generate_node_name(last_node_name);
        nodes_names.push_back(new_node_name);
        if (option == 0)
        {
            tree.add_right(last_node_name, new_node_name);
        }
        else
        {
            tree.add_left(last_node_name, new_node_name);
        }
        last_node_name = new_node_name;
    }
    return nodes_names;
}
template <typename T>
bool is_trees_equal(BinaryTree<T> &tree1, BinaryTree<T> &tree2)
{
    auto it = tree2.begin();
    for (T element : tree1)
    {
        if (*it != element)
        {
            return false;
        }
        ++it;
    }
    return true;
}
//init some common use lambda expressions
const auto simple_generate_node_int = [](const int last_node_name)
{ return last_node_name + randInt(10, 1); };
const auto simple_generate_node_str = [](const string &last_node_name)
{ return last_node_name + randChar(); };

//----------------------------Tests---------------------------
/*
create simple tree by hand and his expected travsal
in the 3 ways to make sure the big test with the recursive method is good
*/
TEST_CASE("basic travesals")
{
    BinaryTree<int> tree;
    CHECK_NOTHROW(tree.add_root(1));
    CHECK_NOTHROW(tree.add_left(1, 2));
    CHECK_NOTHROW(tree.add_left(2, 4));
    CHECK_NOTHROW(tree.add_right(2, 5));
    CHECK_NOTHROW(tree.add_right(1, 3));
    CHECK_NOTHROW(tree.add_right(3, 6));
    const int NUM_OF_NODES = 6;
    array<int, NUM_OF_NODES> expected_preorder = {1, 2, 4, 5, 3, 6};
    size_t i = 0;
    for (auto it = tree.begin_preorder(); it != tree.end_preorder(); ++it, i++)
    {
        CHECK_EQ(*it, expected_preorder.at(i));
    }
    CHECK_EQ(i, NUM_OF_NODES);
    i = 0;
    array<int, NUM_OF_NODES> expected_inorder = {4, 2, 5, 1, 3, 6};
    for (auto it = tree.begin_inorder(); it != tree.end_inorder(); ++it, i++)
    {
        CHECK_EQ(*it, expected_inorder.at(i));
    }
    CHECK_EQ(i, NUM_OF_NODES);
    array<int, NUM_OF_NODES> expected_postorder = {4, 5, 2, 6, 3, 1};
    i = 0;
    for (auto it = tree.begin_postorder(); it != tree.end_postorder(); ++it, i++)
    {
        CHECK_EQ(*it, expected_postorder.at(i));
    }
    CHECK_EQ(i, NUM_OF_NODES);
}
/*
check if begin() & end() return inorder iterator
*/
TEST_CASE("Inorder is defualt")
{
    BinaryTree<int> tree;
    vector<int> nodes_names = buildRandomTree<int>(
        tree, simple_generate_node_int,
        1);
    auto it = tree.begin_inorder();
    for (const auto &element : tree)
    {
        CHECK(element == *it);
        ++it;
    }
}
/*
check inorder,preorder and postorder iterator agianst the simple recursive implemntation
on big(NUM_OF_NODES size) and complex tree
*/
TEST_CASE("Big travesal")
{
    array<size_t, 4> tree_sizes = {0, 1, 50, 100}; //can make more sizes here(becosse bash grade the sizes are small)
    for (auto tree_size : tree_sizes)
    {
        BinaryTree<int> tree;
        vector<int> nodes_names = buildRandomTree<int>(
            tree, simple_generate_node_int, 1, tree_size);
        vector<int> right_answers;
        right_answers.reserve(nodes_names.size());
        inorder<int>(right_answers, tree.get_root());
        size_t i = 0;
        for (auto it = tree.begin_inorder(); it != tree.end_inorder(); ++it)
        {
            CHECK_EQ(right_answers.at(i++), *it);
        }
        CHECK_EQ(nodes_names.size(), i);
        right_answers.clear();
        postorder<int>(right_answers, tree.get_root());
        i = 0;
        for (auto it = tree.begin_postorder(); it != tree.end_postorder(); ++it)
        {
            CHECK_EQ(right_answers.at(i++), *it);
        }
        CHECK_EQ(nodes_names.size(), i);
        right_answers.clear();
        preorder<int>(right_answers, tree.get_root());
        i = 0;
        for (auto it = tree.begin_preorder(); it != tree.end_preorder(); ++it)
        {
            CHECK_EQ(right_answers.at(i++), *it);
        }
        CHECK_EQ(nodes_names.size(), i);
    }
}
TEST_CASE("Empty tree - destractor & iterators")
{
    BinaryTree<double> tree;
    int count = 0;
    for (auto it = tree.begin_preorder(); it != tree.end_preorder(); ++it)
    {
        count++;
    }
    CHECK_EQ(count, 0);
    for (auto it = tree.begin_postorder(); it != tree.end_postorder(); ++it)
    {
        count++;
    }
    CHECK_EQ(count, 0);
    for (auto it = tree.begin_inorder(); it != tree.end_inorder(); ++it)
    {
        count++;
    }
    CHECK_EQ(count, 0);
}
TEST_CASE("Add nodes")
{
    BinaryTree<double> tree;
    //Exceptions
    CHECK_THROWS(tree.add_left(rand_double(), rand_double()));
    CHECK_THROWS(tree.add_right(rand_double(), rand_double()));
    double root_val = rand_double();
    tree.add_root(root_val);
    CHECK(tree.get_root());
    CHECK_EQ(tree.get_root()->data, root_val);
    CHECK_THROWS(tree.add_left(root_val + 1, rand_double()));
    CHECK_THROWS(tree.add_right(root_val + 1, rand_double()));
    //Replace exiting
    root_val++;
    tree.add_root(root_val);
    CHECK_EQ(tree.get_root()->data, root_val);
    double left_val = rand_double();
    CHECK_NOTHROW(tree.add_left(root_val, left_val));
    CHECK_NOTHROW(tree.add_left(root_val, left_val + 1));
    CHECK_EQ(tree.get_root()->left->data, left_val + 1);
    double right_val = rand_double();
    CHECK_NOTHROW(tree.add_right(root_val, right_val));
    CHECK_NOTHROW(tree.add_right(root_val, right_val + 1));
    CHECK_EQ(tree.get_root()->right->data, right_val + 1);
}
TEST_CASE("operator !=")
{
    BinaryTree<int> tree;
    tree.add_root(5);
    tree.add_left(5, 10);
    auto it1 = tree.begin_inorder();
    ++it1;
    CHECK(tree.begin_inorder() != it1);
    auto it2 = tree.begin_preorder();
    ++it2;
    CHECK(tree.begin_preorder() != it2);
    auto it3 = tree.begin_postorder();
    ++it3;
    CHECK(tree.begin_postorder() != it3);
}
TEST_CASE("change elements value")
{
    BinaryTree<int> tree;
    buildRandomTree<int>(tree, simple_generate_node_int, 1);
    const int MAX_VAL = 50;
    const int fillVal = randInt(MAX_VAL);
    for (int &element : tree) //use inorder
    {
        element = fillVal;
    }
    for (auto it = tree.begin_postorder(); it != tree.end_postorder(); ++it) //use diffrent iterator
    {
        CHECK_EQ(*it, fillVal);
    }
}
TEST_CASE("copy contractor & operator=")
{
    BinaryTree<int> tree;
    buildRandomTree<int>(tree, simple_generate_node_int, 1);
    BinaryTree<int> tree2{tree};
    CHECK(is_trees_equal<int>(tree, tree2));
    for (int &element : tree)
    {
        element++;
    }
    CHECK_FALSE(is_trees_equal<int>(tree, tree2));
    tree = tree2;
    CHECK(is_trees_equal<int>(tree, tree2));
}
//------------------------------------------------------------------------------
// need to make some changes in out custom iterator to make it works

TEST_CASE("Algorithms Use 1")
{
    SUBCASE("minmax & count")
    {
        BinaryTree<string> tree;
        const string init_str = "node";
        vector<string> nodes_names = buildRandomTree<string>(tree, simple_generate_node_str, init_str);
        auto [minVal1, maxVal1] = minmax_element(tree.begin(), tree.end());
        auto [minVal2, maxVal2] = minmax_element(nodes_names.begin(), nodes_names.end());
        CHECK_EQ(*minVal1, *minVal2);
        CHECK_EQ(*maxVal1, *maxVal2);
        string exists_element = nodes_names[(size_t)randInt(nodes_names.size() - 1)];
        long long res = count(tree.begin(), tree.end(), exists_element);
        CHECK(res != 0);
        long long res2 = count(tree.begin_inorder(), tree.begin_inorder(), "j"); //minumum len is 4
        CHECK_EQ(res2, 0);
    }
    SUBCASE("fill & for_each")
    {
        BinaryTree<int> tree;
        buildRandomTree<int>(tree, simple_generate_node_int, 10);
        fill(tree.begin_preorder(), tree.end_preorder(), 2);
        for_each(tree.begin_preorder(), tree.end_preorder(), [](int element)
                 { CHECK_EQ(element, 2); });
    }
    SUBCASE("replace & min_element")
    {
        BinaryTree<int> tree;
        vector<int> nodes_names = buildRandomTree<int>(tree, simple_generate_node_int, 1);
        int min_node = *min_element(tree.begin_preorder(), tree.end_preorder());
        replace(tree.begin_preorder(), tree.end_preorder(), min_node, min_node * 2);
        int min_node2 = *min_element(tree.begin_preorder(), tree.end_preorder());
        CHECK(min_node != min_node2);
    }
}