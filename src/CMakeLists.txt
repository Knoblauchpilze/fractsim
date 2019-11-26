project (fractsim_lib)

set (CMAKE_CXX_STANDARD 14)

#set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

set (SOURCES
  FractalRenderer.cc
  MandelbrotOptions.cc
  JuliaOptions.cc
  NewtonOptions.cc
  RenderingStatus.cc
  )

add_library (fractsim_lib SHARED
  ${SOURCES}
  )

set(SDL_MINIMAL_DUMMY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_link_libraries(fractsim_lib
  sdl_core
  )

target_include_directories (fractsim_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
