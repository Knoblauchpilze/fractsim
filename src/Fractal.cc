
# include "Fractal.hh"

# include <iostream>
# include <iomanip>

namespace fractsim {

  Fractal::Fractal(const utils::Sizef& canvas,
                   const utils::Boxf& area):
    utils::CoreObject(std::string("fractal_proxy")),

    m_canvas(),
    m_dims(),
    m_area(),
    m_cellDelta(),

    m_data()
  {
    setService(std::string("fractal"));

    resize(canvas);
    setRenderingArea(area);
  }

  void
  Fractal::assignValueForCoord(const utils::Vector2f& p,
                               float confidence)
  {
    // Clamp the confidence to a valid range.
    float clamped = std::min(1.0f, std::max(0.0f, confidence));

    // Compute the coordinates of the input point so that we can fill
    // in the internal cache. We should also check whether the cache
    // is valid.
    utils::Vector2i cell = computeCellFromCoords(p);

    setOrThrow(cell, clamped);
  }

  sdl::core::engine::BrushShPtr
  Fractal::createBrush(sdl::core::engine::GradientShPtr gradient) {
    // Check consistency.
    if (gradient == nullptr) {
      error(
        std::string("Could not create brush for fractal"),
        std::string("Invalid null gradient")
      );
    }

    // Create the colors representing the brush.
    unsigned total = m_dims.x() * m_dims.y();
    std::vector<sdl::core::engine::Color> colors(total, sdl::core::engine::Color::NamedColor::Black);

    for (unsigned id = 0u ; id < total ; ++id) {
      colors[id] = gradient->getColorAt(m_data[id]);
    }

    // Create a brush from the array of colors.
    sdl::core::engine::BrushShPtr brush = std::make_shared<sdl::core::engine::Brush>(
      std::string("brush_for_") + getName(),
      false
    );

    brush->createFromRaw(utils::Sizei::fromVector(m_dims), colors);

    return brush;
  }

  utils::Vector2i
  Fractal::computeCellFromCoords(const utils::Vector2f& p) {
    // Check consistency.
    if (!m_area.fuzzyContains(p, getToleranceForCells())) {
      error(
        std::string("Could not find cell coordinates for ") + p.toString(),
        std::string("Position is not in the area ") + m_area.toString()
      );
    }

    // Compute the expected cell using the delta. We protect
    // ourselves against rounding errors: the distance *must*
    // always be positive (see `RenderingTile::render` method
    // for more info).
    float dToLeft = std::max(0.0f, p.x() - m_area.getLeftBound());
    float dToBottom = std::max(0.0f, p.y() - m_area.getBottomBound());

    if (p.y() < -0.75f && p.x() > 0.598f) {
      log(
        "p: " + p.toString() + ", area: " + m_area.toString() +
        ", dist: " + std::to_string(dToLeft) + "x" + std::to_string(dToBottom) +
        ", delta: " + m_cellDelta.toString() +
        ", cell " + utils::Vector2i(
          static_cast<int>(std::floor(dToLeft / m_cellDelta.x())),
          static_cast<int>(std::floor(dToBottom / m_cellDelta.y()))
        ).toString() +
        ", fCell " + utils::Vector2f(dToLeft / m_cellDelta.x(), dToBottom / m_cellDelta.y()).toString()
      );
    }

    // We want to round down the coordinates: we consider that
    // the pixel is stored in the smallest cell that contains
    // part of it, both along the `x` and `y` axis.
    return utils::Vector2i(
      static_cast<int>(std::round(dToLeft / m_cellDelta.x())),
      static_cast<int>(std::round(dToBottom / m_cellDelta.y()))
    );
  }

  void
  Fractal::setOrThrow(const utils::Vector2i& cell,
                      float value)
  {
    // Check whether the internal cache is valid.
    if (m_data.empty()) {
      error(
        std::string("Could not set value ") + std::to_string(value) + " for cell " + cell.toString(),
        std::string("Invalid internal cache")
      );
    }

    // Clamp input data. Note that we perform an inversion of the internal data array
    // along the `y` axis: indeed as we will use it to generate a surface we need to
    // account for the axis inversion that will be applied there.
    int x = std::min(m_dims.x() - 1, std::max(cell.x(), 0));
    int y = std::min(m_dims.y() - 1, std::max(m_dims.y() - 1 - cell.y(), 0));

    unsigned p = y * m_dims.x() + x;

    if (p >= m_data.size()) {
      error(
        std::string("Could not set value ") + std::to_string(value) + " for cell " + cell.toString(),
        std::string("Invalid cell coordinate for dimensions ") + m_dims.toString()
      );
    }

    // Assign the value.
    m_data[p] = value;
  }

}
