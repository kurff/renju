#ifndef CORE_ACTION_H_
#define CORE_ACTION_H_




namespace Beta{
    class Action{
        public:
            Action(){

            }

            ~Action(){

            }
            bool legal(){return legal_;}
        protected:
            bool legal_;
            
            
    };
}



#endif