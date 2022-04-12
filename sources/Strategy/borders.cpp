#include <common.h>
#include <map>
#include "political_map.h"
#include <render/mesh.h>

struct Provinces
{
  int w, h;
  float pixelScale;
  const vector<uint> &provinces;
  map<uint, pair<int, bool>> &borderIdx;
  vector<bool> borderFlags;
  map<uint, vector<uint>> bordersMap;
  vector<vec2> border, borderUp;
  vector<vec2> position, uv;
  vector<uint> indices;
  vector<uvec1> borderId;
  Provinces(PoliticalMap &political_map, float pixel_scale) :
    w(political_map.w), h(political_map.h), pixelScale(pixel_scale),
    provinces(political_map.provincesIdx), borderIdx(political_map.borderIndexes), borderFlags((w+1)*(h+1)*4, false)
  {}
  uint get(int i, int j) const
  {
    return (0 <= i && i < h && 0 <= j && j < w) ? provinces[i * w + j] : PoliticalMap::MAX_PROVINCES;
  }
  uint get_clamp(int i, int j) const
  {
    i = glm::clamp(i, 0, h-1);
    j = glm::clamp(j, 0, w-1);
    return provinces[i * w + j] & PoliticalMap::PROVINCES_MASK;
  }
  uint get_clamp_to_border(int i, int j) const
  {
    return (0 <= i && i < h && 0 <= j && j < w) ? provinces[i * w + j] & PoliticalMap::PROVINCES_MASK : PoliticalMap::MAX_PROVINCES;
  }
  void add_border(uint prov1, uint prov2, uint position, bool up)
  {
    uint key = (prov1 < prov2 ? prov1 | (prov2 << 16u) : prov2 | (prov1 << 16u));
    bordersMap[key].push_back(position | ((up ? 1 : 0) << 31u));
  }

