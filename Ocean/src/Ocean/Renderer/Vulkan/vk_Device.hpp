#pragma once

/**
 * @file vk_Device.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Macros.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        enum class vkPhysicalDeviceFeatureBits : u64 {
            robustBufferAccess                      = 1ULL << 0,
            fullDrawIndexUint32                     = 1ULL << 1,
            imageCubeArray                          = 1ULL << 2,
            independentBlend                        = 1ULL << 3,
            geometryShader                          = 1ULL << 4,
            tessellationShader                      = 1ULL << 5,
            sampleRateShading                       = 1ULL << 6,
            dualSrcBlend                            = 1ULL << 7,
            logicOp                                 = 1ULL << 8,
            multiDrawIndirect                       = 1ULL << 9,
            drawIndirectFirstInstance               = 1ULL << 10,
            depthClamp                              = 1ULL << 11,
            depthBiasClamp                          = 1ULL << 12,
            fillModeNonSolid                        = 1ULL << 13,
            depthBounds                             = 1ULL << 14,
            wideLines                               = 1ULL << 15,
            largePoints                             = 1ULL << 16,
            alphaToOne                              = 1ULL << 17,
            multiViewport                           = 1ULL << 18,
            samplerAnisotropy                       = 1ULL << 19,
            textureCompressionETC2                  = 1ULL << 20,
            textureCompressionASTC_LDR              = 1ULL << 21,
            textureCompressionBC                    = 1ULL << 22,
            occlusionQueryPrecise                   = 1ULL << 23,
            pipelineStatisticsQuery                 = 1ULL << 24,
            vertexPipelineStoresAndAtomics          = 1ULL << 25,
            fragmentStoresAndAtomics                = 1ULL << 26,
            shaderTessellationAndGeometryPointSize  = 1ULL << 27,
            shaderImageGatherExtended               = 1ULL << 28,
            shaderStorageImageExtendedFormats       = 1ULL << 29,
            shaderStorageImageMultisample           = 1ULL << 30,
            shaderStorageImageReadWithoutFormat     = 1ULL << 31,
            shaderStorageImageWriteWithoutFormat    = 1ULL << 32,
            shaderUniformBufferArrayDynamicIndexing = 1ULL << 33,
            shaderSampledImageArrayDynamicIndexing  = 1ULL << 34,
            shaderStorageBufferArrayDynamicIndexing = 1ULL << 35,
            shaderStorageImageArrayDynamicIndexing  = 1ULL << 36,
            shaderClipDistance                      = 1ULL << 37,
            shaderCullDistance                      = 1ULL << 38,
            shaderFloat64                           = 1ULL << 39,
            shaderInt64                             = 1ULL << 40,
            shaderInt16                             = 1ULL << 41,
            shaderResourceResidency                 = 1ULL << 42,
            shaderResourceMinLod                    = 1ULL << 43,
            sparseBinding                           = 1ULL << 44,
            sparseResidencyBuffer                   = 1ULL << 45,
            sparseResidencyImage2D                  = 1ULL << 46,
            sparseResidencyImage3D                  = 1ULL << 47,
            sparseResidency2Samples                 = 1ULL << 48,
            sparseResidency4Samples                 = 1ULL << 49,
            sparseResidency8Samples                 = 1ULL << 50,
            sparseResidency16Samples                = 1ULL << 51,
            sparseResidencyAliased                  = 1ULL << 52,
            variableMultisampleRate                 = 1ULL << 53,
            inheritedQueries                        = 1ULL << 54,

        };  // vkPhysicalDeviceFeatureBits

        class vkDevice {
        public:
            vkDevice() = default;
            // vkDevice(VkPhysicalDevice gpu, cstring* requestedExtensions, u32 extensionCount);
            // ~vkDevice();

            OC_INLINE VkPhysicalDevice GetPhysical() const { return this->m_gpu; }
            OC_INLINE VkDevice GetLogical() const { return this->m_Device; }

        private:
            OC_NO_COPY(vkDevice);

            VkPhysicalDevice m_gpu;
            /** @todo Figure out a more compact way to store the gpuProperties and gpuMemory. */
            vkPhysicalDeviceFeatureBits m_gpuFeatures;
            VkPhysicalDeviceProperties m_gpuProperties;
            VkPhysicalDeviceMemoryProperties m_gpuMemory;

            VkDevice m_Device;

            VkQueue m_Queue;
            u32 m_GraphicsQueueIndex;
            u32 m_PresentQueueIndex;

        };  // vkDevice

    }   // Splash

}   // Ocean
