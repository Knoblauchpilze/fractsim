
# include "RenderingTile.hh"

namespace fractsim {

  RenderingTile::RenderingTile(const utils::Boxf& area,
                               const utils::Vector2i& step,
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
  }

  void
  RenderingTile::render() {
    // Traverse the rendering area and compute each individual point. We
    // can compute the real world delta by applying the discretization
    // interval on the area.
    utils::Vector2f delta(
      m_area.w() / m_discretization.x(),
      m_area.h() / m_discretization.y()
    );

    float xMin = m_area.getLeftBound();
    float yMin = m_area.getBottomBound();

    for (int y = 0 ; y < m_discretization.y() ; ++y) {
      for (int x = 0 ; x < m_discretization.x() ; ++x) {
        // Determine the coordinate of the point to compute.
        utils::Vector2f p(
          xMin + x * delta.x(),
          yMin + y * delta.y()
        );

        // Compute whether it belongs to the fractal at the
        // current level of accuracy.
        unsigned res = m_computing->compute(p);

        // Save the result in the proxy.
        m_proxy->assignValueForCoord(p, 1.0f * res / m_computing->getAccuracy());
      }
    }
  }

}

