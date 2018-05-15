#include "mcts/mcts.hpp"
#include "core/action.hpp"
#include "core/context.hpp"
#include "core/state.hpp"
#include "core/parameters.hpp"
using namespace Beta;

typedef MCTS<ChessState, ChessAction,ChessContext<ChessState,ChessAction>, CPUContext > MCTSDef;

int main(){
    MCTSDef* mcts = new MCTSDef();

    Parameters* parameters = global_parameters();
    read_from_text("parameters.txt", parameters);

    mcts->init(parameters);
    //mcts->run();
    return 0;
}
