#ifndef    FRACTAL_HXX
# define   FRACTAL_HXX

# include "Fractal.hh"

namespace fractsim {

  inline
  Fractal::~Fractal() {}

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

    m_canvas = canvas;

    // We need to convert the canvas to the nearest integer.
    m_dims.x() = static_cast<int>(std::round(canvas.w()));
    m_dims.y() = static_cast<int>(std::round(canvas.h()));

    // Compute the new area covered by a pixel.
    computeCellDelta();

    // Reallocate the cache.
    m_data.resize(m_dims.x() * m_dims.y());
    clear();
  }

  inline
  void
  Fractal::setRenderingArea(const utils::Boxf& area) {
    // Assign the new rendering area and fill the data with invalid value.
    m_area = area;

    // Recompute the new area covered by a single pixel.
    computeCellDelta();

    clear();
  }

  inline
  float
  Fractal::getToleranceForCells() noexcept {
    return 0.000001f;
  }

  inline
  void
  Fractal::clear() {
    std::fill(m_data.begin(), m_data.end(), 0.0f);
  }

  inline
  void
  Fractal::computeCellDelta() {
    m_cellDelta.x() = m_area.w() / m_dims.x();
    m_cellDelta.y() = m_area.h() / m_dims.y();
  }

}

#endif    /* FRACTAL_HXX */
