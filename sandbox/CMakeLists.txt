set(PROJECT_NAME sandbox)
project(${PROJECT_NAME} CXX)

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
add_executable(${PROJECT_NAME}  ${SRC_FILES} ${HEADERS_FILES})

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(WARNING "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    string(CONCAT OUT_DIR
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>/${CMAKE_VS_PLATFORM_NAME}/>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>/${CMAKE_VS_PLATFORM_NAME}/>
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
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
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    string(CONCAT TARGET_NAME
          $<$<CONFIG:Debug>:${PROJECT_NAME}>
        $<$<CONFIG:Release>:${PROJECT_NAME}>
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${PROJECT_NAME}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
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
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_include_directories(${PROJECT_NAME} PRIVATE 
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../STARLIGHT/source;${CMAKE_CURRENT_SOURCE_DIR}/../PACKAGES/libxml2/include>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../STARLIGHT/source;${CMAKE_CURRENT_SOURCE_DIR}/../PACKAGES/libxml2/include>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PRIVATE 
          $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../STARLIGHT/source;${CMAKE_CURRENT_SOURCE_DIR}/../PACKAGES/libxml2/include>
        $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../STARLIGHT/source;${CMAKE_CURRENT_SOURCE_DIR}/../PACKAGES/libxml2/include>
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;LIBXML_STATIC;ALLEGRO_STATICLINK;WIN64;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;_DEBUG;_CONSOLE;UNICODE;_UNICODE>
        $<$<CONFIG:Release>:_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;LIBXML_STATIC;ALLEGRO_STATICLINK;WIN64;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;NDEBUG;_CONSOLE;UNICODE;_UNICODE>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;LIBXML_STATIC;ALLEGRO_STATICLINK;WIN64;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;_DEBUG;_CONSOLE;UNICODE;_UNICODE>
        $<$<CONFIG:Release>:_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;LIBXML_STATIC;ALLEGRO_STATICLINK;WIN64;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;NDEBUG;_CONSOLE;UNICODE;_UNICODE>
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_compile_options(${PROJECT_NAME} PRIVATE
              $<$<CONFIG:Debug>:/Od;/MDd;/W3;/fp:precise;/GR;/bigobj;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
            $<$<CONFIG:Release>:/GL;/O2;/Oi;/MD;/Gy;/W3;/fp:precise;/GR;/bigobj;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
              $<$<CONFIG:Debug>:/Od;/MDd;/W3;/fp:precise;/GR;/bigobj;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
            $<$<CONFIG:Release>:/GL;/O2;/Oi;/MD;/Gy;/W3;/fp:precise;/GR;/bigobj;/EHsc;/GS;/diagnostics:classic;/Zc:wchar_t;/Zc:forScope;/Zc:inline>
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/DEBUG /INCREMENTAL")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/LTCG /DEBUG /INCREMENTAL:NO")
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/DEBUG /INCREMENTAL")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/LTCG /DEBUG /INCREMENTAL:NO")
    endif()
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME} starlight)

# Link with other additional libraries.
target_link_libraries(${PROJECT_NAME} libxml2 starlight opengl32 ws2_32 winmm kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32)
if(MSVC)
    target_link_libraries(${PROJECT_NAME} -LIBPATH:${CMAKE_CURRENT_SOURCE_DIR}/../starlight/bin/Debug/x64/ -LIBPATH:${CMAKE_CURRENT_SOURCE_DIR}/../packages/libxml2/lib/x64/)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} -L${CMAKE_CURRENT_SOURCE_DIR}/../starlight/bin/Debug/x64/ -L${CMAKE_CURRENT_SOURCE_DIR}/../packages/libxml2/lib/x64/)
endif()

