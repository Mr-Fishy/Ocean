#include "vk_Pipeline.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"
#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"
#include "Ocean/Types/FloatingPoints.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkRenderPass::vkRenderPass() {
        
            
        
        }

        vkRenderPass::~vkRenderPass() {
        
            
        
        }

        vkPipeline::vkPipeline() :
            m_Pipeline(VK_NULL_HANDLE),
            m_Layout(VK_NULL_HANDLE),
            m_RenderPasses(0)
        {
            
        }

        vkPipeline::~vkPipeline() {
            
        }

        void vkPipeline::Invalidate() {
            // ============================== VIEWPORT ==============================
            //
            VkViewport viewport {
                0.0f,
                0.0f,
                static_cast<f32>(vkInstance::Get().Swapchain()->Extent().width),
                static_cast<f32>(vkInstance::Get().Swapchain()->Extent().height),
                0.0f,
                1.0f
            };

            // ============================== SCISSOR ==============================
            //
            VkRect2D scissor {
                {
                    0,
                    0
                },
                vkInstance::Get().Swapchain()->Extent()
            };

            // ============================== DYNAMIC STATES ==============================
            //
            FixedArray<VkDynamicState, 2> dynamicStates {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
            };
            VkPipelineDynamicStateCreateInfo dynamicState {
                VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                nullptr,
                0,
                dynamicStates.size(),
                dynamicStates.data()
            };

            VkPipelineViewportStateCreateInfo viewportState {
                VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                nullptr,
                0,
                1,
                &viewport,
                1,
                &scissor
            };

            // ============================== VERTEX INPUT ==============================
            //
            VkPipelineVertexInputStateCreateInfo vertexInput {
                VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO,
                nullptr,
                0,
                0,
                nullptr,
                0,
                nullptr
            };

            VkPipelineInputAssemblyStateCreateInfo inputAssemblyState {
                VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                nullptr,
                0,
                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                VK_FALSE
            };

            // ============================== RASTERIZATION ==============================
            //
            VkPipelineRasterizationStateCreateInfo rasterizationState {
                VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                nullptr,
                0,
                VK_FALSE,
                VK_FALSE,
                VK_POLYGON_MODE_FILL,
                VK_CULL_MODE_BACK_BIT,
                VK_FRONT_FACE_CLOCKWISE,
                VK_FALSE,
                0.0f,
                0.0f,
                0.0f,
                1.0f
            };

            // ============================== MULTISAMPLING ==============================
            //
            VkPipelineMultisampleStateCreateInfo multisampling {
                VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                nullptr,
                0,
                VK_SAMPLE_COUNT_1_BIT,
                VK_FALSE,
                1.0f,
                nullptr,
                VK_FALSE,
                VK_FALSE
            };

            // ============================== DEPTH TESTING ==============================
            //

            // ============================== STENCIL TESTING ==============================
            //

            // ============================== COLOR BLENDING ==============================
            //
            VkPipelineColorBlendAttachmentState colorBlendAttachment {
                VK_TRUE,
                VK_BLEND_FACTOR_SRC_ALPHA,
                VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                VK_BLEND_OP_ADD,
                VK_BLEND_FACTOR_ONE,
                VK_BLEND_FACTOR_ZERO,
                VK_BLEND_OP_ADD,
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
            };

            VkPipelineColorBlendStateCreateInfo colorBlendState {
                VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                nullptr,
                0,
                VK_FALSE,
                VK_LOGIC_OP_COPY,
                1,
                &colorBlendAttachment,
                {
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f
                }
            };

            // ============================== PIPELINE LAYOUT ==============================
            //
            VkPipelineLayoutCreateInfo pipelineLayoutInfo {
                VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                nullptr,
                0,
                0,
                nullptr,
                0,
                nullptr
            };

            vkCheck(
                vkCreatePipelineLayout(vkInstance::Get().Device()->GetLogical(), &pipelineLayoutInfo, nullptr, &this->m_Layout)
            );

            // ============================== SHADER MODULES ==============================
            //
            DynamicArray<VkShaderModule> shaderModules;

            

            // ============================== PIPELINE INFO ==============================
            //
            VkGraphicsPipelineCreateInfo pipelineInfo {
                VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                nullptr,
                0
            };
        }

    } // namespace Splash

} // namespace Ocean
