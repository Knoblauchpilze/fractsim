#ifndef    FRACTAL_RENDERER_HH
# define   FRACTAL_RENDERER_HH

# include <mutex>
# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_graphic/ScrollableWidget.hh>
# include "FractalOptions.hh"
# include "RenderingOptions.hh"

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
       * @brief - Used to retrieve the default key to use to reset the rendering
       *          window to its default value.
       * @return - the key to use to restore the rendering window to its default
       *           value.
       */
      static
      sdl::core::engine::RawKey
      getDefaultResetKey() noexcept;

      /**
       * @brief - Used to schedule a rendering with the specified options. This
       *          method issues a request to the threads pool used by this object
       *          to handle the computations.
       *          Note that this function only needs to acquire the locker on the
       *          rendering options in order to copy the internal attributes.
       */
      void
      scheduleRendering();

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
  };

}

# include "FractalRenderer.hxx"

#endif    /* FRACTAL_RENDERER_HH */
