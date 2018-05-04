#ifndef __UTILS_HPP__
#define __UTILS_HPP__

// sample from 
#include "glog/logging.h"
#include <vector>
#include <cmath>
#include <random>
#include <memory>
using namespace std;


namespace Beta{
    class Dirichlet{
        public:
            Dirichlet(float alpha): alpha_(alpha){
                distribution_.reset(new gamma_distribution<float>(alpha_,1.0) );
                random_generator_.reset(new std::mt19937());
            }
            ~Dirichlet(){

            }

            void run(vector<float>& prior, int K){
                prior.clear();
                for(int i = 0; i < K; ++ i){
                    prior.push_back(distribution_->operator()(*random_generator_));
                }
                float sum = 0.0f;
                for(int i = 0; i < K; ++ i ){
                    sum += prior[i];
                }
                for(int i = 0; i < K; ++ i){
                    prior[i] /= sum;
                    //LOG(INFO)<<"prior: "<<i<<" "<<prior[i];
                }
            }
            template<typename Node>
            void run(vector<Node*>& children){
                vector<float> prior;
                run(prior, children.size());
                int c = 0;
                for(auto child : children){
                    child->action()->set_noise(prior[c]);
                    ++ c;
                }
            }
            const float alpha(){return alpha_;}
        protected:   
            float alpha_;
            std::shared_ptr<gamma_distribution<float> > distribution_;
            std::shared_ptr<std::mt19937> random_generator_;
    };

    class Sample{
        public: 
          Sample(){
              random_generator_.reset(new std::mt19937());        
          }
          ~Sample(){}
            template<typename Node>
            Node* run(const vector<Node*>& children){
                vector<int> distribution;
                for(auto & child: children){
                    distribution.push_back(static_cast<int>(1000.0f*child->action()->confidence()));
                }
                std::discrete_distribution<int > distri(distribution.begin(), distribution.end());
                int index = distri(*random_generator_);
                assert(index < children.size()&& index>=0);
                return children[index];
           }
        protected:
            std::shared_ptr<std::mt19937> random_generator_;
    };








}
#endif