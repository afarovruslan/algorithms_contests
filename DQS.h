/*
Разбиваем массив на блоки длины block_size, в каждом ищем медиану и получаем новый массив B.
Затем запускаем DeterministicQuickSelect(B, n/10) - т е находим медину в новом массиве, делаем Parition по медиане
и запускаемся рекурсивно в какой-то из 3 получившихся частей. Утверждается, что в итоге асимптотика будет O(n)
 */

#pragma once
#include <array>
#include <iostream>
#include <vector>

static const size_t kBlockSize = 5;

void Merge(std::vector<int>& arr, int left, int middle, int right) {
  std::array<int, kBlockSize> new_arr;
  int i = left, j = middle + 1, k = 0;
  while (i <= middle and j <= right) {
    if (arr[i] < arr[j]) {
      new_arr[k] = arr[i];
      ++i;
    } else {
      new_arr[k] = arr[j];
      ++j;
    }
    ++k;
  }
  while (i <= middle) {
    new_arr[k] = arr[i];
    ++i, ++k;
  }
  while (j <= right) {
    new_arr[k] = arr[j];
    ++j, ++k;
  }
  for (int m = 0; m < k; ++m) {
    arr[m + left] = new_arr[m];
  }
}

void MergeSort(std::vector<int>& arr, int left, int right) {
  if (right - left == 0) {
    return;
  }
  int middle = (left + right) / 2;
  MergeSort(arr, left, middle);
  MergeSort(arr, middle + 1, right);
  Merge(arr, left, middle, right);
}

std::pair<int, int> Counting(const std::vector<int>& arr, int x, int left,
                             int right) {
  int count_less = 0, count_eq = 0;
  for (int i = left; i <= right; ++i) {
    if (arr[i] < x) {
      ++count_less;
    }
    if (arr[i] == x) {
      ++count_eq;
    }
  }
  return std::pair<int, int>(count_less, count_eq);
}

void Swap(std::vector<int>& arr, int i, int j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

void PartitionLessEqAndMore(std::vector<int>& arr, int left, int right, int x) {
  int i = left, j = right;
  while (i < j) {
    if (arr[i] <= x) {
      ++i;
    }
    if (arr[j] > x) {
      --j;
    }
    if (i < j and arr[i] > x and arr[j] <= x) {
      Swap(arr, i, j);
      ++i, --j;
    }
  }
}

void PartitionLessAndEqually(std::vector<int>& arr, int left, int size_less,
                             int size_eq, int x) {
  int i = left, j = left + size_less + size_eq - 1;
  while (i < j) {
    if (arr[i] < x) {
      ++i;
    }
    if (arr[j] == x) {
      --j;
    }
    if (arr[i] == x and arr[j] < x) {
      Swap(arr, i, j);
      ++i, --j;
    }
  }
}

void Partition(std::vector<int>& arr, int x, int left, int right,
               std::pair<int, int> p) {
  int size_less = p.first, size_eq = p.second;
  PartitionLessEqAndMore(arr, left, right, x);
  PartitionLessAndEqually(arr, left, size_less, size_eq, x);
}

int DQS(std::vector<int>& arr, int k, int left, int right) {
  if (right - left + 1 <= kBlockSize) {
    MergeSort(arr, left, right);
    return arr[left + k];
  }
  std::vector<int> b((right - left + 1) / kBlockSize + 1, 0);
  int length = 0;
  for (int i = left; i <= right; i += kBlockSize) {
    if (i + static_cast<int>(kBlockSize) - 1 > right) {
      b[length] = DQS(arr, (right - i) / 2, i, right);
    } else {
      b[length] = DQS(arr, 2, i, i + 4);
    }
    ++length;
  }
  int pivot = DQS(b, (length - 1) / 2, 0, length - 1);
  auto count = Counting(arr, pivot, left, right);
  int size_less = count.first, size_eq = count.second;
  Partition(arr, pivot, left, right, count);
  if (k <= size_less - 1) {
    return DQS(arr, k, left, left + size_less - 1);
  }
  if (k <= size_less + size_eq - 1) {
    return pivot;
  }
  return DQS(arr, k - (size_less + size_eq), left + size_less + size_eq, right);
}