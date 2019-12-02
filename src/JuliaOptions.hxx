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

  inline
  unsigned
  JuliaOptions::getDefaultAccuracy() noexcept {
    return 128u;
  }

  inline
  float
  JuliaOptions::getDefaultRealPartConstant() noexcept {
    return 0.3f;
  }

  inline
  float
  JuliaOptions::getDefaultImgPartConstant() noexcept {
    return 0.5f;
  }

}

#endif    /* JULIA_OPTIONS_HXX */
