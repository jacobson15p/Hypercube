#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <random> 
#include <string>  
#include <sstream>
#include <map>

using namespace std;

class Hypercube{
    /*
    Hypercube class of degree d
    */

    public:
        int degree;

        Hypercube(int d){
            degree = d;
        }
        void printEdges(){
            /*
            Print all edges of a hypercube, represented as endpoints of edge
            */

            int nodes = pow(2,degree);
            for(int i = 0; i < nodes; i++){
                for(int j = 0; j < nodes; j++){
                    int comp = i ^ j;
                    for(int k = 0; k < degree; k++){
                        if(comp == pow(2,k)){
                            std::cout<< '(' << i << ',' << j << ')' << endl;
                        }
                    }
                }
            }
        }

        static vector<int> randPerm(vector<int> items){ 
            /*
            Randomly permute a vector

            Args:
                items: vector to be shuffled

            Returns:
                items: shuffled version of items
            */

            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(items.begin(),items.end(),std::default_random_engine(seed));

            return items;
        }

        vector<int> dim_order_routing(int src, int dst){
            /*
            Calculate shortest route between points on a hypercube

            Args:
                src: source node
                dst: destination node

            Returns:
                path: list of nodes representing path between src and dst
            */
            int curr = src;
            std::vector<int> path;
            path.push_back(src);
            for(int i = 0; i<degree; i++){
                if((curr & 0x01 << i)!=(dst & 0x01 << i)){
                    curr = curr^(0x01 << i);
                    path.push_back(curr);
                }
            }
            return path;
        }
        vector<vector<int> > allpath_routing(int src,int dst){
            /*
            Calculate all shortest paths between points on a hypercube

            Args:
                src: source node
                dst: destination node

            Returns:
                paths: vector of all paths between src and dst
            */
            std::vector<vector<int> > paths;
            std::vector<int> diff;

            //Calculate bits which are different
            for(int i = 0; i<degree; i++){
                if((src & 0x01 << i)!=(dst & 0x01 << i)){
                    diff.push_back(i);
                }
            }
            sort(diff.begin(),diff.end());
            
            //Calculate path for each permutation
            do{
                std::vector<int> path;
                int curr = src;
                path.push_back(src);
                for(int i = 0; i<diff.size(); i++){
                    curr = curr^(0x01 << diff[i]);
                    path.push_back(curr);
                }
                paths.push_back(path);
            }while(std::next_permutation(diff.begin(),diff.end()));

            return paths;

        }

        double bottleneckEstimate(vector<vector<int>> paths,double bw){
            /*
            Calculate maximum throughput of a flow with bottleneck estimation

            Args:
                paths: list of all paths in flow
                bw: bandwidth of links

            Returns:
                through: throughput through bottleneck link
            */

           std::map<vector<int>,int> links;
           for(int i = 0; i < paths.size(); ++i){
               for(int j = 0; j<paths[i].size()-1;++j){
                   std::vector<int> link = {paths[i][j],paths[i][j+1]};
                   ++links[link];
               }
           }
           int max_usage = 0;
           for(auto it = links.begin(); it != links.end(); ++it){
               if(it->second > max_usage){
                   max_usage = it->second;
               }
           }
           return bw/max_usage;
        }
};

int main() {
    Hypercube cube(5);
    //cube.printEdges();
    vector<int> path = cube.dim_order_routing(1,13);
    std::cout<<'(';
    for(int i = 0; i < path.size(); i++){
        std::cout<<path[i];
        if(i != path.size()-1){
            std::cout<<',';
        }
        else{
            std::cout<<')'<< endl;
        }
    }
    vector<vector<int> > paths = cube.allpath_routing(6,28);
    for(int i = 0; i < paths.size(); i++){
        std::cout<<'(';
        for(int j = 0; j < paths[0].size(); j++){
            std:: cout<<paths[i][j];
            if(j != paths[0].size()-1){
                std::cout<<',';
            }
            else{
                std::cout<<')'<< endl;
            }
        }

    }
    vector<int> bits = {0,1,2,3,4,5};
    std::cout<<'(';
    bits = Hypercube::randPerm(bits);
    for(int i = 0; i < bits.size(); ++i){
        if(i != bits.size()-1){
            std::cout<<bits[i]<<',';
        }
        else{
            std::cout<<bits[i]<<')'<<endl;
        }

    }

    vector<vector<int>> all_paths = {cube.dim_order_routing(1,13),cube.dim_order_routing(1,5)};
    double bw = cube.bottleneckEstimate(all_paths,1000000000);
    std::cout<<bw<<endl;

    return 0;
}