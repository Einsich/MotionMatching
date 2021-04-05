#pragma once


constexpr float walkForwardSpeed = 1.2f;
constexpr float walkSidewaySpeed = 1.2f;
constexpr float walkBackwardSpeed = 1.f;

constexpr float runForwardSpeed = 2.7f*1.2f;
constexpr float runSidewaySpeed = 2.f*1.2f;
constexpr float runBackwardSpeed = 2.1f*1.2f;

constexpr float hipsHeightStand = 0.967f;
constexpr float hipsHeightCrouch = 0.35f;
constexpr float hipsHeightJum = 1.2f;

constexpr float runSpeeds[3] = {runForwardSpeed, runSidewaySpeed, runBackwardSpeed};
constexpr float walkSpeeds[3] = {walkForwardSpeed, walkSidewaySpeed, walkBackwardSpeed};
