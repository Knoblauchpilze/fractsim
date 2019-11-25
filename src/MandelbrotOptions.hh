#ifndef    MANDELBROT_OPTIONS_HH
# define   MANDELBROT_OPTIONS_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>

namespace fractsim {

  class MandelbrotOptions: public sdl::core::SdlWidget {
    public:

      /**
       * @brief - Used to create a widget representing the possible options
       *          to draw a `Mandelbrot` set. This mainly includes the power
       *          along with the palette and the accuracy of the computation.
       * @param hint - the size hint for this widget.
       * @param parent - the parent of this widget.
       */
      MandelbrotOptions(const utils::Sizef& hint = utils::Sizef(),
                        sdl::core::SdlWidget* parent = nullptr);

      ~MandelbrotOptions() = default;

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

# include "MandelbrotOptions.hxx"

#endif    /* MANDELBROT_OPTIONS_HH */
