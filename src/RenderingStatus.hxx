#ifndef    RENDERING_STATUS_HXX
# define   RENDERING_STATUS_HXX

# include "RenderingStatus.hh"
# include <sstream>

namespace fractsim {

  inline
  sdl::graphic::Button&
  RenderingStatus::getRenderButton() {
    return *getChildAs<sdl::graphic::Button>(getRenderButtonName());
  }

  inline
  void
  RenderingStatus::onZoomChanged(utils::Vector2f zoom) {
    // Compute the mean zoom value by taking the average.
    const float avgZoom = (zoom.x() + zoom.y()) / 2.0f;

    std::stringstream zoomTxtWrapper;
    zoomTxtWrapper << "Zoom: ";
    zoomTxtWrapper << std::fixed << std::setprecision(0);
    zoomTxtWrapper << avgZoom;

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // Try to retrieve the label widget and set the text corresponding
    // to the new zoom level.
    sdl::graphic::LabelWidget* txt = getZoomLabel();
    txt->setText(zoomTxtWrapper.str());
  }

  inline
  void
  RenderingStatus::onCompletionChanged(float perc) {
    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    sdl::graphic::ProgressBar* bar = getProgressBar();
    bar->setCompletion(perc);
  }

  inline
  float
  RenderingStatus::getStatusMaxHeight() noexcept {
    return 200.0f;
  }

  inline
  utils::Sizef
  RenderingStatus::getRenderButtonMaxSize() noexcept {
    return utils::Sizef(200.0f, getStatusMaxHeight());
  }

  inline
  utils::Sizef
  RenderingStatus::getZoomLabelMaxSize() noexcept {
    return utils::Sizef(200.0f, getStatusMaxHeight());
  }

  inline
  const char*
  RenderingStatus::getZoomLabelFont() noexcept {
    return "data/fonts/Goodtime.ttf";
  }

  inline
  float
  RenderingStatus::getGlobalMargins() noexcept {
    return 0.0f;
  }

  inline
  float
  RenderingStatus::getComponentMargins() noexcept {
    return 15.0f;
  }

  inline
  const char*
  RenderingStatus::getRenderButtonName() noexcept {
    return "rendering_status_render";
  }

  inline
  const char*
  RenderingStatus::getZoomLabelName() noexcept {
    return "rendering_status_zoom";
  }

  inline
  const char*
  RenderingStatus::getProgressBarName() noexcept {
    return "rendering_status_progress";
  }

  inline
  sdl::graphic::LabelWidget*
  RenderingStatus::getZoomLabel() {
    return getChildAs<sdl::graphic::LabelWidget>(getZoomLabelName());
  }

  inline
  sdl::graphic::ProgressBar*
  RenderingStatus::getProgressBar() {
    return getChildAs<sdl::graphic::ProgressBar>(getProgressBarName());
  }

}

#endif    /* RENDERING_STATUS_HXX */
