
# include "MandelbrotOptions.hh"
# include <sdl_graphic/GridLayout.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/TextBox.hh>

namespace fractsim {

  MandelbrotOptions::MandelbrotOptions(const utils::Sizef& hint,
                                       sdl::core::SdlWidget* parent):
    OptionsPanel(std::string("mandelbrot_options"),
                 sdl::core::engine::Color::NamedColor::Orange,
                 hint,
                 parent)
  {
    build();
  }

  void
  MandelbrotOptions::build() {
    // Assign a linear layout which will allow positionning items and icons.
    sdl::graphic::GridLayoutShPtr layout = std::make_shared<sdl::graphic::GridLayout>(
      "mandelbrot_options_layout",
      this,
      1u,
      4u
    );

    // And assign the layout to this widget.
    setLayout(layout);

    // We need to register the possible options: each one is composed of a
    // label and of a textbox used to enter the value.
    sdl::graphic::LabelWidget* powerLabel = new sdl::graphic::LabelWidget(
      std::string("power_label"),
      "Power:",
      getLabelDefaultFontName(),
      getLabelDefaultFontSize(),
      sdl::graphic::LabelWidget::HorizontalAlignment::Left,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      this,
      getLabelsBGColor()
    );

    sdl::graphic::TextBox* powerValue = new sdl::graphic::TextBox(
      std::string("power_value"),
      getValueDefaultFontName(),
      std::string(),
      getValueDefaultFontSize(),
      this
    );

    sdl::graphic::LabelWidget* accuracyLabel = new sdl::graphic::LabelWidget(
      std::string("accuracy_label"),
      "Accuracy:",
      getLabelDefaultFontName(),
      getLabelDefaultFontSize(),
      sdl::graphic::LabelWidget::HorizontalAlignment::Left,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      this,
      getLabelsBGColor()
    );

    sdl::graphic::TextBox* accuracyValue = new sdl::graphic::TextBox(
      std::string("accuracy_value"),
      getValueDefaultFontName(),
      std::string(),
      getValueDefaultFontSize(),
      this
    );

    // Deactivate focus for labels.
    powerLabel->setFocusPolicy(sdl::core::FocusPolicy());
    accuracyLabel->setFocusPolicy(sdl::core::FocusPolicy());

    // Add each items to the layout.
    layout->addItem(powerLabel,    0, 0, 1, 1);
    layout->addItem(powerValue,    0, 1, 1, 1);
    layout->addItem(accuracyLabel, 0, 2, 1, 1);
    layout->addItem(accuracyValue, 0, 3, 1, 1);
  }

}
