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
    
    public:
        Node(string name):N_(0.0f),W_(0.0f),Q_(0.0f),P_(0.0f), index_(0), name_(name), keep_flag_(true), parent_(nullptr){
            child_.clear();
            node_state_ = shared_ptr<State>(new State());

            
        }

        Node(const Node<State>& node){
            N_ = node.N();
            W_ = node.W();
            Q_ = node.Q();
            U_ = node.U();
            v_ = node.v();
            keep_flag_ = node.keep_flag(); // if flag == TRUE, keep else remove
            node_state_ = node.state();


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
        
        void get(Node<State>& node){
            lock_guard<mutex> lock(mutex_);



        }


        unsigned long & sindex(){
            lock_guard<mutex> lock(mutex_);
            return index_;
        }
        map<unsigned long, Node<State>* >& schild(){
            lock_guard<mutex> lock(mutex_);
            return child_;
        }
        Node<State>* & sparent(){
            lock_guard<mutex> lock(mutex_);
            return parent_;
        }
        bool & sflag(){
            lock_guard<mutex> lock(mutex_);
            return keep_flag_;
        }

        State& sstate(){
            lock_guard<mutex> lock(mutex_);
            return *(node_state_.get());
        }

        int& scircle_index(){
            lock_guard<mutex> lock(mutex_);
            return circle_index_;
        }

        int& sbatch_index(){
            lock_guard<mutex> lock(mutex_);
            return batch_index_;
        }



        void set_child(){
            lock_guard<mutex> lock(mutex_);
            
            
        }

        void set_index(int index){
             lock_guard<mutex> lock(mutex_);
             index_ = index;
        }
        void set_parent(Node<State>* parent){
            lock_guard<mutex> lock(mutex_);
            parent_ = parent;
        }
        void set_flag(int flag){
            lock_guard<mutex> lock(mutex_);
            flag_ = flag;
        }


        float& sN(){lock_guard<mutex> lock(mutex_);return N_;}
        float& sW(){lock_guard<mutex> lock(mutex_);return W_;}
        float& sQ(){lock_guard<mutex> lock(mutex_);return Q_;}
        float& sP(){lock_guard<mutex> lock(mutex_);return P_;}
        float& sU(){lock_guard<mutex> lock(mutex_);return U_;}
        float& sv(){lock_guard<mutex> lock(mutex_);return v_;}

        void set_action_w(float w){
            action_->set_w(w);
        }

        void set_action_noise(float noise){
            action_->set_noise(noise);
        }

        void set_action_confidence(float confidence){
            action_->set_confidence(confidence);
        }


        const float N(){lock_guard<mutex> lock(mutex_);return N_;}
        const float W(){lock_guard<mutex> lock(mutex_);return W_;}
        const float Q(){lock_guard<mutex> lock(mutex_);return Q_;}
        const float P(){lock_guard<mutex> lock(mutex_);return P_;}
        const float U(){lock_guard<mutex> lock(mutex_);return U_;}
        const float v(){lock_guard<mutex> lock(mutex_);return v_;}
        const string name(){lock_guard<mutex> lock(mutex_);return name_;}
        const unsigned long index(){lock_guard<mutex> lock(mutex_);return index_;}
        const map<unsigned long, Node<State> * > child(){lock_guard<mutex> lock(mutex_);return child_;}
        const Node<State>* parent(){lock_guard<mutex> lock(mutex_);return parent_;}
        const bool keep_flag(){lock_guard<mutex> lock(mutex_);return keep_flag_;}
        const State& state(){lock_guard<mutex> lock(mutex_);return *(node_state_.get());}



        



    

    protected:
        map<unsigned long, Node<State, Action> * > child_;
        Node<State, Action>* parent_;
        shared_ptr<State> node_state_;
        shared_ptr<Action> action_; // edge of 
        unsigned long index_;
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
    typedef Node<State, Action> NodeDef;
    typedef typename map<unsigned long, NodeDef* >::iterator Iterator;
    typedef typename queue<NodeDef * >::iterator QIterator;
    typedef typename queue<Tensor<TesnorCPU>* >::iterator TIterator;
    public:
        Tree(int L, int num_simulation, float tau, float v_resign, float epsilon, int num_thread):L_(L),  num_simulation_(simulation),counter_(0),  v_resign_(v_resign), epsilon_(epsilon), num_thread_(num_thread){

            context_ = shared_ptr<Context>(new ChessContext());
            network_ = shared_ptr<Network>(new Network());
            inv_tau_ = 1.0f/tau;
            thread_pool_.reset(new TaskThreadPool(num_thread_));
            

        }
        ~Tree(){

        }

        void add_node(NodeDef * leaf_node, NodeDef* node){
            //leaf_node->

            DLOG(INFO)<< "adding "<< counter_<<" node";
            node->sindex() = counter_;
            nodes_[counter_] = node;
            leaf_node->schild().insert(std::pair<unsigned long, NodeDef* >(counter_,node));
            node->sparent()=leaf_node;

            ++ counter_;
            
            //tree_.push_back(node);
            //leaf_node->child_ = node;
        }

        bool add_node(NodeDef* node){
            DLOG(INFO)<< "adding "<< counter_<<" node: "<< node->name();
            Iterator it = nodes_.find(node->index());
            if(it != nodes_.end()){
                LOG(INFO)<<" tree already has such node";
                return false;
            }
            node->sindex() = counter_;
            nodes_[counter_] = node;
            ++ counter_;
            return true;
        }

        NodeDef* find(int index){
            Iterator it = nodes_.find(index);
            if(it == nodes_.end()){
                LOG(INFO)<< "can not find " << index;
            }else{
                LOG(INFO)<<"find "<< index;
            }
            return it->second;
        }


        void travel(NodeDef* root){       
            queue<NodeDef* > cache;
            cache.push(root);
            while(cache.size()){
                NodeDef* ele = cache.front();
                DLOG(INFO)<<"visit "<< ele->name()<<" index: "<<ele->index()<<" child size: "<< ele->schild().size();
                for(Iterator it = ele->schild().begin(); it != ele->schild().end(); ++ it){
                    cache.push(it->second);
                }
                cache.pop();
            }
        }

        // remove given node and its descendant
        void clear_node(NodeDef*  node){
            Iterator it = nodes_.find(node->index());
            if(it == nodes_.end()){
                DLOG(INFO)<<"can not find node "<< node->name(); 
                return;
            }
            Iterator it_child = node->sparent()->schild().find(node->index());
            node->sparent()->schild().erase(it_child);
            queue<NodeDef* > cache;
            cache.push(node);

            // get all nodes in the sub-tree of node and remove these node
            // 
            while(cache.size()){
                NodeDef* ele = cache.front();
                for(Iterator it = ele->schild().begin(); it != ele->schild().end(); ++ it){
                    cache.push(it->second);
                } 
                nodes_.erase(nodes_.find(ele->index()));
                cache.pop();
            }
        }

        void reset_root(NodeDef*  node){
            nodes_.clear();
            add_node(node);
        }

        void select(NodeDef* root){
            queue<NodeDef* > cache;
            cache.push(root);
            NodeDef* best;
            while(cache.size()){
                NodeDef* ele = cache.front();
                DLOG(INFO)<<"visit "<< ele->name()<<" index: "<<ele->index()<<" child size: "<< ele->schild().size();
                float maxQ = 0;


                if(ele->schild().size() ==0){

                    // push leaf node
                    {
                        //lock_guard<mutex> lock(mutex_);
                        
                    }


                    contex_->get_legal_action(ele->state());
                    for(int i = 0; i < context_->size_legal_action(); ++ i){
                        NodeDef* node = new NodeDef (context_->get_legal_action(),"Node"+std::to_string(counter_));

                        add_node(ele, node);
                        leafs_.push(node);

                    }
                    //return;

                }

                for(Iterator it = ele->schild().begin(); it != ele->schild().end(); ++ it){
                    if(maxQ <= it->second->Q() + it->second->U()){
                        maxQ = it->second->Q() + it->second->U();
                        best = it->second;
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

                
                //node->scircle_index() = counter % batch_size_;
                //node->sbatch_index() = counter / batch_size_;

                
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
        const unsigned long counter(){return counter_;}
        const int L(){return L_;}
        const queue<NodeDef* > leafs(){return leafs_;}
        const std::map<unsigned long, NodeDef* > nodes(){return nodes_;}



    protected:
        std::map<unsigned long, NodeDef* > nodes_;
        
        //queue<function<void (NodeDef* ) > > tasks_;
        queue<NodeDef* > leafs_;

        // multiple-thread for pararel computation

        shared_ptr<TaskThreadPool> thread_pool_;
        int num_thread_;

        shared_ptr<Network> network_;
        shared_ptr<Context<State, Action> > context_;
        unsigned long counter_;

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