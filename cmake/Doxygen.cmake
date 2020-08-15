option(ENABLE_DOXYGEN "Enable a target for building doxygen.", OFF)

if (${ENABLE_DOXYGEN})
    find_package(doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
        set(DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/output/docs)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT}/cmake_configured @ONLY)
        add_custom_target(doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${DOXYGEN_OUT}
            VERBATIM
        )
    endif()
endif()
