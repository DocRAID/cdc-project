#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::DataODBC" for configuration "Release"
set_property(TARGET Poco::DataODBC APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Poco::DataODBC PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libPocoDataODBC.a"
  )

list(APPEND _cmake_import_check_targets Poco::DataODBC )
list(APPEND _cmake_import_check_files_for_Poco::DataODBC "${_IMPORT_PREFIX}/lib/libPocoDataODBC.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)