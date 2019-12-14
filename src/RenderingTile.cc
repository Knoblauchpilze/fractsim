
# include "RenderingTile.hh"

namespace fractsim {

  RenderingTile::RenderingTile(const utils::Boxf& area,
                               const utils::Sizef& step,
                               FractalOptionsShPtr options,
                               FractalShPtr proxy):
    utils::CoreObject(std::string("tile_") + area.toString()),

    m_area(area),
    m_discretization(step),

    m_computing(options),

    m_proxy(proxy)
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

    if (m_proxy == nullptr) {
      error(
        std::string("Could not create rendering tile"),
        std::string("Invalid fractal proxy")
      );
    }

    setService("tile");
  }

  void
  RenderingTile::render() {
    // Request the cells associated to this tile.
    utils::Boxi cells = m_proxy->generateBoxFromArea(m_area);

    int xMin = cells.getLeftBound();
    int xMax = cells.w();
    int yMin = cells.getBottomBound();
    int yMax = cells.h();

    // Compute all the area or stop when the area is not valid anymore,
    // meaning that some other batch of tiles have been issued.
    for (int y = 0 ; y < yMax ; ++y) {
      for (int x = 0 ; x < xMax ; ++x) {
        // Determine the coordinate of the point to compute.
        utils::Vector2f p(
          m_area.getLeftBound() + x * m_discretization.w(),
          m_area.getBottomBound() + y * m_discretization.h()
        );

        // Determine whether the point still belongs to the area.
        if (!m_area.contains(p)) {
          continue;
        }

        // Compute whether it belongs to the fractal at the
        // current level of accuracy.
        unsigned res = m_computing->compute(p);

        // Save the result in the proxy.
        // TODO: We should probably prevent writing coordinates when the job is not
        // relevant anymore.
        m_proxy->assignValueForCoord(xMin + x, yMin + y, 1.0f * res / m_computing->getAccuracy());
      }
    }
  }

}

