#ifndef MCTS_MCTS_H_
#define MCTS_MCTS_H_

#include <queue>
#include "tree.hpp"
#include "proto/beta.pb.h"
#include "core/parameters.hpp"
namespace Beta{
template <typename StateType, typename ActionType, typename ContextType>
//multiple thread run
class MCTS{
    typedef Node<StateType, ActionType> NodeDef;
    public:
        MCTS();
        ~MCTS();

        bool init(Parameters* parameters);
        void run();
        
    protected:
        shared_ptr<ContextType > context_;
        shared_ptr<Tree<StateType, ActionType, ContextType> > tree_;
        




};
}
#endif