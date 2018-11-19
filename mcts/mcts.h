#ifndef MCTS_MCTS_H_
#define MCTS_MCTS_H_


#include "tree.h"
#include "proto/beta.pb.h"
#include "core/parameters.h"

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
        std::shared_ptr<ContextType > context_;
        std::shared_ptr<Tree<StateType, ActionType> > tree_;

        




};
}
#endif