#ifndef    NEWTON_RENDERING_OPTIONS_HXX
# define   NEWTON_RENDERING_OPTIONS_HXX

# include "NewtonRenderingOptions.hh"

namespace fractsim {

  inline
  utils::Boxf
  NewtonRenderingOptions::getDefaultRenderingWindow() const noexcept {
    return utils::Boxf(0.0f, 0.0f, 4.0f, 3.0f);
  }

  inline
  float
  NewtonRenderingOptions::getNullThreshold() noexcept {
    return 0.000000001f;
  }

  inline
  float
  NewtonRenderingOptions::getRootEqualityThreshold() noexcept {
    return 0.0001f;
  }

  inline
  float
  NewtonRenderingOptions::getConvergenceThreshold() noexcept {
    return 0.0001f;
  }

  inline
  unsigned
  NewtonRenderingOptions::getConvergenceDuration() noexcept {
    return 5u;
  }

  inline
  void
  NewtonRenderingOptions::setCoefficients(const std::vector<Coefficient>& coeffs) noexcept {
    Guard guard(m_propsLocker);

    // Update coefficients and derivative.
    for (unsigned id = 0u ; id < coeffs.size() ; ++id) {
      m_coefficients.push_back(
        InternalCoefficient{
          coeffs[id].degree,
          std::complex<float>(coeffs[id].coeff.x(), coeffs[id].coeff.y())
        }
      );
    }

    computeDerivative();
  }

}

#endif    /* NEWTON_RENDERING_OPTIONS_HXX */
