#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::SevenZip" for configuration "Release"
set_property(TARGET Poco::SevenZip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Poco::SevenZip PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libPocoSevenZip.a"
  )

list(APPEND _cmake_import_check_targets Poco::SevenZip )
list(APPEND _cmake_import_check_files_for_Poco::SevenZip "${_IMPORT_PREFIX}/lib/libPocoSevenZip.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)