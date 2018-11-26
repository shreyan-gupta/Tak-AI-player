#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <vector>

using namespace std;

namespace Minimax {

struct Statistics{
  Statistics(){
    clear();
  }

  void print(int d){
    auto print_vec = [&](vector<int> &arr){
      for(int i=0; i<=d; ++i) cerr << arr[i] << "\t";
      cerr << endl;
    };
    cerr << "STATICTICS" << endl;
    cerr << "depth:    \t"; print_vec(depth);
    cerr << "minimax_call: \t"; print_vec(num_minimax_call);
    cerr << "prune_pv: \t"; print_vec(prune_pv);
    cerr << "prune_killer1: \t"; print_vec(prune_killer1);
    cerr << "prune_killer2: \t"; print_vec(prune_killer2);
    cerr << "prune_other: \t"; print_vec(prune_other);
  }

  void clear(){
    depth = {0,1,2,3,4,5,6,7,8,9,10};
    num_minimax_call = {0,0,0,0,0,0,0,0,0,0,0};
    prune_pv = {0,0,0,0,0,0,0,0,0,0,0};
    prune_killer1 = {0,0,0,0,0,0,0,0,0,0,0};
    prune_killer2 = {0,0,0,0,0,0,0,0,0,0,0};
    prune_other = {0,0,0,0,0,0,0,0,0,0,0};
  }

  vector<int> depth;
  vector<int> num_minimax_call;
  vector<int> prune_pv;
  vector<int> prune_killer1;
  vector<int> prune_killer2;
  vector<int> prune_other;
};

} // namespace Minimax

#endif