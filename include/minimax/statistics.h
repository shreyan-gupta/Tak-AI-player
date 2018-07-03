#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <vector>

using namespace std;

namespace Minimax {

struct Statistics{
  void print(int d){
    auto print_vec = [&](vector<int> &arr){
      for(int i=0; i<=d; ++i) cout << arr[i] << "\t";
      cout << endl;
    };
    cout << "STATICTICS" << endl;
    cout << "depth:    \t"; print_vec(depth);
    cout << "minimax_call: \t"; print_vec(num_minimax_call);
    cout << "prune_pv: \t"; print_vec(prune_pv);
    cout << "prune_killer1: \t"; print_vec(prune_killer1);
    cout << "prune_killer2: \t"; print_vec(prune_killer2);
    cout << "prune_other: \t"; print_vec(prune_other);
  }

  vector<int> depth = {0,1,2,3,4,5,6,7,8,9,10};
  vector<int> num_minimax_call = {0,0,0,0,0,0,0,0,0,0,0};
  vector<int> prune_pv = {0,0,0,0,0,0,0,0,0,0,0};
  vector<int> prune_killer1 = {0,0,0,0,0,0,0,0,0,0,0};
  vector<int> prune_killer2 = {0,0,0,0,0,0,0,0,0,0,0};
  vector<int> prune_other = {0,0,0,0,0,0,0,0,0,0,0};
};

} // namespace Minimax

#endif