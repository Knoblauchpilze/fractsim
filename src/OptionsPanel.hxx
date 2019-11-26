#ifndef    OPTIONS_PANEL_HXX
# define   OPTIONS_PANEL_HXX

# include "OptionsPanel.hh"

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
  OptionsPanel::getLabelDefaultFontSize() noexcept {
    return 15u;
  }

  inline
  const char*
  OptionsPanel::getLabelDefaultFontName() noexcept {
    return "data/fonts/Goodtime.ttf";
  }

  inline
  unsigned
  OptionsPanel::getValueDefaultFontSize() noexcept {
    return 35u;
  }

  inline
  const char*
  OptionsPanel::getValueDefaultFontName() noexcept {
    return "data/fonts/times.ttf";
  }

  inline
  sdl::core::engine::Color
  OptionsPanel::getLabelsBGColor() const noexcept {
    Guard guard(m_locker);

    return m_labelsBgColor;
  }

}

#endif    /* OPTIONS_PANEL_HXX */
