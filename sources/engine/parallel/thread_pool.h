#pragma once
#include <functional>



void add_job(std::function<void()>&& f);
void add_main_thread_job(std::function<void()> &&job);