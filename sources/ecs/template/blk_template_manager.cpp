#include "blk_template.h"
#include "serialization/serialization.h"
#include "application/application_data.h"
#include "data_block/data_block.h"
#include <filesystem>
#include <map>
namespace fs = filesystem;
namespace ecs
{ 
  struct TemplateFile
  {
    string path;
    DataBlockPtr fileBlk;
    TemplateFile(const string &path, DataBlockPtr fileBlk):
      path(path), fileBlk(fileBlk)
    {}
  };
  
  struct RawTemplate
  {
    string name;
    const DataBlock* blk;
    vector<string> extends;
    vector<size_t> extendsIdx;
  };
  struct BlkTemplateManager
  {
    vector<TemplateFile> templatesFiles;
    vector<Template> templates;
    map<string, int> templateMap;
    static BlkTemplateManager &instance() 
    {
      static BlkTemplateManager manager;
      return manager;
    }
    const Template *find(const char *name)
    {
      auto it = templateMap.find(name);
      return it != templateMap.end() ? &templates[it->second] : nullptr;
    }
  };

  const Template* get_template(const char *name)
  {
    return BlkTemplateManager::instance().find(name);
  }
  void invalidate_cached_archetype()
  {
    for (Template &t : BlkTemplateManager::instance().templates)
    {
      t.archetype = nullptr;
      t.containers.clear();
    }
  }
  template<typename T>
  static ComponentInstance create_instance(const DataBlock &blk, const DataBlock::Property &property)
  {
    constexpr string_hash hash = HashedString(nameOf<T>::value);
    return ComponentInstance(ecs::TypeInfo::types()[hash], property.name, blk.get<T>(property));
  }
  GET_FUNCTIONS(instantiate, create_instance)


  static void collect_template_files(vector<TemplateFile> &templatesFiles)
  {
    templatesFiles.clear();
    for (const auto &path : Application::instance().templatePaths)
    {
      if (!fs::exists(path))
        continue;
      for (const auto & entry : fs::recursive_directory_iterator(path))
      {
        if (entry.is_regular_file() && entry.path().extension() == ".blk")
        {
          string strPath = entry.path().string();
          templatesFiles.emplace_back(strPath, make_shared<DataBlock>(strPath));
        }
      }
    }
  }

  static void fill_raw_template(const vector<TemplateFile> &templateFiles, vector<RawTemplate> &rawTemplates)
  {
    for (const TemplateFile &file : templateFiles)
    {
      const DataBlock &blk = *file.fileBlk.get();
      for (size_t tmplId = 0, tmplN = blk.blockCount(); tmplId < tmplN; tmplId++)
      {
        const DataBlock *tmpl = blk.getBlock(tmplId);
        assert(tmpl);
        RawTemplate &rawTemplate = rawTemplates.emplace_back();
        rawTemplate.name = tmpl->name();
        rawTemplate.blk = tmpl;
        for (size_t i = 0, n = tmpl->propertiesCount(); i < n; i++)
        {
          const DataBlock::Property &property = tmpl->getProperty(i);
          if (property.name == "_extends")
            rawTemplate.extends.emplace_back(tmpl->get<string>(property));
        }
      }
    }
  }

  enum {
    NOT_VISITED,
    VISITED,
    VERIFIED,
    NOT_VERIFIED
  };

  static bool validate_template(const vector<RawTemplate> &templates, size_t templateId, vector<int> &status)
  {
    if (status[templateId] == VERIFIED)
      return true;
    if (status[templateId] == NOT_VERIFIED || status[templateId] == VISITED)
      return false;
      
    status[templateId] = VISITED;
    for (const string &extends : templates[templateId].extends)
    {
      bool hasExtends = false;
      for (size_t i = 0, n = templates.size(); i < n; ++i)
        if (templates[i].name == extends)
        {
          if (!validate_template(templates, i, status))
            return false;
          hasExtends = true;
        }
      if (!hasExtends)
        debug_error("don't exists extends \"%s\" for template \"%s\"", extends.c_str(), templates[templateId].name.c_str());
    }
    status[templateId] = VERIFIED;
    return true;
  }

