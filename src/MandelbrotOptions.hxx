#ifndef    MANDELBROT_OPTIONS_HXX
# define   MANDELBROT_OPTIONS_HXX

# include "MandelbrotOptions.hh"

namespace fractsim {

  inline
  void
  MandelbrotOptions::resetOptions(const std::string& dummy) {
    // Check whether the options are visible.
    if (!isVisible()) {
      return;
    }

    // Reset elements.
    {
      const std::lock_guard guard(m_propsLocker);
      initElements();
    }

    // Notify of the rendering area.
    validateOptions(dummy);
  }

  inline
  const char*
  MandelbrotOptions::getPowerValueName() noexcept {
    return "power_value";
  }

  inline
  const char*
  MandelbrotOptions::getAccuracyValueName() noexcept {
    return "accuracy_value";
  }

  inline
  unsigned
  MandelbrotOptions::getDefaultAccuracy() noexcept {
    return 128u;
  }

  inline
  float
  MandelbrotOptions::getDefaultPower() noexcept {
    return 2.0f;
  }

}

#endif    /* MANDELBROT_OPTIONS_HXX */
