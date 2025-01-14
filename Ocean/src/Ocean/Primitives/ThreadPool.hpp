#pragma once

#include "Ocean/Core/Primitives/Array.hpp"
#include "Ocean/Core/Primitives/Queue.hpp"

// std
#include <thread>
#include <mutex>
#include <functional>

namespace Ocean {

	class ThreadPool {
	public:
		void Init();
		void Shutdown();

		void SubmitTask();

	private:
		mutable std::mutex m_Mutex;
		std::condition_variable m_Conditional;

		FixedArray<std::thread> m_Threads;
		b8 m_ShutdownRequested;

		PriorityQueue<std::function<void()>> m_Tasks;

	};

}
