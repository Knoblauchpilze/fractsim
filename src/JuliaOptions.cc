
# include "JuliaOptions.hh"

namespace fractsim {

  JuliaOptions::JuliaOptions(const utils::Sizef& hint,
                             sdl::core::SdlWidget* parent):
    sdl::core::SdlWidget(std::string("julia_options"),
                         hint,
                         parent)
  {
    build();
  }

  void
  JuliaOptions::build() {}

}
