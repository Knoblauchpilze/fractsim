
# include "Fractal.hh"

namespace fractsim {

  Fractal::Fractal(const utils::Sizef& canvas,
                   const utils::Boxf& area):
    utils::CoreObject(std::string("fractal_proxy")),

    m_propsLocker(),

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

  utils::Boxi
  Fractal::generateBoxFromArea(const utils::Boxf& part) {
    // Compute the distance to the left and bottom regions of the
    // general area defined for this object.
    float dToLeft = std::max(0.0f, part.getLeftBound() - m_area.getLeftBound());
    float dToBottom = std::max(0.0f, part.getBottomBound() - m_area.getBottomBound());

    // Compute the ratio between the dimensions of the input box
    // and the internal box.
    float wRatio = part.w() / m_area.w();
    float hRatio = part.h() / m_area.h();

    // Compute the dimensions and the starting position using the
    // internal cells delta.
    utils::Vector2f fStart(dToLeft / m_cellDelta.x(), dToBottom / m_cellDelta.y());
    utils::Sizef fDims(wRatio * m_canvas.w(), hRatio * m_canvas.h());

    // Convert to integer values.
    utils::Vector2i iStart(
      static_cast<int>(std::floor(fStart.x())),
      static_cast<int>(std::floor(fStart.y()))
    );

    utils::Sizei iDims(
      static_cast<int>(std::ceil(fDims.w())),
      static_cast<int>(std::ceil(fDims.h()))
    );

    // Generate a box from this.
    if (iDims.w() % 2 != 0) {
      ++iDims.w();
    }
    if (iDims.h() % 2 != 0) {
      ++iDims.h();
    }

    utils::Boxi cells(
      iStart.x() + iDims.w() / 2,
      iStart.y() + iDims.h() / 2,
      iDims
    );

    return cells;
  }

  void
  Fractal::assignValueForCoord(int x,
                               int y,
                               float confidence)
  {
    // Clamp the confidence to a valid range.
    float clamped = std::min(1.0f, std::max(0.0f, confidence));

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    setOrThrow(utils::Vector2i(x, y), clamped);
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

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

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
