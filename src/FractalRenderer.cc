
# include "FractalRenderer.hh"

namespace fractsim {

  FractalRenderer::FractalRenderer(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::graphic::ScrollableWidget(std::string("fractal_renderer"),
                                   parent,
                                   hint),

    m_propsLocker(),

    m_renderingOpt(RenderingOptions::createDefaultForMandelbrot()),
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

    // Protect from concurrent accesses to retrieve the options and
    // to assign the new fractal options.
    RenderingOptionsShPtr copyOfOpt;

    {
      Guard guard(m_propsLocker);

      copyOfOpt = m_renderingOpt;
      m_fractalOptions = options;
    }

    // Schedule a rendering.
    scheduleRendering(options, copyOfOpt);
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
    RenderingOptionsShPtr rdrOpt;
    FractalOptionsShPtr fractalOpt;

    utils::Vector2i motion = e.getScroll();

    {
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

      rdrOpt = m_renderingOpt;
      fractalOpt = m_fractalOptions;
    }

    // Schedule the rendering.
    scheduleRendering(fractalOpt, rdrOpt);

    return toReturn;
  }

  void
  FractalRenderer::scheduleRendering(FractalOptionsShPtr fractalOpt,
                                     RenderingOptionsShPtr renderingOpt)
  {
    // Check consistency.
    if (fractalOpt == nullptr) {
      log(
        std::string("Could not schedule rendering for fractal (cause: \"Invalid null fractal options\")"),
        utils::Level::Error
      );

      return;
    }
    if (renderingOpt == nullptr) {
      log(
        std::string("Could not schedule rendering for fractal (cause: \"Invalid null rendering options\")"),
        utils::Level::Error
      );

      return;
    }

    // TODO: Implementation.
    log("Should perform rendering with accuracy " + std::to_string(fractalOpt->getAccuracy()), utils::Level::Warning);
  }

}
