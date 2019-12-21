#ifndef    FRACTAL_RENDERER_HH
# define   FRACTAL_RENDERER_HH

# include <mutex>
# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_graphic/ScrollableWidget.hh>
# include <core_utils/ThreadPool.hh>
# include "FractalOptions.hh"
# include "RenderingOptions.hh"
# include "Fractal.hh"

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
       * @brief - Specialziation of the parent method in order to perform the
       *          scrolling on this object. What we want is to move the real
       *          world area associated to the visual representation so that
       *          we get another perspective on the fractal.
       *          The interface is similar to what is expected by the parent
       *          class (see `ScrollableWidget` for more details).
       * @param posToFix - the position in local coordinate frame corresponding
       *                   to the position that should be fixed during the
       *                   scroll operation.
       * @param whereTo - the new position of the `posToFix`. Corresponds to
       *                  the `posToFix` where the `motion` has been applied.
       * @param motion - the motion to apply.
       * @param notify - indication as to this method should emit some signals
       *                 like `onHorizontalAxisChanged`.
       * @return - `true` if the actual rendering area has been changed, and
       *           `false` otherwise.
       */
      bool
      handleContentScrolling(const utils::Vector2f& posToFix,
                             const utils::Vector2f& whereTo,
                             const utils::Vector2f& motion,
                             bool notify = true) override;

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
       * @brief - Used to retrieve the number of *pixel(s)* corresponding to a press
       *          of an arrow key. The large this value the more a single key stroke
       *          will shift the rendering area.
       *          Note that the value is expressed in pixels so that it stays relevant
       *          no matter the zoom level.
       * @return - a value indicating the number of pixels moved when an arrow key is
       *           pressed.
       */
      static
      float
      getArrowKeyMotion() noexcept;

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
       * @brief - Used to retrieve a default value describing how much the repaint
       *          events' areas should be expanded. This process allows to cover for
       *          rounding errors when the tiles are computed and then repainted on
       *          the general canvas. The larger this value the more incertainty can
       *          be erased but if there are consistent imprecisions it might be due
       *          to something deeper so a value too large is not recommended.
       * @return - a value describing how much the areas are expanded when repainting
       *           a computed tile.
       */
      static
      float
      getExpansionThreshold() noexcept;

      /**
       * @brief - Used to return the number of threads to create to process the jobs
       *          related to rendering the fractal.
       * @return - a value used to create the thread pool associated to this renderer.
       */
      static
      unsigned
      getWorkerThreadCount() noexcept;

      /**
       * @brief - Connect signals and build the renderer in a more general way.
       */
      void
      build();

      /**
       * @brief - Used to schedule a rendering with the specified options. This
       *          method issues a request to the threads pool used by this object
       *          to handle the computations.
       *          The user can specify whether this new rendering operation is
       *          meant to invalidate previous results or not. This is useful
       *          for example to process scrolling where we want to keep old tiles
       *          coming as we may scroll faster than the computations can be made.
       *          Note that this function assumes that the locker on the options
       *          is already acquired before calling the method.
       * @param invalidate - `true` if the old jobs should be invalidated (i.e.
       *                     prevented from being notified).
       */
      void
      scheduleRendering(bool invalidate);

      /**
       * @brief - Internal slot used to handle the tiles computed by the thread
       *          pool. The goal is to trigger the creation of the needed repaint
       *          events to display the results of the computation.
       * @param tiles - a list of tiles that just finished rendering.
       */
      void
      handleTilesComputed(const std::vector<utils::AsynchronousJobShPtr>& tiles);

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
       *          Assumes that the locker is already acquired.
       */
      bool
      tilesChanged() const noexcept;

      /**
       * @brief - Used to specify that the tiles have changed and thus that the `m_tex`
       *          texture should be recreated on the next call to `drawContentPrivate`.
       *          Assumes that the locker is already acquired.
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
      convertGlobalToRealWorld(const utils::Vector2f& global);

      /**
       * @brief - Used to expand the input area in all the dimensions as long as the area
       *          stays in the boundaries of this object. This can be used to put some sort
       *          of security margin when performing the repaint.
       * @param area - the area to expand.
       * @return - an expanded version of the area.
       */
      utils::Boxf
      expandByOne(const utils::Boxf& area) const noexcept;

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
      utils::ThreadPoolShPtr m_scheduler;

      /**
       * @brief - Used to keep track of the tiles already rendered so far in the current
       *          rendering operation. This allows to compute some sort of percentage of
       *          completion of the task.
       */
      unsigned m_taskProgress;

      /**
       * @brief - Keep track of the total size of the batch of tasks that were generated
       *          to be scheduled.
       *          Useful in conjunction with the `m_taskProgress` to provide some sort of
       *          completion percentage.
       */
      unsigned m_taskTotal;

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

      /**
       * @brief - Signal emitted whenever a tile has been rendered. The value attached to
       *          the emitted signal corresponds to the percentage of tiles that have been
       *          complete, in the range `[0; 1]`.
       */
      utils::Signal<float> onTileCompleted;
  };

}

# include "FractalRenderer.hxx"

#endif    /* FRACTAL_RENDERER_HH */
