if (MSVC)
	set(GALAXY_LINK_FLAGS
        /ignore:4099
        /MACHINE:X64
	)

	set(GALAXY_LINK_FLAGS_DEBUG
		${GALAXY_LINK_FLAGS}
		/INCREMENTAL
        /DEBUG
        /SUBSYSTEM:CONSOLE
	)

	set(GALAXY_LINK_FLAGS_RELEASE
		${GALAXY_LINK_FLAGS}
		/INCREMENTAL:NO
        /OPT:REF
        /OPT:ICF
        /LTCG:incremental
        /SUBSYSTEM:WINDOWS
        /ENTRY:mainCRTStartup
	)
else()
	set(GALAXY_LINK_FLAGS
		-pthread
		-static
	)

	set(GALAXY_LINK_FLAGS_DEBUG
		${GALAXY_LINK_FLAGS}
	)

	set(GALAXY_LINK_FLAGS_RELEASE
		${GALAXY_LINK_FLAGS}
	)
endif()