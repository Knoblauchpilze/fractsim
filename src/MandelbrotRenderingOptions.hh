#ifndef    MANDELBROT_RENDERING_OPTIONS_HH
# define   MANDELBROT_RENDERING_OPTIONS_HH

# include "FractalOptions.hh"

namespace fractsim {

  class MandelbrotRenderingOptions: public FractalOptions {
    public:

      /**
       * @brief - Create a new set of options to render a Mandelbrot set.
       *          The accuracy is set to a suitable default value, along
       *          with the palette (but the user can still change that id
       *          needed).
       *          The power represents an indication of the exponent used
       *          when computing the series associated to the set. The
       *          default value is used by the canonical Mandelbrot set.
       * @param exponent - the exponent of the series used to compute the
       *                   Mandelbrot set.
       */
      MandelbrotRenderingOptions(float exponent = 2.0f);

      /**
       * @brief - Desctruction of the object.
       */
      ~MandelbrotRenderingOptions() = default;

      float
      getExponent() const noexcept;

      void
      setExponent(float exponent) noexcept;

    private:

      /**
       * @brief - The exponent represents the degree to which each term of the series used
       *          to compute the set is raised at each iteration. Higher degree usually leads
       *          to more bulbs.
       */
      float m_exponent;
  };

}

# include "MandelbrotRenderingOptions.hxx"

#endif    /* MANDELBROT_RENDERING_OPTIONS_HH */
