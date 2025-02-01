#pragma once

// libs
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Strings.hpp"
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        enum vkPhysicalDeviceFeatureBits : u64 {
            robustBufferAccess                      = 0 << 1,
            fullDrawIndexUint32                     = 0 << 2,
            imageCubeArray                          = 0 << 3,
            independentBlend                        = 0 << 4,
            geometryShader                          = 0 << 5,
            tessellationShader                      = 0 << 6,
            sampleRateShading                       = 0 << 7,
            dualSrcBlend                            = 0 << 8,
            logicOp                                 = 0 << 9,
            multiDrawIndirect                       = 0 << 10,
            drawIndirectFirstInstance               = 0 << 11,
            depthClamp                              = 0 << 12,
            depthBiasClamp                          = 0 << 13,
            fillModeNonSolid                        = 0 << 14,
            depthBounds                             = 0 << 15,
            wideLines                               = 0 << 16,
            largePoints                             = 0 << 17,
            alphaToOne                              = 0 << 18,
            multiViewport                           = 0 << 19,
            samplerAnisotropy                       = 0 << 20,
            textureCompressionETC2                  = 0 << 21,
            textureCompressionASTC_LDR              = 0 << 22,
            textureCompressionBC                    = 0 << 23,
            occlusionQueryPrecise                   = 0 << 24,
            pipelineStatisticsQuery                 = 0 << 25,
            vertexPipelineStoresAndAtomics          = 0 << 26,
            fragmentStoresAndAtomics                = 0 << 27,
            shaderTessellationAndGeometryPointSize  = 0 << 28,
            shaderImageGatherExtended               = 0 << 29,
            shaderStorageImageExtendedFormats       = 0 << 30,
            shaderStorageImageMultisample           = 0 << 31,
            shaderStorageImageReadWithoutFormat     = 0 << 32,
            shaderStorageImageWriteWithoutFormat    = 0 << 33,
            shaderUniformBufferArrayDynamicIndexing = 0 << 34,
            shaderSampledImageArrayDynamicIndexing  = 0 << 35,
            shaderStorageBufferArrayDynamicIndexing = 0 << 36,
            shaderStorageImageArrayDynamicIndexing  = 0 << 37,
            shaderClipDistance                      = 0 << 38,
            shaderCullDistance                      = 0 << 39,
            shaderFloat64                           = 0 << 40,
            shaderInt64                             = 0 << 41,
            shaderInt16                             = 0 << 42,
            shaderResourceResidency                 = 0 << 43,
            shaderResourceMinLod                    = 0 << 44,
            sparseBinding                           = 0 << 45,
            sparseResidencyBuffer                   = 0 << 46,
            sparseResidencyImage2D                  = 0 << 47,
            sparseResidencyImage3D                  = 0 << 48,
            sparseResidency2Samples                 = 0 << 49,
            sparseResidency4Samples                 = 0 << 50,
            sparseResidency8Samples                 = 0 << 51,
            sparseResidency16Samples                = 0 << 52,
            sparseResidencyAliased                  = 0 << 53,
            variableMultisampleRate                 = 0 << 54,
            inheritedQueries                        = 0 << 55,

        };  // vkPhysicalDeviceFeatureBits
    
        class vkDevice {
        public:
            vkDevice(VkPhysicalDevice gpu, cstring* requestedExtensions, u32 extensionCount);
            ~vkDevice();

        private:
            VkPhysicalDevice m_gpu;
            VkPhysicalDeviceProperties m_gpuProperties;
            VkPhysicalDeviceFeatures m_gpuFeatures;
            VkPhysicalDeviceMemoryProperties m_gpuMemory;

            VkDevice m_Device;

        };  // vkDevice

    }   // Splash

}   // Ocean
