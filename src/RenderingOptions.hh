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
       *          In order to allow for some discretization, the user should be
       *          able to provide the size of the canvas onto which the fractal
       *          will be displayed. This allows to determine how the `window`
       *          will be split up in individual pixels.
       * @param window - the area to render.
       * @param canvas - the size of the canvas.
       */
      RenderingOptions(const utils::Boxf& window,
                       const utils::Sizef& canvas);

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

      utils::Sizef
      getCanvasSize() const noexcept;

      /**
       * @brief - Assign a new canvas size for this set of options. If the input size is
       *          not valid an error is raised.
       */
      void
      setCanvasSize(const utils::Sizef& canvas);

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

      /**
       * @brief - The size of the canvas onto which the fractal will be drawn. This allows
       *          to determine the number of pixels to compute by computing the size of the
       *          individual pixels through the ratio of the `m_area` and the canvas' size.
       */
      utils::Sizef m_canvas;
  };

  using RenderingOptionsShPtr = std::shared_ptr<RenderingOptions>;
}

# include "RenderingOptions.hxx"

#endif    /* RENDERING_OPTIONS_HH */
