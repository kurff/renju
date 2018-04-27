#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <vector>
#include <map>
#include <string>
#include <queue>


#include "caffe2/core/tensor.h"



#include "glog/logging.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "core/network.hpp"
#include "core/context.hpp"
#include "core/action.hpp"

#include <cmath>


using namespace std;
using namespace caffe2;

#include "mcts/thread_pool.h"

namespace Beta{

template<typename State, typename Action>
class Node{
    typedef typename unsigned long Index;
    public:
        Node(string name):N_(0.0f),W_(0.0f),Q_(0.0f),P_(0.0f), index_(0), name_(name), keep_flag_(true), parent_(nullptr){
            child_.clear();
            node_state_.reset(new State());

            
        }

        Node(const Node<State, Action>& node){
           


        }

        Node(const State & state, string name){
            node_state_ = state;
            name_ = name;

        }



        ~Node(){


        }

        // compute features using 
        void compute_feature(){

        }
    
        //float& sN(){return N_;}
        
        void get(Node<State, Action>& node){
            lock_guard<mutex> lock(mutex_);
            node.N_ = N_;
            node.Q_ = Q_;
            node.W_ = W_;
            node.P_ = P_;
            node.U_ = U_;
            node.v_ = v_;
            node.keep_flag_ = keep_flag_;
            node.name_ = name_;
            node.index_ = index_;
            node.batch_index_ = batch_index_;
            node.circle_index_ = circle_index_;
            node.node_state_ = node_state_;
            node.action_ = action_;
        }
        void set(const Node<State, Action>& node){
            lock_guard<mutex> lock(mutex_);
            child_ = node.child();
            parent_ = node.parent();
            node_state_ = node.node_state();
            action_ = node.action();
            index_ = node.index();
            name_ = node.name();
            N_ = node.N();
            W_ = node.W();
            Q_ = node.Q();
            P_ = node.P();
            U_ = node.U();
            v_ = node.v();
            keep_flag_ = node.keep_flag();
            batch_index_ = node.batch_index();
            circle_index_ = node.circle_index();
        }

        void insert_child(Node<State, Action>* node, Index index){
            lock_guard<mutex> lock(mutex_);
            node->set_index(index);
            child_.insert(std::pair<Index, Node<State,Action> >(node->index(), node) );
            node->set_parent(this);


        }
    
        void set_parent(Node<State,Action>* p){
            parent_ = p;
        }
        void set_index(Index index){index_ = index;}
        void remove_all_child(){child_.clear(); }
        void remove_one_child(Index index){ child_.erase(index);}
    
    public:
        const map<Index, Node<State, Action>*  > child(){return child_;}
        const Node<State, Action>* parent(){return parent_;}
        const shared_ptr<State> node_state(){ return node_state_;}
        const shared_ptr<Action> action(){return action_;}
        const Index index(){return index_;}
        const string name(){return name_;}
        const float N(){return N_;}
        const float W(){return W_;}
        const float Q(){return Q_;}
        const float P(){return P_;}
        const float U(){return U_;}
        const float v(){return v_;}
        const bool keep_flag(){return keep_flag_;}
        const int batch_index(){return batch_index_;}
        const int circle_index(){return circle_index_;}

    protected:
        map<Index,  Node<State, Action>*  > child_;
        Node<State, Action>* parent_;
        shared_ptr<State> node_state_;
        shared_ptr<Action> action_; // edge of 
        Index index_;
        string name_;
        float N_;
        float W_;
        float Q_;
        float P_;
        float U_;
        float v_;
        bool keep_flag_; // if flag == TRUE, keep else remove
        // used to cache tensor
        int batch_index_;
        int circle_index_;
        mutex mutex_;
        //static int index;
};


template<typename State, typename Action>
class Tree{
    typedef typename unsigned long Index;
    typedef Node<State, Action> NodeDef;
    typedef typename map<Index, shared_ptr<NodeDef> >::iterator Iterator;
    public:
        Tree(int L, int num_simulation, float tau, float v_resign, float epsilon, int num_thread):L_(L),  num_simulation_(simulation),counter_(0),  v_resign_(v_resign), epsilon_(epsilon), num_thread_(num_thread){

            context_ = shared_ptr<Context>(new ChessContext());
            network_ = shared_ptr<Network>(new Network());
            inv_tau_ = 1.0f/tau;
            thread_pool_.reset(new TaskThreadPool(num_thread_));
            

        }
        ~Tree(){

        }

        void add_node(NodeDef * leaf_node, NodeDef* node, Index index){
            //leaf_node->
            lock_guard<mutex> lock(mutex_);
            DLOG(INFO)<< "adding "<< index<<" node";
            nodes_[index] = node;
            leaf_node->insert_child(node, index);
        }

        bool add_node(NodeDef* node, Index index){
            lock_guard<mutex> lock(mutex_);
            DLOG(INFO)<< "adding "<< index<<" node: "<< node->name();
            Iterator it = nodes_.find(index);
            if(it != nodes_.end()){
                LOG(INFO)<<" tree already has such node";
                return false;
            }
            node->set_index(index);
            nodes_[index] = node;
            return true;
        }

        NodeDef* find(int index){
            lock_guard<mutex> lock(mutex_);
            Iterator it = nodes_.find(index);
            if(it == nodes_.end()){
                LOG(INFO)<< "can not find " << index;
            }else{
                LOG(INFO)<<"find "<< index;
            }
            return it->second;
        }


