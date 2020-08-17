project(quasar CXX)

set(quasar_src
    "src/qs/anim/Animated.cpp"
    "src/qs/anim/Animated.hpp"
    "src/qs/anim/Animation.cpp"
    "src/qs/anim/Animation.hpp"
    "src/qs/anim/Frame.cpp"
    "src/qs/anim/Frame.hpp"
    "src/qs/core/Renderer.cpp"
    "src/qs/core/Renderer.hpp"
    "src/qs/core/Shader.cpp"
    "src/qs/core/Shader.hpp"
    "src/qs/core/Transform.cpp"
    "src/qs/core/Transform.hpp"
    "src/qs/core/VertexData.cpp"
    "src/qs/core/VertexData.hpp"
    "src/qs/core/Window.cpp"
    "src/qs/core/Window.hpp"
    "src/qs/core/WindowSettings.cpp"
    "src/qs/core/WindowSettings.hpp"
    "src/qs/graphics/Camera.cpp"
    "src/qs/graphics/Camera.hpp"
    "src/qs/graphics/LightSource.cpp"
    "src/qs/graphics/LightSource.hpp"
    "src/qs/graphics/Particle.cpp"
    "src/qs/graphics/Particle.hpp"
    "src/qs/graphics/ParticleGenerator.cpp"
    "src/qs/graphics/ParticleGenerator.hpp"
    "src/qs/graphics/TextureAtlas.cpp"
    "src/qs/graphics/TextureAtlas.hpp"
    "src/qs/shapes/Circle.cpp"
    "src/qs/shapes/Circle.hpp"
    "src/qs/shapes/Line.cpp"
    "src/qs/shapes/Line.hpp"
    "src/qs/shapes/Point.cpp"
    "src/qs/shapes/Point.hpp"
    "src/qs/sprite/AnimatedBatchSprite.cpp"
    "src/qs/sprite/AnimatedBatchSprite.hpp"
    "src/qs/sprite/AnimatedSprite.cpp"
    "src/qs/sprite/AnimatedSprite.hpp"
    "src/qs/sprite/BatchedSprite.cpp"
    "src/qs/sprite/BatchedSprite.hpp"
    "src/qs/sprite/Sprite.cpp"
    "src/qs/sprite/Sprite.hpp"
    "src/qs/sprite/SpriteBatch.cpp"
    "src/qs/sprite/SpriteBatch.hpp"
    "src/qs/text/Character.cpp"
    "src/qs/text/Character.hpp"
    "src/qs/text/Font.cpp"
    "src/qs/text/Font.hpp"
    "src/qs/text/FreeType.cpp"
    "src/qs/text/FreeType.hpp"
    "src/qs/text/Text.cpp"
    "src/qs/text/Text.hpp"
    "src/qs/texture/BaseTexture.cpp"
    "src/qs/texture/BaseTexture.hpp"
    "src/qs/texture/RenderTexture.cpp"
    "src/qs/texture/RenderTexture.hpp"
    "src/qs/texture/Texture.cpp"
    "src/qs/texture/Texture.hpp"
    "src/qs/utils/Meta.cpp"
    "src/qs/utils/Meta.hpp"
    "src/qs/utils/TextureFilters.cpp"
    "src/qs/utils/TextureFilters.hpp"
    "src/qs/vertex/IndexBuffer.cpp"
    "src/qs/vertex/IndexBuffer.hpp"
    "src/qs/vertex/type/InstanceVertex.cpp"
    "src/qs/vertex/type/InstanceVertex.hpp"
    "src/qs/vertex/type/PrimitiveVertex.cpp"
    "src/qs/vertex/type/PrimitiveVertex.hpp"
    "src/qs/vertex/type/SpriteVertex.cpp"
    "src/qs/vertex/type/SpriteVertex.hpp"
    "src/qs/vertex/VertexArray.cpp"
    "src/qs/vertex/VertexArray.hpp"
    "src/qs/vertex/VertexAttribute.cpp"
    "src/qs/vertex/VertexAttribute.hpp"
    "src/qs/vertex/VertexBuffer.cpp"
    "src/qs/vertex/VertexBuffer.hpp"
    "src/qs/vertex/VertexLayout.cpp"
    "src/qs/vertex/VertexLayout.hpp"
)

source_group(${PROJECT_NAME} ${quasar_src})
add_library(${PROJECT_NAME} STATIC ${quasar_src})

target_include_directories(${PROJECT_NAME} PUBLIC
    "src/"
    "../../dependencies/glm/"
    "../../dependencies/glfw3/include/"
    "../../dependencies/stb/"
    "../../dependencies/glad/"
    "../../dependencies/freetype/include/"
    "../../dependencies/c++20/"
    "../protostar/src/"
    "../pulsar/src/"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
    "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
    "NDEBUG"
    ">"
    "GLM_FORCE_XYZW_ONLY"
    "GLFW_INCLUDE_NONE"
    "STB_IMAGE_WRITE_STATIC"
    "STB_IMAGE_WRITE_IMPLEMENTATION"
    "STBI_ONLY_PNG"
    "STB_IMAGE_STATIC"
    "STB_IMAGE_IMPLEMENTATION"
    "UNICODE"
    "_UNICODE"
)

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
        "_CRT_SECURE_NO_WARNINGS"
        "_CRT_SECURE_NO_DEPRECATE"
        "_CRT_NONSTDC_NO_DEPRECATE"
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
)

include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

if (MSVC)
    if (NOT (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC"))
        target_compile_options(${PROJECT_NAME} PRIVATE
            /QIntel-jcc-erratum
        )
    endif()

    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /JMC
            /ZI
            /Od
            /RTC1
            /MTd
            /GS
            /GR
        >
        $<$<CONFIG:Release>:
            /O2
            /Oi
            /Ot
            /GL
            /MT
            /GS-
            /Gy
            /GR-
        >
        /std:c++latest
        /sdl
        /WX-
        /MP
        /GF
        /EHsc
        /fp:except
        /fp:precise
        /bigobj
        /Zc:preprocessor
        /Y-
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        /INCREMENTAL
        /DEBUG
        /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Release>:
        /INCREMENTAL:NO
        /SUBSYSTEM:WINDOWS
        /OPT:REF
        /OPT:ICF
        /LTCG:incremental
        >
        /SAFESEH
        /ignore:4099
        /MACHINE:X64
    )
endif()
