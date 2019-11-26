
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
    sdl::graphic::LabelWidget* powerLabel = createLabel("power_label", "Power:", this);
    sdl::graphic::TextBox* powerValue = createTextBox("power_value", this);

    sdl::graphic::LabelWidget* accuracyLabel = createLabel("accuracy_label", "Accuracy:", this);
    sdl::graphic::TextBox* accuracyValue = createTextBox("accuracy_value", this);

    // Add each items to the layout.
    layout->addItem(accuracyLabel, 0, 0, 1, 1);
    layout->addItem(accuracyValue, 0, 1, 1, 1);
    layout->addItem(powerLabel,    0, 2, 1, 1);
    layout->addItem(powerValue,    0, 3, 1, 1);
  }

}
