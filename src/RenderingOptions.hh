#ifndef    RENDERING_OPTIONS_HH
# define   RENDERING_OPTIONS_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include <maths_utils/Box.hh>
# include <maths_utils/Vector2.hh>

namespace fractsim {

  class RenderingOptions: utils::CoreObject {
    public:

      /**
       * @brief - Create a new set of options to render a fractal. This object
       *          defines some general options like the window to render or the
       *          zooming factor.
       * @param window - the area to render.
       */
      RenderingOptions(const utils::Boxf& window);

      /**
       * @brief - Desctruction of the object.
       */
      ~RenderingOptions() = default;

      utils::Boxf
      getRenderingArea() const noexcept;

      /**
       * @brief - Assigns a new rendering area for these options. Note that both
       *          the current and the initial area are modified by this method
       *          which means that no zoom information is persisted.
       *          In case the new area is not valid an error is raised.
       * @param area - the new area to assign.
       */
      void
      setRenderingArea(const utils::Boxf& area);

      /**
       * @brief - Used to retrieve the zoom factor along each axis.
       * @return - a vector representing the zoom factor along each axis.
       */
      utils::Vector2f
      getZoom() const noexcept;

      /**
       * @brief - Perform a zoom which keeps the `center` at the specified location and
       *          with the specified factor. Note that theoreticall the center could be
       *          outside of the visible range.
       * @param center - the point to fix when performing the zoom.
       * @param factor - a measure of the ratio between the new size and the current size
       *                 of the rendering area.
       */
      void
      zoom(const utils::Vector2f& center,
           float factor = 2.0f);

      /**
       * @brief - Resets the current rendering area to its initial value.
       */
      void
      reset();

      /**
       * @brief - Creates default rendering options to have a valid area to display the
       *          Mandelbrot set.
       * @return - a set of options allowing a good view on the Mandelbrot set.
       */
      static
      RenderingOptions
      createDefaultForMandelbrot() noexcept;

      /**
       * @brief - Creates default rendering options to have a valid area to display a
       *          Julia set.
       * @return - a set of options allowing a good view on a Julia set.
       */
      static
      RenderingOptions
      createDefaultForJulia() noexcept;

      /**
       * @brief - Creates default rendering options to have a valid area to display a
       *          Newton polynomial fractal.
       * @return - a set of options allowing a good view a Newton polynomial fractal.
       */
      static
      RenderingOptions
      createDefaultForNewton() noexcept;

    private:

      /**
       * @brief - Utility value used to keep track of the initial rendering area assigned
       *          to these options. Allows to easily clear the area and reset it to the
       *          initial values.
       */
      utils::Boxf m_initial;

      /**
       * @brief - The rendering area assigned to these options. Define the visible area
       *          that can be achieved when the zooming factor is minimal.
       */
      utils::Boxf m_area;
  };

  using RenderingOptionsShPtr = std::shared_ptr<RenderingOptions>;
}

# include "RenderingOptions.hxx"

#endif    /* RENDERING_OPTIONS_HH */
