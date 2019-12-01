
# include "NewtonOptions.hh"
# include <sdl_graphic/GridLayout.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/TextBox.hh>
# include <sdl_core/FocusPolicy.hh>

namespace fractsim {

  NewtonOptions::NewtonOptions(unsigned maxDegree,
                               const utils::Sizef& hint,
                               sdl::core::SdlWidget* parent):
    OptionsPanel(std::string("newton_options"),
                 sdl::core::engine::Color::NamedColor::Orange,
                 hint,
                 parent),

    m_maxDegree(maxDegree)
  {
    build();
  }

  void
  NewtonOptions::validateOptions(const std::string& dummy) {
    // TODO: Implementation.
    log("Should validate newton options", utils::Level::Warning);
  }

  void
  NewtonOptions::build() {
    // Assign a linear layout which will allow positionning items and icons.
    // Note that we need 3 labels for each coefficient of the polynom and 3
    // more for the constant. We also need to set the accuracy of the process
    // which means another 2 elements.
    sdl::graphic::GridLayoutShPtr layout = std::make_shared<sdl::graphic::GridLayout>(
      "newton_options_layout",
      this,
      1u,
      2u + (1u + m_maxDegree) * 3u // Don't forget the constant.
    );

    // And assign the layout to this widget.
    setLayout(layout);

    // We need to register the possible options: each one is composed of a
    // label and of a textbox used to enter the value.
    sdl::graphic::LabelWidget* accuracyLabel = createLabel("accuracy_label", "Accuracy:", this);
    sdl::graphic::TextBox* accuracyValue = createTextBox("accuracy_value", this);

    sdl::graphic::LabelWidget* constantLabel = createLabel("constant_label", "Constant:", this);
    sdl::graphic::TextBox* realPartConstant = createTextBox("constant_real_part", this);
    sdl::graphic::TextBox* imgPartConstant = createTextBox("constant_img_part", this);

    for (unsigned deg = 0u ; deg < m_maxDegree ; ++deg) {
      std::string degStr = std::to_string(deg + 1u);

      sdl::graphic::LabelWidget* labelDeg = createLabel(
        "deg_" + degStr + "_label",
        "Degree " + degStr + ":",
        this
      );

      sdl::graphic::TextBox* realPartDeg = createTextBox(
        "deg_" + degStr + "_real_part",
        this
      );
      sdl::graphic::TextBox* imgPartDeg = createTextBox(
        "deg_" + degStr + "_img_part",
        this
      );

      // Each degree is added sequentially after the accuracy
      // elements and the constant elements.
      layout->addItem(labelDeg,    0, 5u + 3u * deg + 0u, 1, 1);
      layout->addItem(realPartDeg, 0, 5u + 3u * deg + 1u, 1, 1);
      layout->addItem(imgPartDeg,  0, 5u + 3u * deg + 2u, 1, 1);
    }

    layout->addItem(accuracyLabel, 0, 0, 1, 1);
    layout->addItem(accuracyValue, 0, 1, 1, 1);

    layout->addItem(constantLabel,    0, 2, 1, 1);
    layout->addItem(realPartConstant, 0, 3, 1, 1);
    layout->addItem(imgPartConstant,  0, 4, 1, 1);
  }

}
