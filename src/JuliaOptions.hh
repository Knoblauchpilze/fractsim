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

      /**
       * @brief - Reimplementation of the base class method to reset the options
       *          displayed in thepanel.
       * @param dummy - unused parameter.
       */
      void
      resetOptions(const std::string& dummy) override;

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
       * @brief - Returns a default value for the accuracy.
       * @return - a default value for the accuracy.
       */
      static
      unsigned
      getDefaultAccuracy() noexcept;

      /**
       * @brief - Returns a default value for the real component of the constant
       *          associated to the Julia set.
       * @return - a default value for the real part of the constant.
       */
      static
      float
      getDefaultRealPartConstant() noexcept;

      /**
       * @brief - Returns a default value for the imaginary component of the constant
       *          associated to the Julia set.
       * @return - a default value for the imaginary part of the constant.
       */
      static
      float
      getDefaultImgPartConstant() noexcept;

      /**
       * @brief - Used to create the layout of this options panel. This method
       *          is called right upon building the widget.
       */
      void
      build();

      /**
       * @brief - Used to assign default values to the fields representing the options
       *          for this panel.
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

# include "JuliaOptions.hxx"

#endif    /* JULIA_OPTIONS_HH */
