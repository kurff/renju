#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <vector>
#include <map>
#include <string>
#include <queue>
#include <cmath>




#include "utils/utils.hpp"
#include "glog/logging.h"
#include "utils/utils.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "network/network.hpp"


#include "utils/graphviz.hpp"
#include "utils/utils.hpp"
#include "mcts/thread_pool.h"

#include "mcts/node.h"

using namespace std;
namespace Beta{




template<typename StateType, typename ActionType, typename ContextType>
class Tree{
    typedef Node<StateType, ActionType> NodeDef;
    typedef typename map<Index, NodeDef* >::iterator Iterator;
    public:
        Tree(int L, int num_simulation, float tau, float v_resign, float epsilon, int num_thread, int board_size, int batch_size, int channels):L_(L),  num_simulation_(num_simulation),counter_(0),  v_resign_(v_resign), num_thread_(num_thread){
            context_.reset(new ContextType(epsilon));
            network_.reset(new Network<StateType,ActionType>(board_size, batch_size, channels));
            inv_tau_ = 1.0f/tau;
            thread_pool_.reset(new TaskThreadPool(num_thread_));
            sample_.reset( new Sample() );
            graph_viz_.reset(new GraphViz<NodeDef>() );
        }
        ~Tree(){


        }

        void add_node(NodeDef * leaf_node, NodeDef* node){
            //leaf_node->
            int index = node->index();
            LOG(INFO)<< "adding "<< node->index()<<" node";
            nodes_[index] = node;
            leaf_node->insert_child(node, index);

            graph_viz_->add_node(node);
            graph_viz_->add_edge(leaf_node, node);

        }

        // for simplicity
        
        bool add_node(Index index){
            NodeDef* node = new NodeDef(index);
            graph_viz_->add_node(node);
            return add_node(node, index);
        }

        bool add_node(NodeDef* node){
           // LOG(INFO)<< "adding "<< index<<" node: "<< node->name();
            graph_viz_->add_node(node);
            nodes_[node->index()] = node;
            return true;
        }

        NodeDef* find(int index){
            auto it = nodes_.find(index);
            if(it == nodes_.end()){
                LOG(INFO)<< "can not find " << index;
                return nullptr;
            }else{
                LOG(INFO)<<"find "<< index;
                return it->second;
            }
        }



        void travel(NodeDef* root){       
            queue< NodeDef* > cache;
            cache.push(root);
            while(cache.size()){
                NodeDef* ele = cache.front();
                LOG(INFO)<<"visit "<< ele->name()<<" index: "<<ele->index()<<" child size: "<< ele->schild().size();
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
                LOG(INFO)<<"can not find node "<< node->name(); 
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
                LOG(INFO)<<"visit "<< ele->name()<<" index: "<<ele->index()<<" child size: "<< ele->child().size();
                float maxQ = 0;
                if(ele->child().size() ==0){
                    context_->get_legal_action(*(ele->state()));
                    for(int i = 0; i < context_->size_legal_action(); ++ i){
                        NodeDef* node = new NodeDef (context_->legal_state(i),"Node"+std::to_string(counter_));
                        add_node(ele, node);
                        leafs_.push(node);
                    }
                    //return;
                }
                float QU = 0.0f;
                for(auto child : ele->child() ){
                    child.second->get(QU);
                    if(maxQ <= QU){
                        maxQ = QU;
                        best = child.second;
                    }
                    cache.push(best);
                }
                cache.pop();
            }           
        }





        void copy_to_batch(Node<StateType, ActionType>* node){




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
                network_->forward(*(node->state()));
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
                node->update();
                cache.push(node->parent());
                cache.pop();
            }
        }


        // compute pai
        void pai(NodeDef* root){
            TensorCPU* ptr = root->compute_pai(inv_tau_, max_child_);
            context_->push(ptr);
        }

        void sample(NodeDef* root){
            
            validate_action_.clear();
            for(auto child : root->child() ){
                //child.second->action()->set
                validate_action_.push_back(child.second);
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


        void serialize(){

        }

        void deserialize(){
            
        }
        

        void render(){
            graph_viz_->render();
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
        shared_ptr<Network<StateType, ActionType> > network_;
        shared_ptr<ContextType> context_;
        Index counter_;
        //queue<Tensor<TesnorCPU>* > batch_;
        
        
        vector<NodeDef*> validate_action_;
        
        shared_ptr<Sample> sample_;
        mutex mutex_;
        int L_;
        int num_simulation_;
        float v_resign_;
        float inv_tau_;
        TIndex max_child_;

        shared_ptr<GraphViz<NodeDef> > graph_viz_;


};

} //end of namespace Beta

#endif