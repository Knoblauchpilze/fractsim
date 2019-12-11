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
      sdl::core::engine::Color::NamedColor::White,
      sdl::core::engine::Color::NamedColor::Black
    );

    palette->setColorAt(0.05f, sdl::core::engine::Color::NamedColor::Gray);
    palette->setColorAt(0.10f, sdl::core::engine::Color::NamedColor::White);
    palette->setColorAt(0.15f, sdl::core::engine::Color::NamedColor::Black);
    palette->setColorAt(0.20f, sdl::core::engine::Color::NamedColor::Red);
    palette->setColorAt(0.25f, sdl::core::engine::Color::NamedColor::Green);
    palette->setColorAt(0.30f, sdl::core::engine::Color::NamedColor::Blue);
    palette->setColorAt(0.35f, sdl::core::engine::Color::NamedColor::Yellow);
    palette->setColorAt(0.40f, sdl::core::engine::Color::NamedColor::Orange);
    palette->setColorAt(0.45f, sdl::core::engine::Color::NamedColor::Cyan);
    palette->setColorAt(0.50f, sdl::core::engine::Color::NamedColor::Magenta);
    palette->setColorAt(0.55f, sdl::core::engine::Color::NamedColor::Silver);
    palette->setColorAt(0.60f, sdl::core::engine::Color::NamedColor::Gray);
    palette->setColorAt(0.65f, sdl::core::engine::Color::NamedColor::Maroon);
    palette->setColorAt(0.70f, sdl::core::engine::Color::NamedColor::Olive);
    palette->setColorAt(0.75f, sdl::core::engine::Color::NamedColor::Pink);
    palette->setColorAt(0.80f, sdl::core::engine::Color::NamedColor::Purple);
    palette->setColorAt(0.85f, sdl::core::engine::Color::NamedColor::Teal);
    palette->setColorAt(0.90f, sdl::core::engine::Color::NamedColor::Navy);
    palette->setColorAt(0.95f, sdl::core::engine::Color::NamedColor::CorneFlowerBlue);

    return palette;
  }

}

#endif    /* FRACTAL_OPTIONS_HXX */