  void try_add_border(uint a, uint b, uint c, uint d, uint position, uint direction)
  {
    if (a != b && (a != d || d != c) && (d != c || c != b) && !(a == d && b == c) && !(a == c || b == d))
    {
      uint i = position*4+direction;
      if (!borderFlags[i])
      {
        borderFlags[i] = true;
        int x = position % w;
        int y = position / w;
        start_mark_border(a, b, ivec2(x, y), direction, false);
      }
    }
  }
  void try_add_cyclic_border(uint a, uint b, uint position, uint direction)
  {
    uint i = position*4+direction;
    if (!borderFlags[i])
    {
      borderFlags[i] = true;
      int x = position % w;
      int y = position / w;
      start_mark_border(a, b, ivec2(x, y), direction, true);
    }
  }
  void start_mark_border(uint a, uint b, ivec2 p, uint direction, bool cyclic)
  {
    ivec2 off[4] = {ivec2(0,-1), ivec2(1, 0), ivec2(0, 1), ivec2(-1,0)};
    vector<vec2> borderRaw;
    borderRaw.push_back(p);
    p += off[direction];
    uint dirOpposite = (direction + 2)&3;
    borderFlags[(p.y*w+p.x)*4+dirOpposite] = true;
    borderRaw.push_back(p);
    int prevDirection = direction;
    bool findEnd = false;
    int cnt = 0;
    constexpr int MAX_BORDER_LENGTH = 500;
    while (!findEnd && cnt++ < MAX_BORDER_LENGTH)
    {
      uint samples[4] = {
        get_clamp_to_border(p.y-1, p.x-1),
        get_clamp_to_border(p.y-1, p.x-0),
        get_clamp_to_border(p.y-0, p.x-0),
        get_clamp_to_border(p.y-0, p.x-1)
      };
      findEnd = true;
      for (int j = 0, i = (prevDirection + 3)&3  ; j < 3; j++, i = (i + 1)&3)
      {
        if (samples[i] == a && samples[(i+1)&3] == b)
        {
          uint pos = (p.y*w+p.x)*4;
          borderFlags[pos+i] = true;
          p += off[i];
          pos = (p.y*w+p.x)*4;
          uint prevDirOpposite = (i + 2)&3;
          borderFlags[pos+prevDirOpposite] = true;
          if (!cyclic || (cyclic && !borderFlags[pos+i]))
            borderRaw.push_back(p);
          prevDirection = i;
          findEnd = borderFlags[pos+i];
          break;
        }
      }
    }
    if (cnt >= MAX_BORDER_LENGTH)
    {
      debug_error("so long border");
      for (vec2 p : borderRaw)
        debug_log("%.0f %.0f", p.x, p.y);
    }
    else
    {
      prevDirection = (prevDirection+2)&3;
      borderFlags[(p.y*w+p.x)*4+prevDirection] = true;
    }

    border.resize(borderRaw.size());
    constexpr int N = 3;
    float weights[2*N+1] = {0.05, 0.1, 0.2, 0.3, 0.2, 0.1, 0.05 };
    float cyclicsWeights[2*N+1] = {0.00, 0.05, 0.2, 0.5, 0.2, 0.05, 0.00 };
    border[0] = borderRaw[0];
    for (int i = 1, n = borderRaw.size(); i < n-1; i++)
    {
      vec2 weightedPos = vec2(0,0);
      for (int j = i-N, k = 0; j <= i+N; j++, k++)
      {
        int idx = cyclic ? (j + n) % n : 0 <= j ? (j < n ? j : n-1) : 0;
        weightedPos += borderRaw[idx] * (cyclic ? cyclicsWeights[k] : weights[k]);
      }
      border[i] = weightedPos;
    }
    border.back() = borderRaw.back();
    borderUp.resize(border.size());
    for (int i = 0, n = border.size()-1; i < n; i++)
      borderUp[i] = up_axis(normalize(borderRaw[i+1] - borderRaw[i]));
    borderUp.back() = borderUp[border.size()-2];
    for (int i = 0, n = borderUp.size(); i < n-1; i++)
      borderUp[i] = (borderUp[i] + borderUp[i+1]) * 0.5f;
    for (int i = 1, n = borderUp.size(); i < n; i++)
      borderUp[i] = (borderUp[i] + borderUp[i-1]) * 0.5f;
    for (int i = 0, n = borderUp.size(); i < n; i++)
      borderUp[i] = normalize(borderUp[i]);
    bool swaped = b > a;
    if (swaped)
      std::swap(a, b);
    uint key = (a << 16u) | b;
    auto it = borderIdx.find(key);
    int borderIndex = borderIdx.size();
    if (it == borderIdx.end())
      borderIdx[key] = {borderIdx.size(), swaped};
    else
      borderIndex = it->first;
    add_border_mesh_data(borderIndex);

  }
  vec2 up_axis(vec2 d)
  {
    return vec2(-d.y, d.x);
  }
  void add_border_mesh_data(int index)
  {
    if (border.size() < 2)
      return;
    uint vertStart = position.size();
    position.reserve(position.size() + border.size()*2+2);
    uv.reserve(uv.size() + border.size()*2+2);
    indices.reserve(indices.size() + border.size()*6);
    borderId.reserve(borderId.size() + border.size()+1);

    float borderWidth = 1.25f;
    for (uint i = 0, n = border.size(); i < n; i++)
    {
      vec2 up = borderUp[i];
      position.emplace_back((border[i] - up * borderWidth) * pixelScale);
      position.emplace_back((border[i] + up * borderWidth) * pixelScale);
      uv.emplace_back(vec2(i, 0));
      uv.emplace_back(vec2(i, 1));
      borderId.emplace_back(index);
      borderId.emplace_back(index);
    }
    for (uint i = 0, n = border.size()-1; i < n; i++)
    {
      uint a = i * 2, b = a + 1, c = a + 2, d = a + 3;
      indices.push_back(vertStart+a);
      indices.push_back(vertStart+c);
      indices.push_back(vertStart+b);
      indices.push_back(vertStart+b);
      indices.push_back(vertStart+c);
      indices.push_back(vertStart+d);
    }
  }
  Asset<Mesh> generate_mesh()
  {
    return Asset<Mesh>("borders", indices, position, uv, borderId);
  }
};


Asset<Mesh> build_borders(PoliticalMap &political_map, float pixel_scale)
{
  Provinces provinces(political_map, pixel_scale);
  int h = political_map.h, w = political_map.w;
  for (int i = 0; i <= h; i++)
    for (int j = 0; j <= w; j++)
    {
      uint pos = i * provinces.w + j;
      uint a = provinces.get_clamp_to_border(i-1, j-1);
      uint b = provinces.get_clamp_to_border(i-1, j-0);
      uint c = provinces.get_clamp_to_border(i-0, j-0);
      uint d = provinces.get_clamp_to_border(i-0, j-1);
      if (j != 0 && j != w)
      {
        provinces.try_add_border(a,b,c,d, pos, 0);
        provinces.try_add_border(c,d,a,b, pos, 2);
      }
      if (i != 0 && i != h)
      {
        provinces.try_add_border(b,c,d,a, pos, 1);
        provinces.try_add_border(d,a,b,c, pos, 3);
      }
    }
  for (int i = 1; i < h; i++)
    for (int j = 1; j < w; j++)
    {
      uint pos = i * provinces.w + j;
      uint b = provinces.get(i-1, j-0);
      uint c = provinces.get(i-0, j-0);
      uint d = provinces.get(i-0, j-1);
      if (c != d)
        provinces.try_add_cyclic_border(c,d, pos, 2);
      if (b != c)
        provinces.try_add_cyclic_border(b,c, pos, 1);
    }
  return provinces.generate_mesh();
}