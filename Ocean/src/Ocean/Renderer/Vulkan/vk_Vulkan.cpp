#include "vk_Vulkan.hpp"

#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Log.hpp"

namespace Ocean {

    namespace Splash {

        void vkCheck(VkResult result) {
            switch (result) {
                case VK_SUCCESS:
                    // oprint(CONSOLE_TEXT_GREEN("VK_SUCCESS\n"));
                    break;

                case VK_NOT_READY:
                    oprint(CONSOLE_TEXT_YELLOW("VK_NOT_READY\n"));
                    break;

                case VK_TIMEOUT:
                    oprint(CONSOLE_TEXT_CYAN("VK_TIMEOUT\n"));
                    break;

                case VK_EVENT_SET:
                    oprint(CONSOLE_TEXT_GREEN("VK_EVENT_SET\n"));
                    break;

                case VK_EVENT_RESET:
                    oprint(CONSOLE_TEXT_GREEN("VK_EVENT_RESET\n"));
                    break;

                case VK_INCOMPLETE:
                    oprint(CONSOLE_TEXT_YELLOW("VK_INCOMPLETE\n"));
                    break;

                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    oprint(CONSOLE_TEXT_RED("VK_ERROR_OUT_OF_HOST_MEMORY\n"));
                    break;

                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    oprint(CONSOLE_TEXT_RED("VK_ERROR_OUT_OF_DEVICE_MEMORY\n"));
                    break;

                case VK_ERROR_INITIALIZATION_FAILED:
                    oprint(CONSOLE_TEXT_RED("VK_ERROR_INITIALIZATION_FAILED\n"));
                    break;

                case VK_ERROR_DEVICE_LOST:
                    oprint(CONSOLE_TEXT_RED("VK_ERROR_DEVICE_LOST\n"));
                    break;

                case VK_ERROR_MEMORY_MAP_FAILED:
                    oprint(CONSOLE_TEXT_RED("VK_ERROR_MEMORY_MAP_FAILED\n"));
                    break;

                case VK_ERROR_LAYER_NOT_PRESENT:
                    oprint(CONSOLE_TEXT_RED("VK_ERROR_LAYER_NOT_PRESENT\n"));
                    break;

                case VK_ERROR_EXTENSION_NOT_PRESENT:
                    oprint(CONSOLE_TEXT_RED("VK_ERROR_EXTENSION_NOT_PRESENT\n"));
                    break;

                case VK_ERROR_FEATURE_NOT_PRESENT:
                    oprint("VK_ERROR_FEATURE_NOT_PRESENT\n");
                    break;

                case VK_ERROR_INCOMPATIBLE_DRIVER:
                    oprint("VK_ERROR_INCOMPATIBLE_DRIVER\n");
                    break;

                case VK_ERROR_TOO_MANY_OBJECTS:
                    oprint("VK_ERROR_TOO_MANY_OBJECTS\n");
                    break;

                case VK_ERROR_FORMAT_NOT_SUPPORTED:
                    oprint("VK_ERROR_FORMAT_NOT_SUPPORTED\n");
                    break;

                case VK_ERROR_FRAGMENTED_POOL:
                    oprint("VK_ERROR_FRAGMENTED_POOL\n");
                    break;

                case VK_ERROR_UNKNOWN:
                    oprint("VK_ERROR_UNKNOWN\n");
                    break;

                case VK_ERROR_OUT_OF_POOL_MEMORY:
                    oprint("VK_ERROR_OUT_OF_POOL_MEMORY\n");
                    break;

                case VK_ERROR_INVALID_EXTERNAL_HANDLE:
                    oprint("VK_ERROR_INVALID_EXTERNAL_HANDLE\n");
                    break;

                case VK_ERROR_FRAGMENTATION:
                    oprint("VK_ERROR_FRAGMENTATION\n");
                    break;

                case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
                    oprint("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS\n");
                    break;

                case VK_PIPELINE_COMPILE_REQUIRED:
                    oprint("VK_PIPELINE_COMPILE_REQUIRED\n");
                    break;

                case VK_ERROR_NOT_PERMITTED:
                    oprint("VK_ERROR_NOT_PERMITTED\n");
                    break;

                case VK_ERROR_SURFACE_LOST_KHR:
                    oprint("VK_ERROR_SURFACE_LOST_KHR\n");
                    break;

                case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                    oprint("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR\n");
                    break;

                case VK_SUBOPTIMAL_KHR:
                    oprint(CONSOLE_TEXT_YELLOW("VK_SUBOPTIMAL_KHR\n"));
                    break;

                case VK_ERROR_OUT_OF_DATE_KHR:
                    oprint("VK_ERROR_OUT_OF_DATE_KHR\n");
                    break;

                case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
                    oprint("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR\n");
                    break;

                case VK_ERROR_VALIDATION_FAILED_EXT:
                    oprint("VK_ERROR_VALIDATION_FAILED_EXT\n");
                    break;

                case VK_ERROR_INVALID_SHADER_NV:
                    oprint("VK_ERROR_INVALID_SHADER_NV\n");
                    break;

                case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
                    oprint("VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR\n");
                    break;

                case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
                    oprint("VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR\n");
                    break;

                case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
                    oprint("VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR\n");
                    break;

                case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
                    oprint("VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR\n");
                    break;

                case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
                    oprint("VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR\n");
                    break;

                case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
                    oprint("VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR\n");
                    break;

                case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
                    oprint("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT\n");
                    break;

                case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
                    oprint("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT\n");
                    break;

                case VK_THREAD_IDLE_KHR:
                    oprint("VK_THREAD_IDLE_KHR\n");
                    break;

                case VK_THREAD_DONE_KHR:
                    oprint("VK_THREAD_DONE_KHR\n");
                    break;

                case VK_OPERATION_DEFERRED_KHR:
                    oprint("VK_OPERATION_DEFERRED_KHR\n");
                    break;

                case VK_OPERATION_NOT_DEFERRED_KHR:
                    oprint("VK_OPERATION_NOT_DEFERRED_KHR\n");
                    break;

                case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
                    oprint("VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR\n");
                    break;

                case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
                    oprint("VK_ERROR_COMPRESSION_EXHAUSTED_EXT\n");
                    break;

                case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
                    oprint("VK_INCOMPATIBLE_SHADER_BINARY_EXT\n");
                    break;

                case VK_PIPELINE_BINARY_MISSING_KHR:
                    oprint("VK_PIPELINE_BINARY_MISSING_KHR\n");
                    break;

                case VK_ERROR_NOT_ENOUGH_SPACE_KHR:
                    oprint("VK_ERROR_NOT_ENOUGH_SPACE_KHR\n");
                    break;

                case VK_RESULT_MAX_ENUM:
                    oprint("VK_RESULT_MAX_ENUM\n");
                    break;

                default:
                    oprint(CONSOLE_TEXT_RED("Unknown VK_RESULT!\n"));
                    break;
            }
        }

