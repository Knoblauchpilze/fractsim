#ifndef    RENDERING_STATUS_HXX
# define   RENDERING_STATUS_HXX

# include "RenderingStatus.hh"

namespace fractsim {

  inline
  sdl::graphic::Button&
  RenderingStatus::getRenderButton() {
    return *getChildAs<sdl::graphic::Button>(getRenderButtonName());
  }

  inline
  sdl::graphic::Button&
  RenderingStatus::getResetButton() {
    return *getChildAs<sdl::graphic::Button>(getResetButtonName());
  }

  inline
  void
  RenderingStatus::onCompletionChanged(float perc) {
    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    sdl::graphic::ProgressBar* bar = getProgressBar();
    bar->setCompletion(perc);
  }

  inline
  float
  RenderingStatus::getStatusMaxHeight() noexcept {
    return 200.0f;
  }

  inline
  float
  RenderingStatus::getButtonBorderSize() noexcept {
    return 10.0f;
  }

  inline
  utils::Sizef
  RenderingStatus::getRenderButtonMaxSize() noexcept {
    return utils::Sizef(200.0f, getStatusMaxHeight());
  }

  inline
  utils::Sizef
  RenderingStatus::getResetButtonMaxSize() noexcept {
    return utils::Sizef(100.0f, getStatusMaxHeight());
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
  RenderingStatus::getResetButtonName() noexcept {
    return "rendering_status_reset";
  }

  inline
  const char*
  RenderingStatus::getProgressBarName() noexcept {
    return "rendering_status_progress";
  }

  inline
  const char*
  RenderingStatus::getButtonFontName() noexcept {
    return "data/fonts/Goodtime.ttf";
  }

  inline
  sdl::graphic::ProgressBar*
  RenderingStatus::getProgressBar() {
    return getChildAs<sdl::graphic::ProgressBar>(getProgressBarName());
  }

}

#endif    /* RENDERING_STATUS_HXX */
