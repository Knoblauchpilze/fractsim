#ifndef    NEWTON_OPTIONS_HH
# define   NEWTON_OPTIONS_HH

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

    private:

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