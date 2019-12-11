#ifndef    STATUS_BAR_HH
# define   STATUS_BAR_HH

# include <sdl_core/SdlWidget.hh>
# include <maths_utils/Box.hh>
# include <maths_utils/Vector2.hh>
# include <sdl_graphic/LabelWidget.hh>

namespace fractsim {

  class StatusBar: public sdl::core::SdlWidget {
    public:

      /**
       * @brief - Used to create a status bar widget allowing to display the mouse
       *          coordinates in real world coordinate frame and an indication of
       *          the current rendering area.
       *          This is helpful to provide more context to the user about what is
       *          currently displayed in the renderer.
       * @param hint - the size hint for this widget.
       * @param parent - the parent of this widget.
       */
      StatusBar(const utils::Sizef& hint = utils::Sizef(),
                sdl::core::SdlWidget* parent = nullptr);

      ~StatusBar() = default;

      /**
       * @brief - Used to connect external elements which have built-in knowlegde of
       *          the rendering area displayed in the renderer. This information needs
       *          to be passed on to this widget so that we can update the internal
       *          item displaying this information.
       * @param area - the new rendering area associated to the renderer.
       */
      void
      onRenderingAreaChanged(utils::Boxf area);

      /**
       * @brief - Used to connect to external elements which can provide information
       *          on the current position of the mouse. This information can be passed
       *          on to this widget so that we can update the corresponding label.
       *          Note that the input position is assumed to represent the position of
       *          the mouse in real world coordinate frame.
       * @param p - the current position pointed to by the mouse.
       */
      void
      onMouseCoordsChanged(utils::Vector2f p);

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
       * @brief - Defines the font to use for the labels displayed in this status
       *          bar. THis includes both the current mouse position and the area
       *          defined for rendering.
       * @return - a name identifying the font to use for the information panels.
       */
      static
      const char*
      getInfoLabelFont() noexcept;

      /**
       * @brief - Used to defnie the margins of the layout applied around the whole
       *          widget.
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
       * @brief - Used to retrieve the default name for the label displaying mouse
       *          coordinates in real world frame.
       * @return - a string that should be used to provide consistent naming for
       *           the mouse coordinates label.
       */
      static
      const char*
      getMouseCoordsLabelName() noexcept;

      /**
       * @brief - Used to retrieve the default name for the rendering area label.
       * @return - a string that should be used to provide consistent naming for
       *           the rendering area label.
       */
      static
      const char*
      getRenderingAreaLabelName() noexcept;

      /**
       * @brief - Used to build the content of this widget so that it can be  
       *          readily displayed.
       */
      void
      build();

      /**
       * @brief - Used to retrieve the label widget holding the mouse coordinates
       *          in real world frame. Typically used when the coordinates need to
       *          be changed/updated.
       *          The return value is guaranteed to be not `null` if the method
       *          returns. Note that the locker is assumed to already be acquired
       *          upon calling this function.
       * @return - the label widget associated to the mouse coordinates.
       */
      sdl::graphic::LabelWidget*
      getMouseCoordsLabel();

      /**
       * @brief - Used to retrieve the label displaying the current rendering area
       *          associated to the renderer. The return value is guaranteed to be
       *          not `null` if the method returns. Note that the locker is assumed
       *          to already be acquired upon calling this function.
       * @return - the label displaying the rendering area.
       */
      sdl::graphic::LabelWidget*
      getRenderingAreaLabel();

    private:

      /**
       * @brief - A mutex to protect the internal properties of this widget.
       */
      mutable std::mutex m_propsLocker;
  };

}

# include "StatusBar.hxx"

#endif    /* STATUS_BAR_HH */
