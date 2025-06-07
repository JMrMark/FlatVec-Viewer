# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\FlatVec-Viewer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\FlatVec-Viewer_autogen.dir\\ParseCache.txt"
  "FlatVec-Viewer_autogen"
  )
endif()
