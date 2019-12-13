#ifndef    OPTIONS_PANEL_HH
# define   OPTIONS_PANEL_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_engine/Color.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/TextBox.hh>
# include "FractalOptions.hh"

namespace fractsim {

  class OptionsPanel: public sdl::core::SdlWidget {
    public:

      ~OptionsPanel() = default;

      /**
       * @brief - Interface method used when the options registered in this panel
       *          so that a `onOptionsChanged` signal can be emitted. The goal of
       *          this method is to be able to verify the content of the options
       *          defined and build the corresponding fractal settings.
       *          Inheriting classes must reimplement this method.
       * @param dummy - a dummy parameter, representing the name of the caller of
       *                this method. Should not be needed for any process.
       */
      virtual void
      validateOptions(const std::string& dummy) = 0;

      /**
       * @brief - Interface method used when the options defined in the panel are
       *          to be reset. This allows for easy 'go-back-to-safe-state' behavior
       *          that can be controlled by the user.
       * @param dummy - a dummy parameter, representing the name of the caller of
       *                this method. Should not be needed for any process.
       */
      virtual void
      resetOptions(const std::string& dummy) = 0;

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
      virtual unsigned
      getLabelDefaultFontSize() const noexcept;

      /**
       * @brief - Retrieves the default font name to use for labels within this
       *          widget. This defines the look of the text used to display the
       *          name of each option.
       * @return - an integer representing the default font name for labels.
       */
      virtual const char*
      getLabelDefaultFontName() const noexcept;

      /**
       * @brief - Retrieves the maximum height for a label created through the
       *          `createLabel` method. Allows for inheriting classes to define
       *          some other value if needed.
       * @return - a value describing the maximum height of a label element in
       *           this options panel.
       */
      virtual float
      getLabelMaxHeight() const noexcept;

      /**
       * @brief - Retrieves the default font size to use for values within this
       *          widget. Only applies to the size of the text used for values.
       * @return - an integer representing the default font size for this item.
       */
      virtual unsigned
      getValueDefaultFontSize() const noexcept;

      /**
       * @brief - Retrieves the default font name to use for values within this
       *          widget. This defines the look of the text used to display the
       *          value of each option.
       * @return - an integer representing the default font name for values.
       */
      virtual const char*
      getValueDefaultFontName() const noexcept;

      /**
       * @brief - Retrieves the maximum height for a text box created through the
       *          `createTextBox` method. Allows for inheriting classes to define
       *          some other value if needed.
       * @return - a value describing the maximum height of a text box element in
       *           this options panel.
       */
      virtual float
      getValueMaxHeight() const noexcept;

      /**
       * @brief - Retrieves the color to use as a background for all labels used
       *          in this options panel. Provides an easy way for inheriting classes
       *          to access this value after locking the locker.
       * @return - the background color for labels of this options panel.
       */
      sdl::core::engine::Color
      getLabelsBGColor() const noexcept;

      /**
       * @brief - Used to create a label widget with the specified text and name.
       *          This allows inheriting classes to easily create labels for their
       *          own purposes.
       *          Note that the alignment will be set to default values and that
       *          the fonts and size will be picked from the interface methods.
       *          Also the focus policy will be set to none (meaning that the item
       *          won't be able to receive the focus).
       * @param name - the name of the label to create.
       * @param text - the text to set for this label.
       * @param parent - the parent of the label to create.
       * @return - the corresponding label widget.
       */
      sdl::graphic::LabelWidget*
      createLabel(const std::string& name,
                  const std::string& text,
                  sdl::core::SdlWidget* parent) const noexcept;

      /**
       * @brief - Used to create a text box widget with the specified name. This
       *          allows inheriting classes to easily create labels for their own
       *          purposes.
       *          Note that the initial text will be left empty and that the fonts
       *          and size will be picked from the interface methods.
       * @param name - the name of the text box to create.
       * @param parent - the parent of the label to create.
       * @return - the corresponding label widget.
       */
      sdl::graphic::TextBox*
      createTextBox(const std::string& name,
                    sdl::core::SdlWidget* parent) noexcept;

      /**
       * @brief - Used to convert the input text to an unsigned value. In case it
       *          cannot be converted, the provided default value is used.
       *          The user can be notified whether the conversion was successful.
       * @param text - the text to convert.
       * @param def - the default value to use in case the conversion fails.
       * @param converted - output value indicating whether the conversion could
       *                    be performed.
       * @return - the converted value or the default one.
       */
      unsigned
      convertToUnsigned(const std::string& text,
                        unsigned def,
                        bool& converted) const noexcept;

      /**
       * @brief - Similar method to `convertToFloat` but for floating point values.
       * @param def - the default value to use in case the conversion fails.
       * @param converted - output value indicating whether the conversion could
       *                    be performed.
       * @return - the converted value or the default one.
       */
      float
      convertToFloat(const std::string& text,
                     float def,
                     bool& converted) const noexcept;

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

    public:

      /**
       * @brief - This signal can be used for external objects to register whenever
       *          the options for this panel are modified.
       */
      utils::Signal<FractalOptionsShPtr> onOptionsChanged;
  };

}

# include "OptionsPanel.hxx"

#endif    /* OPTIONS_PANEL_HH */
