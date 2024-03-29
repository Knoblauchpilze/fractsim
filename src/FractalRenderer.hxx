#ifndef    FRACTAL_RENDERER_HXX
# define   FRACTAL_RENDERER_HXX

# include "FractalRenderer.hh"

namespace fractsim {

  inline
  FractalRenderer::~FractalRenderer() {
    // Protect from concurrent accesses
    const std::lock_guard guard(m_propsLocker);

    clearTiles();
  }

  inline
  void
  FractalRenderer::updatePrivate(const utils::Boxf& window) {
    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Use the base handler.
    sdl::core::SdlWidget::updatePrivate(window);

    // Update the rendering options if needed.
    if (m_renderingOpt != nullptr) {
      m_renderingOpt->setCanvasSize(window.toSize());
      m_fractalData->resize(window.toSize());

      scheduleRendering(true);
    }
  }

  inline
  bool
  FractalRenderer::keyPressEvent(const sdl::core::engine::KeyEvent& e) {
    // Check whether the key corresponds to the reset key.
    if (e.getRawKey() == getDefaultResetKey()) {
      // Protect from concurrent accesses.
      const std::lock_guard guard(m_propsLocker);

      // Reset the options to the initial viewing window.
      if (m_renderingOpt != nullptr) {
        utils::Boxf area = m_renderingOpt->reset();
        m_fractalData->realWorldResize(area, false);

        // Schedule a rendering.
        scheduleRendering(true);

        // Notify listeners.
        onRenderingAreaChanged.safeEmit(
          std::string("onRenderingAreaChanged(") + area.toString() + ")",
          area
        );
      }
    }

    // Check for arrow keys.
    bool move = false;
    utils::Vector2f motion;
    float delta = getArrowKeyMotion();

    if (e.getRawKey() == sdl::core::engine::RawKey::Left) {
      move = true;
      motion.x() -= delta;
    }
    if (e.getRawKey() == sdl::core::engine::RawKey::Right) {
      move = true;
      motion.x() += delta;
    }
    if (e.getRawKey() == sdl::core::engine::RawKey::Down) {
      move = true;
      motion.y() -= delta;
    }
    if (e.getRawKey() == sdl::core::engine::RawKey::Up) {
      move = true;
      motion.y() += delta;
    }

    // Schedule a scrolling if some motion has been detected.
    if (move) {
      utils::Vector2f center, newCenter;

      {
        const std::lock_guard guard(m_propsLocker);

        center = m_renderingOpt->getRenderingArea().getCenter();
        newCenter = center + motion;
      }

      if (handleContentScrolling(center, newCenter, motion, false)) {
        requestRepaint();
      }
    }

    // Use the base handler to provide a return value.
    return sdl::graphic::ScrollableWidget::keyPressEvent(e);
  }

  inline
  bool
  FractalRenderer::mouseMoveEvent(const sdl::core::engine::MouseEvent& e) {
    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Convert the position to internal coordinates.
    utils::Vector2f conv = convertGlobalToRealWorld(e.getMousePosition());

    // Notify external listeners.
    onCoordChanged.safeEmit(
      std::string("onCoordChanged(") + conv.toString() + ")",
      conv
    );

    // Use the base handler to provide a return value.
    return sdl::graphic::ScrollableWidget::mouseMoveEvent(e);
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
  float
  FractalRenderer::getArrowKeyMotion() noexcept {
    return 15.0f;
  }

  inline
  sdl::core::engine::RawKey
  FractalRenderer::getDefaultResetKey() noexcept {
    return sdl::core::engine::RawKey::R;
  }

  inline
  float
  FractalRenderer::getExpansionThreshold() noexcept {
    return 1.0f;
  }

  inline
  unsigned
  FractalRenderer::getWorkerThreadCount() noexcept {
    return 4u;
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

    float dToLeft = (area.x() - total.getLeftBound()) / total.w();
    float dToBottom = (area.y() - total.getBottomBound()) / total.h();

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

    // Check consistency.
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

  inline
  utils::Vector2f
  FractalRenderer::convertGlobalToRealWorld(const utils::Vector2f& global) {
    // Check if a rendering window is assigned.
    if (m_renderingOpt == nullptr) {
      // Return default coordinates.
      return utils::Vector2f();
    }

    // Map the position to local coordinate frame.
    utils::Vector2f pos = mapFromGlobal(global);

    // Convert into some sort of percentage of the window.
    utils::Sizef thisArea = sdl::core::LayoutItem::getRenderingArea().toSize();

    utils::Vector2f windowPerc(pos.x() / thisArea.w(), pos.y() / thisArea.h());

    // Convert using the dedicated handler.
    utils::Vector2f conv = m_renderingOpt->getPointAt(windowPerc);

    // This is the corresponding real world position.
    return conv;
  }

  inline
  utils::Boxf
  FractalRenderer::expandByOne(const utils::Boxf& area) const noexcept {
    // Retrieve the bounds not to exceed.
    utils::Boxf bounds = LayoutItem::getRenderingArea().toOrigin();

    // Increase the size of the input area by the corresponding threshold.
    utils::Boxf expanded(
      area.getCenter(),
      area.w() + getExpansionThreshold(),
      area.h() + getExpansionThreshold()
    );

    // Do not exceed the bounds and return the area.
    return expanded.intersect(bounds);
  }

}

#endif    /* FRACTAL_RENDERER_HXX */
