#ifndef    JULIA_OPTIONS_HXX
# define   JULIA_OPTIONS_HXX

# include "JuliaOptions.hh"

namespace fractsim {

  inline
  void
  JuliaOptions::resetOptions(const std::string& dummy) {
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
