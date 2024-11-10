# Ocean Engine

TODO

## Usage

TODO

## [Installation Setup](./INSTALL.md)

## Building Ocean

The following are notes to ensure proper runtime (not official instructions yet).

TODO: Make platform detection for selections so that only WSL is funky (hopefully).

As of right now, for all platforms [Steam Audio C API](https://valvesoftware.github.io/steam-audio/downloads.html) needs to be added to the [vendors](./Ocean/vendor) folder.

### Windows

All GLFW options can be disabled, as Windows support is default.

### WSL (Windows Subsystem Linux)

Ensure the only X11 Support is enabled for GLFW.

### Linux

It compiles on WSL so it should work assuming Vulkan drivers aren't broken there too.

### MacOS

Good luck.
