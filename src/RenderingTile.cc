
# include "RenderingTile.hh"

namespace fractsim {

  RenderingTile::RenderingTile(const utils::Boxf& area,
                               const utils::Sizef& step,
                               FractalOptionsShPtr options):
    utils::AsynchronousJob(std::string("tile_") + area.toString()),

    m_area(area),
    m_discretization(step),

    m_computing(options),

    m_dims(),
    m_data()
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

    setService("tile");

    initialize();
  }

  void
  RenderingTile::compute() {
    // Compute the value needed to fill in each cell.
    float xMin = m_area.getLeftBound();
    float yMin = m_area.getBottomBound();

    for (int y = 0 ; y < m_dims.h() ; ++y) {
      unsigned offset = y * m_dims.w();

      for (int x = 0 ; x < m_dims.w() ; ++x) {
        // Compute the point associated to this cell.
        utils::Vector2f p(
          xMin + x * m_discretization.w(),
          yMin + y * m_discretization.h()
        );

        // Compute the divergence count for this point.
        // Normalize the contribution with the maximum
        // confidence.
        float div = m_computing->compute(p);
        float clamped = std::min(1.0f, std::max(0.0f, div));

        m_data[offset + x] = clamped;
      }
    }
  }

  float
  RenderingTile::getConfidenceAt(const utils::Vector2f& p,
                                 bool& inside)
  {
    // Check trivial case where the point does not belong to the tile.
    inside = m_area.contains(p);
    if (!inside) {
      return 0.0f;
    }

    // Compute the local position of `p` within the support area of this tile.
    utils::Vector2f local = p - m_area.getBottomLeftCorner();

    // Transform this coordinate into a cell value.
    utils::Vector2f fCell(
      local.x() / m_discretization.w(),
      local.y() / m_discretization.h()
    );

    // We might or might not hit perfectly a cell with this value. To guarantee
    // a more robust process we will perform bilinear interpolation among the
    // 4 closest cell hit.
    // In order to allow that we will have to first take care of cases where the
    // cell lies on the boundaries of the tile.
    int xMin = static_cast<int>(std::floor(fCell.x()));
    int xMax = static_cast<int>(std::ceil(fCell.x()));
    int yMin = static_cast<int>(std::floor(fCell.y()));
    int yMax = static_cast<int>(std::ceil(fCell.y()));

    xMin = std::min(std::max(0, xMin), m_dims.w() - 1);
    xMax = std::min(std::max(0, xMax), m_dims.w() - 1);

    yMin = std::min(std::max(0, yMin), m_dims.h() - 1);
    yMax = std::min(std::max(0, yMax), m_dims.h() - 1);

    // Retrieve the value at each cell surrounding the point.
    float a = m_data[yMin * m_dims.w() + xMin];
    float b = m_data[yMax * m_dims.w() + xMin];
    float c = m_data[yMin * m_dims.w() + xMax];
    float d = m_data[yMax * m_dims.w() + xMax];

    // Interpolate first along the `x` axis and then along the `y` axis.
    float dx = fCell.x() - xMin;
    float dy = fCell.y() - yMin;

    float cU = b + (d - b) * dx;
    float cD = a + (c - a) * dx;

    float out = cD + (cU - cD) * dy;

    return out;
  }

  void
  RenderingTile::initialize() {
    // We want to render the data needed to represent the `m_area`
    // using as indication the discretization step providing some
    // indication regarding the desired pixel size.
    // Of course most of the time we will not get a perfect ratio
    // between the area and the step so we have to be conservative
    // here and compute a bit more if needed.
    utils::Sizef fCells(
      m_area.w() / m_discretization.w(),
      m_area.h() / m_discretization.h()
    );

    // Be conservative.
    m_dims = utils::Sizei(
      static_cast<int>(std::ceil(fCells.w())),
      static_cast<int>(std::ceil(fCells.h()))
    );

    // Allocate the data array.
    m_data.resize(m_dims.area(), 0u);
  }

}