        void vkGetPhysicalDeviceFeatureBits(VkPhysicalDevice physicalDevice, u64* pFeatures) {
            VkPhysicalDeviceFeatures raw;
            vkGetPhysicalDeviceFeatures(physicalDevice, &raw);

            *pFeatures |= (raw.robustBufferAccess                      == VK_TRUE ? VkPhysicalDeviceFeatureBits::RobustBufferAccess                      : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.fullDrawIndexUint32                     == VK_TRUE ? VkPhysicalDeviceFeatureBits::FullDrawIndexUint32                     : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.imageCubeArray                          == VK_TRUE ? VkPhysicalDeviceFeatureBits::ImageCubeArray                          : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.independentBlend                        == VK_TRUE ? VkPhysicalDeviceFeatureBits::IndependentBlend                        : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.geometryShader                          == VK_TRUE ? VkPhysicalDeviceFeatureBits::GeometryShader                          : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.tessellationShader                      == VK_TRUE ? VkPhysicalDeviceFeatureBits::TessellationShader                      : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sampleRateShading                       == VK_TRUE ? VkPhysicalDeviceFeatureBits::SampleRateShading                       : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.dualSrcBlend                            == VK_TRUE ? VkPhysicalDeviceFeatureBits::DualSrcBlend                            : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.logicOp                                 == VK_TRUE ? VkPhysicalDeviceFeatureBits::LogicOp                                 : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.multiDrawIndirect                       == VK_TRUE ? VkPhysicalDeviceFeatureBits::MultiDrawIndirect                       : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.drawIndirectFirstInstance               == VK_TRUE ? VkPhysicalDeviceFeatureBits::DrawIndirectFirstInstance               : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.depthClamp                              == VK_TRUE ? VkPhysicalDeviceFeatureBits::DepthClamp                              : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.depthBiasClamp                          == VK_TRUE ? VkPhysicalDeviceFeatureBits::DepthBiasClamp                          : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.fillModeNonSolid                        == VK_TRUE ? VkPhysicalDeviceFeatureBits::FillModeNonSolid                        : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.depthBounds                             == VK_TRUE ? VkPhysicalDeviceFeatureBits::DepthBounds                             : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.wideLines                               == VK_TRUE ? VkPhysicalDeviceFeatureBits::WideLines                               : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.largePoints                             == VK_TRUE ? VkPhysicalDeviceFeatureBits::LargePoints                             : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.alphaToOne                              == VK_TRUE ? VkPhysicalDeviceFeatureBits::AlphaToOne                              : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.multiViewport                           == VK_TRUE ? VkPhysicalDeviceFeatureBits::MultiViewport                           : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.samplerAnisotropy                       == VK_TRUE ? VkPhysicalDeviceFeatureBits::SamplerAnisotropy                       : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.textureCompressionETC2                  == VK_TRUE ? VkPhysicalDeviceFeatureBits::TextureCompressionETC2                  : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.textureCompressionASTC_LDR              == VK_TRUE ? VkPhysicalDeviceFeatureBits::TextureCompressionASTCLDR               : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.textureCompressionBC                    == VK_TRUE ? VkPhysicalDeviceFeatureBits::TextureCompressionBC                    : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.occlusionQueryPrecise                   == VK_TRUE ? VkPhysicalDeviceFeatureBits::OcclusionQueryPrecise                   : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.pipelineStatisticsQuery                 == VK_TRUE ? VkPhysicalDeviceFeatureBits::PipelineStatisticsQuery                 : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.vertexPipelineStoresAndAtomics          == VK_TRUE ? VkPhysicalDeviceFeatureBits::VertexPipelineStoresAndAtomics          : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.fragmentStoresAndAtomics                == VK_TRUE ? VkPhysicalDeviceFeatureBits::FragmentStoresAndAtomics                : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderTessellationAndGeometryPointSize  == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderTessellationAndGeometryPointSize  : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderImageGatherExtended               == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderImageGatherExtended               : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderStorageImageExtendedFormats       == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderStorageImageExtendedFormats       : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderStorageImageMultisample           == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderStorageImageMultisample           : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderStorageImageReadWithoutFormat     == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderStorageImageReadWithoutFormat     : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderStorageImageWriteWithoutFormat    == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderStorageImageWriteWithoutFormat    : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderUniformBufferArrayDynamicIndexing == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderUniformBufferArrayDynamicIndexing : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderSampledImageArrayDynamicIndexing  == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderSampledImageArrayDynamicIndexing  : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderStorageBufferArrayDynamicIndexing == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderStorageBufferArrayDynamicIndexing : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderStorageImageArrayDynamicIndexing  == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderStorageImageArrayDynamicIndexing  : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderClipDistance                      == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderClipDistance                      : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderCullDistance                      == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderCullDistance                      : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderFloat64                           == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderFloat64                           : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderInt64                             == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderInt64                             : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderInt16                             == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderInt16                             : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderResourceResidency                 == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderResourceResidency                 : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.shaderResourceMinLod                    == VK_TRUE ? VkPhysicalDeviceFeatureBits::ShaderResourceMinLod                    : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseBinding                           == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseBinding                           : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidencyBuffer                   == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidencyBuffer                   : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidencyImage2D                  == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidencyImage2D                  : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidencyImage3D                  == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidencyImage3D                  : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidency2Samples                 == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidency2Samples                 : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidency4Samples                 == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidency4Samples                 : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidency8Samples                 == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidency8Samples                 : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidency16Samples                == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidency16Samples                : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.sparseResidencyAliased                  == VK_TRUE ? VkPhysicalDeviceFeatureBits::SparseResidencyAliased                  : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.variableMultisampleRate                 == VK_TRUE ? VkPhysicalDeviceFeatureBits::VariableMultisampleRate                 : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
            *pFeatures |= (raw.inheritedQueries                        == VK_TRUE ? VkPhysicalDeviceFeatureBits::InheritedQueries                        : VkPhysicalDeviceFeatureBits::UnsupportedFeature);
        }

    }   // Splash

}   // Ocean
