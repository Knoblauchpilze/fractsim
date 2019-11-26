#ifndef    OPTIONS_PANEL_HH
# define   OPTIONS_PANEL_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_engine/Color.hh>

namespace fractsim {

  class OptionsPanel: public sdl::core::SdlWidget {
    public:

      ~OptionsPanel() = default;
    
    protected:

      /**
       * @brief - Used to create a base class describing an options panel. This
       *          element is mostly used as a common place to define constants
       *          like policies to use or size of ui elements in order to have
       *          a consistent look across all the options panels.
       * @param name - the name of this options panel.
       * @param labelBgColor - the background color to assign to labels in
       *                        this options panel.
       * @param hint - the size hint for this widget.
       * @param parent - the parent of this widget.
       */
      OptionsPanel(const std::string& name,
                   const sdl::core::engine::Color& labelBgColor,
                   const utils::Sizef& hint = utils::Sizef(),
                   sdl::core::SdlWidget* parent = nullptr);

      /**
       * @brief - Retrieves the default font size to use for labels within this
       *          widget. Only applies to the size of the text used for labels.
       * @return - an integer representing the default font size for this item.
       */
      static
      unsigned
      getLabelDefaultFontSize() noexcept;

      /**
       * @brief - Retrieves the default font name to use for labels within this
       *          widget. This defines the look of the text used to display the
       *          name of each option.
       * @return - an integer representing the default font name for labels.
       */
      static
      const char*
      getLabelDefaultFontName() noexcept;

      /**
       * @brief - Retrieves the default font size to use for values within this
       *          widget. Only applies to the size of the text used for values.
       * @return - an integer representing the default font size for this item.
       */
      static
      unsigned
      getValueDefaultFontSize() noexcept;

      /**
       * @brief - Retrieves the default font name to use for values within this
       *          widget. This defines the look of the text used to display the
       *          value of each option.
       * @return - an integer representing the default font name for values.
       */
      static
      const char*
      getValueDefaultFontName() noexcept;

      /**
       * @brief - Retrieves the color to use as a background for all labels used
       *          in this options panel. Provides an easy way for inheriting classes
       *          to access this value after locking the locker.
       * @return - the background color for labels of this options panel.
       */
      sdl::core::engine::Color
      getLabelsBGColor() const noexcept;

    private:

      /**
       * @brief - A mutex to protect the internal properties of this widget.
       */
      mutable std::mutex m_locker;

      /**
       * @brief - Used to describe the background color to use for the labels
       *          of this options panel.
       */
      sdl::core::engine::Color m_labelsBgColor;
  };

}

# include "OptionsPanel.hxx"

#endif    /* OPTIONS_PANEL_HH */
