#ifndef MCTS_NODE_H_
#define MCTS_NODE_H_


#include <string>
#include <memory>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "core/common.h"
#include "caffe/caffe.hpp"
using namespace std;
using namespace caffe;
namespace Beta{
template<typename StateType, typename ActionType>
class Node{
    
    public:
        Node(string name, Index index, float N, float W, float Q, float P, float U, float v, bool keep, 
        int batch_index, int circle_index, int child_index);
        Node(string name);
        Node(string name, Index index);
        Node(Index index);
        Node(const Node<StateType, ActionType>& node);
        ~Node();
        // compute features using 
        void compute_feature();
        void insert_child(Node<StateType, ActionType>* node, Index index);
        void remove_all_child(){child_.clear(); }
        void remove_one_child(Index index){ child_.erase(index);}

        void set(float N, float W, float Q, float P, float U, float v){
            lock_guard<mutex> lock(mutex_);
            N_ = N; W_ = W; Q_ = Q;
            P_ = P; U_ = U; v_ = v;
        }
        void get(float* N, float* W, float* Q, float *P, float *U, float* v){
            lock_guard<mutex> lock(mutex_);
            *N = N_; *W = W_; *Q = Q_;
            *P = P_; *U = U_; *v = v_;
        }

        void update(){
            lock_guard<mutex> lock(mutex_);
            N_ += 1;
            W_ += v_;
            Q_ = W_/ N_;
        }

        Blob* compute_pai(float inv_tau, TIndex max_child);
        // {
        //     lock_guard<mutex> lock(mutex_);
        //     float sum = 0.0f;
            
        //     TensorCPU* tensor = new TensorCPU();
        //     tensor->Reshape(vector<TIndex>{1,max_child});
        //     float * p = tensor->mutable_data<float>();
        //     for(auto child : child_ ){
        //        sum += pow(child.second->N(), inv_tau);
        //     }
        //     for(auto child: child_){
        //         float value = child.second->N() / sum;
        //         p[child.second->child_index()] = value;
        //     }
        //     return tensor;
        // }

        void serialize(const string& file);

        void deserialize(const string& file);

    
    public:
        const map<Index, Node<StateType, ActionType>*  > child(){return child_;}
        const Node<StateType, ActionType>* parent(){return parent_;}
        const shared_ptr<StateType> state(){ return state_;}
        const shared_ptr<ActionType> action(){return action_;}
        const Index index(){return index_;}
        const string name(){return name_;}
        const float N(){return N_;}
        const float W(){return W_;}
        const float Q(){return Q_;}
        const float P(){return P_;}
        const float U(){return U_;}
        const float v(){return v_;}
        const bool keep(){return keep_;}

        const int batch_index(){return batch_index_;}
        const int circle_index(){return circle_index_;}
        const int child_index(){return child_index_;}

        map<Index, Node<StateType, ActionType>* >* mutable_child(){return & child_;}
        Node<StateType, ActionType>** mutable_parent(){return & parent_;}
        Index* mutable_index(){return &index_;}
        string* mutable_name(){return &name_;}
        float* mutable_N(){return &N_;}
        float* mutable_W(){return &W_;}
        float* mutable_Q(){return &Q_;}
        float* mutable_P(){return &P_;}
        float* mutable_U(){return &U_;}
        float* mutable_v(){return &v_;}
        bool* mutable_keep(){return &keep_;}
        int* mutable_batch_index(){return &batch_index_;}
        int* mutable_circle_index(){return &circle_index_;}
        int* mutable_child_index(){return &child_index_;}

    protected:
        map<Index,  Node<StateType, ActionType>*  > child_;
        Node<StateType, ActionType>* parent_;
        shared_ptr<StateType> state_;
        shared_ptr<ActionType> action_; // edge of 
        Index index_;
        string name_;
        float N_;
        float W_;
        float Q_;
        float P_;
        float U_;
        float v_;
        bool keep_; // if flag == TRUE, keep else remove
        // used to cache tensor
        int batch_index_;
        int circle_index_;
        int child_index_;
        mutex mutex_;
        //static int index;
};


}
#endif