#pragma once

// For use by Ocean applications //

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/Application.hpp"
#include "Ocean/Core/Layers/Layer.hpp"
#include "Ocean/Core/Log.hpp"

#include "Ocean/Core/Timestep.hpp"

#include "Ocean/Core/Input.hpp"
#include "Ocean/Core/KeyCodes.hpp"
#include "Ocean/Core/MouseCodes.hpp"
#include "Ocean//Renderer/Camera/OrthoCameraController.hpp"
// #include "Ocean//Renderer/Camera/PerspectiveCameraController.hpp"

#include "Ocean/ImGui/ImGuiLayer.hpp"

// --- Scene & Components --- //
#include "Ocean/Core/Scene/Scene.hpp"
#include "Ocean/Core/Scene/Components.hpp"

// --- Renderer --- //
#include "Ocean/Renderer/Renderer.hpp"
#include "Ocean/Renderer/RenderCommand.hpp"

#include "Ocean/Renderer/Buffer.hpp"
#include "Ocean/Renderer/Shader.hpp"
#include "Ocean/Renderer/Framebuffer.hpp"
#include "Ocean/Renderer/Texture.hpp"
#include "Ocean/Renderer/VertexArray.hpp"

#include "Ocean/Renderer/Camera/OrthoCamera.hpp"
// #include "Ocean/Renderer/Camera/PerspectiveCamera.hpp"

// --- Renderer2D --- //
#include "Ocean/Renderer/Renderer2D.hpp"
