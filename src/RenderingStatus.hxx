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

}

#endif    /* RENDERING_STATUS_HXX */
