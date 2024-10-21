#pragma once

#include "Ocean/Core/Primitives/Array.hpp"

namespace Ocean {

	class ThreadPool {
	public:
		void Init();
		void Shutdown();

		void SubmitTask();

	private:
		// FixedArray<> m_Threads;

		// Queue<> m_Tasks;

	};

}
