
add_library (fractsim_lib SHARED "")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_sources (fractsim_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/FractalRenderer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MandelbrotOptions.cc
	${CMAKE_CURRENT_SOURCE_DIR}/JuliaOptions.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NewtonOptions.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RenderingStatus.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RenderingOptions.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RenderingTile.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Fractal.cc
	${CMAKE_CURRENT_SOURCE_DIR}/StatusBar.cc
	${CMAKE_CURRENT_SOURCE_DIR}/NewtonRenderingOptions.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ColorPalette.cc
	)

target_link_libraries(fractsim_lib
	sdl_core
	sdl_graphic
	)

target_include_directories (fractsim_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)
