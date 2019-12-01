#ifndef    RENDERING_STATUS_HXX
# define   RENDERING_STATUS_HXX

# include "RenderingStatus.hh"

namespace fractsim {

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

  sdl::graphic::ProgressBar*
  RenderingStatus::getProgressBar() {
    return getChildAs<sdl::graphic::ProgressBar>(getProgressBarName());
  }

}

#endif    /* RENDERING_STATUS_HXX */
