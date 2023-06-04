#[=======================================================================[.rst:
FindWaylandScanner
------------------

The module defines the following variables:

``WaylandScanner_EXECUTABLE``
  Path to wayland-scanner command-line tool.
``WaylandScanner_FOUND``
  True if the wayland-scanner command-line tool was found.
``WaylandScanner_VERSION_STRING``
  The version of wayland-scanner found.

The module defines the following ``IMPORTED`` targets (when
``CMAKE_ROLE`` is ``PROJECT``):

``WaylandScanner::WaylandScanner``
  Executable of the wayland-scanner command-line tool.
#]=======================================================================]

include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)

pkg_check_modules(PC_WaylandScanner QUIET wayland-scanner)
pkg_get_variable(PC_WaylandScanner_EXECUTABLE wayland-scanner wayland_scanner)
mark_as_advanced(PC_WaylandScanner_EXECUTABLE)

find_program(WaylandScanner_EXECUTABLE
        NAMES wayland-scanner
        HINTS "${PC_WaylandScanner_EXECUTABLE}")
mark_as_advanced(WaylandScanner_EXECUTABLE)

if(WaylandScanner_EXECUTABLE)
    if(PC_WaylandScanner_VERSION)
        set(WaylandScanner_VERSION_STRING ${PC_WaylandScanner_VERSION})
    else()
        execute_process(COMMAND ${WaylandScanner_EXECUTABLE} --version
                ERROR_VARIABLE _wls_version
                ERROR_STRIP_TRAILING_WHITESPACE)
        if(_wls_version MATCHES "^wayland-scanner [0-9]")
            string(REPLACE "wayland-scanner " ""
                    WaylandScanner_VERSION_STRING "${_wls_version}")
        endif()
        unset(_wls_version)
    endif()

    get_property(_wls_role GLOBAL PROPERTY CMAKE_ROLE)
    if(_wlsrole STREQUAL "PROJECT" AND NOT TARGET WaylandScanner::WaylandScanner)
        add_executable(WaylandScanner::WaylandScanner IMPORTED)
        set_property(TARGET WaylandScanner::WaylandScanner
                PROPERTY IMPORTED_LOCATION "${WaylandScanner_EXECUTABLE}")
    endif()
    unset(_wls_role)
endif()

find_package_handle_standard_args(WaylandScanner
        REQUIRED_VARS WaylandScanner_EXECUTABLE
        VERSION_VAR WaylandScanner_VERSION_STRING)
