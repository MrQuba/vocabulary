#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <random>
#include <string>
#include <vector>

constexpr void operator>>(std::istream &f, std::vector<std::string> &v) {
  std::string l{};
  while (std::getline(f, l))
  if(l[0] != '#')
    v.push_back(l);
}
template <typename T>
std::ostream &operator<<(std::ostream &s, std::vector<T> &v) {
  for (auto e : v)
    s << e << "\n";
  return s;
}
template <typename T> struct initialize_vector : std::vector<T> {
  using vec = std::vector<T>;
  std::shared_ptr<vec> operator()(std::ifstream &f, bool shuffle = false) {
    f >> v;
    if (shuffle) {
      std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
    }
    return std::make_shared<vec>(v);
  }
  vec v;
};
bool error_check(int argc);
struct getword {
  using vec = std::vector<std::string>;
  using iterator = vec::iterator;
  using ptr = std::shared_ptr<vec>;
  getword(ptr ptr, const bool f = true)
      : words(ptr), i(words->begin()), first(f) {}
  std::string operator()() {
    if (i == words->end())
      return "";
    second = std::ranges::find(*i, ';') + 1;
    f = std::string{i->begin(), second - 1};
    s = std::string{second, i->end()};
    auto w = (first) ? f : s;
    i += 1;
    return w;
  }
  bool operator==(const std::string &u) { return u == ((first) ? s : f); }

private:
  const bool first;
  std::string::iterator second;
  std::string f;
  std::string s;
  ptr words;
  iterator i;
};
struct initialize_bool {
  constexpr bool operator()(const std::string &b) {
    return (b == "true" || b == "1") ? true : false;
  }
};
int main(int argc, char *argv[]) {
  if (!error_check(argc))
    return -1;
  std::ifstream f(std::string{argv[1]});
  auto words = getword(initialize_vector<std::string>()(f, true),
                       initialize_bool()(std::string{argv[2]}));
  std::string word = "\n";
  while (true) {
    word = words();
    if(word == "") break;
    std::cout << word << ": ";
    std::string uword;
    std::cin >> uword;
    std::cout << ((words == uword) ? "correct\n" : "wrong\n");
  }
  return 0;
}
bool error_check(int argc) {
  std::cout << ((argc < 2)
                    ? "requires path as second argument and bool as third \n"
                    : ((argc > 3) ? "too many arguments\n" : ""));
  return argc > 2 && argc <= 3;
}