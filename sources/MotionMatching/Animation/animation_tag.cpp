#include "animation_tag.h"
#include <map>

static std::map<std::string, Tag> tagMap;

static Tag get_tag(const std::string &tag, bool added = true)
{
  auto it = tagMap.find(tag);
  if (it == tagMap.end())
  {
    if (added)
    {
      Tag result = 1u << tagMap.size();
      tagMap.try_emplace(tag, result);
      return result;
    }
    else
      return 0;
  }
  else
    return it->second;
}

AnimationTags::AnimationTags(const std::vector<std::string> &tags) : tags(0)
{
  for (const std::string &tag : tags)
    add(tag);
}

bool AnimationTags::contains(const std::string &tag) const
{
  return (tags & get_tag(tag, false)) != 0;
}

void AnimationTags::add(const std::string &tag)
{
  tags |= get_tag(tag);
}

void AnimationTags::clear()
{
  tags = 0;
}

std::string AnimationTags::to_string() const
{
  std::string result;
  for (const auto &p : tagMap)
  {
    if ((p.second & tags) != 0)
      result += p.first + "|";
  }
  if (!result.empty())
    result.erase(result.cend() - 1);
  return result;
}