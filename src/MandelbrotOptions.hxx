#ifndef    MANDELBROT_OPTIONS_HXX
# define   MANDELBROT_OPTIONS_HXX

# include "MandelbrotOptions.hh"

namespace fractsim {

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

}

#endif    /* MANDELBROT_OPTIONS_HXX */
