#ifndef    FRACTAL_RENDERER_HH
# define   FRACTAL_RENDERER_HH

# include <mutex>
# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_graphic/ScrollableWidget.hh>
# include "FractalOptions.hh"
# include "RenderingOptions.hh"
# include "RenderingScheduler.hh"

namespace fractsim {

  class FractalRenderer: public sdl::graphic::ScrollableWidget {
    public:

      /**
       * @brief - Creates a fractal renderer with the specified size hint and
       *          parent widget. This widget is scrollable (meaning the user
       *          can scroll its content).
       * @param hint - the size hint for this widget.
       * @param parent - the parent widget for this element.
       */
      FractalRenderer(const utils::Sizef& sizeHint = utils::Sizef(),
                      sdl::core::SdlWidget* parent = nullptr);

      ~FractalRenderer();

      /**
       * @brief - Used to request a new rendering with the current settings.
       *          Will cancel any running rendering and launch a new one.
       * @param options - the options to use to perform the rendering.
       */
      void
      requestRendering(FractalOptionsShPtr options);

    protected:

      /**
       * @brief - Reimplementation of the base class method to provide update of the
       *          rendering window when a resize is requested.
       * @param window - the available size to perform the update.
       */
      void
      updatePrivate(const utils::Boxf& window) override;

      /**
       * @brief - Reimplementation of the base class method to detect whenever the
       *          reset key is pressed, allowing to set the rendering window to its
       *          default value and thus regain a nice viewpoint.
       * @param e - the event to be interpreted.
       * @return - `true` if the event was recognized and `false` otherwise.
       */
      bool
      keyPressEvent(const sdl::core::engine::KeyEvent& e) override;

      /**
       * @brief - Reimplementation of the base class method to detect whenever the
       *          mouse moves inside the widget. This allows to provide notification
       *          to external listeners by converting the position into a real world
       *          coordinates.
       * @param e - the event to be interpreted.
       * @return - `true` if the event was recognized and `false` otherwise.
       */
      bool
      mouseMoveEvent(const sdl::core::engine::MouseEvent& e) override;

      /**
       * @brief - Reimplementation of the base class method to detect when the wheel
       *          is used: this should trigger the zooming behavior based on the factor
       *          defined for this renderer.
       * @param e - the event to be interpreted.
       * @return - `true` if the event was recognized and `false` otherwise.
       */
      bool
      mouseWheelEvent(const sdl::core::engine::MouseEvent& e) override;

      /**
       * @brief - Reimplementation of the base class method to handle the repaint of the
       *          texture representing the fractal and its display.
       */
      void
      drawContentPrivate(const utils::Uuid& uuid,
                         const utils::Boxf& area) override;

    private:

      /**
       * @brief - Used to retrieve the default factor to use when zooming in.
       * @return - a factor suitable for zooming in operations.
       */
      static
      float
      getDefaultZoomInFactor() noexcept;

      /**
       * @brief - Used to retrieve the default factor to use when zooming out.
       * @return - a factor suitable for zooming out operations.
       */
      static
      float
      getDefaultZoomOutFactor() noexcept;

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
       * @brief - Used to retrieve the default key to use to reset the rendering
       *          window to its default value.
       * @return - the key to use to restore the rendering window to its default
       *           value.
       */
      static
      sdl::core::engine::RawKey
      getDefaultResetKey() noexcept;

      /**
       * @brief - Connect signals and build the renderer in a more general way.
       */
      void
      build();

      /**
       * @brief - Used to schedule a rendering with the specified options. This
       *          method issues a request to the threads pool used by this object
       *          to handle the computations.
       *          Note that this function assumes that the locker on the options
       *          is already acquired before calling the method.
       */
      void
      scheduleRendering();

      /**
       * @brief - Internal slot used to handle the tiles computed by the thread
       *          pool. The goal is to trigger the creation of the needed repaint
       *          events to display the results of the computation.
       * @param tiles - a list of tiles that just finished rendering.
       */
      void
      handleTilesComputed(const std::vector<RenderingTileShPtr>& tiles);

