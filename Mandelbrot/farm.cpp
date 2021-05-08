#include "farm.h"

#include <thread>

void Farm::add_task(Task* task)
{
	queue_.push(task);
}

void Farm::run()
{
	const int num_threads = std::thread::hardware_concurrency(); // function returns num of CPUs
	std::vector<std::thread*> threads_vec;

	std::mutex queue_mutex; // mutex protecting queue

	for (int i = 0; i < num_threads; i++) {
		threads_vec.push_back(new std::thread([&]
			{
				while (!queue_.empty()) {

					Task* task;
					
					queue_mutex.lock();

					task = queue_.front();
					queue_.pop();

					queue_mutex.unlock();
					task->run();
				}
			}));
	}

	for (std::thread* thread : threads_vec) {
		thread->join();
	}
}
