# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/pangolin_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/pangolin_autogen.dir/ParseCache.txt"
  "pangolin_autogen"
  )
endif()
