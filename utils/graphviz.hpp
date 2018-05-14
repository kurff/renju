#ifndef __GRAPHVIZ_HPP__
#define __GRAPHVIZ_HPP__
#include "gvc.h"
#include <map>
#include "glog/logging.h"
using namespace std;
namespace Beta{
    template<typename T>
    class GraphViz{
        
        public:
            GraphViz(){
                gvc_ = gvContext();
                char* argv[4] = {{"exe"},{"-Kdot"},{"-Tpng"},{"-ograph.png"}};
                gvParseArgs(gvc_, 4 , argv);
                g_ = agopen("graph", Agdirected, 0);
            }
            ~GraphViz(){
                gvFreeLayout(gvc_, g_);
                agclose(g_);
                gvFreeContext(gvc_);
            }

            Agnode_t* add_node(T* node){
               Agnode_t* t = agnode(g_, node->name(), 1);
               nodes_.insert(std::make_pair(node->name(), t));
               return t;
            }



            Agedge_t* add_edge(T* parent, T* child){
                auto prt = nodes_.find(parent->name());
                auto cid = nodes_.find(child->name());
                if(prt == nodes_.end()){
                    LOG(INFO)<<"can not find node "<< parent->name();
                    return nullptr;
                }
                if(cid == nodes_.end()){
                    LOG(INFO)<<"can not find node" << child->name();
                    return nullptr;
                }
                Agedge_t* edge = agedge(g_,prt->second, cid->second, parent->name()+"_"+child->name(), 1);
                edges_.insert(std::make_pair(parent->name()+"_"+child->name(), edge ));
                return edge;

            }

            void render(){
                gvLayoutJobs(gvc_, g_);
                /* Write the graph according to -T and -o options */
                gvRenderJobs(gvc_, g_);
            }

        protected:
            Agraph_t *g_;
            std::map<string, Agnode_t*> nodes_;
            std::map<string, Agedge_t*> edges_;
            GVC_t *gvc_;

    };




}









#endif