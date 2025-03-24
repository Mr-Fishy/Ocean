#pragma once

/**
 * @file WindowContext.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/Macros.hpp"

namespace Ocean {

    /**
     * @brief The window handler context.
     */
    class WindowContext {
    public:
        /**
         * @brief Ocean's window platform ID.
         */
        typedef enum PlatformID {
            NONE = 0,
            /** @brief Win32 based windows. */
            WINDOWS,
            /** @brief X11 Linux window server. */
            LINUX_X11,
            /** @brief Wayland Linux window server. */
            LINUX_WAYLAND,
            /** @brief MacOS window server. */
            MACOS_COCOA,

        } PlatformID;

    public:
        /**
         * @brief Initializes the window handler. 
         */
        OC_STATIC void Init();
        /**
         * @brief Shut's down the window handler.
         */
        OC_STATIC void Shutdown();

        /**
         * @brief Get's a PlatformID from the window handler.
         * 
         * @return PlatformID
         */
        OC_STATIC PlatformID GetPlatformID();

    };  // WindowContext

}   // Ocean
