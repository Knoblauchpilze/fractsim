
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

    m_scheduler(std::make_shared<RenderingScheduler>())
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

    // Assign the new options.
    Guard guard(m_propsLocker);

    // Create rendering options if needed.
    if (m_renderingOpt == nullptr) {
      m_renderingOpt = std::make_shared<RenderingOptions>(
        options->getDefaultRenderingWindow(),
        sdl::core::LayoutItem::getRenderingArea().toSize()
      );
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
    scheduleRendering();
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

    // Perform the zoom in operation if needed.
    if (m_renderingOpt == nullptr) {
      log(
        std::string("Discarding zoom ") + (motion.y() > 0 ? "in" : "out") + " event, no rendering window defined",
        utils::Level::Warning
      );

      return toReturn;
    }

    float factor = motion.y() > 0 ? getDefaultZoomInFactor() : getDefaultZoomOutFactor();

    utils::Sizef thisArea = sdl::core::LayoutItem::getRenderingArea().toSize();
    utils::Vector2f mousePos = e.getMousePosition();
    utils::Vector2f conv(mousePos.x() / thisArea.w(), mousePos.y() / thisArea.h());

    utils::Boxf newArea = m_renderingOpt->zoom(conv, factor);
    m_fractalData->setRenderingArea(newArea);

    // Schedule the rendering.
    scheduleRendering();

    return toReturn;
  }

  void
  FractalRenderer::drawContentPrivate(const utils::Uuid& /*uuid*/,
                                      const utils::Boxf& /*area*/)
  {
    // TODO: Implementation.
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
  FractalRenderer::scheduleRendering() {
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

    // Cancel existing rendering operations.
    m_scheduler->cancelJobs();

    // Divide the input window into several tiles and enqueue each job into
    // the scheduler.
    utils::Boxf area = m_renderingOpt->getRenderingArea();
    utils::Sizef canvas = m_renderingOpt->getCanvasSize();

    utils::Sizef tileDims(area.w() / getHorizontalTileCount(), area.h() / getVerticalTileCount());

    utils::Sizef fTilePix(canvas.w() / getHorizontalTileCount(), canvas.h() / getVerticalTileCount());
    utils::Vector2i tilePix(static_cast<int>(std::ceil(fTilePix.w())), static_cast<int>(std::ceil(fTilePix.h())));

    std::vector<RenderingTileShPtr> tiles;
    for (unsigned y = 0u ; y < getVerticalTileCount() ; ++y) {
      for (unsigned x = 0u ; x < getHorizontalTileCount() ; ++x) {
        tiles.push_back(
          std::make_shared<RenderingTile>(
            utils::Boxf(
              area.getLeftBound() + 1.0f * x * tileDims.w() + tileDims.w() / 2.0f,
              area.getTopBound() - 1.0f * y * tileDims.h() - tileDims.h() / 2.0f,
              tileDims
            ),
            tilePix,
            m_fractalOptions,
            m_fractalData
          )
        );
      }
    }

    m_scheduler->enqueueJobs(tiles);

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
      e->addUpdateRegion(mapToGlobal(local));
    }

    postEvent(e);
  }

}
