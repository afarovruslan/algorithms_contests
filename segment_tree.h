#include <concepts>
#include <functional>
#include <vector>

template <typename T, typename Func>
concept IsFitForSegmentTree = std::copy_constructible<T> &&
    requires(const T& value1, const T& value2, Func func, T value) {
  value = func(value1, value2);
};

template <typename T = int, typename Func = std::plus<T>>
requires IsFitForSegmentTree<T, Func>
class SegmentTree {
 public:
  explicit SegmentTree(const std::vector<T>& arr, const T& default_value = 0, const Func& func = Func());
  template <typename U>
  void Set(int ind, U&& value);
  T Get(int l, int r);
  [[nodiscard]] size_t Size() const;

 private:
  [[no_unique_address]] Func func_;
  std::vector<T> tree_;
  T default_value_;

  void Build(size_t vertex, size_t tl, size_t tr, const std::vector<T>& arr);
  template <typename U>
  void Update(size_t vertex, size_t tl, size_t tr, size_t ind, U&& new_value);
  T RecursiveGet(size_t vertex, size_t tl, size_t tr, size_t l, size_t r);
  void RecountVertexValue(size_t vertex);
};

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
SegmentTree<T, Func>::SegmentTree(const std::vector<T>& arr, const T& default_value, const Func& func)
    : func_(func), tree_(arr.size() * 4, default_value), default_value_(default_value) {
  Build(1, 0, Size() - 1, arr);
}

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
template <typename U>
void SegmentTree<T, Func>::Set(int ind, U&& value) {
  Update(1, 0, Size() - 1, ind, std::forward<U>(value));
}

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
T SegmentTree<T, Func>::Get(int l, int r) {
  return RecursiveGet(1, 0, Size() - 1, l, r);
}

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
size_t SegmentTree<T, Func>::Size() const {
  return tree_.size() / 4;
}

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
void SegmentTree<T, Func>::Build(size_t vertex, size_t tl, size_t tr, const std::vector<T>& arr) {
  if (tl == tr) {
    tree_[vertex] = arr[tl];
  } else {
    size_t tm = (tl + tr) / 2;
    Build(2 * vertex, tl, tm, arr);
    Build(2 * vertex + 1, tm + 1, tr, arr);
    RecountVertexValue(vertex);
  }
}

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
template <typename U>
void SegmentTree<T, Func>::Update(size_t vertex, size_t tl, size_t tr, size_t ind, U&& new_value) {
  if (tl == tr) {
    tree_[vertex] = std::forward<U>(new_value);
  } else {
    size_t tm = (tl + tr) / 2;
    if (ind <= tm) {
      Update(2 * vertex, tl, tm, ind,std::forward<U>(new_value));
    } else {
      Update(2 * vertex + 1, tm + 1, tr, ind, std::forward<U>(new_value));
    }
    RecountVertexValue(vertex);
  }
}

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
T SegmentTree<T, Func>::RecursiveGet(size_t vertex, size_t tl, size_t tr, size_t l, size_t r) {
  if (tl == l and tr == r) {
    return tree_[vertex];
  }
  size_t tm = (tl + tr) / 2;
  T ans = default_value_;
  if (l <= tm) {
    ans = func_(ans, RecursiveGet(2 * vertex, tl, tm, l, std::min(r, tm)));
  }
  if (r > tm) {
    ans = func_(ans, RecursiveGet(2 * vertex + 1, tm + 1, tr, std::max(tm + 1, l), r));
  }
  return ans;
}

template <typename T, typename Func>
requires IsFitForSegmentTree<T, Func>
void SegmentTree<T, Func>::RecountVertexValue(size_t vertex) {
  tree_[vertex] = func_(tree_[2 * vertex], tree_[2 * vertex + 1]);
}