#ifndef    FRACTAL_RENDERER_HH
# define   FRACTAL_RENDERER_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_graphic/ScrollableWidget.hh>

namespace fractsim {

  class FractalRenderer: public sdl::graphic::ScrollableWidget {
    public:

      /**
       * @brief - Creates a fractal renderer with the specified size hint and
       *          parent widget. This widget is scrollable (meaning the user
       *          can scroll its content).
       * @param hint - the size hint for this widget.
       * @param parent - the parent widget for this element.
       */
      FractalRenderer(const utils::Sizef& sizeHint = utils::Sizef(),
                      sdl::core::SdlWidget* parent = nullptr);

      ~FractalRenderer() = default;
  };

}

#endif    /* FRACTAL_RENDERER_HH */
