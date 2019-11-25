
# include "FractalRenderer.hh"

namespace fractsim {

  FractalRenderer::FractalRenderer(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::graphic::ScrollableWidget(std::string("fractal_renderer"),
                                   parent,
                                   hint)
  {
    setService(std::string("fractal_renderer"));
  }

}
