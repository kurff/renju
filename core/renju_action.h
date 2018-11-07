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
        
        //return (1-epsilon_)*prior_ + epsilon_*noise_;
        
        const float epsilon();
        const float noise();
        const int x();
        const int y();
        const int c();


        int* mutable_x();
        int* mutable_y();

    protected:  
        int x_; // the 
        int y_;
        int c_;
        float prior_;
        float confidence_;  // confidence_ = (1-epsilon)* w + epsilon * noise_; 
        float noise_;
        float epsilon_;        

};


}

#endif 