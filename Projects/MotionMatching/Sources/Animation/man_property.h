#pragma once


constexpr float walkSpeed = 1.2f;
constexpr float runForwardSpeed = 2.5f*walkSpeed;
constexpr float runSidewaySpeed = 2.f*walkSpeed;
constexpr float runBackwardSpeed = 1.5f*walkSpeed;

constexpr float hipsHeightStand = 0.967f;
constexpr float hipsHeightCrouch = 0.35f;
constexpr float hipsHeightJum = 1.2f;

constexpr float runSpeeds[3] = {runForwardSpeed, runSidewaySpeed, runBackwardSpeed};
