#ifndef    FRACTAL_RENDERER_HXX
# define   FRACTAL_RENDERER_HXX

# include "FractalRenderer.hh"

namespace fractsim {

  inline
  void
  FractalRenderer::updatePrivate(const utils::Boxf& window) {
    // Use the base handler.
    sdl::core::SdlWidget::updatePrivate(window);

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

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
      Guard guard(m_propsLocker);

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
  sdl::core::engine::RawKey
  FractalRenderer::getDefaultResetKey() noexcept {
    return sdl::core::engine::RawKey::R;
  }

}

#endif    /* FRACTAL_RENDERER_HXX */
