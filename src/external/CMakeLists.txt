if (BUILD_EXAMPLES)
    # Detect if gnuplot is installed (required by matplot++)
    find_program(GNUPLOT_EXECUTABLE gnuplot)
    if (NOT GNUPLOT_EXECUTABLE)
        message(FATAL_ERROR "Gnuplot not found. Please install it with 'sudo apt-get install gnuplot'.")
    endif()

    # Detect if matplot++ is installed
    if(EXISTS "${CMAKE_SOURCE_DIR}/src/external/matplotplusplus")
        add_subdirectory(matplotplusplus)
    else()
        message(
                FATAL_ERROR
                "The directory 'external/matplotplusplus' does not exist. "
                "Did you forget to clone the repository? "
                "Go to the root of the repository and run the following command: "
                "git submodule update --init --recursive"
        )
    endif()
endif ()