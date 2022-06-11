#include "application_metainfo.h"
#include "utils/compile_time_string.h"
#include "data_block/data_block_serialization.h"


ApplicationMetaInfo::ApplicationMetaInfo(const string &path)
{
  DataBlock project_config(path);
  read(project_config, *this);
}
