
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
  }

  void
  RenderingTile::render() {
    // The delta is given directly by the `m_discretization` interval.
    // In order to define how many point we will pick to perform the
    // computation, we just have to commensurate this to the size of
    // the area assigned to this tile.
    utils::Vector2f fCount(
      m_area.w() / m_discretization.w(),
      m_area.h() / m_discretization.h()
    );

    // Convert to integer.
    utils::Vector2i iCount(
      static_cast<int>(std::ceil(fCount.x())),
      static_cast<int>(std::ceil(fCount.y()))
    );

    log("Rendering tile with pix " + m_discretization.toString() + ", count is " + iCount.toString() + " (f: " + fCount.toString() + ")");

    float xMin = m_area.getLeftBound();
    float yMin = m_area.getBottomBound();

    bool valid = true;

    // Compute all the area or stop when the area is not valid anymore,
    // meaning that some other batch of tiles have been issued.
    for (int y = 0 ; y < iCount.y() && valid ; ++y) {
      for (int x = 0 ; x < iCount.x() && valid ; ++x) {
        // Determine the coordinate of the point to compute.
        utils::Vector2f p(
          xMin + x * m_discretization.w(),
          yMin + y * m_discretization.h()
        );

        // Determine whether the point still belongs to the area.
        if (!m_area.contains(p)) {
          continue;
        }

        // Compute whether it belongs to the fractal at the
        // current level of accuracy.
        unsigned res = m_computing->compute(p);

        // Save the result in the proxy.
        valid = m_proxy->assignValueForCoord(p, 1.0f * res / m_computing->getAccuracy());
      }
    }
  }

}

