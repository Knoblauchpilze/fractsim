#ifndef    NEWTON_OPTIONS_HXX
# define   NEWTON_OPTIONS_HXX

# include "NewtonOptions.hh"

namespace fractsim {

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

}

#endif    /* NEWTON_OPTIONS_HXX */
