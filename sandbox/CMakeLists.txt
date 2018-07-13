project(sandbox CXX)

################################################################################
# Header files
################################################################################
set(HEADERS_FILES
    source/GameState.hpp
    source/Serialize.hpp
)
source_group("Headers" FILES ${HEADERS_FILES})

################################################################################
# Source files
################################################################################
set(SRC_FILES
    source/GameState.cpp
    source/main.cpp
    source/Serialize.cpp
)
source_group("Sources" FILES ${SRC_FILES})

# Warning: pch and target are the same for every configuration
################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(WARNING "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT OUT_DIR
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>/${CMAKE_VS_PLATFORM_NAME}/>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>/${CMAKE_VS_PLATFORM_NAME}/>
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT TARGET_NAME
          $<$<CONFIG:Debug>:${PROJECT_NAME}>
        $<$<CONFIG:Release>:${PROJECT_NAME}>
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${PROJECT_NAME}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../STARLIGHT/source;${CMAKE_CURRENT_SOURCE_DIR}/../PACKAGES/libxml2/include>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../STARLIGHT/source;${CMAKE_CURRENT_SOURCE_DIR}/../PACKAGES/libxml2/include>
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;LIBXML_STATIC;ALLEGRO_STATICLINK;WIN64;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;_DEBUG;_CONSOLE;UNICODE;_UNICODE>
        $<$<CONFIG:Release>:_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;LIBXML_STATIC;ALLEGRO_STATICLINK;WIN64;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;NDEBUG;_CONSOLE;UNICODE;_UNICODE>
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
              $<$<CONFIG:Debug>:/Od;/MDd;/W3;/fp:precise;/GR;/bigobj;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
            $<$<CONFIG:Release>:/GL;/O2;/Oi;/MD;/Gy;/W3;/fp:precise;/GR;/bigobj;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/DEBUG /INCREMENTAL")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/LTCG /DEBUG /INCREMENTAL:NO")
    endif()
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME} starlight)

# Link with other additional libraries.
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_libraries(${PROJECT_NAME}
          $<$<CONFIG:Debug>:libxml2$<SEMICOLON>starlight$<SEMICOLON>opengl32$<SEMICOLON>ws2_32$<SEMICOLON>winmm$<SEMICOLON>kernel32$<SEMICOLON>user32$<SEMICOLON>gdi32$<SEMICOLON>winspool$<SEMICOLON>comdlg32$<SEMICOLON>advapi32$<SEMICOLON>shell32$<SEMICOLON>ole32$<SEMICOLON>oleaut32$<SEMICOLON>uuid$<SEMICOLON>odbc32$<SEMICOLON>odbccp32>
        $<$<CONFIG:Release>:libxml2$<SEMICOLON>starlight$<SEMICOLON>opengl32$<SEMICOLON>kernel32$<SEMICOLON>winmm$<SEMICOLON>ws2_32$<SEMICOLON>user32$<SEMICOLON>gdi32$<SEMICOLON>winspool$<SEMICOLON>comdlg32$<SEMICOLON>advapi32$<SEMICOLON>shell32$<SEMICOLON>ole32$<SEMICOLON>oleaut32$<SEMICOLON>uuid$<SEMICOLON>odbc32$<SEMICOLON>odbccp32>
    )
endif()
if(MSVC)
    target_link_libraries(${PROJECT_NAME} -LIBPATH:${CMAKE_CURRENT_SOURCE_DIR}/../starlight/bin/Debug/x64/ -LIBPATH:${CMAKE_CURRENT_SOURCE_DIR}/../packages/libxml2/lib/x64/)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} -L${CMAKE_CURRENT_SOURCE_DIR}/../starlight/bin/Debug/x64/ -L${CMAKE_CURRENT_SOURCE_DIR}/../packages/libxml2/lib/x64/)
endif()

