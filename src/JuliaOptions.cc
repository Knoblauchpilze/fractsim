
# include "JuliaOptions.hh"
# include <sdl_graphic/GridLayout.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/TextBox.hh>
# include <sdl_core/FocusPolicy.hh>

namespace fractsim {

  JuliaOptions::JuliaOptions(const utils::Sizef& hint,
                             sdl::core::SdlWidget* parent):
    OptionsPanel(std::string("julia_options"),
                 sdl::core::engine::Color::NamedColor::Orange,
                 hint,
                 parent)
  {
    build();
  }

  void
  JuliaOptions::build() {
    // Assign a linear layout which will allow positionning items and icons.
    sdl::graphic::GridLayoutShPtr layout = std::make_shared<sdl::graphic::GridLayout>(
      "julia_options_layout",
      this,
      1u,
      6u
    );

    // And assign the layout to this widget.
    setLayout(layout);

    // We need to register the possible options: each one is composed of a
    // label and of a textbox used to enter the value.
    sdl::graphic::LabelWidget* realPartLabel = new sdl::graphic::LabelWidget(
      std::string("real_part_label"),
      "Real part:",
      getLabelDefaultFontName(),
      getLabelDefaultFontSize(),
      sdl::graphic::LabelWidget::HorizontalAlignment::Left,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      this,
      getLabelsBGColor()
    );

    sdl::graphic::TextBox* realPartValue = new sdl::graphic::TextBox(
      std::string("real_part_value"),
      getValueDefaultFontName(),
      std::string(),
      getValueDefaultFontSize(),
      this
    );

    sdl::graphic::LabelWidget* imgPartLabel = new sdl::graphic::LabelWidget(
      std::string("img_part_label"),
      "Imaginary part:",
      getLabelDefaultFontName(),
      getLabelDefaultFontSize(),
      sdl::graphic::LabelWidget::HorizontalAlignment::Left,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      this,
      getLabelsBGColor()
    );

    sdl::graphic::TextBox* imgPartValue = new sdl::graphic::TextBox(
      std::string("img_part_value"),
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
    realPartLabel->setFocusPolicy(sdl::core::FocusPolicy());
    imgPartLabel->setFocusPolicy(sdl::core::FocusPolicy());
    accuracyLabel->setFocusPolicy(sdl::core::FocusPolicy());

    // Add each items to the layout.
    layout->addItem(realPartLabel, 0, 0, 1, 1);
    layout->addItem(realPartValue, 0, 1, 1, 1);
    layout->addItem(imgPartLabel,  0, 2, 1, 1);
    layout->addItem(imgPartValue,  0, 3, 1, 1);
    layout->addItem(accuracyLabel, 0, 4, 1, 1);
    layout->addItem(accuracyValue, 0, 5, 1, 1);
  }

}
