#ifndef    COLOR_PALETTE_HXX
# define   COLOR_PALETTE_HXX

# include "ColorPalette.hh"

namespace fractsim {

  inline
  ColorPalette::ColorPalette(unsigned count,
                             const Mode& mode,
                             float s,
                             float v):
    utils::CoreObject(std::string("color_palette_s(") + std::to_string(s) + ")_v(" + std::to_string(v) + ")"),

    m_size(count),
    m_mode(mode),

    m_s(std::min(1.0f, std::max(0.0f, s))),
    m_v(std::min(1.0f, std::max(0.0f, v))),

    m_colors()
  {
    generate();
  }

  inline
  const std::vector<sdl::core::engine::Color>&
  ColorPalette::getColors() const noexcept {
    return m_colors;
  }

  inline
  float
  ColorPalette::getGenerationPattern() noexcept {
    return 0.618033988749895f;
  }

}

#endif    /* COLOR_PALETTE_HXX */
