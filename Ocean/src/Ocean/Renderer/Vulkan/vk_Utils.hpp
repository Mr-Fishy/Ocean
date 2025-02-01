#pragma once

#include "Ocean/Primitives/Log.hpp"
#include "Ocean/Primitives/Macros.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        OC_STATIC_INLINE void vkCheck(VkResult result) {
            switch (result) {
                case VK_SUCCESS:
                    oprint("VK_SUCCESS\n");
                    break;

                case VK_NOT_READY:
                    oprint("VK_NOT_READY\n");
                    break;

                case VK_TIMEOUT:
                    oprint("VK_TIMEOUT\n");
                    break;

                case VK_EVENT_SET:
                    oprint("VK_EVENT_SET\n");
                    break;

                case VK_EVENT_RESET:
                    oprint("VK_EVENT_RESET\n");
                    break;

                case VK_INCOMPLETE:
                    oprint("VK_INCOMPLETE\n");
                    break;

                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    oprint("VK_ERROR_OUT_OF_HOST_MEMORY\n");
                    break;

                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    oprint("VK_ERROR_OUT_OF_DEVICE_MEMORY\n");
                    break;

                case VK_ERROR_INITIALIZATION_FAILED:
                    oprint("VK_ERROR_INITIALIZATION_FAILED\n");
                    break;

                case VK_ERROR_DEVICE_LOST:
                    oprint("VK_ERROR_DEVICE_LOST\n");
                    break;

                case VK_ERROR_MEMORY_MAP_FAILED:
                    oprint("VK_ERROR_MEMORY_MAP_FAILED\n");
                    break;

                case VK_ERROR_LAYER_NOT_PRESENT:
                    oprint("VK_ERROR_LAYER_NOT_PRESENT\n");
                    break;

                case VK_ERROR_EXTENSION_NOT_PRESENT:
                    oprint("VK_ERROR_EXTENSION_NOT_PRESENT\n");
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
                    oprint("VK_SUBOPTIMAL_KHR\n");
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

            }
        }

    }   // Splash

}   // Ocean
