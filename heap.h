#include <algorithm>
#include <concepts>
#include <vector>
#include <functional>

template <typename T, typename Comp = std::less<T>>
requires std::default_initializable<T> && std::swappable<T>
class BinaryHeap {
 public:
  using vertex_type = size_t;
  using value_type = T;
  using reference = value_type&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using const_reference = const value_type&;
  using pointer = T*;
  using const_pointer = const pointer;

  explicit BinaryHeap(const Comp& comp = Comp());

  const_reference GetMin() const;
  reference GetMin();
  template <typename U>
  void Insert(U&& value);
  void ExtractMin();
  template <typename Changer>
  void Change(size_t number_request, Changer&& changer);
  // example of usage Change
  /*
  void DecreaseKey(size_t number_request, long long delta) {
    Change(number_request, [delta](T& x) { x -= delta; });
  }
   */
  [[nodiscard]] size_type Size() const;

 private:
  static const vertex_type kRoot = 1;

  [[no_unique_address]] Comp comp_;
  std::vector<T> heap_;
  std::vector<size_t> vertexes_;
  std::vector<vertex_type> numbers_req_;

  void Swap(vertex_type vertex1, vertex_type vertex2);
  void SiftUp(vertex_type vertex);
  void SiftDown(vertex_type vertex);
};

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
BinaryHeap<T, Comp>::BinaryHeap(const Comp& comp)
    : comp_(comp), heap_(1), numbers_req_{0} {}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
typename BinaryHeap<T, Comp>::reference BinaryHeap<T, Comp>::GetMin() {
  vertexes_.push_back(Size());
  return heap_[kRoot];
}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
typename BinaryHeap<T, Comp>::const_reference BinaryHeap<T, Comp>::GetMin() const {
  return const_cast<BinaryHeap*>(this)->GetMin();
}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
template <typename U>
void BinaryHeap<T, Comp>::Insert(U&& value) {
  heap_.push_back(std::forward<U>(value));
  numbers_req_.push_back(vertexes_.size());
  vertexes_.push_back(Size());
  SiftUp(Size());
}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
void BinaryHeap<T, Comp>::ExtractMin() {
  Swap(kRoot, Size());
  heap_.pop_back();
  SiftDown(kRoot);
  vertexes_.push_back(Size());
}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
template <typename Changer>
void BinaryHeap<T, Comp>::Change(size_t number_request, Changer&& changer) {
  vertex_type vertex = vertexes_[number_request];
  std::invoke(std::forward<Changer>(changer), heap_[vertex]);
  SiftUp(vertex);
  SiftDown(vertex);
  vertexes_.push_back(Size());
}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
typename BinaryHeap<T, Comp>::size_type BinaryHeap<T, Comp>::Size() const { return heap_.size() - 1; }

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
void BinaryHeap<T, Comp>::Swap(BinaryHeap::vertex_type vertex1, BinaryHeap::vertex_type vertex2) {
  size_t number_req_vertex1 = numbers_req_[vertex1];
  size_t number_req_vertex2 = numbers_req_[vertex2];
  numbers_req_[vertex1] = numbers_req_[vertex2];
  numbers_req_[vertex2] = number_req_vertex1;
  std::swap(vertexes_[number_req_vertex1], vertexes_[number_req_vertex2]);
  std::swap(heap_[vertex1], heap_[vertex2]);
}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
void BinaryHeap<T, Comp>::SiftUp(BinaryHeap::vertex_type vertex) {
  while (vertex != kRoot and comp_(heap_[vertex], heap_[vertex / 2])) {
    Swap(vertex, vertex / 2);
    vertex = vertex / 2;
  }
}

template <typename T, typename Comp>
requires std::default_initializable<T> && std::swappable<T>
void BinaryHeap<T, Comp>::SiftDown(BinaryHeap::vertex_type vertex) {
  while (2 * vertex <= Size()) {
    vertex_type sun_vertex = 2 * vertex;
    if (2 * vertex + 1 <= Size() and
        comp_(heap_[2 * vertex + 1], heap_[2 * vertex])) {
      sun_vertex = 2 * vertex + 1;
    }
    if (!comp_(heap_[sun_vertex], heap_[vertex])) {
      break;
    }
    Swap(vertex, sun_vertex);
    vertex = sun_vertex;
  }
}