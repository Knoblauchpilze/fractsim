#ifndef    RENDERING_TILE_HXX
# define   RENDERING_TILE_HXX

# include "RenderingTile.hh"

namespace fractsim {

  inline
  RenderingTile::RenderingTile(const utils::Boxf& area,
                               const utils::Vector2i& step,
                               FractalOptionsShPtr options):
    utils::CoreObject(std::string("tile_") + area.toString()),

    m_area(area),
    m_discretization(step),

    m_computing(options)
  {
    // Check consistency.
    if (!m_area.valid()) {
      error(
        std::string("Could not create rendering tile"),
        std::string("Invalid rendering area ") + area.toString()
      );
    }

    if (m_computing == nullptr) {
      error(
        std::string("Could not create rendering tile"),
        std::string("Invalid computing options")
      );
    }
  }

}

#endif    /* RENDERING_TILE_HXX */
