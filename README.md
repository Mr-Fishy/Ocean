# OceanEngine
 
The goal of Ocean Engine is to build a software & game application development suite. Built with multiple available rendering pipelines, the hope is to enable applications to have a static / user reactive frame buffer, that also allows a constant framerate area of the canvas asynchronously for cases when a non-static pane is used within a UI canvas (such as a simulation application).

| Sub-System             | Description                                                                                            | Importance |
| ---------------------- | ------------------------------------------------------------------------------------------------------ | ---------- |
| Entry Point            | The entry point is the application runtime entry point that determines what happens at startup / exit. | Required   |
| Application Layer      | Application lifecycle operations (timing, frames, audio, etc.)                                         | Required   |
| Window Layer           | The window will handle platform application handling (input → events)                                  | Required   |
| Event Manager          | Handles input events from user or platform through the [[Window Layer]].                               | Required   | 
| Render Pipeline        | Draws graphics to screen canvas.                                                                       | Required   |
| Render API Abstraction | Enables easier render API options (i.e. Vulcan, OpenGL, DirectX, etc.)                                 | Required   |
| Debugging Support      | System-wide debugging & logging for development / measurement utilities.                               | Required   |
| Scripting API          | Create a scripting API with higher level language (likely C#).                                         | High       |
| Memory Systems         | Memory management / allocation and tracking.                                                           | Required   |
| Entity System          | Object management system (ECS) for game objects or application objects to store attributes.            | Required   |
| Physics Engine         | Physics engine for simulation / game object simulation.                                                | High       |
| File IO System         | Read / write file management & data management system.                                                 | Required   |
| Build System           | Data compiling & formatting for runtime efficiency. File updating at preview runtime.                  | Required   |
|                        |                                                                                                        |            |

### Engine Plan
| Stage |     | Description                                                              | Status      |
| ----- | --- | ------------------------------------------------------------------------ | ----------- |
| 1     |     | Complete base engine to have basic functionality                         |             |
|       | 1.1 | Create an entry point for the application                                | Done        |
|       | 1.2 | Implement a logging system for data and errors                           | Done        |
|       | 1.3 | Build a application layer to handle the lifetime                         | In Progress |
|       | 1.4 | Build a window layer to handle the application per platform              | In Progress |
|       | 1.5 | Build an event manager to send events from the window to the application | In Progress |
|       | 1.6 |                                                                          |             |
|       | 1.X | Extract editor application from engine core                              | Not Started | 
| 2     |     | Create UI support for application development                            |Not Started|
|       | 2.1 |                                                                          |             |
| 3     |     | Extract Editor to independent application                                |Not Started|
|       | 3.1 |                                                                          |             |
| 4     |     | Introduce advanced functionality and default systems                     |Not Started|
|       | 4.1 |                                                                          |             |
