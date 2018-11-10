#ifndef CORE_RENJU_ACTION_H__
#define CORE_RENJU_ACTION_H__

#include "core/action.h"

#include <vector>
using namespace std;

namespace Beta{
class RenJuAction: public Action{
    public:
        RenJuAction();
        ~RenJuAction();
        RenJuAction(int c, int y, int x);
        RenJuAction(const RenJuAction& action);

        const float prior();
        const float confidence();

        void calc_confidence();
        
        const float epsilon();
        const float noise();
        const int x();
        const int y();
        const int c();


        int* mutable_x();
        int* mutable_y();
        int* mutable_c();

        float* mutable_prior();
        float* mutable_confidence();
        float* mutable_noise();
        float* mutable_epsilon();



    protected:  
        int x_;
        int y_;
        int c_;
        float prior_;
        float confidence_;   
        float noise_;
        float epsilon_;        

};


}

#endif 