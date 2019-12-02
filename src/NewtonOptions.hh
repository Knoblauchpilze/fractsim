#ifndef    NEWTON_OPTIONS_HH
# define   NEWTON_OPTIONS_HH

# include <string>
# include <maths_utils/Size.hh>
# include "OptionsPanel.hh"

namespace fractsim {

  class NewtonOptions: public OptionsPanel {
    public:

      /**
       * @brief - Used to create a widget representing the possible options
       *          to draw a `Newton` fractal. THis kind of fractal is based
       *          on a polynomial and on finding its root.
       *          The information needed to be able to draw one includes the
       *          coefficient of the polynom to `solve` along with desired
       *          accuracy for the computations.
       * @param maxDegree - the maximum degree of the polynom handled by the
       *                    options panel.
       * @param hint - the size hint for this widget.
       * @param parent - the parent of this widget.
       */
      NewtonOptions(unsigned maxDegree,
                    const utils::Sizef& hint = utils::Sizef(),
                    sdl::core::SdlWidget* parent = nullptr);

      ~NewtonOptions() = default;

      /**
       * @brief - Reimplementation of the base class method to retrieve the options
       *          needed to perform the rendering of a Newton polynomial fractal.
       * @param dummy - unused parameter.
       */
      void
      validateOptions(const std::string& dummy) override;

    private:

      /**
       * @brief - Returns a default name for the textbox receiving the real part of
       *          the coefficient for the input degree.
       * @return - a name for the real part of the coefficient.
       */
      static
      std::string
      getCoefficientRealPartValueName(unsigned degree) noexcept;

      /**
       * @brief - Returns a default name for the textbox receiving the imaginary
       *          part of the coefficient for the input degree.
       * @return - a name for the imaginary part of the coefficient.
       */
      static
      std::string
      getCoefficientImgPartValueName(unsigned degree) noexcept;

      /**
       * @brief - Returns a default name for the accuracy textbox.
       * @return - a name for the accuracy value.
       */
      static
      const char*
      getAccuracyValueName() noexcept;

      /**
       * @brief - Used to create the layout of this options panel. This method
       *          is called right upon building the widget.
       */
      void
      build();

    private:

      /**
       * @brief - Describes the maximum degree of the polynom that can be configured
       *          with the settings available on this panel.
       */
      unsigned m_maxDegree;
  };

}

# include "NewtonOptions.hxx"

#endif    /* NEWTON_OPTIONS_HH */
