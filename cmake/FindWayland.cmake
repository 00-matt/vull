include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)

set(Wayland_KNOWN_COMPONENTS client cursor egl server)

if(NOT Wayland_FIND_COMPONENTS)
    set(Wayland_FIND_COMPONENTS ${WAYLAND_KNOWN_COMPONENTS})
endif()

foreach(component ${Wayland_FIND_COMPONENTS})
    if(NOT component IN_LIST Wayland_KNOWN_COMPONENTS)
        # TODO: If not a required component, warn instead of fatal error.
        message(FATAL_ERROR "Unknown component ${component}")
    endif()

    if(PkgConfig_FOUND)
        pkg_check_modules(PC_Wayland_${component} QUIET wayland-${component})
    endif()

    find_path(Wayland_${component}_INCLUDE_DIR
            NAMES wayland-${component}.h
            HINTS ${PC_Wayland_${component}_INCLUDE_DIRS})
    find_library(Wayland_${component}_LIBRARY
            NAMES wayland-${component}
            HINTS ${PC_Wayland_${component}_LIBRARY_DIRS})

    set(FPHSA_NAME_MISMATCHED 1)
    find_package_handle_standard_args(Wayland_${component}
            FOUND_VAR Wayland_${component}_FOUND
            REQUIRED_VARS Wayland_${component}_LIBRARY Wayland_${component}_INCLUDE_DIR
            VERSION_VAR PC_Wayland_${component}_VERSION)
    unset(FPHSA_NAME_MISMATCHED)

    mark_as_advanced(Wayland_${component}_LIBRARY Wayland_${component}_INCLUDE_DIR)

    if(Wayland_${component}_FOUND)
        list(APPEND Wayland_LIBRARIES "${Wayland_${component}_LIBRARY}")
        list(APPEND Wayland_INCLUDE_DIRS "${Wayland_${component}_INCLUDE_DIR}")

        if(NOT TARGET Wayland::${component})
            add_library(Wayland::${component} UNKNOWN IMPORTED)
            set_target_properties(Wayland::${component} PROPERTIES
                    IMPORTED_LOCATION "${Wayland_${component}_LIBRARY}"
                    INTERFACE_COMPILE_OPTIONS "${PC_Wayland_${component}_DEFINITIONS}"
                    INTERFACE_INCLUDE_DIRECTORIES "${Wayland_${component}_INCLUDE_DIR}")
        endif()
        list(APPEND Wayland_TARGETS Wayland::${component})
    endif()
endforeach()

find_package_handle_standard_args(Wayland
        FOUND_VAR Wayland_FOUND
        REQUIRED_VARS Wayland_LIBRARIES Wayland_INCLUDE_DIRS
        HANDLE_COMPONENTS)
