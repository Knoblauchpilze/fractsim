
# include "JuliaOptions.hh"
# include <sstream>
# include <iomanip>
# include <sdl_graphic/GridLayout.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/TextBox.hh>
# include <sdl_core/FocusPolicy.hh>
# include "JuliaRenderingOptions.hh"

namespace fractsim {

  JuliaOptions::JuliaOptions(const utils::Sizef& hint,
                             sdl::core::SdlWidget* parent):
    OptionsPanel(std::string("julia_options"),
                 sdl::core::engine::Color::NamedColor::Orange,
                 hint,
                 parent),

    m_propsLocker()
  {
    build();
  }

  void
  JuliaOptions::validateOptions(const std::string& /*dummy*/) {
    // Check whether the options are visible.
    if (!isVisible()) {
      return;
    }

    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Retrieve the children for each value to retrieve.
    sdl::graphic::TextBox* accuracyTB = getChildAs<sdl::graphic::TextBox>(getAccuracyValueName());
    sdl::graphic::TextBox* realTB = getChildAs<sdl::graphic::TextBox>(getConstantRealPartValueName());
    sdl::graphic::TextBox* imgTB = getChildAs<sdl::graphic::TextBox>(getConstantImgPartValueName());

    // Retrieve and convert the values to build the options.
    std::string accuracyText = accuracyTB->getValue();
    std::string realText = realTB->getValue();
    std::string imgText = imgTB->getValue();
    bool converted = false;

    unsigned accuracy = utils::convert(accuracyText, getDefaultAccuracy(), converted);
    float realC = utils::convert(realText, getDefaultRealPartConstant(), converted);
    float imgC = utils::convert(imgText, getDefaultImgPartConstant(), converted);
    utils::Vector2f c(realC, imgC);

    JuliaRenderingOptionsShPtr opt = std::make_shared<JuliaRenderingOptions>();
    opt->setAccuracy(accuracy);
    opt->setConstant(c);

    onOptionsChanged.safeEmit(
      std::string("onOptionsChanged(") + std::to_string(accuracy) + ", " + c.toString() + ")",
      opt
    );
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
    sdl::graphic::LabelWidget* accuracyLabel = createLabel("accuracy_label", "Accuracy:", this);
    sdl::graphic::TextBox* accuracyValue = createTextBox(getAccuracyValueName(), this);

    sdl::graphic::LabelWidget* realPartLabel = createLabel("real_part_label", "Real part:", this);
    sdl::graphic::TextBox* realPartValue = createTextBox(getConstantRealPartValueName(), this);

    sdl::graphic::LabelWidget* imgPartLabel = createLabel("img_part_label", "Imaginary part:", this);
    sdl::graphic::TextBox* imgPartValue = createTextBox(getConstantImgPartValueName(), this);

    // Add each item to the layout.
    layout->addItem(accuracyLabel, 0, 0, 1, 1);
    layout->addItem(accuracyValue, 0, 1, 1, 1);
    layout->addItem(realPartLabel, 0, 2, 1, 1);
    layout->addItem(realPartValue, 0, 3, 1, 1);
    layout->addItem(imgPartLabel,  0, 4, 1, 1);
    layout->addItem(imgPartValue,  0, 5, 1, 1);

    // Assign default values to elements.
    initElements();
  }

  void
  JuliaOptions::initElements() {
    // Retrieve elements.
    sdl::graphic::TextBox* accuracyTB = getChildAs<sdl::graphic::TextBox>(getAccuracyValueName());
    sdl::graphic::TextBox* realTB = getChildAs<sdl::graphic::TextBox>(getConstantRealPartValueName());
    sdl::graphic::TextBox* imgTB = getChildAs<sdl::graphic::TextBox>(getConstantImgPartValueName());

    // Assign default values.
    std::stringstream formatter;
    formatter << std::fixed << std::setprecision(1);

    formatter << getDefaultRealPartConstant();
    realTB->setValue(formatter.str());

    formatter.str("");
    formatter.clear();
    formatter << getDefaultImgPartConstant();
    imgTB->setValue(formatter.str());

    formatter.str("");
    formatter.clear();
    formatter << std::fixed << std::setprecision(0);
    formatter << getDefaultAccuracy();
    accuracyTB->setValue(formatter.str());
  }

}
