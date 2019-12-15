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
    float len = 0.0f, tmp = 0.0f;
    unsigned terms = 0u;
    utils::Vector2f cur = p;
    utils::Vector2f c = getConstant();

    while (len < thresh && terms < acc) {
      tmp = cur.x() * cur.x() - cur.y() * cur.y() + c.x();
      cur.y() = 2.0f * cur.x() * cur.y() + c.y();
      cur.x() = tmp;

      len = cur.lengthSquared();

      if (len < thresh) {
        ++terms;
      }
    }

    return 1.0f * terms / acc;
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

}

#endif    /* JULIA_RENDERING_OPTIONS_HXX */
