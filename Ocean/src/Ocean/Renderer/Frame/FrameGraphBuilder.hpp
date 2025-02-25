#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Renderer/Frame/FrameGraph.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        typedef enum FrameGraphResourceType {
            Attachment,
            Texture,
            Buffer,
            Reference,

        } FrameGraphResourceType;

        struct FrameGraphResourceInfo {
            b8 external = false;

            union {
                struct {
                    sizet size;
                    VkBufferUsageFlags flags;

                } buffer;

                struct {
                    u32 width;
                    u32 height;
                    u32 depth;

                    

                } texture;

            } info;

        };  // FrameGraphResourceInfo

        struct FrameGraphResourceInputCreation {
            /** @brief The type of resource that the input is. */
            FrameGraphResourceType type;
            /** @brief  */

        };  // FrameGraphResourceInputCreation

        class FrameGraphBuilder {
            public:
                static void Build(cstring source);

                // static  CreateNodeOutput();

            };  // FrameGraphBuilder

    }   // Splash

}   // Ocean
