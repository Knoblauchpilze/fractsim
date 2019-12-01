#ifndef    JULIA_RENDERING_OPTIONS_HXX
# define   JULIA_RENDERING_OPTIONS_HXX

# include "JuliaRenderingOptions.hh"

namespace fractsim {

  inline
  JuliaRenderingOptions::JuliaRenderingOptions(const utils::Vector2f& constant):
    FractalOptions(getDefaultAccuracy(), getDefaultPalette()),

    m_constant(constant)
  {}

  inline
  utils::Vector2f
  JuliaRenderingOptions::getConstant() const noexcept {
    return m_constant;
  }

  inline
  void
  JuliaRenderingOptions::setConstant(const utils::Vector2f& constant) noexcept {
    m_constant = constant;
  }

}

#endif    /* JULIA_RENDERING_OPTIONS_HXX */
