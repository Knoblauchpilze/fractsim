#ifndef    RENDERING_STATUS_HH
# define   RENDERING_STATUS_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_engine/Color.hh>
# include <sdl_graphic/Button.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/ProgressBar.hh>

namespace fractsim {

  class RenderingStatus: public sdl::core::SdlWidget {
    public:

      /**
       * @brief - Used to create a rendering status widget allowing to display
       *          information about the current rendering being performed in
       *          the application or general information about the fractal being
       *          drawn.
       * @param hint - the size hint for this widget.
       * @param parent - the parent of this widget.
       */
      RenderingStatus(const utils::Sizef& hint = utils::Sizef(),
                      sdl::core::SdlWidget* parent = nullptr);

      ~RenderingStatus() = default;

      /**
       * @brief - Used to retrive the render button associated to this status.
       *          This method is only meant as a way to connect elements to the
       *          `onClick` signal of this button, lacking of a better way.
       *          The method may raise an error in case the render button is
       *          not defined.
       * @return - a reference to the render button associated to this status.
       */
      sdl::graphic::Button&
      getRenderButton();

      /**
       * @brief - Similar to the `getRenderButton` method but used to retrieve
       *          the reset button. Still not perfect in terms of design but it
       *          does the job.
       * @return - a reference to the reset button associated to this status.
       */
      sdl::graphic::Button&
      getResetButton();

      /**
       * @brief - Used to connect external elements which can provide information
       *          about the current completion of the rendering task.
       * @param perc - a value in the range `[0; 1]` indicating the completion
       *               percentage for the current rendering task.
       */
      void
      onCompletionChanged(float perc);

    protected:

      /**
       * @brief - Retrieves the maximum height for this component. This is usually
       *          assigned at construction and considered enough to display all the
       *          info needed.
       * @return - a value describing the maximum height of this component.
       */
      static
      float
      getStatusMaxHeight() noexcept;

      /**
       * @brief - Retrieves the maximum size for the render button. As we only want
       *          the button to be large enough so that the text is fully visible it
       *          is not needed to grow indefinitely.
       * @return - a value describing the maximum size for the render button.
       */
      static
      utils::Sizef
      getRenderButtonMaxSize() noexcept;

      /**
       * @brief - Retrieves the maximum size for the reset button. Very similar to
       *          the purpose achieved by the `getRenderButtonMaxSize`.
       * @return - a value describing the maximum size of the reset button.
       */
      static
      utils::Sizef
      getResetButtonMaxSize() noexcept;

      /**
       * @brief - Used to defnie the margins of the layout applied around the whole
       *          widget. Usually `0`.
       * @return - a value representing the global margins for this widget.
       */
      static
      float
      getGlobalMargins() noexcept;

      /**
       * @brief - Used to define the margins between the component of this status.
       * @return - a value representing the margins between each component of the
       *           widget.
       */
      static
      float
      getComponentMargins() noexcept;

      /**
       * @brief - Used to retrieve the default name for the render button.
       * @return - a string that should be used to provide consistent naming for
       *           the render button.
       */
      static
      const char*
      getRenderButtonName() noexcept;

      /**
       * @brief - Similar to the `getRenderButtonName` but retrieves the default
       *          name to use for the reset button.
       * @return - a string that should be used to provide consistent naming for
       *           the reset button.
       */
      static
      const char*
      getResetButtonName() noexcept;

      /**
       * @brief - Used to retrieve the default name for the progress bar.
       * @return - a string that should be used to provide consistent naming for
       *           the progress bar.
       */
      static
      const char*
      getProgressBarName() noexcept;

      /**
       * @brief - Retrieve a font name that can be used for buttons in this widget.
       * @return - a string representing the font to use for labels of buttons in
       *           this widget.
       */
      static
      const char*
      getButtonFontName() noexcept;

      /**
       * @brief - Used to build the content of this widget so that it can be  
       *          readily displayed.
       */
      void
      build();

      /**
       * @brief - Used to retrieve the progress bar holding the current completion
       *          percentage of the rendering task monitored by this object.
       *          The return value is guaranteed to be not `null` if the method
       *          returns. Note that the locker is assumed to already be acquired
       *          upon calling this function.
       * @return - the progress bar associated to this status.
       */
      sdl::graphic::ProgressBar*
      getProgressBar();

    private:

      /**
       * @brief - A mutex to protect the internal properties of this widget.
       */
      mutable std::mutex m_propsLocker;
  };

}

# include "RenderingStatus.hxx"

#endif    /* RENDERING_STATUS_HH */
