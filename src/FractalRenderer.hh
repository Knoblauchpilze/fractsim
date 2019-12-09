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

      ~FractalRenderer() = default;

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
       * @brief - Reimplementation of the base class method to be able to provide a
       *          locking behavior whenever this method is called. This allows to
       *          ensure thread safety when processing events as the rendering of the
       *          thread pool might issue posting repaint events anytime.
       *          It also means that most of the methods requiring a lock on this
       *          object's properties do not need to care about acquiring the lock as
       *          it is most probably already acquired (given that most behaviors are
       *          tied to some event being processed).
       * @param e - the event to handle.
       * @return - `true` if the event was recognized, false otherwise.
       */
      bool
      handleEvent(sdl::core::engine::EventShPtr e) override;

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
  };

}

# include "FractalRenderer.hxx"

#endif    /* FRACTAL_RENDERER_HH */
