#ifndef    JULIA_OPTIONS_HH
# define   JULIA_OPTIONS_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>

namespace fractsim {

  class JuliaOptions: public sdl::core::SdlWidget {
    public:

      /**
       * @brief - Used to create a widget representing the possible options
       *          to draw a `Julia` set. This mainly includes the power, the
       *          constant to add at each step of the series along with the
       *          palette and the accuracy of the computation.
       * @param hint - the size hint for this widget.
       * @param parent - the parent of this widget.
       */
      JuliaOptions(const utils::Sizef& hint = utils::Sizef(),
                   sdl::core::SdlWidget* parent = nullptr);

      ~JuliaOptions() = default;

    private:

      /**
       * @brief - Used to create the layout of this options panel. This method
       *          is called right upon building the widget.
       */
      void
      build();

    private:
  };

}

# include "JuliaOptions.hxx"

#endif    /* JULIA_OPTIONS_HH */
