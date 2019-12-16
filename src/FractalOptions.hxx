#ifndef    FRACTAL_OPTIONS_HXX
# define   FRACTAL_OPTIONS_HXX

# include "FractalOptions.hh"

namespace fractsim {

  inline
  FractalOptions::FractalOptions(unsigned accuracy,
                                 unsigned wrapping,
                                 sdl::core::engine::GradientShPtr palette):
    utils::CoreObject("fratcal_options"),

    m_accuracy(accuracy),
    m_wrapping(wrapping),
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
      sdl::core::engine::gradient::Mode::Linear
    );

    // Define a palette. This one was derived from the post at this link:
    // https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia
    // Note that we included a stop at `1` with a black color so that the
    // points belonging to any fractal are still displayed in the same way.
    palette->setColorAt(0.0000f, sdl::core::engine::Color::fromRGB(0.0000f, 0.0275f, 0.3922f));
    palette->setColorAt(0.1600f, sdl::core::engine::Color::fromRGB(0.1255f, 0.4196f, 0.7961f));
    palette->setColorAt(0.4200f, sdl::core::engine::Color::fromRGB(0.9294f, 1.0000f, 1.0000f));
    palette->setColorAt(0.6425f, sdl::core::engine::Color::fromRGB(1.0000f, 0.6667f, 0.0000f));
    palette->setColorAt(0.8575f, sdl::core::engine::Color::fromRGB(0.0000f, 0.0078f, 0.0000f));
    palette->setColorAt(1.0000f, sdl::core::engine::Color::NamedColor::Black);

    return palette;
  }

  inline
  unsigned
  FractalOptions::getDefaultPaletteWrapping() noexcept {
    return 30u;
  }

  inline
  unsigned
  FractalOptions::getPaletteWrapping() const noexcept {
    return m_wrapping;
  }

  inline
  float
  FractalOptions::performWrapping(float val) const noexcept {
    // Compute the range occupied by a full turn of the palette.
    float wrap = 1.0f * getPaletteWrapping() / getAccuracy();

    // Compute the percentage of the available interval reached
    // by the input value.
    float perc = val / getAccuracy();

    // The wrapped cound it the remainder of the reached percentage
    // in the division with the wrapping interval. We then normalize
    // the output value so as not to lose any palette values.
    return std::fmod(perc, wrap) / wrap;
  }

}

#endif    /* FRACTAL_OPTIONS_HXX */
