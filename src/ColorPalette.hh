#ifndef    COLOR_PALETTE_HH
# define   COLOR_PALETTE_HH

# include <vector>
# include <core_utils/CoreObject.hh>
# include <sdl_engine/Color.hh>

namespace fractsim {

  class ColorPalette: public utils::CoreObject {
    public:

      /**
       * @brief - Defines the color generation scheme to apply for this palette.
       */
      enum class Mode {
        GoldenAngle,
        Random
      };

    public:

      /**
       * @brief - Create a new color palette with the specified parameters. This
       *          palette uses a HSV based generator with a golden ratio pattern
       *          to create color. It is then converted back to RGB data. It can
       *          also use some pseudo random-colors to provide more vibrant
       *          colors. The user can specify the appropriate mode through the
       *          constructor.
       *          Suitable default value are provided for the saturation and value
       *          if needed.
       * @param count - the number of colors to generate.
       * @param mode - the color generation scehme to apply to the generation.
       * @param s - the saturation of the colors to produce. This value should be
       *            in the range `[0; 1]`.
       * @param v - the value of the colors to produce. This value should be in
       *            the range `[0; 1]`.
       * @param coeffs - the list of the coefficient defining the polynom.
       */
      ColorPalette(unsigned count,
                   const Mode& mode,
                   float s = 0.5f,
                   float v = 0.95f);

      /**
       * @brief - Desctruction of the object.
       */
      ~ColorPalette() = default;

      /**
       * @brief - Used to retrieve the colors generated for this palette.
       * @¶eturn - a vector containing all the colors generated for this palette.
       */
      const std::vector<sdl::core::engine::Color>&
      getColors() const noexcept;

      /**
       * @brief - Performs the generation of the colors based on the input parameters.
       */
      void
      generate();

    private:

        /**
         * @brief - Provides a value to use when rotating the colors generated in the
         *          `HSV` color space. This ratio should lead to evenly distributed
         *          colors among the possible hues.
         *          Given this article: https://en.wikipedia.org/wiki/Golden_angle we
         *          use this value as a basis.
         * @return - a number allowing evenly distributed values within a color space.
         */
        static
        float
        getGenerationPattern() noexcept;

        /**
         * @brief - Used to generate colors using a golden angle scheme. Produces
         *          neutral colors based on the internal luminance and saturation
         *          values.
         */
        void
        generateGoldenAngle();

        /**
         * @brief - Alternalte method to generate a palette using a golden angle pattern.
         */
        void
        generateRandom();

    private:

      /**
       * @brief - The expected size of the palette. The larger this value the more likely
       *          it is to have color with similar tint though.
       */
      unsigned m_size;

      /**
       * @brief - The color generation scheme to apply when generating colors.
       */
      Mode m_mode;

      /**
       * @brief - Internal reference describing the `saturation` for the colors produced
       *          with this palette.
       */
      float m_s;

      /**
       * @brief - Internal reference describing the `value` of the colors produced with
       *          this palette.
       */
      float m_v;

      /**
       * @brief - The colors generated for this palette.
       */
      std::vector<sdl::core::engine::Color> m_colors;
  };

}

# include "ColorPalette.hxx"

#endif    /* COLOR_PALETTE_HH */
