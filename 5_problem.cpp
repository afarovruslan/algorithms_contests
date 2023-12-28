#include <iostream>
#include <vector>

class ThreeDimFenwickTree {
 public:
  explicit ThreeDimFenwickTree(int n)
      : size_(n),
        tree_(n, std::vector<std::vector<int>>(n, std::vector<int>(n, 0))) {}
  void Update(int x, int y, int z, int delta) {
    for (int i = x; i < size_; i = G(i)) {
      for (int j = y; j < size_; j = G(j)) {
        for (int k = z; k < size_; k = G(k)) {
          tree_[i][j][k] += delta;
        }
      }
    }
  }
  int GetSum(int x1, int y1, int z1, int x2, int y2, int z2) {
    return GetPrefSum(x2, y2, z2) - GetPrefSum(x1 - 1, y2, z2) -
        GetPrefSum(x2, y1 - 1, z2) - GetPrefSum(x2, y2, z1 - 1) +
        GetPrefSum(x2, y1 - 1, z1 - 1) + GetPrefSum(x1 - 1, y2, z1 - 1) +
        GetPrefSum(x1 - 1, y1 - 1, z2) - GetPrefSum(x1 - 1, y1 - 1, z1 - 1);
  }

 private:
  int size_;
  std::vector<std::vector<std::vector<int>>> tree_;

  static int F(int i) { return i & (i + 1); }
  static int G(int i) { return i | (i + 1); }
  int GetPrefSum(int x, int y, int z) {
    int sum = 0;
    for (int i = x; i >= 0; i = F(i) - 1) {
      for (int j = y; j >= 0; j = F(j) - 1) {
        for (int k = z; k >= 0; k = F(k) - 1) {
          sum += tree_[i][j][k];
        }
      }
    }
    return sum;
  }
};

int main() {
  int n;
  std::cin >> n;
  int m;
  std::cin >> m;
  ThreeDimFenwickTree three_dim_fenwick_tree(n);
  int x, y, z, x1, y1, z1, x2, y2, z2, k;
  while (m != 3) {
    if (m == 1) {
      std::cin >> x >> y >> z >> k;
      three_dim_fenwick_tree.Update(x, y, z, k);
    } else {
      std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
      std::cout << three_dim_fenwick_tree.GetSum(x1, y1, z1, x2, y2, z2)
                << '\n';
    }
    std::cin >> m;
  }
  return 0;
}
