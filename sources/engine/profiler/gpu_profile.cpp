#include "profiler.h"
#include <glad/glad.h>
#include <queue>

struct GPULabel
{
  GLuint query, query1;
  const char *label;
  bool isOpen;
};

static queue<GLuint> query_id_pull;
static vector<GPULabel> queries;

GLuint add_qpu_query_id()
{
  GLuint query;
  if (!query_id_pull.empty())
  {
    query = query_id_pull.front();
    query_id_pull.pop();
  }
  else
  {
    glGenQueries(1, &query);
  }
  return query;
}

void process_gpu_time_queries()
{
  if (!queries.empty())
    queries.emplace_back(GPULabel{0, 0, nullptr, false});
  else
    return;
  bool isReady = false;
  for (auto [startQuery, endQuery, label, isOpen] : queries)
  {
    if (!label)
    {
      isReady = true;
      break;
    }
    GLint ready, ready1;
    glGetQueryObjectiv(startQuery, GL_QUERY_RESULT_AVAILABLE, &ready);
    glGetQueryObjectiv(endQuery, GL_QUERY_RESULT_AVAILABLE, &ready1);
    if (!ready || !ready1)
      break;
  }
  if (!isReady)
    return;
  int i = 0;
  for (auto [startQuery, endQuery, label, isOpen] : queries)
  {
    if (!label)
    {
      break;
    }
    if (isOpen)
    {
      get_gpu_profiler().open_label(label);
    }
    else
    {
      GLuint64 start, end;
      glGetQueryObjectui64v(startQuery, GL_QUERY_RESULT, &start);
      glGetQueryObjectui64v(endQuery, GL_QUERY_RESULT, &end);
      get_gpu_profiler().close_label((end - start) * 0.000001, label);
      query_id_pull.push(startQuery);
      query_id_pull.push(endQuery);
    }
    i++;
  }
  queries.erase(queries.begin(), queries.begin() + i + 1);
}

ProfilerLabelGPU::ProfilerLabelGPU(const char *label):
  stopped(false), label(label)
{
  startQuery = add_qpu_query_id();
  endQuery = add_qpu_query_id();
  glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, label);
  glQueryCounter(startQuery, GL_TIMESTAMP );
  queries.emplace_back(GPULabel{startQuery, endQuery, label, true});
}
ProfilerLabelGPU::~ProfilerLabelGPU()
{
  stop();
}
void ProfilerLabelGPU::stop()
{
  if (!stopped)
  {
    glQueryCounter(endQuery, GL_TIMESTAMP);
    glPopDebugGroup();
    stopped = true;
    queries.emplace_back(GPULabel{startQuery, endQuery, label, false});
  }
}

Profiler &get_gpu_profiler()
{
  static Profiler profiler;
  return profiler;
}