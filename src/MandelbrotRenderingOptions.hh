#ifndef    MANDELBROT_RENDERING_OPTIONS_HH
# define   MANDELBROT_RENDERING_OPTIONS_HH

# include <memory>
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

      /**
       * @brief - Specialization of the base class to provide a nice viewpoint
       *          for Mandelbrot set.
       * @return - a suited default rendering area to visualize Mandelbrot set.
       */
      utils::Boxf
      getDefaultRenderingWindow() const noexcept override;

      /**
       * @brief - Specialization of the base class to provide an answer to whether
       *          the input point belongs to the Mandelbrot set or not.
       * @param p - the point to determine whether it belongs to the fractal.
       * @return - a value indicating the level of confidence that this point is
       *           part of the fractal.
       */
      float
      compute(const utils::Vector2f& p) const noexcept override;

      float
      getExponent() const noexcept;

      void
      setExponent(float exponent) noexcept;

    private:

      /**
       * @brief - Used to retrieve the threshold above which a terms is known to
       *          be diverging. This allows to speed up the computations.
       * @return - a value above which the series for any point has to diverge.
       */
      static
      float
      getDivergenceThreshold() noexcept;

    private:

      /**
       * @brief - The exponent represents the degree to which each term of the series used
       *          to compute the set is raised at each iteration. Higher degree usually leads
       *          to more bulbs.
       */
      float m_exponent;
  };

  using MandelbrotRenderingOptionsShPtr = std::shared_ptr<MandelbrotRenderingOptions>;
}

# include "MandelbrotRenderingOptions.hxx"

#endif    /* MANDELBROT_RENDERING_OPTIONS_HH */
