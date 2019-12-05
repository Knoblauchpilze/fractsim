#ifndef    NEWTON_RENDERING_OPTIONS_HXX
# define   NEWTON_RENDERING_OPTIONS_HXX

# include "NewtonRenderingOptions.hh"

namespace fractsim {

  inline
  NewtonRenderingOptions::NewtonRenderingOptions(const std::vector<Coefficient>& coeffs):
    FractalOptions(getDefaultAccuracy(), getDefaultPalette()),

    m_coefficients(coeffs)
  {}

  inline
  utils::Boxf
  NewtonRenderingOptions::getDefaultRenderingWindow() const noexcept {
    return utils::Boxf(0.0f, 0.0f, 4.0f, 3.0f);
  }

  inline
  unsigned
  NewtonRenderingOptions::compute(const utils::Vector2f& p) const noexcept {
    // TODO: Implementation.
    return p.length() < 3.0f ? 0u : getAccuracy();
  }

  inline
  const std::vector<NewtonRenderingOptions::Coefficient>&
  NewtonRenderingOptions::getCoefficients() const noexcept {
    return m_coefficients;
  }

  inline
  void
  NewtonRenderingOptions::setCoefficients(const std::vector<Coefficient>& coeffs) noexcept {
    m_coefficients = coeffs;
  }

}

#endif    /* NEWTON_RENDERING_OPTIONS_HXX */
