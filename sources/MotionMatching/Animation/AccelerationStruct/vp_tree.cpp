#include "vp_tree.h"
#include <algorithm>
template<typename It>
static float sort_with_metric(It begin, It end, const std::function<float(const VPTree::T&, const VPTree::T&)> &norma)
{
  size_t dist = end - begin;
  if (dist == 1)
    return 0;
  const VPTree::T *root = (begin + rand()%dist)->p;
  std::sort(begin, end, [&](const VPTree::Node &a, const VPTree::Node &b){return norma(*root, *a.p) < norma(*root, *b.p);});
  float maxDist = norma(*(end - 1)->p, *begin->p);
  size_t count = dist - 1;
  size_t leftCount = (count + 1) >> 1u;
  float distToLeft = norma(*(begin + leftCount)->p, *begin->p);
  float distToRight = leftCount < count ? norma(*(begin + leftCount+1)->p, *begin->p) : distToLeft + 1.f;
  begin->bound = (distToLeft + distToRight) * 0.5f;
  begin->nearestNeighbor = norma(*(begin + 1)->p, *begin->p);
  ++begin;
  sort_with_metric(begin, begin + leftCount,  norma);
  if (leftCount < count)
    sort_with_metric(begin + leftCount, end, norma);
  return maxDist;
}

VPTree::VPTree(AnimationTags tag, std::vector<Node> &&m_points, std::function<float(const T&, const T&)> &&norma):
  tag(tag), points(std::move(m_points)), norma(norma)
{
  maxRadius = sort_with_metric(this->points.begin(), points.end(), norma);
  //debug_log("point = %d tags = %s max dist = %f", points.size(), tag.to_string().c_str(), maxRadius);
}

template<typename It>
struct Solver
{
  It begin, end, out;
  const VPTree::T &point;
  float search_radius;
  float tolerance_erorr;
  int counter;
  const std::function<float(const VPTree::T&, const VPTree::T&)> &norma;
  Solver(It begin, It end, const VPTree::T &point,
      float tolerance_erorr,
      const std::function<float(const VPTree::T&, const VPTree::T&)> &norma)
  : begin(begin), end(end), out(end),
   point(point), search_radius(0), tolerance_erorr(tolerance_erorr), counter(0), norma(norma)
  {}

  bool find_closest(It &out, int &counter, float &search_radius)
  {
    this->search_radius = search_radius;
    this->counter = 0;
    bool result = find_closest(begin, end);
    out = this->out;
    counter = this->counter;
    search_radius = this->search_radius;
    return result;
  }
  void find_closest_approx(int &counter, float &search_radius)
  {
    this->search_radius = search_radius;
    this->counter = 0;
    find_closest_approx(begin, end);
    counter = this->counter;
    search_radius = this->search_radius;
  }
private:
  bool find_closest(It begin, It end)
  {
    if (search_radius <= 0)
      return false;
    size_t dist = end - begin;
    float rootNorma = norma(*begin->p, point);
    bool hasRoot = rootNorma <= search_radius;
    counter++;
    if (hasRoot)
    {
      out = begin;
      search_radius = std::min(rootNorma, search_radius);
      if (rootNorma < tolerance_erorr)
      {
        search_radius = 0;
        return true;
      }
    }
    if (dist == 1 || rootNorma < begin->nearestNeighbor * 0.5)
      return hasRoot;

    size_t count = dist - 1;
    size_t leftCount = (count + 1) >> 1u;
    It leftBegin = begin + 1;
    bool childSearch = false;

    if (rootNorma + search_radius <= begin->bound || leftCount == count)
    {
      childSearch = find_closest(leftBegin, leftBegin + leftCount);
    }
    else
    {
      if (begin->bound + search_radius <= rootNorma)
      {
        childSearch = find_closest(leftBegin + leftCount, end);
      }
      else
      {
        bool leftSearch = find_closest(leftBegin, leftBegin + leftCount);
        childSearch = (!leftSearch && find_closest(leftBegin + leftCount, end)) ||
            (leftSearch && rootNorma + search_radius >= begin->bound && find_closest(leftBegin + leftCount, end));
        childSearch |= leftSearch; 
      }
    }
    return hasRoot || childSearch;
  }
  
  void find_closest_approx(It begin, It end)
  {
    size_t dist = end - begin;
    if (dist == 0)
      return;
    float rootNorma = norma(*begin->p, point);
    counter++;
    search_radius = std::min(rootNorma, search_radius);
    size_t count = dist - 1;
    size_t leftCount = (count + 1) >> 1u;
    It leftBegin = begin + 1;

    if (rootNorma < begin->bound || leftCount == count)
    {
      find_closest_approx(leftBegin, leftBegin + leftCount);
    }
    else
    {
      find_closest_approx(leftBegin + leftCount, end);
    }
  }
};

static uint64_t sum = 0, sumX = 0, sumAprx=0;

std::pair<uint, uint> VPTree::find_closest(const T &point, float tolerance_error) const
{
  float searchRadius = 100000.f;
  auto out = points.cend();
  int counter = 0, cnt_aprx=0;
  Solver solver(points.cbegin(), points.cend(), point, tolerance_error, norma);

  solver.find_closest_approx(cnt_aprx, searchRadius);

  if (solver.find_closest(out, counter, searchRadius))
  {
    sum += counter;
    sumAprx += cnt_aprx;
    sumX += points.size();

    return {out->clip, out->frame};
  }
  debug_log("-");
  
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
float VPTree::average_perf() const
{
  return (double)sum / sumX;
}