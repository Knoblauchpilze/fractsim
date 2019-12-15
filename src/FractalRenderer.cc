
# include "FractalRenderer.hh"
# include <sdl_engine/PaintEvent.hh>

namespace fractsim {

  FractalRenderer::FractalRenderer(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::graphic::ScrollableWidget(std::string("fractal_renderer"),
                                   parent,
                                   hint),

    m_propsLocker(),

    m_renderingOpt(nullptr),
    m_fractalOptions(nullptr),
    m_fractalData(nullptr),

    m_scheduler(std::make_shared<RenderingScheduler>()),
    m_taskProgress(0u),
    m_taskTotal(1u),

    m_tex(),
    m_tilesRendered(true),

    onZoomChanged(),
    onCoordChanged(),
    onRenderingAreaChanged(),
    onTileCompleted()
  {
    setService(std::string("fractal_renderer"));

    build();
  }

  void
  FractalRenderer::requestRendering(FractalOptionsShPtr options) {
    // Check consistency.
    if (options == nullptr) {
      log(
        "Trying to perform rendering with invalid null options, discarding request",
        utils::Level::Warning
      );

      return;
    }

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // Create rendering options if needed.
    if (m_renderingOpt == nullptr) {
      m_renderingOpt = std::make_shared<RenderingOptions>(
        options->getDefaultRenderingWindow(),
        sdl::core::LayoutItem::getRenderingArea().toSize()
      );
    }
    else {
      m_renderingOpt->setRenderingArea(options->getDefaultRenderingWindow(), true);
      m_fractalData->realWorldResize(m_renderingOpt->getRenderingArea(), true);
    }

    // Create fractal data if needed.
    if (m_fractalData == nullptr) {
      m_fractalData = std::make_shared<Fractal>(
        m_renderingOpt->getCanvasSize(),
        m_renderingOpt->getRenderingArea()
      );
    }

    m_fractalOptions = options;

    // Schedule a rendering.
    scheduleRendering(true);

    // Notify listeners of the new area.
    onRenderingAreaChanged.safeEmit(
      std::string("onRenderingAreaChanged(") + m_renderingOpt->getRenderingArea().toString() + ")",
      m_renderingOpt->getRenderingArea()
    );
  }

  bool
  FractalRenderer::handleContentScrolling(const utils::Vector2f& /*posToFix*/,
                                          const utils::Vector2f& /*whereTo*/,
                                          const utils::Vector2f& motion,
                                          bool /*notify*/)
  {
    // We want to apply a motion of `motion` in local coordinate frame to the
    // underlying support area. In order to do that we need to convert the
    // motion into a real world coordinate frame.
    Guard guard(m_propsLocker);

    // Note: we need to invert the motion's direction for some reasons.
    utils::Sizef pixSize = m_fractalData->getPixelSize();
    utils::Vector2f realWorldMotion(
      -motion.x() * pixSize.w(),
      -motion.y() * pixSize.h()
    );

    // Compute the new rendering area by offseting the old one with the motion.
    utils::Boxf area = m_renderingOpt->getRenderingArea();
    utils::Boxf newArea(
      area.x() + realWorldMotion.x(),
      area.y() + realWorldMotion.y(),
      area.toSize()
    );

    log("Moving from " + area.toString() + " to " + newArea.toString() + " (motion: " + motion.toString() + ", real: " + realWorldMotion.toString() + ")", utils::Level::Verbose);

    // Update the rendering area.
    m_renderingOpt->setRenderingArea(newArea, false);
    m_fractalData->realWorldResize(newArea, false);

    // Schedule a rendering.
    scheduleRendering(false);

    // Trigger new signals to notify listeners.
    onRenderingAreaChanged.safeEmit(
      std::string("onRenderingAreaChanged(") + newArea.toString() + ")",
      newArea
    );

    // Notify the caller that we changed the area.
    return true;
  }

  bool
  FractalRenderer::mouseWheelEvent(const sdl::core::engine::MouseEvent& e) {
    // We want to trigger zooming operations only when the mouse is inside
    // this widget.
    bool toReturn = sdl::graphic::ScrollableWidget::mouseWheelEvent(e);

    if (!isMouseInside()) {
      return toReturn;
    }

    // Protect from concurrent accesses to perform the zoom operation and
    // also schedule the rendering.
    utils::Vector2i motion = e.getScroll();

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // Perform the zoom in operation if needed.
    if (m_renderingOpt == nullptr) {
      log(
        std::string("Discarding zoom ") + (motion.y() > 0 ? "in" : "out") + " event, no rendering window defined",
        utils::Level::Warning
      );

      return toReturn;
    }

    float factor = motion.y() > 0 ? getDefaultZoomInFactor() : getDefaultZoomOutFactor();

    utils::Vector2f conv = convertLocalToRealWorld(e.getMousePosition());

    utils::Boxf newArea = m_renderingOpt->zoom(conv, factor);
    m_fractalData->realWorldResize(newArea, true);

    // Schedule the rendering.
    scheduleRendering(true);

    // Trigger new signals to notify listeners.
    onZoomChanged.safeEmit(
      std::string("onZoomChanged(") + m_renderingOpt->getZoom().toString() + ")",
      m_renderingOpt->getZoom()
    );
    onRenderingAreaChanged.safeEmit(
      std::string("onRenderingAreaChanged(") + newArea.toString() + ")",
      newArea
    );

    return toReturn;
  }

  void
  FractalRenderer::drawContentPrivate(const utils::Uuid& uuid,
                                      const utils::Boxf& area)
  {
    // Acquire the lock on the attributes of this widget.
    Guard guard(m_propsLocker);

    // Load the tiles: this should happen only if some tiles have been
    // rendered since the last draw operation. This status is kept by
    // the `m_tilesRendered` boolean. Generally this indicates that a
    // compute operation is running and that some more tiles have been
    // received.
    if (tilesChanged()) {
      // Load the text.
      loadTiles();

      // The tiles have been updated.
      m_tilesRendered = false;
    }

    // Check whether there's something to display.
    if (!m_tex.valid()) {
      return;
    }

    // Convert the area to local so that we blit only the right area of
    // the texture representing the fractal.
    utils::Boxf thisArea = LayoutItem::getRenderingArea().toOrigin();
    utils::Sizef sizeEnv = getEngine().queryTexture(uuid);
    utils::Sizef texSize = getEngine().queryTexture(m_tex);

    utils::Boxf srcArea = thisArea.intersect(area);
    utils::Boxf dstArea = thisArea.intersect(area);

    utils::Boxf srcEngine = convertToEngineFormat(srcArea, texSize);
    utils::Boxf dstEngine = convertToEngineFormat(dstArea, sizeEnv);

    getEngine().drawTexture(m_tex, &srcEngine, &uuid, &dstEngine);
  }

  void
  FractalRenderer::build() {
    // Connect the results provider signal of the thread pool to the local slot.
    m_scheduler->onTilesRendered.connect_member<FractalRenderer>(
      this,
      &FractalRenderer::handleTilesComputed
    );
  }

  void
  FractalRenderer::scheduleRendering(bool invalidate) {
    // Check consistency.
    if (m_fractalOptions == nullptr) {
      log(
        std::string("Could not schedule rendering for fractal (cause: \"Invalid null fractal options\")"),
        utils::Level::Error
      );

      return;
    }
    if (m_renderingOpt == nullptr) {
      log(
        std::string("Could not schedule rendering for fractal (cause: \"Invalid null rendering options\")"),
        utils::Level::Error
      );

      return;
    }

    // Cancel existing rendering operations if needed.
    if (invalidate) {
      m_scheduler->cancelJobs();
    }

    // Generate the launch schedule.
    std::vector<RenderingTileShPtr> tiles = m_fractalData->generateRenderingTiles(m_fractalOptions);

    // Return early if nothing needs to be scheduled. We still want to trigger a repaint
    // though so we need to mark the tiles as dirty.
    if (tiles.empty()) {
      setTilesChanged();

      return;
    }

    m_scheduler->enqueueJobs(tiles, invalidate);

    // Notify listeners that the progression is no `0`.
    m_taskProgress = 0u;
    m_taskTotal = tiles.size();

    onTileCompleted.safeEmit(
      std::string("onTileCompleted(0.0)"),
      0.0f
    );

    // Start the computing.
    m_scheduler->notifyRenderingJobs();
  }

  void
  FractalRenderer::handleTilesComputed(const std::vector<RenderingTileShPtr>& tiles) {
    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // Post a repaint event for each area that has been rendered.
    sdl::core::engine::PaintEventShPtr e = std::make_shared<sdl::core::engine::PaintEvent>();

    for (unsigned id = 0u ; id < tiles.size() ; ++id) {
      utils::Boxf local = convertFractalAreaToLocal(tiles[id]->getArea());

      // Expand the bounding box in order to prevent weird artifacts when the area is not
      // exactly matching the previous one.
      local = expandByOne(local);

      if (local.valid()) {
        e->addUpdateRegion(mapToGlobal(local));
      }

      // Also register this tile to the local fractal proxy.
      m_fractalData->registerDataTile(m_renderingOpt->getMeanZoom(), tiles[id]);
    }

    postEvent(e);

    // The tiles need to be rendered again.
    setTilesChanged();

    // Some more tiles have been processed.
    m_taskProgress += tiles.size();

    float perc = 1.0f * m_taskProgress / m_taskTotal;

    onTileCompleted.safeEmit(
      std::string("onTileCompleted(") + std::to_string(perc) + ")",
      perc
    );
  }

}
