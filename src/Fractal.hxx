#ifndef    FRACTAL_HXX
# define   FRACTAL_HXX

# include "Fractal.hh"

namespace fractsim {

  inline
  utils::Sizef
  Fractal::getPixelSize() const noexcept {
    const std::lock_guard guard(m_propsLocker);

    return getPixelSizePrivate();
  }

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
    const std::lock_guard guard(m_propsLocker);

    m_canvas = canvas;
    m_tiles.clear();
  }

  inline
  void
  Fractal::realWorldResize(const utils::Boxf& area,
                           bool zoom)
  {
    // Check consistency.
    if (!area.valid()) {
      error(
        std::string("Could not assign new rendering area to fractal"),
        std::string("Invalid input size ") + area.toString()
      );
    }

    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    m_area = area;
    // We want to perform a clean of the cache in case the zoom
    // parameter indicates it.
    if (zoom) {
      m_tiles.clear();
    }
  }

  inline
  void
  Fractal::registerDataTile(float zoom,
                            RenderingTileShPtr tile)
  {
    // Prevent invalid tiles.
    if (tile == nullptr) {
      error(
        std::string("Could not register data tile for fractal"),
        std::string("Invalid null tile")
      );
    }

    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Check whether the zoom level is still consistent with what we
    // are keeping internally. If this is the case the tile can be
    // appended to the cache, otherwise we need to reset it.
    if (!utils::fuzzyEqual(m_zoomLevel, zoom, getZoomComparisonThreshold())) {
      m_tiles.clear();

      m_zoomLevel = zoom;
    }

    m_tiles.push_back(tile);
  }

  inline
  float
  Fractal::getZoomComparisonThreshold() noexcept {
    return 0.000001f;
  }

  inline
  unsigned
  Fractal::getHorizontalTileCount() noexcept {
    return 4u;
  }

  inline
  unsigned
  Fractal::getVerticalTileCount() noexcept {
    return 4u;
  }

  inline
  utils::Sizef
  Fractal::getPixelSizePrivate() const noexcept {
    return utils::Sizef(m_area.w() / m_canvas.w(), m_area.h() / m_canvas.h());
  }

}

#endif    /* FRACTAL_HXX */
