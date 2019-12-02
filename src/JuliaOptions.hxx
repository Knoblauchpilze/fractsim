#ifndef    JULIA_OPTIONS_HXX
# define   JULIA_OPTIONS_HXX

# include "JuliaOptions.hh"

namespace fractsim {

  inline
  const char*
  JuliaOptions::getConstantRealPartValueName() noexcept {
    return "real_part_value";
  }

  inline
  const char*
  JuliaOptions::getConstantImgPartValueName() noexcept {
    return "img_part_value";
  }

  inline
  const char*
  JuliaOptions::getAccuracyValueName() noexcept {
    return "accuracy_value";
  }

}

#endif    /* JULIA_OPTIONS_HXX */
