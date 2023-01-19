#pragma once
#include <functional>


void wait_jobs();
void add_job(std::function<void()>&& f);
void add_main_thread_job(std::function<void()> &&job);
int get_current_thread_id();
int get_worker_count();