  static void validate_templates(vector<RawTemplate> &templates)
  {
    vector<int> status(templates.size(), NOT_VISITED);
    for (size_t i = 0, n = templates.size(); i < n; ++i)
    {
      if (!validate_template(templates, i, status))
      {
        debug_error("template \"%s\" has cyclic dependencies!", templates[i].name.c_str());
      }
    }
    size_t first = 0, last = templates.size();
    for(; first < last; ++first)
      if (status[first] != VERIFIED)
        break;
    if (first != last)
      for(size_t i = first, n = templates.size(); ++i < n;)
        if (status[i] != VERIFIED)
        {
          templates[first] = std::move(templates[i]);
          first++;
        }
    templates.erase(templates.begin() + first, templates.end());
  }

  static void init_templates(map<string, int> &templateMap, vector<RawTemplate> &rawTemplates, vector<Template> &templates)
  {
    templateMap.clear();
    templates.clear();
    templates.resize(rawTemplates.size());
    for (size_t i = 0, n = rawTemplates.size(); i < n; ++i)
      templateMap.emplace(rawTemplates[i].name, i);

    for (size_t i = 0, n = rawTemplates.size(); i < n; ++i)
    {
      templates[i].name = rawTemplates[i].name;
      for (const string &extends : rawTemplates[i].extends)
      {
        auto it = templateMap.find(extends);
        if (it != templateMap.end())
          rawTemplates[i].extendsIdx.emplace_back(it->second);
      }
    }
  }

  void init_components_from_blk(const DataBlock *tmpl, vector<ComponentInstance> &components)
  {
    const auto &typeMap = ecs::TypeInfo::types();
    for (size_t i = 0, n = tmpl->propertiesCount(); i < n; i++)
    {
      const DataBlock::Property &property = tmpl->getProperty(i);
      if (property.name != "_extends" && property.name != "_template")
      {
        components.emplace_back(instantiate[property.type](*tmpl, property));
      }
    }
    for (size_t i = 0, n = tmpl->blockCount(); i < n; i++)
    {
      const DataBlock *property = tmpl->getBlock(i);
  
      auto it = typeMap.find(HashedString(property->type()));
      if (it != typeMap.end())
      {
        const ecs::TypeInfo &typeInfo = it->second;
        components.emplace_back(typeInfo, property->name());
        typeInfo.blkReader(*property, components.back().get_data());
      }
      else
      {
        debug_error("unknown type %s:%s", property->name().c_str(),  property->type().c_str());
      }
    }
  }

  static void init_components(const RawTemplate &rawTemplate, vector<ComponentInstance> &components)
  {
    init_components_from_blk(rawTemplate.blk, components);
    
    const auto &typeMap = ecs::TypeInfo::types();
    auto it = typeMap.find(HashedString("EntityId"));
    assert(it != typeMap.end());//we should know EntityId
    components.emplace_back(ComponentInstance(it->second, "eid"));
  }



  static void linearize_extends(
      const vector<RawTemplate> &rawTemplates,
      vector<Template> &templates,
      size_t templateId)
  {
    Template &blkTemplate = templates[templateId];
    if (!blkTemplate.components.empty())
      return;
    
    for (size_t i : rawTemplates[templateId].extendsIdx)
    {
      linearize_extends(rawTemplates, templates, i);
      patch_components(blkTemplate.components, templates[i].components);
    }
    vector<ComponentInstance> components;
    init_components(rawTemplates[templateId], components);
    patch_components(blkTemplate.components, std::move(components));

  }

  static void linearize_extends(vector<RawTemplate> &rawTemplates, vector<Template> &templates)
  {
    for (size_t i = 0, n = templates.size(); i < n; ++i)
    {
      linearize_extends(rawTemplates, templates, i);
    }
  }

  void load_templates_from_blk()
  {
    BlkTemplateManager &manager = BlkTemplateManager::instance();
    
    collect_template_files(manager.templatesFiles);

    vector<RawTemplate> rawTemplates;
    fill_raw_template(manager.templatesFiles, rawTemplates);
  
    validate_templates(rawTemplates);//remove cyclic dependencies
  
    init_templates(manager.templateMap, rawTemplates, manager.templates);

    linearize_extends(rawTemplates, manager.templates);
  }
}