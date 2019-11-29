#ifndef    RENDERING_STATUS_HH
# define   RENDERING_STATUS_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_engine/Color.hh>

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
       * @brief - Used to build the content of this widget so that it can be  
       *          readily displayed.
       */
      void
      build();

    private:

      /**
       * @brief - A mutex to protect the internal properties of this widget.
       */
      mutable std::mutex m_locker;
  };

}

# include "RenderingStatus.hxx"

#endif    /* RENDERING_STATUS_HH */
