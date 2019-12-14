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
    // TODO: Implementation. We should keep an internal array of roots for the polynom.
    // And we should also change the accuracy so that it does not modify the value of
    // the pixel in `RenderinTile.cc:45`.
    // Basically what we could do is create a palette which has as many colors as there
    // are roots for the polynom (i.e. the max degree) and change the accuracy to be `1`
    // for the outside world and the specified value for internal use.
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
