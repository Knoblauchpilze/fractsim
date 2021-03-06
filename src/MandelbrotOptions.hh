#ifndef    MANDELBROT_OPTIONS_HH
# define   MANDELBROT_OPTIONS_HH

# include <maths_utils/Size.hh>
# include <sdl_engine/Color.hh>
# include "OptionsPanel.hh"

namespace fractsim {

  class MandelbrotOptions: public OptionsPanel {
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

      /**
       * @brief - Reimplementation of the base class method to retrieve the options
       *          needed to perform the rendering of a Mandelbrot fractal.
       * @param dummy - unused parameter.
       */
      void
      validateOptions(const std::string& dummy) override;

      /**
       * @brief - Reimplementation of the base class method to reset the options
       *          displayed in thepanel.
       * @param dummy - unused parameter.
       */
      void
      resetOptions(const std::string& dummy) override;

    private:

      /**
       * @brief - Returns a default name for the power texbox.
       * @return - a name for the power value.
       */
      static
      const char*
      getPowerValueName() noexcept;

      /**
       * @brief - Returns a default name for the accuracy textbox.
       * @return - a name for the accuracy value.
       */
      static
      const char*
      getAccuracyValueName() noexcept;

      /**
       * @brief - Returns a default value for the accuracy.
       * @return - a default value for the accuracy.
       */
      static
      unsigned
      getDefaultAccuracy() noexcept;

      /**
       * @brief - Returns a default value for the exponent of the mandelbrot set.
       * @return - a default value for the exponent.
       */
      static
      float
      getDefaultPower() noexcept;

      /**
       * @brief - Used to create the layout of this options panel. This method
       *          is called right upon building the widget.
       */
      void
      build();

      /**
       * @brief - Used to assign default values to the fields representing the
       *          options for this panel.
       */
      void
      initElements();

    private:

      /**
       * @brief - Protect this object from concurrent accesses.
       */
      mutable std::mutex m_propsLocker;
  };

}

# include "MandelbrotOptions.hxx"

#endif    /* MANDELBROT_OPTIONS_HH */
