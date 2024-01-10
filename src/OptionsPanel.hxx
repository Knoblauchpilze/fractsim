#ifndef    OPTIONS_PANEL_HXX
# define   OPTIONS_PANEL_HXX

# include "OptionsPanel.hh"
# include <sdl_graphic/Validator_utils.hxx>

namespace fractsim {

  inline
  OptionsPanel::OptionsPanel(const std::string& name,
                             const sdl::core::engine::Color& labelBgColor,
                             const utils::Sizef& hint,
                             sdl::core::SdlWidget* parent):
    sdl::core::SdlWidget(name,
                         hint,
                         parent),

    m_labelsBgColor(labelBgColor)
  {}

  inline
  unsigned
  OptionsPanel::getLabelDefaultFontSize() const noexcept {
    return 15u;
  }

  inline
  const char*
  OptionsPanel::getLabelDefaultFontName() const noexcept {
    return "data/fonts/Goodtime.ttf";
  }

  inline
  float
  OptionsPanel::getLabelMaxHeight() const noexcept {
    return 50.0f;
  }

  inline
  unsigned
  OptionsPanel::getValueDefaultFontSize() const noexcept {
    return 35u;
  }

  inline
  const char*
  OptionsPanel::getValueDefaultFontName() const noexcept {
    return "data/fonts/times.ttf";
  }

  inline
  float
  OptionsPanel::getValueMaxHeight() const noexcept {
    return 60.0f;
  }

  inline
  sdl::core::engine::Color
  OptionsPanel::getLabelsBGColor() const noexcept {
    const std::lock_guard guard(m_locker);

    return m_labelsBgColor;
  }

  inline
  sdl::graphic::LabelWidget*
  OptionsPanel::createLabel(const std::string& name,
                            const std::string& text,
                            sdl::core::SdlWidget* parent) const noexcept
  {
    sdl::graphic::LabelWidget* label = new sdl::graphic::LabelWidget(
      name,
      text,
      getLabelDefaultFontName(),
      getLabelDefaultFontSize(),
      sdl::graphic::LabelWidget::HorizontalAlignment::Left,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      parent,
      getLabelsBGColor()
    );

    label->setFocusPolicy(sdl::core::FocusPolicy());
    label->setMaxSize(
      utils::Sizef(
        std::numeric_limits<float>::max(),
        getLabelMaxHeight()
      )
    );

    return label;
  }

  inline
  sdl::graphic::TextBox*
  OptionsPanel::createTextBox(const std::string& name,
                              sdl::core::SdlWidget* parent) noexcept
  {
    sdl::graphic::TextBox* textBox = new sdl::graphic::TextBox(
      name,
      getValueDefaultFontName(),
      std::string(),
      getValueDefaultFontSize(),
      parent
    );

    textBox->setMaxSize(
      utils::Sizef(
        std::numeric_limits<float>::max(),
        getValueMaxHeight()
      )
    );

    // Connect the value changed signal to the options internal handling slot.
    textBox->onValueChanged.connect_member<OptionsPanel>(
      this,
      &OptionsPanel::validateOptions
    );

    return textBox;
  }

}

#endif    /* OPTIONS_PANEL_HXX */
