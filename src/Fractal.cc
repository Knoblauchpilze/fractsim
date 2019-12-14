
# include "Fractal.hh"

namespace fractsim {

  Fractal::Fractal(const utils::Sizef& canvas,
                   const utils::Boxf& area):
    utils::CoreObject(std::string("fractal_proxy")),

    m_propsLocker(),

    m_canvas(),
    m_area(),

    m_tiles()
  {
    setService(std::string("fractal"));

    resize(canvas);
    realWorldResize(area);
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

    // We want to create an output brush with a size that is as close
    // as `m_canvas` as possible. To do so we will need to retrieve
    // the color from the rendering tiles for each pixel.
    // We know that the `m_area` is supposed to be represented with a
    // brush of size `m_canvas`. This allows to put an upper bound on
    // the area that can be covered by a pixel.
    // Once we computed this information, we can create a local array
    // with as many pixels as required and start to fill each cell.
    // The way we want to fill cell is by averaging the values of all
    // the tiles that contains it.

    // Allocate the output canvas.
    utils::Sizei iCanvasSize(
      static_cast<int>(std::round(m_canvas.w())),
      static_cast<int>(std::round(m_canvas.h()))
    );

    sdl::core::engine::Color def = gradient->getColorAt(0.0f);

    std::vector<sdl::core::engine::Color> colors(iCanvasSize.area(), def);

    // Compute the sampling interval to render the output canvas.
    utils::Sizef pixSize(
      m_area.w() / m_canvas.w(),
      m_area.h() / m_canvas.h()
    );

    float xMin = m_area.getLeftBound();
    float yMin = m_area.getBottomBound();

    // Populate the output canvas with data from the rendering tiles.
    for (int y = 0 ; y < iCanvasSize.h() ; ++y) {
      // Compute the coordinate of this pixel in the output canvas. Note that
      // we perform an inversion of the internal data array along the `y` axis:
      // indeed as we will use it to generate a surface we need to account for
      // the axis inversion that will be applied there.
      unsigned offset = (iCanvasSize.h() - 1 - y) * iCanvasSize.w();

      for (int x = 0 ; x < iCanvasSize.w() ; ++x) {
        // Compute the local coordinate of the point.
        // Compute the point associated to this cell.
        utils::Vector2f p(
          xMin + x * pixSize.w(),
          yMin + y * pixSize.h()
        );

        float totConf = 0.0f;
        unsigned count = 0u;
        bool in = true;

        for (unsigned id = 0u ; id < m_tiles.size() ; ++id) {
          float confidence = m_tiles[id]->getConfidenceAt(p, in);
          if (in) {
            totConf += confidence;
            ++count;
          }
        }

        // Check whether we could find some data for this point.
        if (count == 0u) {
          colors[offset + x] = def;
        }
        else {
          colors[offset + x] = gradient->getColorAt(totConf / count);
        }
      }
    }

    // Create a brush from the array of colors.
    sdl::core::engine::BrushShPtr brush = std::make_shared<sdl::core::engine::Brush>(
      std::string("brush_for_") + getName(),
      false
    );

    brush->createFromRaw(iCanvasSize, colors);

    return brush;
  }

}
