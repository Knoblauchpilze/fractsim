
# include "FractalRenderer.hh"

namespace fractsim {

  FractalRenderer::FractalRenderer(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::graphic::ScrollableWidget(std::string("fractal_renderer"),
                                   parent,
                                   hint),

    m_propsLocker()
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

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // TODO: Implementation.
    log("Should perform rendering with accuracy " + std::to_string(options->getAccuracy()), utils::Level::Warning);
  }

}
