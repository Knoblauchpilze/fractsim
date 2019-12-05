#ifndef    RENDERING_TILE_HH
# define   RENDERING_TILE_HH

# include <maths_utils/Box.hh>
# include <maths_utils/Vector2.hh>
# include <core_utils/CoreObject.hh>
# include "FractalOptions.hh"

namespace fractsim {

  class RenderingTile: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new rendering tile with the associated rendering area.
       *          The tile also needs some options to be provided in order to be
       *          able to perform the computations.
       *          In case the area or the options are not valid an error is raised.
       * @param area - the rendering area for which the computations should be done.
       * @param step - indicates the number of individual pixels that should be used
       *               to represent the input area.
       * @param options - the options to use to compute the fractal (accuracy, actual
       *                  formula, etc.).
       */
      RenderingTile(const utils::Boxf& area,
                    const utils::Vector2i& step,
                    FractalOptionsShPtr options);

      ~RenderingTile() = default;

    private:

      /**
       * @brief - The rendering area for which the computations should be performed.
       */
      utils::Boxf m_area;

      /**
       * @brief - Holds the discretization to apply to the area to obtain individual
       *          pixels. Basically we will divide the `m_area` into this many pixels
       *          and perform the computation for each one of them.
       */
      utils::Vector2i m_discretization;

      /**
       * @brief - Describes the object to use to perform the computation for each
       *          individual pixel of the rendering area. Allows to abstract away
       *          the complexity of having distinct tiles for each different type
       *          of fractal.
       */ 
      FractalOptionsShPtr m_computing;
  };

}

# include "RenderingTile.hxx"

#endif    /* RENDERING_TILE_HH */
