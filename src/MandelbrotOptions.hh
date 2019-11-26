#ifndef    MANDELBROT_OPTIONS_HH
# define   MANDELBROT_OPTIONS_HH

# include <maths_utils/Size.hh>
# include <sdl_core/SdlWidget.hh>
# include <sdl_engine/Color.hh>

namespace fractsim {

  class MandelbrotOptions: public sdl::core::SdlWidget {
    public:

      /**
       * @brief - Used to create a widget representing the possible options
       *          to draw a `Mandelbrot` set. This mainly includes the power
       *          along with the palette and the accuracy of the computation.
       * @param hint - the size hint for this widget.
       * @param parent - the parent of this widget.
       * @param labelBgColor - the background color to assign to labels in
       *                        this options panel.
       */
      MandelbrotOptions(const utils::Sizef& hint = utils::Sizef(),
                        sdl::core::SdlWidget* parent = nullptr,
                        const sdl::core::engine::Color& labelBgColor = sdl::core::engine::Color::NamedColor::Orange);

      ~MandelbrotOptions() = default;

    private:

      /**
       * @brief - Retrieves the default font size to use for labels within this
       *          widget. Only applies to the size of the text used for labels.
       * @return - an integer representing the default font size for this item.
       */
      static
      unsigned
      getLabelDefaultFontSize() noexcept;

      /**
       * @brief - Retrieves the default font name to use for labels within this
       *          widget. This defines the look of the text used to display the
       *          name of each option.
       * @return - an integer representing the default font name for labels.
       */
      static
      const char*
      getLabelDefaultFontName() noexcept;

      /**
       * @brief - Retrieves the default font size to use for values within this
       *          widget. Only applies to the size of the text used for values.
       * @return - an integer representing the default font size for this item.
       */
      static
      unsigned
      getValueDefaultFontSize() noexcept;

      /**
       * @brief - Retrieves the default font name to use for values within this
       *          widget. This defines the look of the text used to display the
       *          value of each option.
       * @return - an integer representing the default font name for values.
       */
      static
      const char*
      getValueDefaultFontName() noexcept;

      /**
       * @brief - Used to create the layout of this options panel. This method
       *          is called right upon building the widget.
       */
      void
      build();

    private:

      /**
       * @brief - Used to describe the background color to use for the labels
       *          of this options panel.
       */
      sdl::core::engine::Color m_labelsBgColor;
  };

}

# include "MandelbrotOptions.hxx"

#endif    /* MANDELBROT_OPTIONS_HH */
