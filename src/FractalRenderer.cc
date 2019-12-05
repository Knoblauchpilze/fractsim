
# include "FractalRenderer.hh"

namespace fractsim {

  FractalRenderer::FractalRenderer(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::graphic::ScrollableWidget(std::string("fractal_renderer"),
                                   parent,
                                   hint),

    m_propsLocker(),

    m_renderingOpt(nullptr),
    m_fractalOptions(nullptr)
  {
    setService(std::string("fractal_renderer"));
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

    utils::Sizef thisArea = sdl::core::LayoutItem::getRenderingArea().toSize();
    utils::Vector2f mousePos = e.getMousePosition();
    utils::Vector2f conv(mousePos.x() / thisArea.w(), mousePos.y() / thisArea.h());

    m_renderingOpt->zoom(conv, factor);

    // Schedule the rendering.
    scheduleRendering();

    return toReturn;
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

    // TODO: Implementation.
    log("Should perform rendering with accuracy " + std::to_string(m_fractalOptions->getAccuracy()), utils::Level::Warning);
  }

}
