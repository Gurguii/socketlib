file(REMOVE_RECURSE
  "libgsocket.pdb"
  "libgsocket.so"
  "libgsocket.so.1"
  "libgsocket.so.1.0.0"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/gsocket.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
