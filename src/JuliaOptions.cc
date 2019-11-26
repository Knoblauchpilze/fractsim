
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
    sdl::graphic::LabelWidget* realPartLabel = createLabel("real_part_label", "Power:", this);
    sdl::graphic::TextBox* realPartValue = createTextBox("real_part_value", this);

    sdl::graphic::LabelWidget* imgPartLabel = createLabel("img_part_label", "Power:", this);
    sdl::graphic::TextBox* imgPartValue = createTextBox("img_part_value", this);

    sdl::graphic::LabelWidget* accuracyLabel = createLabel("accuracy_label", "Accuracy:", this);
    sdl::graphic::TextBox* accuracyValue = createTextBox("accuracy_value", this);

    // Add each items to the layout.
    layout->addItem(accuracyLabel, 0, 0, 1, 1);
    layout->addItem(accuracyValue, 0, 1, 1, 1);
    layout->addItem(realPartLabel, 0, 2, 1, 1);
    layout->addItem(realPartValue, 0, 3, 1, 1);
    layout->addItem(imgPartLabel,  0, 4, 1, 1);
    layout->addItem(imgPartValue,  0, 5, 1, 1);
  }

}
