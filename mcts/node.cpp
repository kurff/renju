#include "core/state.h"
#include "core/renju_action.h"
#include "mcts/node.h"

namespace Beta{
    template<typename StateType, typename ActionType>
    Node<StateType, ActionType>::Node(string name, Index index, float N, float W, float Q, float P, float U, float v, bool keep, 
    int batch_index, int circle_index, int child_index): name_(name), index_(index),
    N_(N), W_(W), Q_(Q), P_(P), U_(U), v_(v),keep_(keep), batch_index_(batch_index),
    circle_index_(circle_index),child_index_(child_index){
        child_.clear();
        state_.reset(new StateType());
        action_.reset(new ActionType());
        parent_=nullptr;
    }



    template<typename StateType, typename ActionType>
    Node<StateType, ActionType>::~Node(){

    }
    template<typename StateType, typename ActionType>
    Node<StateType, ActionType>::Node(string name){
        

    }
    template<typename StateType, typename ActionType>
    Node<StateType, ActionType>::Node(string name, Index index){

    }
    template<typename StateType, typename ActionType>
    Node<StateType, ActionType>::Node(Index index){


    }

    template<typename StateType, typename ActionType>
    Node<StateType, ActionType>::Node(const Node<StateType, ActionType>& node){

    }

    template<typename StateType, typename ActionType>
    void Node<StateType, ActionType>::serialize(const string& file){

    }

    template<typename StateType, typename ActionType>
    void Node<StateType, ActionType>::deserialize(const string& file){

    }

    template<typename StateType, typename ActionType>
    void Node<StateType, ActionType>::compute_feature(){

    }

    template<typename StateType, typename ActionType>
    Blob<float>* Node<StateType, ActionType>::compute_pai(float inv_tau, TIndex max_child){

    }

    template<typename StateType, typename ActionType>
    void Node<StateType, ActionType>::insert_child(Node<StateType, ActionType>* node, Index index){
        std::lock_guard<mutex> lock(mutex_);
        //node->set_index(index);
        child_.insert(std::make_pair(node->index(), node) );
        //node->set_parent(this);
    }

    template class Node<State, RenJuAction>;






}