#ifndef    MANDELBROT_RENDERING_OPTIONS_HXX
# define   MANDELBROT_RENDERING_OPTIONS_HXX

# include "MandelbrotRenderingOptions.hh"

namespace fractsim {

  inline
  MandelbrotRenderingOptions::MandelbrotRenderingOptions(float exponent):
    FractalOptions(getDefaultAccuracy(), getDefaultPalette()),

    m_exponent(exponent)
  {}

  inline
  utils::Boxf
  MandelbrotRenderingOptions::getDefaultRenderingWindow() const noexcept {
    return utils::Boxf(-0.7f, 0.0f, 2.6f, 2.0f);
  }

  inline
  float
  MandelbrotRenderingOptions::getExponent() const noexcept {
    return m_exponent;
  }

  inline
  void
  MandelbrotRenderingOptions::setExponent(float exponent) noexcept {
    m_exponent = exponent;
  }

}

#endif    /* MANDELBROT_RENDERING_OPTIONS_HXX */
