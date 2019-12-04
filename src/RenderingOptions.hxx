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
  void
  RenderingOptions::reset() {
    m_area = m_initial;
  }

  inline
  RenderingOptionsShPtr
  RenderingOptions::createDefaultForMandelbrot() noexcept {
    return std::make_shared<RenderingOptions>(utils::Boxf(-0.7f, 0.0f, 2.6f, 2.0f));
  }

  inline
  RenderingOptionsShPtr
  RenderingOptions::createDefaultForJulia() noexcept {
    return std::make_shared<RenderingOptions>(utils::Boxf(0.0f, 0.0f, 2.0f, 2.0f));
  }

  inline
  RenderingOptionsShPtr
  RenderingOptions::createDefaultForNewton() noexcept {
    return std::make_shared<RenderingOptions>(utils::Boxf(0.0f, 0.0f, 4.0f, 3.0f));
  }

}

#endif    /* RENDERING_OPTIONS_HXX */
