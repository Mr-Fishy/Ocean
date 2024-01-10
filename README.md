## Ocean
The goal of Ocean is to build a software & game application development suite. Built with multiple available rendering pipelines.

## Build Instructions
Building the OceanEngine project is very easy, currently only Premake is present for building. Simply enter the __scripts__ folder and run the .bat file for Windows. This will automatically clone any submodules and generate all of the project files for Visual Studio 2022.

Curently only Windows and Visual Studio 2022 are supported for building. 

### Ocean Core
The foundation of the engine is named Ocean Core. This will handl all of the needed processes to run an efficient application.

### Coral Editor
Coral Editor will be the GUI editor available with Ocean.

### Engine Plan
| Stage |     | Description                                                               | Status      |
| ----- | --- | ------------------------------------------------------------------------- | ----------- |
|       |     |                                                                           |             |
| 1     |     | Complete base engine to have basic functionality                          | In Progress |
|       | 1.1 | Create an entry point for the application                                 | Complete    |
|       | 1.2 | Implement a logging system for data and errors                            | Complete    |
|       | 1.3 | Build a application layer to handle the lifetime                          | Complete    |
|       | 1.4 | Build a window layer to handle the application per platform               | Complete    |
|       | 1.5 | Build an event manager to send events from the window to the application  | Complete    |
|       | 1.6 | Integrate ImGui for GUI, may be replaced in future                        | Complete    |
|       | 1.7 | Build a rendering pipeline with OpenGL → Will have Vulkan support later   | Complete    |
|       | 1.8 | Build a proper Entity Component System (ECS)                              | In Progress |
|       | 1.9 | Implement a memory allocator to better compress and handle runtime memory.| In Progress |
|       | 1.10| Build serialization system for scenes and files                           | Not Started |
|       |     |                                                                           |             |
| 2     |     | Create UI support for application development (replace ImGui with own UI) | Not Started |
|       | 2.1 |                                                                           |             |
|       |     |                                                                           |             |
| 3     |     | Implement multi-renderer support (Vulkan and maybe DirectX)               | Not Started |
|       | 3.1 |                                                                           |             |
|       |     |                                                                           |             |
| 4     |     | Introduce advanced functionality and default systems                      | Not Started |
|       | 4.1 |                                                                           |             |
|       |     |                                                                           |             |
| X     |     | Release v1 when in stable and fully functional state.                     | In Progress |

## Current Engine Resources
LICENSES AND ORIGINAL REPOSITORIES STORED / SAVED  IN SUBMODULES

### CTTI
https://github.com/Manu343726/ctti

### ENTT
https://github.com/skypjack/entt

### GLFW
https://github.com/glfw/glfw

### GLAD
https://github.com/Dav1dde/glad

### GLM
https://github.com/g-truc/glm

### IMGUI
https://github.com/ocornut/imgui

### MEMORY-ALLOCATORS
https://github.com/mtrebi/memory-allocators

