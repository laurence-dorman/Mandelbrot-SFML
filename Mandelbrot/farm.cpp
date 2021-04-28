#include "farm.h"

// FIXME - You may need to add #includes here (e.g. <thread>)

#include <thread>

void Farm::add_task(Task* task)
{
	// FIXME - Implement this
	queue_.push(task);
}

void Farm::run()
{
	// FIXME - Implement this

	const int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread*> threads_vec;

	for (int i = 0; i < num_threads; i++) {
		threads_vec.push_back(new std::thread([&]
			{
				while (!queue_.empty()) {
					Task* task;
					std::mutex queue_mutex;

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
