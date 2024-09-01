#pragma once

#include "Ocean/Core/Primitives/Memory.hpp"

namespace Ocean {

	class VulkanProfiler {
	public:
		VulkanProfiler() = default;
		~VulkanProfiler() = default;

		void Init(Allocator* allocator, u32 maxFrames);
		void Shutdown();

		void Update();

		void ImguiDraw();

	private:
		Allocator* p_Allocator;



	};

}	// Ocean
