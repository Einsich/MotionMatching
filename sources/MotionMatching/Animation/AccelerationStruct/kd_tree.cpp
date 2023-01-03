#include "kd_tree.h"



constexpr int featureSize = (int)AnimationFeaturesNode::Count * sizeof(vec3);
constexpr int trajectorySize = AnimationTrajectory::PathLength * sizeof(TrajectoryPoint);
constexpr int KdTreeNodeSize = featureSize * 2 + trajectorySize;
static_assert(sizeof(AnimationTrajectory) == trajectorySize);
static_assert(sizeof(NodeFeatures) == featureSize*2);
static_assert(sizeof(KdTree::T) == KdTreeNodeSize);
constexpr int KdTreeDimensions = KdTreeNodeSize / sizeof(float);
constexpr int NodesDimensions = featureSize*2 / sizeof(float);


template<typename It = std::vector<KdTree::Node>::iterator>
static void build_kd_tree(It from, It to, uint dimension)
{
  uint count = to - from;
  if (count < 2)
    return;
  std::sort(from, to, [dimension](const KdTree::Node &a, const KdTree::Node &b){
    return a.get(dimension) < b.get(dimension);
  });
  It root = from;
  from = from + 1; 
  It split = from + rand() % (count-1);
  std::swap(*split, *root);


  dimension = (dimension+1) % KdTreeDimensions;
  build_kd_tree(from, split, dimension);
  build_kd_tree(split, to, dimension);
}

KdTree::KdTree(const MotionMatchingSettings &settings, AnimationTags tag, std::vector<KdTree::Node> &&points, std::function<float(const T&, const T&)> &&norma):
tag(tag), points(std::move(points)), norma(norma)
{
  build_kd_tree(this->points.begin(), this->points.end(), NodesDimensions);
  int N = (int)AnimationFeaturesNode::Count;
  int id = 0;
  for (int i = 0; i < N; i++)
  {
    float w = settings.nodeWeights[i] * settings.poseMatchingWeight * settings.realism;
    weights.push_back(w);
    weights.push_back(w);
    weights.push_back(w);
    weightsId.push_back(id);
    weightsId.push_back(id);
    weightsId.push_back(id);
    id++;
  } 
  for (int i = 0; i < N; i++)
  {
    float w = settings.velocitiesWeights[i] * settings.velocityMatchingWeight * settings.realism;
    weights.push_back(w);
    weights.push_back(w);
    weights.push_back(w);
    weightsId.push_back(id);
    weightsId.push_back(id);
    weightsId.push_back(id);
    id++;
  }

  for (int i = 0; i < AnimationTrajectory::PathLength; i++)
  {
    weights.push_back(settings.goalPathMatchingWeight);
    weights.push_back(settings.goalPathMatchingWeight);
    weights.push_back(settings.goalPathMatchingWeight);
    weightsId.push_back(id);
    weightsId.push_back(id);
    weightsId.push_back(id);
    id++;
    weights.push_back(settings.goalVelocityWeight);
    weights.push_back(settings.goalVelocityWeight);
    weights.push_back(settings.goalVelocityWeight);
    weightsId.push_back(id);
    weightsId.push_back(id);
    weightsId.push_back(id);
    id++;
    weights.push_back(settings.goalAngularVelocityWeight);
    weightsId.push_back(id);
    id++;
  }
  assert(weights.size() == KdTreeDimensions);

}

template<typename It = std::vector<KdTree::Node>::iterator>
struct Solver
{
  It begin, end, out;
  const KdTree::T &point;
  const std::vector<float> &weights;
  const std::vector<int> &weightsId;
  float search_radius;
  float tolerance_erorr;
  int counter;
  const std::function<float(const KdTree::T&, const KdTree::T&)> &norma;
  Solver(It begin, It end, const KdTree::T &point, const std::vector<float> &weights, const std::vector<int> &weightsId,
      float tolerance_erorr,
      const std::function<float(const KdTree::T&, const KdTree::T&)> &norma)
  : begin(begin), end(end), out(end), 
   point(point), weights(weights), weightsId(weightsId),
   search_radius(0), tolerance_erorr(tolerance_erorr), counter(0), norma(norma)
  {}

  bool find_closest(It &out, int &counter, float &search_radius)
  {
    this->search_radius = search_radius;
    this->counter = 0;
    bool result = find_closest(begin, end, NodesDimensions);
    out = this->out;
    counter = this->counter;
    search_radius = this->search_radius;
    return result;
  }
private:
  bool find_closest(It begin, It end, uint dimension)
  {
    if (search_radius <= 0)
      return false;
    size_t dist = end - begin;
    float rootNorma = norma(*begin->p, point);
    float x = begin->get(dimension);
    float y = ((const float*)&point)[dimension];
    float d = abs(x - y) * weights[dimension];
    dimension = (dimension+2) % KdTreeDimensions;

    bool hasRoot = rootNorma < search_radius;
    counter++;
    if (hasRoot)
    {
      out = begin;
      search_radius = rootNorma;
      if (rootNorma < tolerance_erorr)
      {
        search_radius = 0;
        return true;
      }
    }
    if (dist == 1)// || rootNorma < begin->nearestNeighbor * 0.5)
      return hasRoot;

    size_t count = dist - 1;
    size_t leftCount = count >> 1u;
    It leftBegin = begin + 1;
    if (search_radius <= d)
    {
      return ((y < x) ?
        (leftCount > 0 ? find_closest(leftBegin, leftBegin + leftCount, dimension) : false):
        find_closest(leftBegin + leftCount, end, dimension)) || hasRoot;
    }
    else
    {
      bool result = false;
      if (y < x)
      {
        result |= leftCount > 0 ? find_closest(leftBegin, leftBegin + leftCount, dimension) : false;
        result |= search_radius > d ? find_closest(leftBegin + leftCount, end, dimension) : false;
      }
      else
      {
        result |= find_closest(leftBegin + leftCount, end, dimension);
        result |= search_radius > d && leftCount > 0 ? find_closest(leftBegin, leftBegin + leftCount, dimension) : false;
      }
      return result || hasRoot;
    }
  }
};

static uint64_t sum = 0, sumX = 0;\
std::pair<uint, uint> KdTree::find_closest(const T &point, float tolerance_error) const
{
  float searchRadius = 100000.f;
  auto out = points.cend();
  int counter = 0;
  Solver solver(points.cbegin(), points.cend(), point, weights, weightsId, tolerance_error, norma);
  if (solver.find_closest(out, counter, searchRadius))
  {
    sum += counter;
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
    debug_log("error %f kd tree[%d] brute force[%d]", error, out - points.cbegin(), bestInd);

  return {points[bestInd].clip, points[bestInd].frame};
}

float KdTree::average_perf() const
{
  return (double)sum / sumX;
}