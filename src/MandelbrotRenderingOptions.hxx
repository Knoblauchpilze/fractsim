#ifndef    MANDELBROT_RENDERING_OPTIONS_HXX
# define   MANDELBROT_RENDERING_OPTIONS_HXX

# include "MandelbrotRenderingOptions.hh"
# include <complex>

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
  unsigned
  MandelbrotRenderingOptions::compute(const utils::Vector2f& p) const noexcept {
    // Compute terms of the series until it diverges.
    unsigned acc = getAccuracy();
    float n = getExponent();
    float thresh = getDivergenceThreshold();
    float len = 0.0f;
    unsigned terms = 0u;
    std::complex<float> cur(0.0f, 0.0f);
    std::complex<float> c(p.x(), p.y());

    while (len < thresh && terms < acc) {
      cur = std::pow(cur, n) + c;

      len = norm(cur);
      if (len < thresh) {
        ++terms;
      }
    }

    return terms;
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

  inline
  float
  MandelbrotRenderingOptions::getDivergenceThreshold() noexcept {
    return 4.0f;
  }

}

#endif    /* MANDELBROT_RENDERING_OPTIONS_HXX */
