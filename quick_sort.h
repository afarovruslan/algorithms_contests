#include "DQS.h"

void QuickSort(std::vector<int>& arr, int left, int right) {
  if (right - left <= 0) {
    return;
  }
  int pivot = DQS(arr, (right - left) / 2, left, right);
  auto count = Counting(arr, pivot, left, right);
  int size_less = count.first, size_eq = count.second;
  Partition(arr, pivot, left, right, count);
  if (size_less != 0) {
    QuickSort(arr, left, left + size_less - 1);
  }
  QuickSort(arr, left + size_less + size_eq, right);
}