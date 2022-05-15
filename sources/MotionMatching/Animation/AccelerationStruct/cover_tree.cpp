#include "cover_tree.h"
#include <algorithm>

template<typename It, typename Norma>
static void build_tree(It node, It from, It to, float bound_radius, Norma norma)
{
  if (from == to)
  {
    node->bound = 0;
    node->nearestNeighbor = 100000000;
    return;
  }
  node->bound = bound_radius;
  node->nearestNeighbor = norma(*node->p, *from->p);
  while (from != to)
  {
    size_t dist = to - from;
    const CoverTree::T *root = (from + dist-1)->p;
    std::sort(from, to, [&](const CoverTree::Node &a, const CoverTree::Node &b){return norma(*root, *a.p) < norma(*root, *b.p);});
    It head = from;
    ++from;
    float radius = bound_radius*0.4f;
    auto lowerBound = std::lower_bound(from, to, *head, 
        [&](const CoverTree::Node &a, const CoverTree::Node &){return norma(*root, *a.p) < radius;});

    
    float bound = norma(*root, *(lowerBound-1)->p);
    build_tree(head, from, lowerBound, bound, norma);
    from = lowerBound;

    node->childs.emplace_back(head - node);
  }
}

CoverTree::CoverTree(AnimationTags tag, std::vector<Node> &&points_, std::function<float(const T&, const T&)> &&norma):
  tag(tag), points(std::move(points_)), norma(norma)
{
  
  const CoverTree::T *root = (points.begin() + points.size()/2)->p;
  std::sort(points.begin(), points.end(), [&](const CoverTree::Node &a, const CoverTree::Node &b){return norma(*root, *a.p) < norma(*root, *b.p);});
  maxRadius = norma(*root, *points.back().p);
  build_tree(points.begin(), points.begin()+1, points.end(), maxRadius, norma);
  //debug_log("point = %d tags = %s max dist = %f", points.size(), tag.to_string().c_str(), maxRadius);
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

  void find_closest_approx(int &counter, float &search_radius)
  {
    this->search_radius = search_radius;
    this->counter = 0;
    find_closest_approx(begin);
    counter = this->counter;
    search_radius = this->search_radius;
  }
  bool find_closest(It &out, int &counter, float &search_radius)
  {
    this->search_radius = search_radius;
    this->counter = 0;
    bool result = find_closest(begin);
    out = this->out;
    counter = this->counter;
    search_radius = this->search_radius;
    return result;
  }
private:
  void find_closest_approx(It begin)
  {
    float rootNorma = norma(*begin->p, point);
    counter++;
    if (rootNorma < begin->bound)
    {
      search_radius = std::min(rootNorma, search_radius);
      for (uint childDiff: begin->childs)
      {
        It child = begin + childDiff;
        find_closest_approx(child);
      }
    }
  }
  bool find_closest(It begin)
  {
    if (search_radius <= 0)
      return false;
    float rootNorma = norma(*begin->p, point);
    bool rootInside = rootNorma < search_radius;
    counter++;
    if (rootInside)
    {
      out = begin;
      search_radius = std::min(rootNorma, search_radius);
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
      result |= find_closest(child);
    }
    return result;
  }
};

static uint64_t sum = 0, sumX = 0, sumAprx=0;

std::pair<uint, uint> CoverTree::find_closest(const T &point, float tolerance_error) const
{
  float searchRadius = 100000.f;//glm::max(maxRadius, 100.f);
  auto out = points.cend();
  int counter = 0,cnt_aprx=0;
  Solver solver(points.cbegin(), points.cend(), point, tolerance_error, norma);
  int sampleCount = points.size()*0.005f;
  for (int i = 0; i < sampleCount; i++)
  {
    searchRadius = std::min(norma(*points[rand()%points.size()].p, point), searchRadius);
  }    
  cnt_aprx+=sampleCount;
  //solver.find_closest_approx(cnt_aprx, searchRadius);

  if (solver.find_closest(out, counter, searchRadius))
  {
    sum += counter;
    sumAprx += cnt_aprx;
    sumX += points.size();
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
    debug_log("error %f cover tree[%d] brute force[%d]", error, out - points.cbegin(), bestInd);

  return {points[bestInd].clip, points[bestInd].frame};
}
float CoverTree::average_perf() const
{
  return (double)sum / sumX;
}