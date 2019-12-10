#ifndef    RENDERING_OPTIONS_HXX
# define   RENDERING_OPTIONS_HXX

# include "RenderingOptions.hh"

namespace fractsim {

  inline
  utils::Boxf
  RenderingOptions::getRenderingArea() const noexcept {
    return m_area;
  }

  inline
  void
  RenderingOptions::setRenderingArea(const utils::Boxf& area) {
    // Check consistency.
    if (!area.valid()) {
      error(
        std::string("Could not set rendering area for options"),
        std::string("Invalid area ") + area.toString()
      );
    }

    // Update both the current and initial area.
    m_initial = area;
    reset();
  }

  inline
  utils::Vector2f
  RenderingOptions::getZoom() const noexcept {
    return utils::Vector2f(m_initial.w() / m_area.w(), m_initial.h() / m_area.h());
  }

  inline
  utils::Boxf
  RenderingOptions::reset() {
    m_area = m_initial;

    return m_area;
  }

  inline
  utils::Sizef
  RenderingOptions::getCanvasSize() const noexcept {
    return m_canvas;
  }

  inline
  void
  RenderingOptions::setCanvasSize(const utils::Sizef& canvas) {
    // Check consistency.
    if (!canvas.valid()) {
      error(
        std::string("Could not set canvas size for options"),
        std::string("Invalid size ") + canvas.toString()
      );
    }

    m_canvas = canvas;
  }

  inline
  utils::Vector2f
  RenderingOptions::getPointAt(const utils::Vector2f& p) const noexcept {
    return utils::Vector2f(
      m_area.x() + p.x() * m_area.w(),
      m_area.y() + p.y() * m_area.h()
    );
  }

}

#endif    /* RENDERING_OPTIONS_HXX */
