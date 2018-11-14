#include "mcts/node.h"

namespace Beta{
    template<typename StateType, typename ActionType>
    Node::Node(string name, Index index, float N, float W, float Q, float P, float U, float v, bool keep, 
    int batch_index, int circle_index, int child_index): name_(name), index_(index),
    N_(N), W_(W), Q_(Q), P_(P), U_(U), v_(v),keep_(keep), batch_index_(batch_index),
    circle_index_(circle_index),child_index_(child_index){
        child_.clear();
        state_.reset(new StateType());
        action_.reset(new ActionType());
        parent_=nullptr;
    }



    template<typename StateType, typename ActionType>
    Node::~Node(){

    }
    template<typename StateType, typename ActionType>
    Node::Node(string name){
        

    }
    template<typename StateType, typename ActionType>
    Node::Node(string name, Index index){

    }
    template<typename StateType, typename ActionType>
    Node::Node(Index index){


    }

    template<typename StateType, typename ActionType>
    Node::Node(const Node<StateType, ActionType>& node){

    }

    template<typename StateType, typename ActionType>
    void Node::serialize(const string& file){

    }

    template<typename StateType, typename ActionType>
    void Node::deserialize(const string& file){

    }

    template<typename StateType, typename ActionType>
    void Node::compute_feature(){

    }

    template<typename StateType, typename ActionType>
    Blob<float>* Node::compute_pai(float inv_tau, TIndex max_child){

    }

    template<typename StateType, typename ActionType>
    void Node::insert_child(Node<StateType, ActionType>* node, Index index){
        lock_guard<mutex> lock(mutex_);
        node->set_index(index);
        child_.insert(std::make_pair(node->index(), node) );
        node->set_parent(this);
    }







}