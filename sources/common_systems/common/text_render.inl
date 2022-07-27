#include <ecs.h>
#include <render/global_uniform.h>
#include <render/mesh.h>
#include <render/shader/shader.h>
#include <render/render.h>
#include <imgui.h>
#include <memory/tmp_allocator.h>
#include <ecs/editor_events.h>

enum class TextAlignment
{
  Left, Middle, Right
};

static void load_fonts()
{
  static const ImWchar ranges[] =
  {
      0x0020, 0x00FF, // Basic Latin + Latin Supplement
      0x0400, 0x044F, // Cyrillic
      0,
  };
  ImGuiIO& io = ImGui::GetIO();
  ImFontAtlas* atlas = io.Fonts;
  atlas->AddFontDefault();
  atlas->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 14.f, nullptr, ranges);
  atlas->AddFontFromFileTTF("C:/Windows/Fonts/times.ttf", 14.f, nullptr, ranges);
  atlas->Build();
}

#include <type_registration.h>
ECS_REGISTER_TYPE(TextAlignment, TextAlignment);

static GLuint CreateFontsTexture()
{
  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);  

  GLuint fontTexture;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

  glGenTextures(1, &fontTexture);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef GL_UNPACK_ROW_LENGTH
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif   
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED , GL_UNSIGNED_BYTE, pixels);

  return fontTexture;
}

struct TextRender : ecs::Singleton
{
  Mesh quad;
  Shader shader;
  int transformVarId, colorVarId, altasVarId, altasBinding;
  bool invalid = true;
  uint altasTexId = 0;
  TextRender()
  {
    shader = get_shader("text_shader");
    if (!shader || shader.get_samplers().empty())
      return;
    load_fonts();
    std::vector<uint> indices = {0,2,1,0,3,2};
    std::vector<vec2> corners =  {vec2(-1, -1), vec2(1,-1), vec2(1,1),vec2(-1,1)};
    quad = Mesh(indices, corners);
    quad.clear_cpu_data();
    transformVarId = glGetUniformLocation(shader.get_shader_program(), "transform");
    colorVarId = glGetUniformLocation(shader.get_shader_program(), "color");
    altasVarId = shader.get_samplers()[0].shaderLocation;
    altasBinding = shader.get_samplers()[0].binding;
    altasTexId = CreateFontsTexture();
    add_storage_buffer("TextData", 0, 0);
    invalid = false;
  }
};


void setup_text_buffer(ecs::vector<vec4> &glyph_buffer, const ecs::string &text, TextAlignment aligment, vec2 pivot)
{
  const ImGuiIO& io = ImGui::GetIO();
  const ImFontAtlas* atlas = io.Fonts;
  const ImFont* font = atlas->Fonts[0];

  int instanceCount = text.size();
  glyph_buffer.resize(instanceCount * 2);

  vec2 pos(0.f);
  float maxWidth = 0, maxHeight = 0;
  struct LineInfo
  {
    uint to;//[ )
    float width;
  };

  eastl::vector<LineInfo, tmp_allocator> lines;
  uint bufferIdx = 0;
  for (uint i = 0, n = instanceCount; i < n; i++) 
  {
    char c = text[i];
    bool usualChar = false;
    switch (c)
    {
    case '\n':
      if (pos.x > 0)
      {
        lines.push_back(LineInfo{bufferIdx, pos.x});
        maxWidth = std::max(maxWidth, pos.x);
      }
      maxHeight += font->FontSize;
      pos.x = 0;
      pos.y += font->FontSize;
      break;
    
    default: usualChar = true;
      break;
    }
    if (!usualChar)
      continue;

    const ImFontGlyph* glyph = font->FindGlyph(c);
    glyph = glyph ? glyph : font->FallbackGlyph;

    glyph_buffer[bufferIdx+0] = vec4(glyph->U0, glyph->V0, glyph->U1-glyph->U0, glyph->V1-glyph->V0);
    glyph_buffer[bufferIdx+1] = vec4(pos, 0,0) + vec4(glyph->X0, glyph->Y0, glyph->X1-glyph->X0, glyph->Y1-glyph->Y0);
    pos.x += glyph->AdvanceX;
    
    bufferIdx += 2;
  }
  lines.push_back(LineInfo{bufferIdx, pos.x});

  glyph_buffer.resize(bufferIdx);//trim from special chars


  maxWidth = std::max(maxWidth, pos.x);
  maxHeight += font->FontSize;

  uint curLine = 0;
  vec4 toCenterOffset = vec4(maxWidth * (-pivot.x - 1.f), maxHeight * (pivot.y - 1.f), 0, 0) * 0.5f;
  for (uint i = 0; i < glyph_buffer.size(); i+=2)
  {
    if (lines[curLine].to <= i)
      curLine++;
    glyph_buffer[i + 1] += toCenterOffset;
    switch (aligment)
    {
    case TextAlignment::Middle : glyph_buffer[i + 1].x += (maxWidth - lines[curLine].width) * 0.5f;
      break;
    case TextAlignment::Right : glyph_buffer[i + 1].x += maxWidth - lines[curLine].width;
      break;
    default:
      break;
    }
  }
}


EVENT() text_changed(const OnEntityEdited&,
    ecs::vector<vec4> &glyph_buffer,
    const ecs::string &text,
    TextAlignment aligment,
    vec2 pivot,
    const TextRender &textRender)
{
  if (!textRender.invalid)
    setup_text_buffer(glyph_buffer, text, aligment, pivot);
}

EVENT() text_appear(const ecs::OnEntityCreated&,
    ecs::vector<vec4> &glyph_buffer,
    const ecs::string &text,
    TextAlignment aligment,
    vec2 pivot,
    const TextRender &textRender)
{
  if (!textRender.invalid)
    setup_text_buffer(glyph_buffer, text, aligment, pivot);
}

template<typename Callable> 
void gather_text(Callable);

SYSTEM(stage=render_ui; scene=game, editor) text_render(const EditorRenderSettings &editorSettings, const TextRender &textRender)
{
  if (textRender.invalid)
    return;

  bool wire_frame = editorSettings.wire_frame;

  UniformBuffer &textData = get_buffer("TextData");
  textData.bind();


  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  auto[w,h] = get_resolution();
  float dw = 2.f / w, dh = 2.f / h;
  auto shader = textRender.shader;

  shader.use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textRender.altasTexId);
  glUniform1i(textRender.altasVarId, textRender.altasBinding);
  
  QUERY()gather_text([&](const ecs::string &text, const ecs::vector<vec4> &glyph_buffer, ivec2 position, float scale, const vec4 &color)
  {
    constexpr int instanceSize = sizeof(vec4)*2;
    int instanceCount = text.size();
    textData.update_buffer_and_flush(glyph_buffer.data(), instanceCount * instanceSize);

  
    mat3 transform(1.f);
    transform[0][0] = scale * dw;
    transform[1][1] = -scale * dh;
    transform[2][0] = position.x * dw-1.f;
    transform[2][1] = position.y * dh-1.f;

    shader.set_mat3x3(textRender.transformVarId, transform);
    shader.set_vec4(textRender.colorVarId, color);


    textRender.quad.render_instances(instanceCount, wire_frame);
  });
}