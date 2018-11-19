
#ifndef CORE_RENJU_CONTEXT_H_
#define CORE_RENJU_CONTEXT_H_
#include "core/context.h"
namespace Beta{

  template<typename StateType, typename ActionType>
  class RenJuContext: public Context<StateType, ActionType>{
    public:
      RenJuContext(float epsilon);

      ~RenJuContext();

      void get_legal_action(const StateType& state);

      protected:
        

  };

}




#endif