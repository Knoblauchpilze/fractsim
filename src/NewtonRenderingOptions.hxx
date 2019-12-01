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
  const std::vector<Coefficient>&
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
