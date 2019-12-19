#ifndef    RENDERING_TILE_HH
# define   RENDERING_TILE_HH

# include <memory>
# include <maths_utils/Box.hh>
# include <maths_utils/Vector2.hh>
# include <core_utils/CoreObject.hh>
# include <core_utils/ThreadPool.hh>
# include "FractalOptions.hh"

namespace fractsim {

  class RenderingTile: public utils::CoreObject, public utils::AsynchronousJob {
    public:

      /**
       * @brief - Creates a new rendering tile with the associated rendering area.
       *          The tile also needs some options to be provided in order to be
       *          able to perform the computations.
       *          In case the area or the options are not valid an error is raised.
       * @param area - the rendering area for which the computations should be done.
       * @param step - represents the real world area covered by a single pixel. Can
       *               be used as an indication of how many pixels are needed for
       *               this tile.
       * @param options - the options to use to compute the fractal (accuracy, actual
       *                  formula, etc.).
       */
      RenderingTile(const utils::Boxf& area,
                    const utils::Sizef& step,
                    FractalOptionsShPtr options);

      ~RenderingTile() = default;

      /**
       * @brief - Reimplementation of the `AsynchronousJob` method allowing to perform
       *          the rendering for this tile.
       */
      void
      compute() override;

      /**
       * @brief - Returns the area associated to this tile in the fractal's coordinate frame.
       * @¶eturn - a box representing the area associated to this tile.
       */
      utils::Boxf
      getArea() const noexcept;

      /**
       * @brief - Used to retrieve the confidence computed for the input point `p`. If the
       *          point does not belong to the area of this tile the `inside` boolean is
       *          set to `false` and a default color is returned (but should not be used or
       *          relied upon).
       * @param p - the point for which the confidence should be returned.
       * @param inside - output value indicating whether the point is inside or outside of
       *                 the area defined for this tile.
       * @return - a value in the range `[0; 1]` indicating the level of confidence for the
       *           input position.
       */
      float
      getConfidenceAt(const utils::Vector2f& p,
                      bool& inside);

    private:

      /**
       * @brief - Used to initialize the tile in order to be ready to perform the rendering.
       *          This includes allocating the internal data array along with pre-computing
       *          some data needed to render.
       */
      void
      initialize();

    private:

      /**
       * @brief - The rendering area for which the computations should be performed.
       */
      utils::Boxf m_area;

      /**
       * @brief - Holds the real world area covered by a single pixel. Basically we
       *          can extract a discretization interval from this value.
       */
      utils::Sizef m_discretization;

      /**
       * @brief - Describes the object to use to perform the computation for each
       *          individual pixel of the rendering area. Allows to abstract away
       *          the complexity of having distinct tiles for each different type
       *          of fractal.
       */
      FractalOptionsShPtr m_computing;

      /**
       * @brief - The dimensions of the internal data array. Computed from the area
       *          and the discretization step, it is conservative in the sense that
       *          each cell covers at most `m_discretization` real world area.
       */
      utils::Sizei m_dims;

      /**
       * @brief - The internal data array which will contain the divergence values
       *          for each cell of the tile. This array divides the `m_area` into
       *          cells of at most `m_discretization` area, and compute the value
       *          needed to diverge for each one of them.
       */
      std::vector<float> m_data;
  };

  using RenderingTileShPtr = std::shared_ptr<RenderingTile>;
}

# include "RenderingTile.hxx"

#endif    /* RENDERING_TILE_HH */
