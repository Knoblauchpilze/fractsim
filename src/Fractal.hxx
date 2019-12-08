#ifndef    FRACTAL_HXX
# define   FRACTAL_HXX

# include "Fractal.hh"

namespace fractsim {

  inline
  void
  Fractal::assignValueForCoord(const utils::Vector2f& p,
                               float confidence)
  {
    // Clamp the confidence to a valid range.
    float clamped = std::min(1.0f, std::max(0.0f, confidence));

    if (clamped > 0.0f) {
      log("Should set " + p.toString() + " with confidence " + std::to_string(clamped), utils::Level::Warning);
    }

    // TODO: Check computation, we appear to never reach a value different from 0.
    // TODO: Handle saving.
  }

}

#endif    /* FRACTAL_HXX */
