#include "farm.h"

#include <thread>

void Farm::add_task(Task* task)
{
	queue_.push(task);
}

void Farm::run()
{
	const int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread*> threads_vec;

	for (int i = 0; i < num_threads; i++) {
		threads_vec.push_back(new std::thread([&]
			{
				while (!queue_.empty()) {
					std::mutex queue_mutex;
					queue_mutex.lock();
					Task* task;

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
