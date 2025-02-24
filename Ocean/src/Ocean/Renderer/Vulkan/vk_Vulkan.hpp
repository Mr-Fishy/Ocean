#pragma once

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Macros.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        OC_UNUSED void vkCheck(VkResult result);

        enum VkPhysicalDeviceFeatureBits : u64 {
            UnsupportedFeature                      = 0ULL << 0,

            RobustBufferAccess                      = 1ULL << 0,
            FullDrawIndexUint32                     = 1ULL << 1,
            ImageCubeArray                          = 1ULL << 2,
            IndependentBlend                        = 1ULL << 3,
            GeometryShader                          = 1ULL << 4,
            TessellationShader                      = 1ULL << 5,
            SampleRateShading                       = 1ULL << 6,
            DualSrcBlend                            = 1ULL << 7,
            LogicOp                                 = 1ULL << 8,
            MultiDrawIndirect                       = 1ULL << 9,
            DrawIndirectFirstInstance               = 1ULL << 10,
            DepthClamp                              = 1ULL << 11,
            DepthBiasClamp                          = 1ULL << 12,
            FillModeNonSolid                        = 1ULL << 13,
            DepthBounds                             = 1ULL << 14,
            WideLines                               = 1ULL << 15,
            LargePoints                             = 1ULL << 16,
            AlphaToOne                              = 1ULL << 17,
            MultiViewport                           = 1ULL << 18,
            SamplerAnisotropy                       = 1ULL << 19,
            TextureCompressionETC2                  = 1ULL << 20,
            TextureCompressionASTCLDR               = 1ULL << 21,
            TextureCompressionBC                    = 1ULL << 22,
            OcclusionQueryPrecise                   = 1ULL << 23,
            PipelineStatisticsQuery                 = 1ULL << 24,
            VertexPipelineStoresAndAtomics          = 1ULL << 25,
            FragmentStoresAndAtomics                = 1ULL << 26,
            ShaderTessellationAndGeometryPointSize  = 1ULL << 27,
            ShaderImageGatherExtended               = 1ULL << 28,
            ShaderStorageImageExtendedFormats       = 1ULL << 29,
            ShaderStorageImageMultisample           = 1ULL << 30,
            ShaderStorageImageReadWithoutFormat     = 1ULL << 31,
            ShaderStorageImageWriteWithoutFormat    = 1ULL << 32,
            ShaderUniformBufferArrayDynamicIndexing = 1ULL << 33,
            ShaderSampledImageArrayDynamicIndexing  = 1ULL << 34,
            ShaderStorageBufferArrayDynamicIndexing = 1ULL << 35,
            ShaderStorageImageArrayDynamicIndexing  = 1ULL << 36,
            ShaderClipDistance                      = 1ULL << 37,
            ShaderCullDistance                      = 1ULL << 38,
            ShaderFloat64                           = 1ULL << 39,
            ShaderInt64                             = 1ULL << 40,
            ShaderInt16                             = 1ULL << 41,
            ShaderResourceResidency                 = 1ULL << 42,
            ShaderResourceMinLod                    = 1ULL << 43,
            SparseBinding                           = 1ULL << 44,
            SparseResidencyBuffer                   = 1ULL << 45,
            SparseResidencyImage2D                  = 1ULL << 46,
            SparseResidencyImage3D                  = 1ULL << 47,
            SparseResidency2Samples                 = 1ULL << 48,
            SparseResidency4Samples                 = 1ULL << 49,
            SparseResidency8Samples                 = 1ULL << 50,
            SparseResidency16Samples                = 1ULL << 51,
            SparseResidencyAliased                  = 1ULL << 52,
            VariableMultisampleRate                 = 1ULL << 53,
            InheritedQueries                        = 1ULL << 54,

        };  // vkPhysicalDeviceFeatureBits

        OC_UNUSED void vkGetPhysicalDeviceFeatureBits(VkPhysicalDevice physicalDevice, u64* pFeatures);

        // struct VkPhsycalDevicePropertiesCompressed {
        //     u32 apiVersion;
        //     u32 driverVersion;
        //     u32 vendorID;
        //     u32 deviceID;

        //     VkPhysicalDeviceType deviceType;

        //     u8 deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
        //     u8 pipelineCacheUUID[VK_UUID_SIZE];

        //     struct VkPhysicalDeviceLimitsCompressed {
        //         u16 maxImageDimension1D;
        //         u16 maxImageDimension2D;
        //         // maxImageDimension3D
        //         u16 maxImageDimensionCube;
        //         u16 maxImageArrayLayers;
        //         // maxTexelBufferElements
        //         u32 maxUniformBufferRange;
        //         // maxStorageBufferRange
        //         // maxPushConstantsSize
        //         // maxMemoryAllocationCount
        //         // maxSamplerAllocationCount
        //         u16 bufferImageGranularity;
        //         // sparseAddressSpaceSize
        //         // maxBoundDescriptorSets
        //         u8 maxPerStageDescriptorSamplers;
        //         u8 maxPerStageDescriptorUniformBuffers;
        //         u8 maxPerStageDescriptorStorageBuffers;
        //         u8 maxPerStageDescriptorSampledImages;
        //         u8 maxPerStageDescriptorStorageImages;
        //         // maxPerStageDescriptorInputAttachments
        //         u8 maxPerStageResources;
        //         u16 maxDescriptorSetSamplers;
        //         u8 maxDescriptorSetUniformBuffers;
        //         // maxDescriptorSetUniformBuffersDynamic
        //         u8 maxDescriptorSetStorageBuffers;
        //         // maxDescriptorSetStorageBuffersDynamic
        //         u16 maxDescriptorSetSampledImages;
        //         u8 maxDescriptorSetStorageImages;
        //         // maxDescriptorSetInputAttachments
        //         // maxVertexInputAttributes
        //         // maxVertexInputBindings
        //         // maxVertexInputAttributeOffset
        //         // maxVertexInputBindingStride
        //         // maxVertexOutputComponents
        //         // maxTessellationGenerationLevel
        //         // maxTessellationPatchSize
        //         // maxTessellationControlPerVertexInputComponents
        //         // maxTessellationControlPerVertexOutputComponents
        //         // maxTessellationControlPerPatchOutputComponents
        //         // maxTessellationControlTotalOutputComponents
        //         // maxTessellationEvaluationInputComponents
        //         // maxTessellationEvaluationOutputComponents
        //         // maxGeometryShaderInvocations
        //         // maxGeometryInputComponents
        //         // maxGeometryOutputComponents
        //         // maxGeometryOutputVertices
        //         // maxGeometryTotalOutputComponents
        //         // maxFragmentInputComponents
        //         // maxFragmentOutputAttachments
        //         // maxFragmentDualSrcAttachments
        //         u8 maxFragmentCombinedOutputResources;
        //         // maxComputeSharedMemorySize
        //         // maxComputeWorkGroupCount
        //         u16 maxComputeWorkGroupInvocations;
        //         u16 maxComputeWorkGroupSize[3];
        //         // subPixelPrecisionBits
        //         u8 subTexelPrecisionBits;
        //         u8 mipmapPrecisionBits;
        //         // maxDrawIndexedIndexValue
        //         // maxDrawIndirectCount
        //         u8 maxSamplerLodBias;
        //         // maxSamplerAnisotropy
        //         // maxViewports
        //         // maxViewportDimensions
        //         // viewportBoundsRange
        //         // viewportSubPixelBits
        //         // minMemoryMapAlignment
        //         // minTexelBufferOffsetAlignment
        //         // minUniformBufferOffsetAlignment
        //         // minStorageBufferOffsetAlignment
        //         // minTexelOffset
        //         // maxTexelOffset
        //         // minTexelGatherOffset
        //         // maxTexelGatherOffset
        //         // minInterpolationOffset
        //         // maxInterpolationOffset
        //         // subPixelInterpolationOffsetBits
        //         // maxFramebufferWidth
        //         // maxFramebufferHeight
        //         // maxFramebufferLayers
        //         // framebufferColorSampleCounts
        //         // framebufferDepthSampleCounts
        //         // framebufferStencilSampleCounts
        //         // framebufferNoAttachmentsSampleCounts
        //         // maxColorAttachments
        //         // sampledImageColorSampleCounts
        //         // sampledImageIntegerSampleCounts
        //         // sampledImageDepthSampleCounts
        //         // sampledImageStencilSampleCounts
        //         // storageImageSampleCounts
        //         // maxSampleMaskWords
        //         // timestampComputeAndGraphics
        //         // timestampPeriod
        //         // maxClipDistances
        //         // maxCullDistances
        //         // maxCombinedClipAndCullDistances
        //         // discreteQueuePriorities
        //         // pointSizeRange
        //         // lineWidthRange
        //         f32 pointSizeGranularity;
        //         // lineWidthGranularity
        //         // strictLines
        //         // standardSampleLocations
        //         // optimalBufferCopyOffsetAlignment
        //         // optimalBufferCopyRowPitchAlignment
        //         // nonCoherentAtomSize

        //     };  // VkPhysicalDeviceLimitsCompressed

        //     VkPhysicalDeviceLimitsCompressed limits;

        //     struct VkPhysicalDeviceSparsePropertiesCompressed {


        //     };  // VkPhysicalDeviceSparsePropertiesCompressed

        //     VkPhysicalDeviceSparsePropertiesCompressed sparseProperties;

        // };  // VkPhsycalDevicePropertiesCompressed

        // struct VkPhysicalDeviceMemoryProperties {

        // };  // VkPhysicalDeviceMemoryProperties

    }   // Splash

}   // Ocean
