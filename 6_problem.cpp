#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Point {
  long long x;
  long long y;
  long long w;
  int last_ind;

  Point(long long x, long long y, long long w, int last_ind)
      : x(x), y(y), w(w), last_ind(last_ind) {}
  Point(const Point& last_point, int new_ind)
      : x(last_point.x), y(last_point.y), w(last_point.w), last_ind(new_ind) {}
};

struct Comp1 {
  bool operator()(const Point& p1, const Point& p2) {
    if (p1.x != p2.x) {
      return p1.x < p2.x;
    }
    return p1.y < p2.y;
  }
};

struct Comp2 {
  bool operator()(const Point& p1, const Point& p2) { return p1.y < p2.y; }
};

static int F(int i) { return i & (i + 1); }
static int G(int i) { return i | (i + 1); }

class FenwickTree {
 public:
  explicit FenwickTree(const std::vector<Point>& array, const int kShiftInd)
      : size_(static_cast<int>(array.size())),
        arr_(array),
        new_ind_(size_, 0),
        shift_ind_(kShiftInd) {
    Sort();
    long long sum;
    for (int i = 0; i < size_; ++i) {
      sum = 0;
      for (int j = F(i); j <= i; ++j) {
        sum += arr_[j].w;
      }
      tree_.push_back(sum);
    }
  }
  void Set(int ind, long long new_weight) {
    ind -= shift_ind_;
    int new_ind = new_ind_[ind];
    long long delta_weight = new_weight - arr_[new_ind].w;
    arr_[new_ind].w = new_weight;
    Update(new_ind, delta_weight);
  }
  long long GetSum(int ry) {
    int ind_y = LowerBoundByY(ry);
    return GetPrefSum(ind_y);
  }

 private:
  int size_;
  std::vector<Point> arr_;
  std::vector<long long> tree_;
  std::vector<int> new_ind_;
  int shift_ind_;
  long long GetPrefSum(int r) {
    long long ans = 0;
    for (int i = r; i >= 0; i = F(i) - 1) {
      ans += tree_[i];
    }
    return ans;
  }
  int LowerBoundByY(int ry) {
    int l = -1, r = size_, m;
    while (r - l > 1) {
      m = (l + r) / 2;
      if (arr_[m].y <= ry) {
        l = m;
      } else {
        r = m;
      }
    }
    return l;
  }
  void Update(int ind, long long delta_weight) {
    for (int i = ind; i < size_; i = G(i)) {
      tree_[i] += delta_weight;
    }
  }
  void Sort() {
    std::sort(arr_.begin(), arr_.end(), Comp2());
    for (int i = 0; i < size_; ++i) {
      new_ind_[arr_[i].last_ind] = i;
    }
  }
};

class FenwickFenwickTree {
 public:
  explicit FenwickFenwickTree(std::vector<Point>& pointers)
      : size_(static_cast<int>(pointers.size())),
        pointers_(pointers),
        new_ind_(std::vector<int>(size_, 0)) {
    Sort();
    std::vector<Point> tmp;
    for (int i = 0; i < static_cast<int>(pointers.size()); ++i) {
      int f_i = F(i);
      for (int j = f_i; j <= i; ++j) {
        tmp.emplace_back(Point(pointers[j], j - f_i));
      }
      fenwick_tree_.emplace_back(FenwickTree(tmp, f_i));
      tmp.clear();
      tmp.shrink_to_fit();
    }
  }
  long long Get(int rx, int ry) {
    long long sum = 0;
    int ind_x = LowerBoundByX(rx);
    for (int i = ind_x; i >= 0; i = F(i) - 1) {
      sum += fenwick_tree_[i].GetSum(ry);
    }
    return sum;
  }
  void Set(int ind, long long x) {
    int new_ind = new_ind_[ind];
    for (int i = new_ind; i < size_; i = G(i)) {
      fenwick_tree_[i].Set(new_ind, x);
    }
  }

 private:
  int size_;
  std::vector<FenwickTree> fenwick_tree_;
  std::vector<Point>& pointers_;
  std::vector<int> new_ind_;

  int LowerBoundByX(int rx) {
    int l = -1, r = size_, m;
    while (r - l > 1) {
      m = (l + r) / 2;
      if (pointers_[m].x <= rx) {
        l = m;
      } else {
        r = m;
      }
    }
    return l;
  }
  void Sort() {
    std::sort(pointers_.begin(), pointers_.end(), Comp1());
    for (int i = 0; i < size_; ++i) {
      new_ind_[pointers_[i].last_ind] = i;
    }
  }
};

void GetNumbers(size_t l, const std::string& request, int& numb1, int& numb2) {
  std::string str_numb1, str_numb2;
  size_t i = l;
  while (request[i] != ' ') {
    str_numb1 += request[i];
    ++i;
  }
  ++i;
  while (i < request.size()) {
    str_numb2 += request[i];
    ++i;
  }
  numb1 = std::stoi(str_numb1);
  numb2 = std::stoi(str_numb2);
}

int main() {
  int n;
  std::cin >> n;
  std::vector<Point> pointers;
  long long x, y, w;
  for (int i = 0; i < n; ++i) {
    std::cin >> x >> y >> w;
    pointers.emplace_back(Point{x, y, w, i});
  }
  FenwickFenwickTree fenwick_fenwick_tree(pointers);
  int m;
  std::cin >> m;
  std::string request;
  std::getline(std::cin, request);
  int rx, ry, i, z;
  for (int j = 0; j < m; ++j) {
    std::getline(std::cin, request);
    if (request[0] == 'g') {
      GetNumbers(4, request, rx, ry);
      std::cout << fenwick_fenwick_tree.Get(rx, ry) << '\n';
    } else {
      GetNumbers(7, request, i, z);
      fenwick_fenwick_tree.Set(i - 1, static_cast<long long>(z));
    }
  }
  return 0;
}