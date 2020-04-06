
/**
 * @brief - Reimplementation of a program started in 04/2010 as a
 *          training and a tool to visualize fractals of various
 *          types. This is a good first test scenario for the new
 *          engine framework as it is quite a complete application.
 *          Implemented from 25/11/2019 - 17/12/2019.
 */

# include <core_utils/StdLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <sdl_app_core/SdlApplication.hh>
# include <core_utils/CoreException.hh>
# include <sdl_core/SdlWidget.hh>
# include "FractalRenderer.hh"
# include "MandelbrotOptions.hh"
# include "JuliaOptions.hh"
# include "NewtonOptions.hh"
# include "RenderingStatus.hh"
# include "StatusBar.hh"

// TODO: At high zoom level the precision of the float is not enough. We could go
// with `double` but it would just temporarily hide the problem and not solve it.
// What would be cool is to introduce an arbitrary precision library like GMP.
// See here: https://gmplib.org/manual/Initializing-Floats.html#Initializing-Floats
// The `CMake` is already updated, and there's some sample code in the `GMPBox` and
// `GMPVector2` class.
// What is left to do is to use these types of values for most of the places where
// the `utils::Vector2f` and `utils::Boxf` classes are used. This will require some
// effort are there are a lot of manipulations that are done with these objects.
// Also we have to find a way to pass around the `mpf_t` elements (for method like
// the `x` in `GMPVector2` or `w` in `GMPBox`): should we pass them by pointer or
// reference or whatever ? Probably not pass them at all would be cool but it might
// require a lot of efforts.
// This would truly solve the problems of precision though, even if it might be at
// the cost of performance.

int main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger logger;
  utils::LoggerLocator::provide(&logger);

  const std::string service("fractsim");
  const std::string module("main");

  // Create the application window parameters.
  const std::string appName = std::string("fractsim");
  const std::string appTitle = std::string("The best way to get wallpapers (c)");
  const std::string appIcon = std::string("data/img/icon.bmp");
  const utils::Sizei size(800, 600);

  sdl::app::SdlApplicationShPtr app = nullptr;

  try {
    app = std::make_shared<sdl::app::SdlApplication>(
      appName,
      appTitle,
      appIcon,
      size,
      true,
      utils::Sizef(0.4f, 0.6f),
      50.0f,
      60.0f
    );

    // Create the layout of the window: the main tab is a scrollable widget
    // allowing the display of the fractal. The right dock widget allows to
    // control the computation parameters of the fractal.
    fractsim::FractalRenderer* renderer = new fractsim::FractalRenderer();
    app->setCentralWidget(renderer);

    fractsim::MandelbrotOptions* mandelOpt = new fractsim::MandelbrotOptions();
    app->addDockWidget(mandelOpt, sdl::app::DockWidgetArea::RightArea, std::string("Mandelbrot"));

    fractsim::JuliaOptions* juliaOpt = new fractsim::JuliaOptions();
    app->addDockWidget(juliaOpt, sdl::app::DockWidgetArea::RightArea, std::string("Julia"));

    fractsim::NewtonOptions* newtonOpt = new fractsim::NewtonOptions(5u);
    app->addDockWidget(newtonOpt, sdl::app::DockWidgetArea::RightArea, std::string("Newton"));

    fractsim::RenderingStatus* status = new fractsim::RenderingStatus();
    app->addDockWidget(status, sdl::app::DockWidgetArea::TopArea);

    fractsim::StatusBar* bar = new fractsim::StatusBar();
    app->setStatusBar(bar);

    // Connect the options changed signal to the request rendering slot.
    mandelOpt->onOptionsChanged.connect_member<fractsim::FractalRenderer>(
      renderer,
      &fractsim::FractalRenderer::requestRendering
    );
    juliaOpt->onOptionsChanged.connect_member<fractsim::FractalRenderer>(
      renderer,
      &fractsim::FractalRenderer::requestRendering
    );
    newtonOpt->onOptionsChanged.connect_member<fractsim::FractalRenderer>(
      renderer,
      &fractsim::FractalRenderer::requestRendering
    );

    // Connect the render button to the options panel slots.
    status->getRenderButton().onClick.connect_member<fractsim::MandelbrotOptions>(
      mandelOpt,
      &fractsim::MandelbrotOptions::validateOptions
    );
    status->getRenderButton().onClick.connect_member<fractsim::JuliaOptions>(
      juliaOpt,
      &fractsim::JuliaOptions::validateOptions
    );
    status->getRenderButton().onClick.connect_member<fractsim::NewtonOptions>(
      newtonOpt,
      &fractsim::NewtonOptions::validateOptions
    );

    status->getResetButton().onClick.connect_member<fractsim::MandelbrotOptions>(
      mandelOpt,
      &fractsim::MandelbrotOptions::resetOptions
    );
    status->getResetButton().onClick.connect_member<fractsim::JuliaOptions>(
      juliaOpt,
      &fractsim::JuliaOptions::resetOptions
    );
    status->getResetButton().onClick.connect_member<fractsim::NewtonOptions>(
      newtonOpt,
      &fractsim::NewtonOptions::resetOptions
    );

    renderer->onTileCompleted.connect_member<fractsim::RenderingStatus>(
      status,
      &fractsim::RenderingStatus::onCompletionChanged
    );

    renderer->onCoordChanged.connect_member<fractsim::StatusBar>(
      bar,
      &fractsim::StatusBar::onMouseCoordsChanged
    );
    renderer->onZoomChanged.connect_member<fractsim::StatusBar>(
      bar,
      &fractsim::StatusBar::onZoomChanged
    );
    renderer->onRenderingAreaChanged.connect_member<fractsim::StatusBar>(
      bar,
      &fractsim::StatusBar::onRenderingAreaChanged
    );

    // Run it.
    app->run();
  }
  catch (const utils::CoreException& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught internal exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (const std::exception& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (...) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Unexpected error while setting up application"),
      module,
      service
    );
  }

  app.reset();

  // All is good.
  return EXIT_SUCCESS;
}
