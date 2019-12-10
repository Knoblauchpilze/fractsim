#ifndef    FRACTAL_RENDERER_HXX
# define   FRACTAL_RENDERER_HXX

# include "FractalRenderer.hh"

namespace fractsim {

  inline
  FractalRenderer::~FractalRenderer() {
    // Protect from concurrent accesses
    Guard guard (m_propsLocker);

    clearTiles();
  }

  inline
  void
  FractalRenderer::updatePrivate(const utils::Boxf& window) {
    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // Use the base handler.
    sdl::core::SdlWidget::updatePrivate(window);

    // Update the rendering options if needed.
    if (m_renderingOpt != nullptr) {
      m_renderingOpt->setCanvasSize(window.toSize());
      m_fractalData->resize(window.toSize());

      scheduleRendering();
    }
  }

  inline
  bool
  FractalRenderer::keyPressEvent(const sdl::core::engine::KeyEvent& e) {
    // Check whether the key corresponds to the reset key.
    if (e.getRawKey() == sdl::core::engine::RawKey::R) {
      // Protect from concurrent accesses.
      Guard guard(m_propsLocker);

      // Reset the options to the initial viewing window.
      if (m_renderingOpt != nullptr) {
        utils::Boxf area = m_renderingOpt->reset();
        m_fractalData->setRenderingArea(area);

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
    // The `area` represents a real world coordinate of the area. We need to
    // use the rendering options if any to determine the screen area which
    // corresponds to the same area.
    if (m_renderingOpt == nullptr) {
      return utils::Boxf();
    }

    utils::Boxf total = m_renderingOpt->getRenderingArea();
    utils::Sizef canvas = m_renderingOpt->getCanvasSize();

    float dToLeft = (area.x() - total.x()) / total.w();
    float dToBottom = (area.y() - total.y()) / total.h();

    utils::Vector2f areaToResize(canvas.w() / total.w(), canvas.h() / total.h());

    return utils::Boxf(
      -canvas.w() / 2.0f + dToLeft * canvas.w(),
      -canvas.h() / 2.0f + dToBottom * canvas.h(),
      area.w() * areaToResize.x(),
      area.h() * areaToResize.y()
    );
  }

  inline
  void
  FractalRenderer::clearTiles() {
    if (m_tex.valid()) {
      getEngine().destroyTexture(m_tex);
      m_tex.invalidate();
    }
  }

  inline
  bool
  FractalRenderer::tilesChanged() const noexcept {
    return m_tilesRendered;
  }

  void
  FractalRenderer::setTilesChanged() noexcept {
    m_tilesRendered = true;
  }

  inline
  void
  FractalRenderer::loadTiles() {
    // Clear any existing texture representing the tiles.
    clearTiles();

    // Check whether the settings for the fractal are already
    // assigned: if this is not the case there's nothing to
    // create and nothing to display.
    if (m_fractalData == nullptr || m_fractalOptions == nullptr) {
      return;
    }

    // Create the brush representing the tile using the palette
    // provided by the user.
    sdl::core::engine::BrushShPtr brush = m_fractalData->createBrush(
      m_fractalOptions->getPalette()
    );

    // check consistency.
    if (brush == nullptr) {
      error(
        std::string("Could not create texture to represent fractal"),
        std::string("Failed to create brush data")
      );
    }

    // Use the brush to create a texture.
    m_tex = getEngine().createTextureFromBrush(brush);

    if (!m_tex.valid()) {
      error(
        std::string("Could not create texture to represent fractal"),
        std::string("Failed to transform brush into texture")
      );
    }
  }

}

#endif    /* FRACTAL_RENDERER_HXX */
