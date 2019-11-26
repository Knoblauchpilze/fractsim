#ifndef    MANDELBROT_OPTIONS_HXX
# define   MANDELBROT_OPTIONS_HXX

# include "MandelbrotOptions.hh"

namespace fractsim {

  inline
  unsigned
  MandelbrotOptions::getLabelDefaultFontSize() noexcept {
    return 15u;
  }

  inline
  const char*
  MandelbrotOptions::getLabelDefaultFontName() noexcept {
    return "data/fonts/Goodtime.ttf";
  }

  inline
  unsigned
  MandelbrotOptions::getValueDefaultFontSize() noexcept {
    return 35u;
  }

  inline
  const char*
  MandelbrotOptions::getValueDefaultFontName() noexcept {
    return "data/fonts/times.ttf";
  }

}

#endif    /* MANDELBROT_OPTIONS_HXX */
