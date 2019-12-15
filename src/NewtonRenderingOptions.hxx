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
  sdl::core::engine::Color
  NewtonRenderingOptions::generateRandomColor(float val,
                                              float count) noexcept
  {
    // This function generates vibrant, "evenly spaced" colours (i.e. no clustering). This
    // is ideal for creating easily distinguishable vibrant markers in Google Maps and other
    // apps. This solution was originally found here:
    // https://stackoverflow.com/questions/1484506/random-color-generator
    // And apparently comes from there:  Adam Cole, 2011-Sept-14
    // http://mjijackson.com/2008/02/rgb-to-hsl-and-rgb-to-hsv-color-model-conversion-algorithms-in-javascript

    float r, g, b;
    float perc = val / count;

    float i = std::floor(perc * 6.0f);
    float f = perc * 6.0f - i;
    float q = 1.0f - f;

    int det = static_cast<int>(std::round(std::fmod(i, 6.0f)));

    switch (det) {
      case 0:
        r = 1.0f; g = f;    b = 0.0f;
        break;
      case 1:
        r = q;    g = 1.0f; b = 0.0f;
        break;
      case 2:
        r = 0.0f; g = 1.0f; b = f;
        break;
      case 3:
        r = 0.0f; g = q;    b = 1.0f;
        break;
      case 4:
        r = f;    g = 0.0f; b = 1.0f;
        break;
      case 5:
      default:
        r = 1.0f; g = 0.0f; b = q;
        break;
    }

    return sdl::core::engine::Color::fromRGB(r, g, b);
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
