#include <algorithm>
#include <iostream>
#include <vector>

static int F(int i) { return i & (i + 1); }
static int G(int i) { return i | (i + 1); }

class PersistentFenwickTree {
 public:
  explicit PersistentFenwickTree(size_t size);
  long long GetPrefSum(int r);
  void Update(int ind, int delta);

 private:
  size_t size_;
  std::vector<long long> tree_;
};

PersistentFenwickTree::PersistentFenwickTree(size_t size)
    : size_(size),
      tree_(std::vector<long long>(size_, 0)) {}

long long PersistentFenwickTree::GetPrefSum(int r) {
  long long sum = 0;
  for (int i = r; i >= 0; i = F(i) - 1) {
    sum += tree_[i];
  }
  return sum;
}

void PersistentFenwickTree::Update(int ind, int delta) {
  for (size_t i = ind; i < size_; i = G(i)) {
    tree_[i] += delta;
  }
}

int LargestLessOrEqX(const std::vector<int>& array, int x) {
  int l = -1, r = static_cast<int>(array.size()), m;
  while (r - l > 1) {
    m = (l + r) / 2;
    if (array[m] <= x) {
      l = m;
    } else {
      r = m;
    }
  }
  return l;
}

void ReadData(const int& q, std::vector<int>& sorted_array,
              std::vector<std::pair<char, int>>& requests) {
  char request;
  int x;
  for (int i = 0; i < q; ++i) {
    std::cin >> request >> x;
    if (request == '+') {
      sorted_array.push_back(x);
    }
    requests.emplace_back(request, x);
  }
}

void AnswerForRequests(const std::vector<std::pair<char, int>>& requests,
                       const std::vector<int>& sorted_array) {
  PersistentFenwickTree persistent_fenwick_tree(
      static_cast<int>(sorted_array.size()));
  int ind;
  for (const auto& request : requests) {
    ind = LargestLessOrEqX(sorted_array, request.second);
    if (request.first == '?') {
      std::cout << persistent_fenwick_tree.GetPrefSum(ind) << '\n';
    } else {
      persistent_fenwick_tree.Update(ind, request.second);
    }
  }
}

int main() {
  int q;
  std::cin >> q;
  std::vector<int> sorted_array;
  std::vector<std::pair<char, int>> requests;
  ReadData(q, sorted_array, requests);
  std::sort(sorted_array.begin(), sorted_array.end());
  AnswerForRequests(requests, sorted_array);
}