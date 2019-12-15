#ifndef    FRACTAL_HH
# define   FRACTAL_HH

# include <mutex>
# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include <maths_utils/Vector2.hh>
# include <maths_utils/Size.hh>
# include <maths_utils/Box.hh>
# include <sdl_engine/Brush.hh>
# include <sdl_engine/Gradient.hh>
# include "RenderingTile.hh"
# include "FractalOptions.hh"

namespace fractsim {

  class Fractal: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new fractal object with the specified canvas size and
       *          rendering area. The rendering area is supposed to be drawn on
       *          a canvas having dimensions of `canvas`.
       *          The process uses the internal tiles to retrieve a suited color
       *          for each pixel of the canvas through the `createBrush` method.
       * @param canvas - the canvas size associated to the fractal. Used as an
       *                 indication of the output image desired.
       * @param area - the rendering area represented by this fractal. This area
       *               is in real world coordinate.
       */
      Fractal(const utils::Sizef& canvas,
              const utils::Boxf& area);

      /**
       * @brief - Destruction of the object.
       */
      ~Fractal() = default;

      /**
       * @brief - Used to retrieve the size of a pixel in real world coordinate. Uses the
       *          internal values to get the canvas size and the real world area that is
       *          represented.
       * @return - the output vector represents the size of a single pixel in real world
       *           coordinate.
       */
      utils::Sizef
      getPixelSize() const noexcept;

      /**
       * @brief - Assign a new canvas size, which means that the brush generated by the
       *          `createBrush` method will produce data matching this size.
       *          The larger this area the more memory and time it will take/use for the
       *          brush's creation.
       * @param canvas - the new canvas' size assigned to this fractal.
       */
      void
      resize(const utils::Sizef& canvas);

      /**
       * @brief - Assigns a new rendering area for this fractal. Offsets the rendering
       *          area represented by the canvases produced by this object.
       *          The user needs to indicate whether the resize include a zooming part
       *          or if it doesn't (in which case it probably just indicate a shift of
       *          the area).
       * @param area - the rendering area associated to this fractal.
       * @param zoom - `true` if the resize includes a zooming part, `false` otherwise.
       */
      void
      realWorldResize(const utils::Boxf& area,
                      bool zoom);

      /**
       * @brief - Used to register a new data tile to be used when producing images for
       *          the output canvas. This tile is not used right away, a new call to the
       *          `createBrush` method should be issued for that.
       *          The tile is registered with the specified zoom level which indicates
       *          the precision of the data. A different zoom level that what is defined
       *          internally triggers a reset of the cache.
       * @param zoom - the zoom level for the input data tile.
       * @param tile - the data tile to register.
       */
      void
      registerDataTile(float zoom,
                       RenderingTileShPtr tile);

      /**
       * @brief - Used to generate a set of tiles allowing to produce a complete rendering
       *          for the area associated to this fractal. We assume that the zoom level
       *          does not change and thus try to take benefit from the existing cache of
       *          tiles already available.
       *          Uses the provided options to populate the tiles.
       * @param opt - the fractal options to assign to each tile.
       * @return - a list of tiles that should be rendered to create the missing data.
       */
      std::vector<RenderingTileShPtr>
      generateRenderingTiles(FractalOptionsShPtr opt);

      /**
       * @brief - Create a new brush that can be used to create a texture representing this
       *          fractal. The gradient in input is used to fetch colors based on the data
       *          available for the convergence of the fractal.
       * @param gradient - the palette to use to colorize the fractal.
       * @return - a pointer to a brush representing the fractal.
       */
      sdl::core::engine::BrushShPtr
      createBrush(sdl::core::engine::GradientShPtr gradient);

    private:

      /**
       * @brief - Used to retrieve a value allowing to compare two zoom levels. When the
       *          distance between both is smaller than this threshold we consider them
       *          to be equal.
       * @return - a value that can be used as a threshold to compare zoom levels.
       */
      static
      float
      getZoomComparisonThreshold() noexcept;

      /**
       * @brief - Provide a value describing how many tiles should be used to
       *          divide the workload along the horizontal axis. This allows
       *          to perform some sort of parallelization.
       * @return - the number of tiles to create horizontally.
       */
      static
      unsigned
      getHorizontalTileCount() noexcept;

      /**
       * @brief - Similar to `getHorizontalTileCount` but for the vertical axis.
       * @return - the number of tiles to create vertically.
       */
      static
      unsigned
      getVerticalTileCount() noexcept;

      /**
       * @brief - Similar to the `getPixelSize` method but does not attempt to acquire the
       *          lock on the internal properties. This is meant for internal usage.
       * @return - the output vector represents the size of a single pixel in real world
       *           coordinate.
       */
      utils::Sizef
      getPixelSizePrivate() const noexcept;

    private:

      /**
       * @brief - Used to protect this object from concurrent accesses.
       */
      mutable std::mutex m_propsLocker;

      /**
       * @brief - The size of the canvas associated to this fractal.
       */
      utils::Sizef m_canvas;

      /**
       * @brief - The area assigned to this fractal. This allows to determine which part of
       *          the internal cache corresponds to which position in the real world.
       */
      utils::Boxf m_area;

      /**
       * @brief - Holds the current zoom level associated to the data tiles. As long as some
       *          tiles are coming up with the same zoom level they are accumulated. If the
       *          zoom level is changed, the cache is reset.
       */
      float m_zoomLevel;

      /**
       * @brief - Contains all the tiles registered for this fractal so far. These tiles are
       *          used when performing the creation of the brush to create the visual display
       *          for this fractal.
       */
      std::vector<RenderingTileShPtr> m_tiles;
  };

  using FractalShPtr = std::shared_ptr<Fractal>;
}

# include "Fractal.hxx"

#endif    /* FRACTAL_HH */
