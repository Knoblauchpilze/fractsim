#ifndef    FRACTAL_HXX
# define   FRACTAL_HXX

# include "Fractal.hh"

namespace fractsim {

  inline
  void
  Fractal::resize(const utils::Sizef& canvas) {
    // Check consistency.
    if (!canvas.valid()) {
      error(
        std::string("Could not assign new canvas size to fractal"),
        std::string("Invalid input size ") + canvas.toString()
      );
    }

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    m_canvas = canvas;
    m_tiles.clear();
  }

  inline
  void
  Fractal::realWorldResize(const utils::Boxf& area) {
    // Check consistency.
    if (!area.valid()) {
      error(
        std::string("Could not assign new rendering area to fractal"),
        std::string("Invalid input size ") + area.toString()
      );
    }

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    m_area = area;
    m_tiles.clear();
  }

  inline
  void
  Fractal::registerDataTile(RenderingTileShPtr tile) {
    // Prevent invalid tiles.
    if (tile == nullptr) {
      error(
        std::string("Could not register data tile for fractal"),
        std::string("Invalid null tile")
      );
    }

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    m_tiles.push_back(tile);
  }

  inline
  float
  Fractal::getToleranceForCells() noexcept {
    return 0.000001f;
  }

}

#endif    /* FRACTAL_HXX */
