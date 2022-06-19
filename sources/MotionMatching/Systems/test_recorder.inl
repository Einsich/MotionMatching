#include <ecs.h>
#include <type_registration.h>
#include <imgui.h>
#include "Animation/Test/animation_tester.h"
#include "Animation/Test/animation_test.h"
#include <application/time.h>

ECS_REGISTER_TYPE_AND_VECTOR(AnimationTest, AnimationTest, false, true);

SYSTEM(stage=ui) recorder_ui(
  ecs::vector<AnimationTest> &tests,
  int &recordedTest,
  int &recordedState,
  float &recorderStartTime)
{
  ImGui::Begin("Test list");

  constexpr int BUF_SIZE = 100;
  char buf[BUF_SIZE];
  static std::string lastName;
  if (ImGui::InputText("Test name", buf, BUF_SIZE))
    lastName = std::string(buf);

  if (ImGui::Button("new test", ImVec2(70, 20)))
  {

    if (lastName == "")
    {
      snprintf(buf, BUF_SIZE, "test %llu", tests.size());
      lastName = std::string(buf);
    }
    AnimationTest &test = tests.emplace_back();
    test.name = lastName;
    lastName = "";
  }
  
  for (uint i = 0; i < tests.size(); i++)
  {
    AnimationTest &test = tests[i];
    ImGui::Text("%s, t = %f, k = %llu, m = %llu", test.name.c_str(), test.totalTime, test.keyboardEvents.size(), test.mouseMoveEvents.size());
    ImGui::SameLine();
    snprintf(buf, BUF_SIZE, "edit[%d]", i);
    if (ImGui::Button(buf, ImVec2(60, 20)))
      recordedTest = i;
  }
  ImGui::End();
  if (recordedTest >= 0)
  {
    AnimationTest &test = tests[recordedTest];
    ImGui::Begin("Test editor");    
    ImGui::SameLine();
    if (ImGui::Button("close", ImVec2(50, 20)))
    { 
      recordedTest = -1;
      recordedState = 0;
    }
    snprintf(buf, BUF_SIZE, "%s", test.name.c_str());
    if (ImGui::InputText("Test name", buf, BUF_SIZE))
      test.name = std::string(buf);
    if (recordedState == 0)//can record
    {
      if (ImGui::Button("record", ImVec2(50, 20)))
      {
        recordedState = 1;//record
        recorderStartTime = Time::time();
        test.keyboardEvents.clear();
        test.mouseMoveEvents.clear();
      }
    }
    else
    {
      if (ImGui::Button("stop", ImVec2(50, 20)))
      {
        recordedState = 0;//recorded
      }
      test.totalTime = Time::time() - recorderStartTime;
    }
    ImGui::End();
  }

}

EVENT() listener_keybord(
  const KeyEventAnyActionKey &e,
  ecs::vector<AnimationTest> &tests,
  int recordedTest,
  int recordedState,
  float recorderStartTime)
{
  if (recordedTest >= 0 && recordedState == 1)
  {
    KeyEventAnyActionKey event = e;
    event.time -= recorderStartTime;
    tests[recordedTest].keyboardEvents.push_back(event);
  }
}

EVENT() listener_mousemove(
  const MouseMoveEvent &e,
  ecs::vector<AnimationTest> &tests,
  int recordedTest,
  int recordedState,
  float recorderStartTime)
{
  if (recordedTest >= 0 && recordedState == 1)
  {
    MouseMoveEvent event = e;
    event.time -= recorderStartTime;
    tests[recordedTest].mouseMoveEvents.push_back(event);
  }
}