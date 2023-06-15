#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::Zip" for configuration "Debug"
set_property(TARGET Poco::Zip APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Poco::Zip PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libPocoZipd.a"
  )

list(APPEND _cmake_import_check_targets Poco::Zip )
list(APPEND _cmake_import_check_files_for_Poco::Zip "${_IMPORT_PREFIX}/debug/lib/libPocoZipd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
