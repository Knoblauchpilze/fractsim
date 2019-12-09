#ifndef    FRACTAL_RENDERER_HXX
# define   FRACTAL_RENDERER_HXX

# include "FractalRenderer.hh"

namespace fractsim {

  inline
  bool
  FractalRenderer::handleEvent(sdl::core::engine::EventShPtr e) {
    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // Use the base class handler.
    // TODO: Maybe we should reimplement `processEvents`.
    return sdl::core::SdlWidget::handleEvent(e);
  }

  inline
  void
  FractalRenderer::updatePrivate(const utils::Boxf& window) {
    // Use the base handler.
    sdl::core::SdlWidget::updatePrivate(window);

    // Update the rendering options if needed.
    if (m_renderingOpt != nullptr) {
      m_renderingOpt->setCanvasSize(window.toSize());

      scheduleRendering();
    }
  }

  inline
  bool
  FractalRenderer::keyPressEvent(const sdl::core::engine::KeyEvent& e) {
    // Check whether the key corresponds to the reset key.
    if (e.getRawKey() == sdl::core::engine::RawKey::R) {
      // Reset the options to the initial viewing window.
      if (m_renderingOpt != nullptr) {
        m_renderingOpt->reset();

        // Schedule a rendering.
        scheduleRendering();
      }
    }

    // Use the base handler to provide a return value.
    return sdl::graphic::ScrollableWidget::keyPressEvent(e);
  }

  inline
  float
  FractalRenderer::getDefaultZoomInFactor() noexcept {
    return 2.0f;
  }

  inline
  float
  FractalRenderer::getDefaultZoomOutFactor() noexcept {
    // Inverse of the default zoom in factor.
    return 1.0f / getDefaultZoomInFactor();
  }

  inline
  unsigned
  FractalRenderer::getHorizontalTileCount() noexcept {
    return 5u;
  }

  inline
  unsigned
  FractalRenderer::getVerticalTileCount() noexcept {
    return 3u;
  }

  inline
  sdl::core::engine::RawKey
  FractalRenderer::getDefaultResetKey() noexcept {
    return sdl::core::engine::RawKey::R;
  }

  inline
  utils::Boxf
  FractalRenderer::convertFractalAreaToLocal(const utils::Boxf& area) const {
    // TODO: Implementation.
    return area;
  }

}

#endif    /* FRACTAL_RENDERER_HXX */
