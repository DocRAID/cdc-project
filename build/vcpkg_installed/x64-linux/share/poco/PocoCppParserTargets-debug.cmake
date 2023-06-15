#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::CppParser" for configuration "Debug"
set_property(TARGET Poco::CppParser APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Poco::CppParser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libPocoCppParserd.a"
  )

list(APPEND _cmake_import_check_targets Poco::CppParser )
list(APPEND _cmake_import_check_files_for_Poco::CppParser "${_IMPORT_PREFIX}/debug/lib/libPocoCppParserd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
