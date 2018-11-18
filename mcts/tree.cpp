#include <vector>
#include <map>
#include <string>
#include <queue>
#include <cmath>


#include "mcts/tree.h"

using namespace std;
namespace Beta{
    template<typename StateType, typename ActionType>
    Tree<StateType, ActionType>::Tree(int L, int num_simulation, float tau, float v_resign, float epsilon, int num_thread, int board_size, int batch_size, int channels):L_(L),  num_simulation_(num_simulation),counter_(0),  v_resign_(v_resign), num_thread_(num_thread){
            context_.reset(new ContextType(epsilon));
            network_.reset(new Network<StateType,ActionType>(board_size, batch_size, channels));
            inv_tau_ = 1.0f/tau;
            //thread_pool_.reset(new TaskThreadPool(num_thread_));
            sample_.reset( new Sample() );
            graph_viz_.reset(new GraphViz<NodeDef>() );
    }
    template<typename StateType, typename ActionType>
    Tree<StateType, ActionType>::~Tree(){


    }
    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::add_node(NodeDef * leaf_node, NodeDef* node){
        int index = node->index();
        LOG(INFO)<< "adding "<< node->index()<<" node";
        nodes_[index] = node;
        leaf_node->insert_child(node, index);
        graph_viz_->add_node(node);
        graph_viz_->add_edge(leaf_node, node);

    }

        // for simplicity
    template<typename StateType, typename ActionType>
    bool Tree<StateType, ActionType>::add_node(Index index){
        NodeDef* node = new NodeDef(index);
        graph_viz_->add_node(node);
        return add_node(node, index);
    }

    template<typename StateType, typename ActionType>
    bool Tree<StateType, ActionType>::add_node(NodeDef* node){
        // LOG(INFO)<< "adding "<< index<<" node: "<< node->name();
        graph_viz_->add_node(node);
        nodes_[node->index()] = node;
        return true;
    }

    template<typename StateType, typename ActionType>
    NodeDef* Tree<StateType, ActionType>::find(int index){
        auto it = nodes_.find(index);
            if(it == nodes_.end()){
                LOG(INFO)<< "can not find " << index;
                return nullptr;
            }else{
                LOG(INFO)<<"find "<< index;
                return it->second;
            }
        }

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::travel(NodeDef* root){       
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

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::remove_child_node(NodeDef* node){
            node->remove_child();
    }
        // remove given node and its descendant
    
    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::remove_itself_and_child(NodeDef*  node){
        auto it = nodes_.find(node->index());
        if(it == nodes_.end()){
            LOG(INFO)<<"can not find node "<< node->name(); 
            return;
        }
        node->parent()->remove_one_child(node->index());
        queue<NodeDef* > cache;
        cache.push(node);

        while(cache.size()){
            NodeDef* ele = cache.front();
            for(auto x : ele->child()){
                cache.push(x.second);
            } 
            nodes_.erase(ele->index());
            cache.pop();
        }
    }

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::reset_root(NodeDef*  node){
        nodes_.clear();
        add_node(node, node->index());
    }

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::select(NodeDef* root){
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



    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::copy_to_batch(Node<StateType, ActionType>* node){




    }



    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::expand_and_evaluate(){
        while(leafs_.size()){
            NodeDef* node = leafs_.front();                
            network_->forward(*(node->state()));
            backup(node);
            leafs_.pop();
        }
            // push leaf node into leafs_ for evaluation
    }
    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::backup(NodeDef* node){
        queue<NodeDef* > cache;
        cache.push(node);
        while(cache.size()){
            NodeDef* node = cache.front();
            node->update();
            cache.push(node->parent());
            cache.pop();
        }
    }


    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::pai(NodeDef* root){
        Blob<float>* ptr = root->compute_pai(inv_tau_, max_child_);

    }

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::sample(NodeDef* root){
        validate_action_.clear();
        for(auto child : root->child() ){
            validate_action_.push_back(child.second);
        }
    }


    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::run(NodeDef* root){
        for(int i = 0; i < num_simulation_; ++ i){
            select(root);
            expand_and_evaluate();
        }

        pai(root);

    }

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::serialize(){

    }

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::deserialize(){
            
    }
        

    template<typename StateType, typename ActionType>
    void Tree<StateType, ActionType>::render(){
        graph_viz_->render();
    }

    template<typename StateType, typename ActionType>
    const size_t Tree<StateType, ActionType>::size(){
        return nodes_.size();
    }
    template<typename StateType, typename ActionType>
    const Index Tree<StateType, ActionType>::counter(){
        return counter_;
    }
    template<typename StateType, typename ActionType>
    const int Tree<StateType, ActionType>::L(){
        return L_;
    }
    template<typename StateType, typename ActionType>
    queue<NodeDef* >* Tree<StateType, ActionType>::leafs(){
        return &leafs_;
    }
    template<typename StateType, typename ActionType>
    std::map<Index, NodeDef* >* Tree<StateType, ActionType>::nodes(){
        return &nodes_;
    }

} //end of namespace Beta

