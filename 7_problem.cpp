#include <cmath>
#include <iostream>
#include <vector>

class SparseTable {
 public:
  SparseTable(std::vector<int>& array);
  int GetSecondOrdStatistic(size_t l, size_t r);

 private:
  std::vector<int> array_;
  std::vector<std::vector<int>> sparse_table_;
  std::vector<int> maximus_power_of_two_;

  void CountingPowersOfTwo();
  void PreCounting();
  size_t IndMin(size_t l, size_t r);
};

SparseTable::SparseTable(std::vector<int>& array)
    : array_(array), maximus_power_of_two_(std::vector<int>(array.size() + 1)) {
  CountingPowersOfTwo();
  PreCounting();
}

void SparseTable::CountingPowersOfTwo() {
  maximus_power_of_two_[0] = 0;
  maximus_power_of_two_[1] = 0;
  for (size_t i = 2; i <= array_.size(); ++i) {
    maximus_power_of_two_[i] = 1 + maximus_power_of_two_[i / 2];
  }
}

void SparseTable::PreCounting() {
  sparse_table_.push_back(std::vector<int>(array_.size()));
  for (size_t i = 0; i < array_.size(); ++i) {
    sparse_table_[0][i] = i;
  }
  size_t ind1, ind2, right_border;
  for (int k = 1; k <= maximus_power_of_two_[array_.size() - 1]; ++k) {
    sparse_table_.push_back(std::vector<int>(array_.size()));
    for (size_t l = 0; l < array_.size(); ++l) {
      ind1 = sparse_table_[k - 1][l];
      right_border = std::min(static_cast<size_t>(l + pow(2, k - 1)),
                              static_cast<size_t>(array_.size()) - 1);
      ind2 = sparse_table_[k - 1][right_border];
      sparse_table_[k][l] = (array_[ind1] < array_[ind2] ? ind1 : ind2);
    }
  }
}

size_t SparseTable::IndMin(size_t l, size_t r) {
  int power = maximus_power_of_two_[r - l + 1];
  size_t ind1 = sparse_table_[power][l];
  size_t ind2 =
      sparse_table_[power][static_cast<size_t>(r - pow(2, power) + 1)];
  size_t ind_min = (array_[ind1] < array_[ind2] ? ind1 : ind2);
  return ind_min;
}

int SparseTable::GetSecondOrdStatistic(size_t l, size_t r) {
  size_t min_ind = IndMin(l, r);
  if (min_ind == l) {
    return array_[IndMin(l + 1, r)];
  }
  if (min_ind == r) {
    return array_[IndMin(l, r - 1)];
  }
  size_t ind1 = IndMin(l, min_ind - 1);
  size_t ind2 = IndMin(min_ind + 1, r);
  int second_statistic = std::min(array_[ind1], array_[ind2]);
  return second_statistic;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  SparseTable sparse_table(arr);
  size_t l, r;
  for (int i = 0; i < m; ++i) {
    std::cin >> l >> r;
    std::cout << sparse_table.GetSecondOrdStatistic(l - 1, r - 1) << "\n";
  }
  return 0;
}