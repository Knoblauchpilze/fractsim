
# include "MandelbrotOptions.hh"

namespace fractsim {

  MandelbrotOptions::MandelbrotOptions(const utils::Sizef& hint,
                                       sdl::core::SdlWidget* parent):
    sdl::core::SdlWidget(std::string("mandelbrot_options"),
                         hint,
                         parent)
  {
    build();
  }

  void
  MandelbrotOptions::build() {}

}
