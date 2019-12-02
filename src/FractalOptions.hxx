#ifndef    FRACTAL_OPTIONS_HXX
# define   FRACTAL_OPTIONS_HXX

# include "FractalOptions.hh"

namespace fractsim {

  inline
  FractalOptions::FractalOptions(unsigned accuracy,
                                 sdl::core::engine::GradientShPtr palette):
    utils::CoreObject("fratcal_options"),

    m_accuracy(accuracy),
    m_palette(nullptr)
  {
    setService("options");

    setPalette(palette);
  }

  inline
  unsigned
  FractalOptions::getAccuracy() const noexcept {
    return m_accuracy;
  }

  inline
  void
  FractalOptions::setAccuracy(unsigned acc) noexcept {
    m_accuracy = acc;
  }

  inline
  sdl::core::engine::GradientShPtr
  FractalOptions::getPalette() const noexcept {
    return m_palette;
  }

  inline
  void
  FractalOptions::setPalette(sdl::core::engine::GradientShPtr palette) {
    // Check consistency.
    if (palette == nullptr) {
      error(
        std::string("Could not assign new palette to fractal options"),
        std::string("Invalid null palette")
      );
    }

    m_palette = palette;
  }

  inline
  unsigned
  FractalOptions::getDefaultAccuracy() noexcept {
    return 128u;
  }

  inline
  sdl::core::engine::GradientShPtr
  FractalOptions::getDefaultPalette() noexcept {
    sdl::core::engine::GradientShPtr palette;

    palette = std::make_shared<sdl::core::engine::Gradient>(
      "default_palette",
      sdl::core::engine::gradient::Mode::Linear,
      sdl::core::engine::Color::NamedColor::Red,
      sdl::core::engine::Color::NamedColor::Black
    );

    palette->setColorAt(0.2f, sdl::core::engine::Color::NamedColor::Purple);
    palette->setColorAt(0.4f, sdl::core::engine::Color::NamedColor::Blue);
    palette->setColorAt(0.6f, sdl::core::engine::Color::NamedColor::Yellow);
    palette->setColorAt(0.8f, sdl::core::engine::Color::NamedColor::Green);

    return palette;
  }

}

#endif    /* FRACTAL_OPTIONS_HXX */
