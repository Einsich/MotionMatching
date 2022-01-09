#include "cover_tree.h"
#include <algorithm>

template<typename It, typename Norma>
static void build_tree(It node, It from, It to, float bound_radius, Norma norma)
{
  node->bound = norma(*node->p, *(to-1)->p);
  node->nearestNeighbor = norma(*node->p, *from->p);
  while (from != to)
  {
    size_t dist = to - from;
    const CoverTree::T *root = (from + rand()%dist)->p;
    std::sort(from, to, [&](const CoverTree::Node &a, const CoverTree::Node &b){return norma(*root, *a.p) < norma(*root, *b.p);});
    It head = from;
    ++from;
    head->bound = norma(*head->p, *(to-1)->p)+0.00001f;
    auto lowerBound = std::lower_bound(from, to, *head, 
        [&](const CoverTree::Node &a, const CoverTree::Node &){return norma(*root, *a.p) < bound_radius;});
    if (lowerBound != to)
    {
      build_tree(head, from, lowerBound, bound_radius * 0.5f, norma);
      from = lowerBound;
    }
    node->childs.emplace_back(head - node);
  }
}

CoverTree::CoverTree(AnimationTags tag, std::vector<Node> &&points_, std::function<float(const T&, const T&)> &&norma):
  tag(tag), points(std::move(points_)), norma(norma)
{
  
  const CoverTree::T *root = (points.begin() + points.size()/2)->p;
  std::sort(points.begin(), points.end(), [&](const CoverTree::Node &a, const CoverTree::Node &b){return norma(*root, *a.p) < norma(*root, *b.p);});
  maxRadius = norma(*root, *points.back().p);
  build_tree(points.begin(), points.begin()+1, points.end(), maxRadius * 0.5f, norma);
  debug_log("point = %d tags = %d max dist = %f", points.size(), tag.tags, maxRadius);
}

template<typename It>
struct Solver
{
  It begin, out;
  const CoverTree::T &point;
  float search_radius;
  float tolerance_erorr;
  int counter;
  const std::function<float(const CoverTree::T&, const CoverTree::T&)> &norma;
  Solver(It begin, It end, const CoverTree::T &point,
      float tolerance_erorr,
      const std::function<float(const CoverTree::T&, const CoverTree::T&)> &norma)
  : begin(begin), out(end),
   point(point), search_radius(0), tolerance_erorr(tolerance_erorr), counter(0), norma(norma)
  {}

  bool find_closect(It &out, int &counter, float &search_radius)
  {
    this->search_radius = search_radius;
    bool result = find_closect(begin);
    out = this->out;
    counter = this->counter;
    search_radius = this->search_radius;
    return result;
  }
private:
  bool find_closect(It begin)
  {
    if (search_radius <= 0)
      return false;
    float rootNorma = norma(*begin->p, point);
    bool rootInside = rootNorma < search_radius;
    counter++;
    if (rootInside)
    {
      out = begin;
      search_radius = rootNorma;
      if (rootNorma < tolerance_erorr)
      {
        search_radius = 0;
        return true;
      }
    }

    if (rootNorma < begin->nearestNeighbor * 0.5)
      return rootInside;
    if (begin->bound + search_radius < rootNorma)
      return false;
    bool result = rootInside;
    for (uint childDiff: begin->childs)
    {
      It child = begin + childDiff;
      result |= find_closect(child);
    }
    return result;
  }
};

std::pair<uint, uint> CoverTree::find_closect(const T &point, float tolerance_error) const
{
  float searchRadius = 100000.f;//glm::max(maxRadius, 100.f);
  auto out = points.cend();
  int counter = 0;
  static uint64_t count = 0, sum = 0, sumX = 0;
  count++;
  Solver solver(points.cbegin(), points.cend(), point, tolerance_error, norma);
  if (solver.find_closect(out, counter, searchRadius))
  {
    sum += counter;
    sumX += points.size();
    if ((count & (count - 1)) == 0)
      debug_log("average perf %f",  (double)sum / sumX);
    return {out->clip, out->frame};
  }
  
  float minRes = norma(*points[0].p, point);
  uint bestInd = 0;
  for (uint nextFrame = 1, n = points.size(); nextFrame < n; nextFrame++)
  {
    float result = norma(*points[nextFrame].p, point);
    if (result < minRes)
    {
      minRes = result;
      bestInd = nextFrame;
    }
  }
  float error = glm::abs(searchRadius - norma(*points[bestInd].p, point));
  if (error > 0.1f)
    debug_log("error %f vp tree[%d] brute force[%d]", error, out - points.cbegin(), bestInd);

  return {points[bestInd].clip, points[bestInd].frame};
}