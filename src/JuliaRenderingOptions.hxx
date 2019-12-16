#ifndef    JULIA_RENDERING_OPTIONS_HXX
# define   JULIA_RENDERING_OPTIONS_HXX

# include "JuliaRenderingOptions.hh"
# include <complex>

namespace fractsim {

  inline
  JuliaRenderingOptions::JuliaRenderingOptions(const utils::Vector2f& constant):
    FractalOptions(getDefaultAccuracy(), getDefaultPalette()),

    m_constant(constant)
  {}

  inline
  utils::Boxf
  JuliaRenderingOptions::getDefaultRenderingWindow() const noexcept {
    return utils::Boxf(0.0f, 0.0f, 2.0f, 2.0f);
  }

  inline
  float
  JuliaRenderingOptions::compute(const utils::Vector2f& p) const noexcept {
    // Compute terms of the series until it diverges.
    unsigned acc = getAccuracy();
    float thresh = getDivergenceThreshold();
    unsigned over = getSmoothingOvershoot();
    float len = 0.0f, tmp = 0.0f;
    unsigned terms = 0u, conv = 0u;

    utils::Vector2f cur = p;
    utils::Vector2f c = getConstant();

    while ((len < thresh && terms < acc) || terms - conv < over) {
      tmp = cur.x() * cur.x() - cur.y() * cur.y() + c.x();
      cur.y() = 2.0f * cur.x() * cur.y() + c.y();
      cur.x() = tmp;

      len = cur.lengthSquared();
      if (len >= thresh && conv == 0u) {
        conv = terms;
      }
      ++terms;
    }

    // Smooth the iterations count with some mathematical magic.
    // Resources: http://www.fractalforums.com/programming/julia-smooth-colouring-how-to-do/
    // Note that some information was found here to complement what is explained in the post:
    // https://stackoverflow.com/questions/369438/smooth-spectrum-for-mandelbrot-set-rendering?rq=1
    // But it didn't work out and we resorted to use the same algorithm as used in the Mandelbrot
    // case.
    float sTerms = 1.0f * terms;
    if (terms < acc) {
      sTerms = terms + 1.0f - std::log(std::log(std::sqrt(len))) / std::log(getDefaultExponent());
    }

    return sTerms / acc;
  }

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

  inline
  float
  JuliaRenderingOptions::getDivergenceThreshold() noexcept {
    return 4.0f;
  }

  inline
  unsigned
  JuliaRenderingOptions::getSmoothingOvershoot() noexcept {
    return 3u;
  }

  inline
  float
  JuliaRenderingOptions::getDefaultExponent() noexcept {
    return 2.0f;
  }
}

#endif    /* JULIA_RENDERING_OPTIONS_HXX */