      /**
       * @brief - Used to convert the input area expressed in the fractal's coordinate
       *          frame into an area expressed in local coordinate frame. This is used
       *          to make the association between the tiles and the corresponding area
       *          displayed.
       * @param area - an area assumed to be expressed in fractal's coordinate frame.
       * @return - a box representing the area in local coordinate frame where the input
       *           area is displayed.
       */
      utils::Boxf
      convertFractalAreaToLocal(const utils::Boxf& area) const;

      /**
       * @brief - Used to clear the texture associated to this fractal.
       */
      void
      clearTiles();

      /**
       * @brief - Used to determine whether the tiles have changed since the creation
       *          of the texture representing them. If this is the case it means that
       *          the `m_tex` should be recreated.
       *          Assumes that the locker is alreadu acquired.
       */
      bool
      tilesChanged() const noexcept;

      /**
       * @brief - Used to specify that the tiles have changed and thus that the `m_tex`
       *          texture should be recreated on the next call to `drawContentPrivate`.
       *          Assumes that the locker is alreadu acquired.
       */
      void
      setTilesChanged() noexcept;

      /**
       * @brief - Performs the creation of the texture representing this fractal from
       *          the data associated to it. Assumes that the locker is already acquired.
       */
      void
      loadTiles();

      /**
       * @brief - Used to convert the input position expressed in global coordinate frame
       *          into a position expressed in real world coordinate.
       *          Note that this method assumes that the locker is already acquired.
       * @param global - the global coordinate frame position to convert.
       * @return - the corresponding position in real world coordinate frame.
       */
      utils::Vector2f
      convertLocalToRealWorld(const utils::Vector2f& global);

    private:

      /**
       * @brief - A mutex allowing to protect this widget from concurrent accesses.
       */
      mutable std::mutex m_propsLocker;

      /**
       * @brief - Holds all the needed options to define the rendering window when
       *          computing the fractal.
       */
      RenderingOptionsShPtr m_renderingOpt;

      /**
       * @brief - Describes the options to use to render the fractal. While rendering
       *          options are more general and usually refer to settings applicable no
       *          matter the fractal type, this attribute contains values specific on
       *          the type of fractal to render. It contains the current options to
       *          use to perform a rendering.
       *          This value is supposed to be null in case no fractal has already been
       *          renderer.
       */ 
      FractalOptionsShPtr m_fractalOptions;

      /**
       * @brief - Holds the data representing the fractal displayed by this renderer.
       *          The data is composed of raw data that need to be interpreted with the
       *          rendering options (like the palette) defined in the `m_fractalOptions`.
       *          When a repaint event is issued the data is built from this object.
       */
      FractalShPtr m_fractalData;

      /**
       * @brief - Convenience object allowing to schedule the rendering.
       */
      RenderingSchedulerShPtr m_scheduler;

      /**
       * @brief - The index returned by the engine for the texture representing the fractal
       *          on screen. It is rendered from the tiles' data computed internally and is
       *          valid as long as the `m_tilesRendered` boolean is set to `false`.
       */
      utils::Uuid m_tex;

      /**
       * @brief - This value indicates whether the `m_tex` identifier is still valid or not.
       *          Each time a tile is rendered this value is set to `true` indicating that
       *          the texture representing the fractal needs to be updated.
       */
      bool m_tilesRendered;

    public:

      /**
       * @brief - Signal emitted whenver the zoom value is modified by some internal actions
       *          like scroling. Listeners can register to this signal when needed.
       *          The zoom information contains the information about both axes.
       */
      utils::Signal<utils::Vector2f> onZoomChanged;

      /**
       * @brief - Signal emitted whenever the coordinates of the point located under the mouse
       *          is changed. This is usually to keep track of said position (using a label for
       *          example).
       */
      utils::Signal<utils::Vector2f> onCoordChanged;

      /**
       * @brief - Signal emitted whenever the rendering area associated to this renderer is
       *          modified. This allows external listeners to be notified about such events
       *          and maybe keep track of the current rendering area.
       */
      utils::Signal<utils::Boxf> onRenderingAreaChanged;
  };

}

# include "FractalRenderer.hxx"

#endif    /* FRACTAL_RENDERER_HH */
