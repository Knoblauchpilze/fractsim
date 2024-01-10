
# include "Fractal.hh"

namespace fractsim {

  Fractal::Fractal(const utils::Sizef& canvas,
                   const utils::Boxf& area):
    utils::CoreObject(std::string("fractal_proxy")),

    m_propsLocker(),

    m_canvas(),
    m_area(),

    m_zoomLevel(),
    m_renderedArea(),
    m_tilesCount(),
    m_tiles()
  {
    setService(std::string("fractal"));

    resize(canvas);
    realWorldResize(area, false);
  }

  std::vector<RenderingTileShPtr>
  Fractal::generateRenderingTiles(FractalOptionsShPtr opt) {
    // In order to render the fractal, we need to perform some computations. In order
    // to speed things up we divide the workload into small tiles representing some
    // portion of the total area.
    // When some tiles are already available, we would ideally not want to recompute
    // everything and produce only the missing tiles.
    // To do that we will distinguish between two main cases:
    //   - when there's already a cache.
    //   - when there's no cache.
    //
    // In case there's no cache available, it probably mean that either no rendering
    // has ever been performed for this fractal or a resize of the canvas has occurred
    // not long ago or that the zoom level has changed. In any case it means that we
    // should start fresh and we can just go ahead (Office Space TM) and compute a
    // regular tiling for the area to render.
    //
    // When we have some caching on the other hand, we already defined a tiling when
    // the first rendering was performed. This tiling should be kept and extended if
    // possible. To do that, we will work in terms of layers. Basically we will check
    // for corners of the new rendering area and see which ones lie outside of the
    // already computed area. We will extend the area until we have a new tiling that
    // encompasses all four corners of the desired rendering area.
    // This approach guarantees that:
    //   - we keep the initial tiling and only recompute what's needed
    //   - we keep a rendered area that is rectangular and thus can easily be checked
    //     for intersections.

    // Check whether some cache is available.
    if (m_tiles.empty()) {
      return generateDefaultTiling(opt);
    }

    // We already have some cache available. We we have to do is to determine the
    // size of a tile, and add some until all four corners of the new rendering area
    // fit in the `m_renderedArea`.
    utils::Sizef tileDims(
      m_area.w() / getHorizontalTileCount(),
      m_area.h() / getVerticalTileCount()
    );

    // We need to compute how many tiles we need to add along each direction.
    // This can be computed by determining the distance from the boundary of
    // the rendered area to the boundary of the requested area and dividing
    // this distance by the size of a tile.
    float toLeft = m_renderedArea.getLeftBound() - m_area.getLeftBound();
    float toRight = m_area.getRightBound() - m_renderedArea.getRightBound();
    float toBottom = m_renderedArea.getBottomBound() - m_area.getBottomBound();
    float toTop = m_area.getTopBound() - m_renderedArea.getTopBound();

    int tilesToTheLeft = toLeft < 0.0f ? 0u : static_cast<int>(std::ceil(toLeft / tileDims.w()));
    int tilesToTheRight = toRight < 0.0f ? 0u : static_cast<int>(std::ceil(toRight / tileDims.w()));
    int tilesToTheBottom = toBottom < 0.0f ? 0u : static_cast<int>(std::ceil(toBottom / tileDims.h()));
    int tilesToTheTop = toTop < 0.0f ? 0u : static_cast<int>(std::ceil(toTop / tileDims.h()));

    verbose("Expanding rendered area by [" + std::to_string(tilesToTheLeft) + ", " + std::to_string(tilesToTheRight) + ", " + std::to_string(tilesToTheBottom) + ", " + std::to_string(tilesToTheTop) + "] (cache: " + std::to_string(m_tiles.size()) + ")");

    // Create all the tiles. We will skip the one from the central area as they have
    // already been rendered and saved into the cache.
    std::vector<RenderingTileShPtr> tiles;

    utils::Sizef pixSize = getPixelSizePrivate();

    int xInter = tilesToTheLeft + m_tilesCount.x();
    int yInter = tilesToTheBottom + m_tilesCount.y();

    int xMax = xInter + tilesToTheRight;
    int yMax = yInter + tilesToTheTop;

    // Compute the expected rendered area: this is basically the current rendering area
    // with all the tiles that need to be added.
    utils::Boxf expectedRendered(
      m_renderedArea.x() + (tilesToTheRight - tilesToTheLeft) * tileDims.w() / 2.0f,
      m_renderedArea.y() + (tilesToTheTop - tilesToTheBottom) * tileDims.h() / 2.0f,
      xMax * tileDims.w(),
      yMax * tileDims.h()
    );

    for (int y = 0 ; y < yMax ; ++y) {
      for (int x = 0 ; x < xMax ; ++x) {
        // Skip the tiles that belong to the rendered area.
        if (x >= tilesToTheLeft && x < xInter &&
            y >= tilesToTheBottom && y < yInter)
        {
          continue;
        }

        // This is a new tile, add it to the tiling to render.
        tiles.push_back(
          std::make_shared<RenderingTile>(
            utils::Boxf(
              expectedRendered.getLeftBound() + 1.0f * x * tileDims.w() + tileDims.w() / 2.0f,
              expectedRendered.getBottomBound() + 1.0f * y * tileDims.h() + tileDims.h() / 2.0f,
              tileDims
            ),
            pixSize,
            opt
          )
        );
      }
    }

    // Update expected rendered area and tiles count.
    m_renderedArea = expectedRendered;
    m_tilesCount.x() = xMax;
    m_tilesCount.y() = yMax;

    return tiles;
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
    const std::lock_guard guard(m_propsLocker);

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

  std::vector<RenderingTileShPtr>
  Fractal::generateDefaultTiling(FractalOptionsShPtr opt) {
    // We know that the `m_area` should be divided into a certain amount of tiles.
    // We also know the size of the canvas so we can estimate the pixel size.
    // Once this is done, we just divide the area into tiles and associate the right
    // part to each one.
    utils::Sizef tileDims(
      m_area.w() / getHorizontalTileCount(),
      m_area.h() / getVerticalTileCount()
    );
    utils::Sizef pixSize = getPixelSizePrivate();

    std::vector<RenderingTileShPtr> tiles;

    for (unsigned y = 0u ; y < getVerticalTileCount() ; ++y) {
      for (unsigned x = 0u ; x < getHorizontalTileCount() ; ++x) {
        tiles.push_back(
          std::make_shared<RenderingTile>(
            utils::Boxf(
              m_area.getLeftBound() + 1.0f * x * tileDims.w() + tileDims.w() / 2.0f,
              m_area.getBottomBound() + 1.0f * y * tileDims.h() + tileDims.h() / 2.0f,
              tileDims
            ),
            pixSize,
            opt
          )
        );
      }
    }

    // The rendered area corresponds to the entirety of the area. We can also assign
    // the dimensions of the tiling.
    m_renderedArea = m_area;
    m_tilesCount.x() = getHorizontalTileCount();
    m_tilesCount.y() = getVerticalTileCount();

    return tiles;
  }

}
