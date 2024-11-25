#pragma once

#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/Strings.hpp"

// libs
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace Ocean {

    namespace Vulkan {

        namespace Devices {

            struct QueueCreation : public VkDeviceQueueCreateInfo {
                QueueCreation() {
                    this->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                }
                /**
                * @brief Construct a new Device Queue Creation object.
                * 
                * @param familyIndex 
                * @param count 
                * @param queuePriorities 
                * @param createFlags 
                * @param next 
                */
                QueueCreation(
                    u32 familyIndex, u32 count, float* queuePriorities,
                    u32 createFlags = 0, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    this->pNext = next;
                    this->flags = createFlags;
                    this->queueFamilyIndex = familyIndex;
                    this->queueCount = count;
                    this->pQueuePriorities = queuePriorities;
                }

            };  // QueueCreation

            struct CreateInfo : public VkDeviceCreateInfo {
                CreateInfo() {
                    this->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                }
                /**
                * @brief Construct a new Device Creation object.
                * 
                * @param queueCount 
                * @param queues 
                * @param enabledFeatures 
                * @param extensionCount 
                * @param enabledExtensions 
                * @param createFlags 
                * @param next 
                */
                CreateInfo(
                    u32 queueCount, VkDeviceQueueCreateInfo* queues,
                    VkPhysicalDeviceFeatures* enabledFeatures,
                    u32 extensionCount, cstring const* enabledExtensions,
                    u32 createFlags = 0, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                    this->pNext = next;
                    this->flags = createFlags;
                    this->queueCreateInfoCount = queueCount;
                    this->pQueueCreateInfos = queues;
                    this->enabledLayerCount = 0;
                    this->ppEnabledLayerNames = nullptr;
                    this->enabledExtensionCount = extensionCount;
                    this->ppEnabledExtensionNames = enabledExtensions;
                    this->pEnabledFeatures = enabledFeatures;
                }
                /**
                * @brief Construct a new Device Creation object.
                * 
                * @param queueCount 
                * @param queues 
                * @param enabledFeatures 
                * @param extensionCount 
                * @param enabledExtensions 
                * @param layerCount 
                * @param enabledLayers 
                * @param createFlags 
                * @param next 
                */
                CreateInfo(u32 queueCount, VkDeviceQueueCreateInfo* queues,
                    VkPhysicalDeviceFeatures* enabledFeatures,
                    u32 extensionCount, cstring const* enabledExtensions,
                    u32 layerCount, cstring const* enabledLayers,
                    u32 createFlags = 0, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                    this->pNext = next;
                    this->flags = createFlags;
                    this->queueCreateInfoCount = queueCount;
                    this->pQueueCreateInfos = queues;
                    this->enabledLayerCount = layerCount;
                    this->ppEnabledLayerNames = enabledLayers;
                    this->enabledExtensionCount = extensionCount;
                    this->ppEnabledExtensionNames = enabledExtensions;
                    this->pEnabledFeatures = enabledFeatures;
                }

            };  // CreateInfo

        }   // Device

        namespace ImageViews {

            struct CreateInfo : public VkImageViewCreateInfo {
                CreateInfo() {
                    this->sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                }
                /**
                 * @brief Create a Info object.
                 * 
                 * @param format 
                 * @param image 
                 * @param flags 
                 * @param next 
                 */
                CreateInfo(
                    VkFormat format, VkImage image,
                    u32 flags = 0, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                    this->pNext = next;
                    this->flags = flags;

                    this->image = image;
                    this->viewType = VK_IMAGE_VIEW_TYPE_2D;
                    this->format = format;
                    this->components = {
                        VK_COMPONENT_SWIZZLE_R,
                        VK_COMPONENT_SWIZZLE_G,
                        VK_COMPONENT_SWIZZLE_B,
                        VK_COMPONENT_SWIZZLE_A,
                    };
                    this->subresourceRange = {
                        VK_IMAGE_ASPECT_COLOR_BIT,
                        0,
                        1,
                        0,
                        1,
                    };
                }

            };  // CreateInfo

        }   // Swapchains

        // Command Pool

        struct CommandPoolCreateInfo : public VkCommandPoolCreateInfo {
            CommandPoolCreateInfo() {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            }
            /**
             * @brief Construct a new Command Pool Creation object.
             * 
             * @param createFlags 
             * @param familyIndex 
             * @param next 
             */
            CommandPoolCreateInfo(
                u32 createFlags, u32 familyIndex,
                void* next = nullptr
            ) {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                this->pNext = next;
                this->flags = createFlags;
                this->queueFamilyIndex = familyIndex;
            }

        };  // CommandPoolCreateInfo

        // Command Buffers

        struct PrimaryCommandBufferAllocation : public VkCommandBufferAllocateInfo {
            PrimaryCommandBufferAllocation() {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                this->level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            }
            /**
             * @brief Construct a new Primary Command Buffer Allocation object.
             * 
             * @param pool 
             * @param bufferCount 
             * @param next 
             */
            PrimaryCommandBufferAllocation(
                VkCommandPool pool, u32 bufferCount,
                void* next = nullptr
            ) {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                this->pNext = next;
                this->commandPool = pool;
                this->level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                this->commandBufferCount = bufferCount;
            }

        };  // PrimaryCommandBufferAllocation

        struct SecondaryCommandBufferAllocation : public VkCommandBufferAllocateInfo {
            SecondaryCommandBufferAllocation() {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                this->level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
            }
            /**
             * @brief Construct a new Secondary Command Buffer Allocation object.
             * 
             * @param pool 
             * @param bufferCount 
             * @param next 
             */
            SecondaryCommandBufferAllocation(
                VkCommandPool pool, u32 bufferCount,
                void* next = nullptr
            ) {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                this->pNext = next;
                this->commandPool = pool;
                this->level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
                this->commandBufferCount = bufferCount;
            }

        };  // SecondaryCommandBufferAllocation

        struct OneTimeCommandBufferBegin : public VkCommandBufferBeginInfo {
            OneTimeCommandBufferBegin() {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                this->flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            }
            /**
             * @brief Construct a new Command Buffer Begin object.
             * 
             * @param inheritanceInfo 
             * @param next 
             */
            OneTimeCommandBufferBegin(
            const VkCommandBufferInheritanceInfo* inheritanceInfo, void* next = nullptr
            ) {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                this->pNext = next;
                this->flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                this->pInheritanceInfo = inheritanceInfo;
            }

        };  // OneTimeCommandBufferBegin

        struct RenderPassCommandBufferBegin : public VkCommandBufferBeginInfo {
            RenderPassCommandBufferBegin() {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                this->flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
            }
            /**
             * @brief Construct a new Command Buffer Begin object.
             * 
             * @param inheritanceInfo 
             * @param next 
             */
            RenderPassCommandBufferBegin(
            const VkCommandBufferInheritanceInfo* inheritanceInfo, void* next = nullptr
            ) {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                this->pNext = next;
                this->flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
                this->pInheritanceInfo = inheritanceInfo;
            }

        };  // RenderPassCommandBufferBegin

        struct SimultaneousCommandBufferBegin : public VkCommandBufferBeginInfo {
            SimultaneousCommandBufferBegin() {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                this->flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            }
            /**
             * @brief Construct a new Command Buffer Begin object.
             * 
             * @param inheritanceInfo 
             * @param next 
             */
            SimultaneousCommandBufferBegin(
            const VkCommandBufferInheritanceInfo* inheritanceInfo, void* next = nullptr
            ) {
                this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                this->pNext = next;
                this->flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
                this->pInheritanceInfo = inheritanceInfo;
            }

        };  // SimultaneousCommandBufferBegin

        // Memory

        struct MemoryAllocation : public VkMemoryAllocateInfo {
            MemoryAllocation() {
                this->sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            }
            /**
             * @brief Construct a new Memory Allocation object.
             * 
             * @param size Specifies the size to allocate in bytes.
             * @param memoryType Specifies the type of GPU memory to use.
             * @param next TODO
             */
            MemoryAllocation(
                u64 size, u32 memoryType, 
                void* next = nullptr
            ) {
                this->sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                this->pNext = next;
                this->allocationSize = size;
                this->memoryTypeIndex = memoryType;
            }

        };  // MemoryAllocation

        namespace Buffers {

            struct ExclusiveCreateInfo : public VkBufferCreateInfo {
                ExclusiveCreateInfo() {
                    this->sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                    this->sharingMode = VK_SHARING_MODE_EXCLUSIVE;
                }
                /**
                * @brief Construct a new Exclusive Buffer Creation object.
                * 
                * @param bufferSize Specifies the size of the buffer in bytes.
                * @param bufferUsage Specifies the usage of the buffer.
                * @param createFlags TODO
                * @param familyIndexCount TODO
                * @param familyIndices TODO
                * @param next TODO
                */
                ExclusiveCreateInfo(
                    u64 bufferSize, u32 bufferUsage,
                    u32 createFlags = 0, u32 familyIndexCount = 0, u32* familyIndices = nullptr, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                    this->pNext = next;
                    this->flags = createFlags;
                    this->size = bufferSize;
                    this->usage = bufferUsage;
                    this->sharingMode = VK_SHARING_MODE_EXCLUSIVE;
                    this->queueFamilyIndexCount = familyIndexCount;
                    this->pQueueFamilyIndices = familyIndices;
                }

            };  // ExclusiveCreateInfo

            struct ConcurrentCreateInfo : public VkBufferCreateInfo {
                ConcurrentCreateInfo() {
                    this->sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                    this->sharingMode = VK_SHARING_MODE_CONCURRENT;
                }
                /**
                * @brief Construct a new Exclusive Buffer Creation object.
                * 
                * @param bufferSize Specifies the size of the buffer in bytes.
                * @param bufferUsage Specifies the usage of the buffer.
                * @param createFlags TODO
                * @param familyIndexCount TODO
                * @param familyIndices TODO
                * @param next TODO
                */
                ConcurrentCreateInfo(
                    u64 bufferSize, u32 bufferUsage,
                    u32 createFlags = 0, u32 familyIndexCount = 0, u32* familyIndices = nullptr, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                    this->pNext = next;
                    this->flags = createFlags;
                    this->size = bufferSize;
                    this->usage = bufferUsage;
                    this->sharingMode = VK_SHARING_MODE_CONCURRENT;
                    this->queueFamilyIndexCount = familyIndexCount;
                    this->pQueueFamilyIndices = familyIndices;
                }

            };  // ConcurrentCreation

        }   // Buffer

        namespace Framebuffers {

            struct CreateInfo : public VkFramebufferCreateInfo {
                CreateInfo() {
                    this->sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                }
                CreateInfo(
                    VkRenderPass renderPass, u32 layerCount,
                    u32 attachmentCount, const VkImageView* attachments,
                    u32 width, u32 height,
                    u32 createFlags = 0, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    this->pNext = next;
                    this->flags = createFlags;
                    this->renderPass = renderPass;
                    this->attachmentCount = attachmentCount;
                    this->pAttachments = attachments;
                    this->width = width;
                    this->height = height;
                    this->layers = layerCount;
                }

            };  // CreateInfo

        }   // Framebuffer

        namespace Shaders {
        
            struct ModuleCreateInfo : public VkShaderModuleCreateInfo {
                ModuleCreateInfo() {
                    this->sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                }
                /**
                * @brief Construct a new Shader Module Creation object.
                * 
                * @param size Specifies the size of the shader code.
                * @param code Specifies the pointer to the code in memory.
                * @param createFlags TODO
                * @param next TODO
                */
                ModuleCreateInfo(
                    sizet size, char* code,
                    u32 createFlags = 0, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    this->pNext = next;
                    this->flags = createFlags;
                    this->codeSize = size;
                    this->pCode = reinterpret_cast<const u32*>(code);
                }

            };  // ModuleCreateInfo

        }   // Shader

        namespace RenderPass {

            struct CreateInfo : public VkRenderPassCreateInfo {
                CreateInfo() {
                    this->sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                    this->pNext = nullptr;
                    this->flags = 0;
                }

            };  // CreateInfo

            struct ColorAttachmentReference : public VkAttachmentReference {
                ColorAttachmentReference() {
                    this->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                }
                ColorAttachmentReference(u32 attachment) {
                    this->attachment = attachment;
                    this->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                }

            };  // ColorAttachmentReference

            struct ColorAttachmentDescription : public VkAttachmentDescription {
                ColorAttachmentDescription() {
                    this->samples = VK_SAMPLE_COUNT_1_BIT;

                    this->loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                    this->storeOp = VK_ATTACHMENT_STORE_OP_STORE;

                    this->stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                    this->stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

                    this->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                    this->finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                }
                /**
                * @brief Construct a new Render Pass Color Attachment object.
                * 
                * @param format 
                * @param flags 
                */
                ColorAttachmentDescription(
                    VkFormat format,
                    u32 flags = 0
                ) {
                    this->flags = flags;
                    this->format = format;
                    this->samples = VK_SAMPLE_COUNT_1_BIT;

                    this->loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                    this->storeOp = VK_ATTACHMENT_STORE_OP_STORE;

                    this->stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                    this->stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

                    this->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                    this->finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                }

            };  // RenderPassColorAttachment

            struct GraphicsSubpassDependency : public VkSubpassDependency {
                GraphicsSubpassDependency() {
                    this->srcSubpass = VK_SUBPASS_EXTERNAL;
                    this->dstSubpass = 0;

                    this->srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                    this->dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

                    this->srcAccessMask = 0;
                    this->dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                }

            };  // GraphicsSubpassDependency

            struct GraphicsSubpassDescription : public VkSubpassDescription {
                GraphicsSubpassDescription() {
                    this->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                }
                GraphicsSubpassDescription(
                    u32 colorCount, VkAttachmentReference* colorAttachments,
                    u32 flags = 0
                ) {
                    this->flags = flags;
                    this->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

                    this->inputAttachmentCount = 0;
                    this->pInputAttachments = nullptr;

                    this->colorAttachmentCount = colorCount;
                    this->pColorAttachments = colorAttachments;

                    this->pResolveAttachments = nullptr;
                    this->pDepthStencilAttachment = nullptr;

                    this->preserveAttachmentCount = 0;
                    this->pPreserveAttachments = nullptr;
                }

            };  // SubpassDescription

        }   // RenderPass

        namespace Pipeline {
        
            struct VertexStageInfo : public VkPipelineShaderStageCreateInfo {
                VertexStageInfo() {
                    this->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    this->stage = VK_SHADER_STAGE_VERTEX_BIT;
                }
                /**
                 * @brief Construct a new Vertex Stage Info object.
                 * 
                 * @param module 
                 * @param name 
                 * @param flags 
                 * @param info 
                 * @param next 
                 */
                VertexStageInfo(
                    VkShaderModule module, cstring name,
                    u32 flags = 0, VkSpecializationInfo* info = nullptr, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    this->pNext = next;
                    this->flags = flags;
                    this->stage = VK_SHADER_STAGE_VERTEX_BIT;
                    this->module = module;
                    this->pName = name;
                    this->pSpecializationInfo = info;
                }

            };  // VertexStageInfo

            struct FragmentStageInfo : public VkPipelineShaderStageCreateInfo {
                FragmentStageInfo() {
                    this->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                }
                /**
                 * @brief Construct a new Vertex Stage Info object.
                 * 
                 * @param module 
                 * @param name 
                 * @param flags 
                 * @param info 
                 * @param next 
                 */
                FragmentStageInfo(
                    VkShaderModule module, cstring name,
                    u32 flags = 0, VkSpecializationInfo* info = nullptr, void* next = nullptr
                ) {
                    this->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    this->pNext = next;
                    this->flags = flags;
                    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                    this->module = module;
                    this->pName = name;
                    this->pSpecializationInfo = info;
                }

            };  // FragmentStageInfo



        }   // Pipeline

    }   // Vulkan

}   // Ocean