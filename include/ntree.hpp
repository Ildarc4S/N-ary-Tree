#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <typename T> 
class NTree {
public:
  NTree(size_t max_child, T data, std::string index);

  void addChild(T data, const std::string &index);
  void addChild(T data, size_t child_index);
  void generateChildren(int depth, float probability);

  std::pair<NTree<T> *, NTree<T> *> getTwoRandomNodes();
  void getTwoRandomAndFindCommon();
  NTree<T> *getChild(size_t child_index);
  T getChildValue(size_t child_index);
  void print(int level = 0);

  void collectNodes(std::vector<NTree<T> *> &nodes);
  NTree<T> *findNode(const std::string &index);
  std::string findCommonNode(const std::string &index_one,
                             const std::string &index_two);

  size_t max_child;
  T data;
  std::vector<std::unique_ptr<NTree<T>>> children;
  std::string index;
};

extern template class NTree<int>;