        void travel(NodeDef* root){       
            queue< NodeDef* > cache;
            cache.push(root);
            while(cache.size()){
                NodeDef* ele = cache.front();
                DLOG(INFO)<<"visit "<< ele->name()<<" index: "<<ele->index()<<" child size: "<< ele->schild().size();
                for(auto x : ele->child()){
                    cache.push(x.second);
                }
                cache.pop();
            }
        }

        void remove_child_node(NodeDef* node){
            node->remove_child();
        }
        // remove given node and its descendant
        void remove_itself_and_child(NodeDef*  node){
            Iterator it = nodes_.find(node->index());
            if(it == nodes_.end()){
                DLOG(INFO)<<"can not find node "<< node->name(); 
                return;
            }
            node->parent()->remove_one_child(node->index());
            queue<NodeDef* > cache;
            cache.push(node);

            // get all nodes in the sub-tree of node and remove these node
            // 
            while(cache.size()){
                NodeDef* ele = cache.front();
                for(auto x : ele->child()){
                    cache.push(x.second);
                } 
                nodes_.erase(ele->index());
                cache.pop();
            }
        }

        void reset_root(NodeDef*  node){
            nodes_.clear();
            add_node(node, node->index());
        }

        void select(NodeDef* root){
            queue<NodeDef* > cache;
            cache.push(root);
            NodeDef* best;
            while(cache.size()){
                NodeDef* ele = cache.front();
                DLOG(INFO)<<"visit "<< ele->name()<<" index: "<<ele->index()<<" child size: "<< ele->child().size();
                float maxQ = 0;


                if(ele->child().size() ==0){
                    contex_->get_legal_action(ele->node_state());
                    for(int i = 0; i < context_->size_legal_action(); ++ i){
                        NodeDef* node = new NodeDef (context_->get_legal_action(),"Node"+std::to_string(counter_));
                        add_node(ele, node);
                        leafs_.push(node);
                    }
                    //return;
                }
                shared_ptr<NodeDef> ptr(new NodeDef());
                for(auto child : ele->child() ){
                    child.second->get(ptr.get());
                    if(maxQ <= ptr->Q() + ptr->U()){
                        maxQ = ptr->Q() + ptr->U();
                        best = child.second;
                    }
                    cache.push(best);
                }
                cache.pop();
            }           
        }





        void copy_to_batch(Node<State>* node){




        }




        void expand_and_evaluate(){
            // add child node into leaf node
            
            //int counter = 0;
            // batch_.clear();

            // int num_leafs = leafs_.size();
            // int num_batch = num_leafs / batch_size_;
            // for(int i = 0; i < num_batch; ++ i){
            //     Tensor<TesnorCPU>* tensor = new Tensor<TesnorCPU> ();
            //     tensor->Reshape(vector<int>{batch_size_, height_, width_, channel_});
            //     batch_.push(tensor);
            // }





            while(leafs_.size()){
                NodeDef* node = leafs_.front();                
                network_->forward(node->state());
                backup(node);
                leafs_.pop();

            }
            // push leaf node into leafs_ for evaluation
        }
        void backup(NodeDef* node){
            queue<NodeDef* > cache;
            cache.push(node);
            while(cache.size()){
                
                NodeDef* node = cache.front();

                node->sN() += 1;
                node->sW() += node->sv();
                node->sQ() = node->sW() / node->sN();
                cache.push(node->sparent());
                cache.pop();
            }
        }


        // compute pai
        void pai(NodeDef* root){
            Tensor<TesnorCPU>* ptr = new Tensor<TensorCPU>();
            ptr->Reshape();
            float sum = 0.0f;
            Iterator it;
            for(it = root->schild().begin(); it != root->schild().end(); ++ it){
                sum += pow(it->second->N(), inv_tau_);
            }
            float* p = ptr->mutable_data<float>();
            for(it = root->schild().begin(); it != root->schild().end(); ++ it){
                float value = it->second->N() / sum;
                p[it->second->y() * ptr->dim(1) * ptr->dim(2) + it->second->x() * ptr->dim(2) + it->second->c() ] = value;
                // context configuration
                it->second->set_action_w(value);



            }
            contex_->push(ptr);
        }

        void sample(NodeDef* root){
            Iterator it;


            for(it = root->schild().begin(); it!= root->schild().end(); ++ it){

            }


        }



        void run(NodeDef* root){

            for(int i = 0; i < num_simulation_; ++ i){
                select(root);
                expand_and_evaluate();
            }


            // compute pai
            pai(root);









        }



        const size_t size(){return nodes_.size();}
        const Index counter(){return counter_;}
        const int L(){return L_;}
        const queue<NodeDef* > leafs(){return leafs_;}
        const std::map<Index, NodeDef* > nodes(){return nodes_;}



    protected:
        std::map<Index, NodeDef* > nodes_;
        
        //queue<function<void (NodeDef* ) > > tasks_;
        queue<NodeDef* > leafs_;
        // multiple-thread for pararel computation
        shared_ptr<TaskThreadPool> thread_pool_;
        int num_thread_;
        shared_ptr<Network> network_;
        shared_ptr<Context<State, Action> > context_;
        Index counter_;
        //queue<Tensor<TesnorCPU>* > batch_;
        mutex mutex_;
        int L_;
        int num_simulation_;
        float v_resign_;
        float inv_tau_;
        float epsilon_;
};

} //end of namespace Beta

#endif