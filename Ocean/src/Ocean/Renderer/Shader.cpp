#include "Shader.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Log.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_Shader.hpp"
#include "Ocean/Renderer/Vulkan/vk_Shader.hpp"

// libs
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>

#include <glslang/SPIRV/GlslangToSpv.h>

/** @todo Figure out why this does not link correctly from glslang. */
const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxMeshOutputVerticesEXT = */ 256,
    /* .maxMeshOutputPrimitivesEXT = */ 256,
    /* .maxMeshWorkGroupSizeX_EXT = */ 128,
    /* .maxMeshWorkGroupSizeY_EXT = */ 128,
    /* .maxMeshWorkGroupSizeZ_EXT = */ 128,
    /* .maxTaskWorkGroupSizeX_EXT = */ 128,
    /* .maxTaskWorkGroupSizeY_EXT = */ 128,
    /* .maxTaskWorkGroupSizeZ_EXT = */ 128,
    /* .maxMeshViewCountEXT = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }
};

namespace Ocean {

    namespace Splash {

        EShLanguage ShaderStageToEShLanguage(Shader::Compiler::ShaderStage stage) {
            switch (stage) {
            case Shader::Compiler::VERTEX_SHADER:
                return EShLanguage::EShLangVertex;

            case Shader::Compiler::FRAGMENT_SHADER:
                return EShLanguage::EShLangFragment;

            case Shader::Compiler::GEOMETRY_SHADER:
                return EShLanguage::EShLangGeometry;
            }

            throw Exception(Error::INVALID_ARGUMENT, "Given invalid ShaderStage! ShaderStageToEShLanguage Failure.");
        }



        DynamicArray<u32> Shader::Compiler::CompileToSpirv(const cstring source, ShaderStage stage) {
            if (s_Initialized)
                Init();

            const EShLanguage internalStage = ShaderStageToEShLanguage(stage);

            glslang::TShader shader(internalStage);

            shader.setStrings(&source, 1);
            shader.setPreamble("");

            shader.setEnvInput(glslang::EShSourceGlsl, internalStage, glslang::EShClientVulkan, 100);
            shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_3);
            shader.setEnvTarget(glslang::EshTargetSpv, glslang::EShTargetSpv_1_0);

            shader.parse(
                &DefaultTBuiltInResource,
                100,
                false,
                EShMsgDefault
            );

            oprint("glslang Parsing Shader: %s\n", shader.getInfoLog());

            glslang::TProgram program;

            program.addShader(&shader);
            program.link(EShMsgDefault);

            oprint("glslang Linking Program: %s\n", program.getInfoLog());

            glslang::TIntermediate* intermediate = program.getIntermediate(internalStage);

            DynamicArray<u32> spirv;
            glslang::GlslangToSpv(*intermediate, spirv);

            return spirv;
        }

        void Shader::Compiler::Init() {
            glslang::InitializeProcess();

            oprint("glslang GLSL Version: %s\n", glslang::GetGlslVersionString());
        }

        void Shader::Compiler::Shutdown() {
            glslang::FinalizeProcess();
        }

        Ref<Shader> Shader::Create(const cstring vertexSource, const cstring fragmentSource, const cstring geometrySource) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glShader>(vertexSource, fragmentSource, geometrySource);

                case RendererAPI::Vulkan:
                    return MakeRef<vkShader>(vertexSource, fragmentSource, geometrySource);
            }

            throw Exception(Error::YOU_FUCKED_UP, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
