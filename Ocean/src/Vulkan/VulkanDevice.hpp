#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"
#include "Ocean/Core/Types/Strings.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

namespace Ocean {

	class Allocator;

	class CommandBuffer;
	class DeviceFrame;
	class Device;



	struct DeviceTimestamp {
		u32 Start, End;

		f64 Elapsedms;

		u16 ParentIndex, Depth;

		u32 Color, FrameIndex;

		cstring Name;

	};	// DeviceTimestamp



	class DeviceTimestampManager {
	public:
		void Init(Allocator* allocator, u16 queriesPerFrame, DeviceTimestamp* timestampsToFill);
		void Shutdown();

		b8 HasValidQueries() const;

		void Reset();
		u32 Resolve(u32 currentFrame, DeviceTimestamp* timestampsToFill);

		u32 Push(u32 currentFrame, cstring name);
		u32 Pop(u32 currentFrame);

	private:
		Allocator* p_Allocator;

		DeviceTimestamp* p_Timestamps;
		u64* p_TimestampData;

		u32 m_QueriesPerFrame;
		u32 m_CurrentQuery;
		u32 m_ParentIndex;
		u32 m_Depth;

		b8 m_CurrentFrameResolved = false;

	};



	struct DeviceInfo {
		Allocator* allocator = nullptr;
		

	};	// DeviceInfo

	class Device : public Service {
	public:
		Device() = default;
		~Device() = default;

		OCEAN_DECLARE_SERVICE(Device);

		void Init(void* config) override;
		void Shutdown() override;



	};

}	// Ocean
