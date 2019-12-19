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
  RenderingOptions.cc
  RenderingTile.cc
  Fractal.cc
  StatusBar.cc
  NewtonRenderingOptions.cc
  ColorPalette.cc
  )

add_library (fractsim_lib SHARED
  ${SOURCES}
  )

set(FRACTSIM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_link_libraries(fractsim_lib
  sdl_core
  sdl_graphic
  )

target_include_directories (fractsim_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
