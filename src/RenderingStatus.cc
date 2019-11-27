
# include "RenderingStatus.hh"

namespace fractsim {

  RenderingStatus::RenderingStatus(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::core::SdlWidget(std::string("rendering_status"),
                         hint,
                         parent,
                         sdl::core::engine::Color::NamedColor::Silver),

    m_locker()
  {
    build();
  }

  void
  RenderingStatus::build() {
    // Set a focus policy which do not allow to hover or click on this widget.
    setFocusPolicy(sdl::core::FocusPolicy());

    // The rendering status is composed of a display allowing to display the
    // current zoom level, the progress bar of the current computation, some
    // indication of the palette to use and a render button.
  }

}
