
# include "NewtonOptions.hh"
# include <sstream>
# include <iomanip>
# include <sdl_graphic/GridLayout.hh>
# include <sdl_graphic/LabelWidget.hh>
# include <sdl_graphic/TextBox.hh>
# include <sdl_core/FocusPolicy.hh>
# include "NewtonRenderingOptions.hh"

namespace fractsim {

  NewtonOptions::NewtonOptions(unsigned maxDegree,
                               const utils::Sizef& hint,
                               sdl::core::SdlWidget* parent):
    OptionsPanel(std::string("newton_options"),
                 sdl::core::engine::Color::NamedColor::Orange,
                 hint,
                 parent),

    m_propsLocker(),
    m_maxDegree(maxDegree)
  {
    build();
  }

  void
  NewtonOptions::validateOptions(const std::string& /*dummy*/) {
    // Check whether the options are visible.
    if (!isVisible()) {
      return;
    }

    // Protect from concurrent accesses.
    Guard guard(m_propsLocker);

    // Retrieve the children for each value to retrieve.
    sdl::graphic::TextBox* accuracyTB = getChildAs<sdl::graphic::TextBox>(getAccuracyValueName());

    // Retrieve and convert the values to build the options.
    std::string accuracyText = accuracyTB->getValue();
    bool converted = false;
    std::vector<NewtonRenderingOptions::Coefficient> coeffs;

    for (unsigned deg = 0u ; deg <= m_maxDegree ; ++deg) {
      sdl::graphic::TextBox* realTB = getChildAs<sdl::graphic::TextBox>(
        getCoefficientRealPartValueName(deg)
      );
      sdl::graphic::TextBox* imgTB = getChildAs<sdl::graphic::TextBox>(
        getCoefficientImgPartValueName(deg)
      );

      std::string realText = realTB->getValue();
      std::string imgText = imgTB->getValue();

      float realC = utils::convert(realText, getDefaultRealPartCoefficient(deg), converted);
      float imgC = utils::convert(imgText, getDefaultImgPartCoefficient(deg), converted);

      coeffs.push_back(NewtonRenderingOptions::Coefficient{1.0f * deg, utils::Vector2f(realC, imgC)});
    }

    unsigned accuracy = utils::convert(accuracyText, getDefaultAccuracy(), converted);

    NewtonRenderingOptionsShPtr opt = std::make_shared<NewtonRenderingOptions>(coeffs);
    opt->setAccuracy(accuracy);

    onOptionsChanged.safeEmit(
      std::string("onOptionsChanged(") + std::to_string(accuracy) + ", " + std::to_string(m_maxDegree) + ")",
      opt
    );
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
    sdl::graphic::TextBox* realPartConstant = createTextBox(getCoefficientRealPartValueName(0u), this);
    sdl::graphic::TextBox* imgPartConstant = createTextBox(getCoefficientImgPartValueName(0u), this);

    for (unsigned deg = 0u ; deg < m_maxDegree ; ++deg) {
      std::string degStr = std::to_string(deg + 1u);

      sdl::graphic::LabelWidget* labelDeg = createLabel(
        "deg_" + degStr + "_label",
        "Degree " + degStr + ":",
        this
      );

      sdl::graphic::TextBox* realPartDeg = createTextBox(
        getCoefficientRealPartValueName(deg + 1u),
        this
      );
      sdl::graphic::TextBox* imgPartDeg = createTextBox(
        getCoefficientImgPartValueName(deg + 1u),
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

    // Assign default values to elements.
    initElements();
  }

  void
  NewtonOptions::initElements() {
    // Unlike other sets of options, we have to retrieve and set elements
    // at the same time.
    sdl::graphic::TextBox* accuracyTB = getChildAs<sdl::graphic::TextBox>(getAccuracyValueName());

    std::stringstream formatter;
    formatter << std::fixed << std::setprecision(0);
    formatter << getDefaultAccuracy();
    accuracyTB->setValue(formatter.str());

    formatter << std::fixed << std::setprecision(1);

    for (unsigned deg = 0u ; deg <= m_maxDegree ; ++deg) {
      sdl::graphic::TextBox* realTB = getChildAs<sdl::graphic::TextBox>(
        getCoefficientRealPartValueName(deg)
      );
      sdl::graphic::TextBox* imgTB = getChildAs<sdl::graphic::TextBox>(
        getCoefficientImgPartValueName(deg)
      );

      // Assign default values.
      formatter.str("");
      formatter.clear();
      formatter << getDefaultRealPartCoefficient(deg);
      realTB->setValue(formatter.str());

      formatter.str("");
      formatter.clear();
      formatter << getDefaultImgPartCoefficient(deg);
      imgTB->setValue(formatter.str());
    }
  }

}
