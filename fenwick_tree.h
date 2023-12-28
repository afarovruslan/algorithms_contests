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