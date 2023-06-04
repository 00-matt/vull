#[=======================================================================[.rst:
FindWaylandProtocols
--------------------

The module defines the following variables:

``WaylandProtocols_DIR``
  Path to Wayland extension schema files.
``WaylandScanner_FOUND``
  True if Wayland extension schema files were found.
``WaylandScanner_VERSION_STRING``
  The version of Wayland protocols found.
#]=======================================================================]

include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)

pkg_check_modules(PC_WaylandProtocols QUIET wayland-protocols)
pkg_get_variable(WaylandProtocols_DIR wayland-protocols pkgdatadir)
set(WaylandProtocols_VERSION_STRING ${PC_WaylandProtocols_VERSION})

find_package_handle_standard_args(WaylandProtocols
        REQUIRED_VARS WaylandProtocols_DIR
        VERSION_VAR WaylandProtocols_VERSION_STRING)
