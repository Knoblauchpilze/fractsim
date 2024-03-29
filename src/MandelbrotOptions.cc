
# include "MandelbrotOptions.hh"
# include <sstream>
# include <iomanip>
# include <sdl_graphic/GridLayout.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/TextBox.hh>
# include "MandelbrotRenderingOptions.hh"

namespace fractsim {

  MandelbrotOptions::MandelbrotOptions(const utils::Sizef& hint,
                                       sdl::core::SdlWidget* parent):
    OptionsPanel(std::string("mandelbrot_options"),
                 sdl::core::engine::Color::NamedColor::Orange,
                 hint,
                 parent),

    m_propsLocker()
  {
    build();
  }

  void
  MandelbrotOptions::validateOptions(const std::string& /*dummy*/) {
    // Check whether the options are visible.
    if (!isVisible()) {
      return;
    }

    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Retrieve the children for each value to retrieve.
    sdl::graphic::TextBox* accuracyTB = getChildAs<sdl::graphic::TextBox>(getAccuracyValueName());
    sdl::graphic::TextBox* powerTB = getChildAs<sdl::graphic::TextBox>(getPowerValueName());

    // Retrieve and convert the values to build the options.
    std::string accuracyText = accuracyTB->getValue();
    std::string powerText = powerTB->getValue();
    bool converted = false;

    unsigned accuracy = utils::convert(accuracyText, getDefaultAccuracy(), converted);
    float power = utils::convert(powerText, getDefaultPower(), converted);

    MandelbrotRenderingOptionsShPtr opt = std::make_shared<MandelbrotRenderingOptions>();
    opt->setAccuracy(accuracy);
    opt->setExponent(power);

    onOptionsChanged.safeEmit(
      std::string("onOptionsChanged(") + std::to_string(accuracy) + ", " + std::to_string(power) + ")",
      opt
    );
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
    sdl::graphic::LabelWidget* accuracyLabel = createLabel("accuracy_label", "Accuracy:", this);
    sdl::graphic::TextBox* accuracyValue = createTextBox(getAccuracyValueName(), this);

    sdl::graphic::LabelWidget* powerLabel = createLabel("power_label", "Power:", this);
    sdl::graphic::TextBox* powerValue = createTextBox(getPowerValueName(), this);

    // Add each item to the layout.
    layout->addItem(accuracyLabel, 0, 0, 1, 1);
    layout->addItem(accuracyValue, 0, 1, 1, 1);
    layout->addItem(powerLabel,    0, 2, 1, 1);
    layout->addItem(powerValue,    0, 3, 1, 1);

    // Assign default values to elements.
    initElements();
  }

  void
  MandelbrotOptions::initElements() {
    // Retrieve elements.
    sdl::graphic::TextBox* accuracyTB = getChildAs<sdl::graphic::TextBox>(getAccuracyValueName());
    sdl::graphic::TextBox* powerTB = getChildAs<sdl::graphic::TextBox>(getPowerValueName());

    // Assign default values.
    std::stringstream formatter;
    formatter << std::fixed << std::setprecision(0);

    formatter << getDefaultPower();
    powerTB->setValue(formatter.str());

    formatter.str("");
    formatter.clear();
    formatter << getDefaultAccuracy();
    accuracyTB->setValue(formatter.str());
  }

}
