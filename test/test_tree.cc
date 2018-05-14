#include "gtest/gtest.h"
#include "mcts/tree.hpp"
#include "core/parameters.hpp"
#include "core/context.hpp"
#include "core/state.hpp"
#include "core/action.hpp"
using namespace Beta;
TEST(NodeTest, Allocate){
    Node<ChessState, ChessAction >* node = new Node<ChessState, ChessAction>("1");
    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->N(),0.0);
    EXPECT_EQ(node->P(),0.0);
    EXPECT_EQ(node->child().size(),0);
    EXPECT_EQ(node->parent(),nullptr);
}


TEST(TreeTest, Allocate){
    typedef typename Beta::Tree<ChessState, ChessAction,ChessContext<ChessState,ChessAction>, CPUContext > TreeDef;
    TreeDef* tree = new TreeDef(5,1,0.1,0.1,0.1,1,15,1,3);

    //Tree<int, int, int>* tree = new Tree<int, int, int>(1);
    EXPECT_NE(tree, nullptr);
    //EXPECT_EQ()
}

TEST(TreeTest, AddDeleteNode){
    typedef typename Beta::Tree<ChessState, ChessAction,ChessContext<ChessState,ChessAction>, CPUContext > TreeDef;
    typedef typename Beta::Node<ChessState, ChessAction> NodeDef;
    TreeDef* tree = new TreeDef(5,1,0.1,0.1,0.1,1,15,1,3);
    //tree->add_node(0);
    Index parent = 0;
    Index child = 1;
    tree->add_node(parent);
    NodeDef* node = new NodeDef(child);
    //tree->add_node(,node, child);

    
    
    
    // Tree<int, int , int>* tree = new Tree<int, int ,int>(0);
    // Node<int>* node = new Node<int>("0");
    // Node<int>* node1 = new Node<int>("1");
    // tree->add_node(node);
    // tree->add_node(node,node1);
    // Node<int>* node2 = new Node<int>("2");
    // Node<int>* node3 = new Node<int>("3");
    // tree->add_node(node, node2);
    // tree->add_node(node,node3);
    // Node<int>* node4 = new Node<int>("4");
    // Node<int>* node5 = new Node<int>("5");
    // tree->add_node(node3,node4);
    // tree->add_node(node, node5);
    // tree->travel(node);
    // tree->find(1);
    // DLOG(INFO)<< tree->size();
    // tree->clear_node(node3);
    // tree->clear_node(node5);
    // DLOG(INFO)<< tree->size();
    // tree->travel(node);
    // tree->clear_node(node5);




}


TEST(PARA, test){
    //Parameters* param = new Parameters();
    Parameters* param = global_parameters();
    read_from_text("./parameters.txt", param);
    
}