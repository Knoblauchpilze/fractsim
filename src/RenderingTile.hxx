#ifndef    RENDERING_TILE_HXX
# define   RENDERING_TILE_HXX

# include "RenderingTile.hh"

namespace fractsim {

  inline
  utils::Boxf
  RenderingTile::getArea() const noexcept {
    return m_area;
  }

}

#endif    /* RENDERING_TILE_HXX */
