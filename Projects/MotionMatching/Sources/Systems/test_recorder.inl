#include "ecs/ecs.h"
#include "imgui/imgui.h"
#include "Animation/Test/animation_tester.h"
#include "motion_matching_scene.h"
#include "Time/time.h"


SYSTEM(ecs::SystemOrder::UI) recorder_ui(
  MotionMatchingScene &motionMatchingScene,
  int &recordedTest,
  int &recordedState,
  float &recorderStartTime)
{
  ImGui::Begin("Test list");

  auto &tests = motionMatchingScene.dataBase->tests;
  constexpr int BUF_SIZE = 100;
  char buf[BUF_SIZE];
  static std::string lastName;
  if (ImGui::InputText("Test name", buf, BUF_SIZE))
    lastName = std::string(buf);

  if (ImGui::Button("new test", ImVec2(70, 20)))
  {

    if (lastName == "")
    {
      snprintf(buf, BUF_SIZE, "test %ld", tests.size());
      lastName = std::string(buf);
    }
    AnimationTest &test = tests.emplace_back();
    test.name = lastName;
    lastName = "";
  }
  
  for (int i = 0; i < tests.size(); i++)
  {
    AnimationTest &test = tests[i];
    ImGui::Text("%s, t = %f, k = %ld, m = %ld", test.name.c_str(), test.totalTime, test.keyboardEvents.size(), test.mouseMoveEvents.size());
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
  const KeyboardEvent &e,
  MotionMatchingScene &motionMatchingScene,
  int recordedTest,
  int recordedState,
  float recorderStartTime)
{
  if (recordedTest >= 0 && recordedState == 1)
  {
    KeyboardEvent event = e;
    event.time -= recorderStartTime;
    motionMatchingScene.dataBase->tests[recordedTest].keyboardEvents.push_back(event);
  }
}

EVENT() listener_mousemove(
  const MouseMoveEvent &e,
  MotionMatchingScene &motionMatchingScene,
  int recordedTest,
  int recordedState,
  float recorderStartTime)
{
  if (recordedTest >= 0 && recordedState == 1)
  {
    MouseMoveEvent event = e;
    event.time -= recorderStartTime;
    motionMatchingScene.dataBase->tests[recordedTest].mouseMoveEvents.push_back(event);
  }
}