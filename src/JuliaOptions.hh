#ifndef    JULIA_OPTIONS_HH
# define   JULIA_OPTIONS_HH

# include <maths_utils/Size.hh>
# include "OptionsPanel.hh"

namespace fractsim {

  class JuliaOptions: public OptionsPanel {
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

      /**
       * @brief - Reimplementation of the base class method to retrieve the options
       *          needed to perform the rendering of a Julia set.
       * @param dummy - unused parameter.
       */
      void
      validateOptions(const std::string& dummy) override;

    private:

      /**
       * @brief - Returns a default name for the textbox receiving the real part of
       *          the constant to use to compute the Julia set.
       * @return - a name for the real part of the constant.
       */
      static
      const char*
      getConstantRealPartValueName() noexcept;

      /**
       * @brief - Returns a default name for the textbox receiving the imaginary
       *          part of the constant to use to compute the Julia set.
       * @return - a name for the imaginary part of the constant.
       */
      static
      const char*
      getConstantImgPartValueName() noexcept;

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
  };

}

# include "JuliaOptions.hxx"

#endif    /* JULIA_OPTIONS_HH */
