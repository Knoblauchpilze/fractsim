#ifndef    NEWTON_OPTIONS_HXX
# define   NEWTON_OPTIONS_HXX

# include "NewtonOptions.hh"

namespace fractsim {

  inline
  void
  NewtonOptions::resetOptions(const std::string& dummy) {
    // Check whether the options are visible.
    if (!isVisible()) {
      return;
    }

    // Reset elements.
    {
      Guard guard(m_propsLocker);
      initElements();
    }

    // Notify of the rendering area.
    validateOptions(dummy);
  }

  inline
  std::string
  NewtonOptions::getCoefficientRealPartValueName(unsigned degree) noexcept {
    return "deg_" + std::to_string(degree) + "_real_part";
  }

  inline
  std::string
  NewtonOptions::getCoefficientImgPartValueName(unsigned degree) noexcept {
    return "deg_" + std::to_string(degree) + "_img_part";
  }

  inline
  const char*
  NewtonOptions::getAccuracyValueName() noexcept {
    return "accuracy_value";
  }

  inline
  unsigned
  NewtonOptions::getDefaultAccuracy() noexcept {
    return 128u;
  }

  inline
  float
  NewtonOptions::getDefaultRealPartCoefficient(unsigned degree) noexcept {
    if (degree == 5u) {
      return 1.0f;
    }
    if (degree == 0u) {
      return -1.0f;
    }

    return 0.0f;
  }

  inline
  float
  NewtonOptions::getDefaultImgPartCoefficient(unsigned /*degree*/) noexcept {
    // The degree is not used.
    return 0.0f;
  }

}

#endif    /* NEWTON_OPTIONS_HXX */
