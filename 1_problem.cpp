#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class SegmentTreeOnSum {
 private:
  size_t size_arr_;
  std::vector<int> tree_;

  void RecursiveUpdate(size_t vertex, size_t tl, size_t tr, size_t ind,
                       int delta) {
    if (tl == tr) {
      tree_[vertex] += delta;
    } else {
      size_t tm = (tl + tr) / 2;
      if (ind <= tm) {
        RecursiveUpdate(2 * vertex, tl, tm, ind, delta);
      } else {
        RecursiveUpdate(2 * vertex + 1, tm + 1, tr, ind, delta);
      }
      tree_[vertex] = tree_[2 * vertex] + tree_[2 * vertex + 1];
    }
  }
  int RecursiveGetSum(size_t vertex, size_t tl, size_t tr, size_t l, size_t r) {
    if (tl == l and tr == r) {
      return tree_[vertex];
    }
    size_t tm = (tl + tr) / 2;
    int ans = 0;
    if (l <= tm) {
      ans += RecursiveGetSum(2 * vertex, tl, tm, l, std::min(r, tm));
    }
    if (r > tm) {
      ans +=
          RecursiveGetSum(2 * vertex + 1, tm + 1, tr, std::max(tm + 1, l), r);
    }
    return ans;
  }

 public:
  SegmentTreeOnSum() : size_arr_(42195 + 1), tree_(size_arr_ * 4, 0) {}
  void Update(size_t ind, int delta) {
    RecursiveUpdate(1, 0, size_arr_ - 1, ind, delta);
  }
  int GetSum(size_t l, size_t r) {
    return RecursiveGetSum(1, 0, size_arr_ - 1, l, r);
  }
};

void RecordUserAndPage(const std::string& request, size_t& user, size_t& page) {
  std::string str_user, str_page;
  size_t ind = 4;
  while (request[ind] != ' ') {
    str_user += request[ind];
    ++ind;
  }
  ++ind;
  while (ind < request.length()) {
    str_page += request[ind];
    ++ind;
  }
  user = std::stoi(str_user);
  page = std::stoi(str_page);
}

void RecordUser(const std::string& request, size_t& user) {
  std::string str_user;
  for (size_t ind = 6; ind < request.length(); ++ind) {
    str_user += request[ind];
  }
  user = std::stoi(str_user);
}

int main() {
  size_t n;
  std::cin >> n;
  std::vector<size_t> users(100000 + 1, 0);
  std::string request, str_user, str_page;
  std::getline(std::cin, request);
  size_t page, user;
  SegmentTreeOnSum segment_tree_on_sum;
  size_t count_users = 0;
  for (size_t i = 0; i < n; ++i) {
    std::getline(std::cin, request);
    if (request[0] == 'R') {
      RecordUserAndPage(request, user, page);
      if (users[user] == 0) {
        ++count_users;
      } else {
        segment_tree_on_sum.Update(users[user], -1);
      }
      users[user] = page;
      segment_tree_on_sum.Update(page, 1);
    } else {
      RecordUser(request, user);
      std::cout << std::setprecision(6);
      if (users[user] == 0) {
        std::cout << 0 << '\n';
      } else if (count_users == 1) {
        std::cout << 1 << '\n';
      } else {
        std::cout << static_cast<double>(
            segment_tree_on_sum.GetSum(0, users[user] - 1)) /
            static_cast<double>((count_users - 1))
                  << '\n';
      }
    }
  }
  return 0;
}

