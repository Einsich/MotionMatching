#pragma once
#include <functional>



void start_job(std::function<void()>&& f);