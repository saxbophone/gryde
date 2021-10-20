include(GNUInstallDirs)

add_library(gryde STATIC)
# ALIAS target to export a namespaced target even when building in-tree
add_library(Gryde::gryde ALIAS gryde)
# add source files
add_subdirectory(src)
# library public header files location
target_include_directories(
    gryde PUBLIC
    # different include directory path depending on if using the local or installed version of library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# pass in version of project as preprocessor definitions
target_compile_definitions(
    gryde PRIVATE
    -DGRYDE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DGRYDE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DGRYDE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DGRYDE_VERSION_STRING=${GRYDE_ESCAPED_VERSION_STRING}
)
# set up version and soversion for the main library object
set_target_properties(
    gryde PROPERTIES
    VERSION ${GRYDE_VERSION_STRING}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_Gryde_MAJOR_VERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_Gryde_MINOR_VERSION ${PROJECT_VERSION_MINOR}
)
set_property(
    TARGET gryde
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${Gryde_MAJOR_VERSION}.${Gryde_MINOR_VERSION}"
)
# inherit common project compiler options
target_link_libraries(
    gryde
        PRIVATE
            $<BUILD_INTERFACE:gryde-compiler-options>
)

# install if we're not being built as a sub-project
if (NOT GRYDE_SUBPROJECT)
    # library
    install(
        TARGETS gryde
        EXPORT GrydeTargets
        # when a static library
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a shared library on UNIX
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a DLL on Windows
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    # public headers
    install(
        DIRECTORY "include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    # CMake export
    install(
        EXPORT GrydeTargets
        FILE GrydeTargets.cmake
        NAMESPACE Gryde::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Gryde"
    )
    # CMake package
    include(CMakePackageConfigHelpers)
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/GrydeConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Gryde"
    )
    # generate the version file for the config file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/GrydeConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY SameMinorVersion
    )
    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/GrydeConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/GrydeConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Gryde"
    )
endif()
