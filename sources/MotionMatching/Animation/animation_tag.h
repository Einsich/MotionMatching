#pragma once
#include <vector>
#include <string>
#include <map>

using Tag = uint64_t;//can be changed to bitvector

struct AnimationTags
{
  Tag tags;
  AnimationTags() = default;
  AnimationTags(const std::vector<std::string> &tags);
  bool contains(const std::string &tag) const;
  void add(const std::string &tag);
  void clear();
  std::string to_string() const;
};

void register_tags(const std::vector<std::string> &tags);
const std::map<std::string, Tag>& get_tag_map();