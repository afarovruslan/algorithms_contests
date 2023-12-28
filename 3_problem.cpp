#include <iostream>
#include <vector>

int max_x = 200000;

class SegmentTreeOnMax {
 private:
  int size_a_;
  std::vector<int> tree_;
  std::vector<int> a_;
  std::vector<int> indexes_;

  void Build(int vertex, int tl, int tr) {
    if (tl == tr) {
      tree_[vertex] = a_[tl];
      indexes_[vertex] = tl + 1;
    } else {
      int tm = (tl + tr) / 2;
      Build(2 * vertex, tl, tm);
      Build(2 * vertex + 1, tm + 1, tr);
      tree_[vertex] = std::max(tree_[2 * vertex], tree_[2 * vertex + 1]);
    }
  }
  void Update(int vertex, int tl, int tr, int ind, int new_val) {
    if (tl == tr) {
      tree_[vertex] = new_val;
    } else {
      int tm = (tl + tr) / 2;
      if (tm >= ind) {
        Update(vertex * 2, tl, tm, ind, new_val);
      } else {
        Update(vertex * 2 + 1, tm + 1, tr, ind, new_val);
      }
      tree_[vertex] = std::max(tree_[2 * vertex], tree_[2 * vertex + 1]);
    }
  }
  int MostLeftUpperBound(int vertex, int x) {
    int left_son = 2 * vertex;
    int right_son = 2 * vertex + 1;
    if (left_son <= 4 * size_a_ - 1 and tree_[left_son] >= x) {
      return MostLeftUpperBound(left_son, x);
    }
    if (right_son <= 4 * size_a_ - 1 and tree_[right_son] >= x) {
      return MostLeftUpperBound(right_son, x);
    }
    return indexes_[vertex];
  }
  std::vector<int> SegmentCoverage(int ind, int x) {
    int vertex = 1;
    int tl = 0, tr = size_a_ - 1;
    std::vector<int> vertexes;
    int left_son = 2 * vertex;
    int right_son = 2 * vertex + 1;
    while (left_son <= 4 * size_a_ - 1) {
      int tm = (tl + tr) / 2;
      if (tm >= ind and right_son <= 4 * size_a_ - 1) {
        if (tree_[right_son] >= x) {
          vertexes.push_back(right_son);
        }
        if (tree_[left_son] >= x) {
          vertex = left_son;
          tr = tm;
        } else {
          break;
        }
      } else if (tree_[right_son] >= x) {
        vertex = right_son;
        tl = tm + 1;
      } else {
        break;
      }
      left_son = 2 * vertex;
      right_son = 2 * vertex + 1;
    }
    if (tl == tr and tl >= ind and tree_[vertex] >= x) {
      vertexes.push_back(vertex);
    }
    return vertexes;
  }

 public:
  SegmentTreeOnMax(std::vector<int>& arr, int size)
      : size_a_(size),
        tree_(4 * size_a_, -max_x),
        a_(arr),
        indexes_(4 * size_a_, 0) {
    indexes_[0] = -1;
    Build(1, 0, size_a_ - 1);
  }
  void Set(int i, int x) { Update(1, 0, size_a_ - 1, i, x); }
  int Get(int ind, int x) {
    std::vector<int> vertexes = SegmentCoverage(ind, x);
    if (vertexes.empty()) {
      return -1;
    }
    int numb_vertex = static_cast<int>(vertexes.size()) - 1;
    while (numb_vertex >= 0 and tree_[vertexes[numb_vertex]] < x) {
      --numb_vertex;
    }
    int vertex = vertexes[numb_vertex];
    if (tree_[vertex] < x) {
      return -1;
    }
    return MostLeftUpperBound(vertex, x);
  }
};

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }
  SegmentTreeOnMax segment_tree_on_max(a, n);
  int request, ind, x;
  for (int i = 0; i < m; ++i) {
    std::cin >> request >> ind >> x;
    if (request == 0) {
      segment_tree_on_max.Set(ind - 1, x);
    } else {
      std::cout << segment_tree_on_max.Get(ind - 1, x) << '\n';
    }
  }
}