#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <vector>
#include <map>
#include <string>
#include <queue>
#include <cmath>




#include "utils/utils.hpp"
#include "glog/logging.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "network/network.h"


#include "utils/graphviz.hpp"
#include "utils/utils.hpp"
#include "mcts/thread_pool.h"
#include "mcts/node.h"

using namespace std;
namespace Beta{




template<typename StateType, typename ActionType>
class Tree{
    typedef Node<StateType, ActionType> NodeDef;
    public:
        Tree(int L, int num_simulation, float tau, float v_resign, float epsilon, int num_thread, int board_size, int batch_size, int channels);
        ~Tree();

        void add_node(NodeDef * leaf_node, NodeDef* node);
        bool add_node(Index index);
        bool add_node(NodeDef* node);
        NodeDef* find(int index);
        void travel(NodeDef* root);

        void remove_child_node(NodeDef* node);

        void remove_itself_and_child(NodeDef*  node);
        
        void reset_root(NodeDef*  node);
        
        void select(NodeDef* root);
        
        void copy_to_batch(Node<StateType, ActionType>* node);




        void expand_and_evaluate();

        void backup(NodeDef* node);


        // compute pai
        void pai(NodeDef* root);

        void sample(NodeDef* root);



        void run(NodeDef* root);


        void serialize();

        void deserialize();
        

        void render();


        const size_t size();
        const Index counter();
        const int L();
        const queue<NodeDef* >* leafs();
        const std::map<Index, NodeDef* >* nodes();



    protected:
        std::map<Index, NodeDef* > nodes_;
        //queue<function<void (NodeDef* ) > > tasks_;
        queue<NodeDef* > leafs_;
        // multiple-thread for pararel computation
        shared_ptr<TaskThreadPool> thread_pool_;
        int num_thread_;
        Index counter_;
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