#include "segment_tree.h"
#include <iostream>

int main() {
  int n;
  std::cin >> n;
  std::vector<int> odd_indexes_a, even_indexes_a;
  int a;
  for (int i = 0; i < n; ++i) {
    std::cin >> a;
    if (i % 2 == 0) {
      odd_indexes_a.push_back(a);
    } else {
      even_indexes_a.push_back(a);
    }
  }
  SegmentTree segment_tree_on_sum_odd_indexes(odd_indexes_a);
  SegmentTree segment_tree_on_sum_even_indexes(even_indexes_a);

  int l, r, i, j, request, m;
  std::cin >> m;
  for (int k = 0; k < m; ++k) {
    std::cin >> request;
    if (request == 0) {
      std::cin >> i >> j;
      --i;
      if (i % 2 == 0) {
        segment_tree_on_sum_odd_indexes.Set(i / 2, j);
      } else {
        segment_tree_on_sum_even_indexes.Set(i / 2, j);
      }
    } else {
      std::cin >> l >> r;
      --l, --r;
      int positive_sum, negative_sum;
      if (l % 2 == 0) {
        positive_sum =
            segment_tree_on_sum_odd_indexes.Get((l + 1) / 2, r / 2);
        if (l == r) {
          negative_sum = 0;
        } else {
          negative_sum =
              segment_tree_on_sum_even_indexes.Get((l + 1) / 2, (r - 1) / 2);
        }
      } else {
        positive_sum =
            segment_tree_on_sum_even_indexes.Get(l / 2, (r - 1) / 2);
        if (l == r) {
          negative_sum = 0;
        } else {
          negative_sum =
              segment_tree_on_sum_odd_indexes.Get((l + 1) / 2, r / 2);
        }
      }
      std::cout << positive_sum - negative_sum << "\n";
    }
  }
  return 0;
}