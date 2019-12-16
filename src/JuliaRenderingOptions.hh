#ifndef    JULIA_RENDERING_OPTIONS_HH
# define   JULIA_RENDERING_OPTIONS_HH

# include <memory>
# include "FractalOptions.hh"
# include <maths_utils/Vector2.hh>

namespace fractsim {

  class JuliaRenderingOptions: public FractalOptions {
    public:

      /**
       * @brief - Create a new set of options to render a Julia set. Such
       *          a set is characterized by a constant that is added to
       *          each iteration of the series.
       * @param constant - the constant to add at each step when computing
       *                   the series.
       */
      JuliaRenderingOptions(const utils::Vector2f& constant = utils::Vector2f(0.3f, 0.5f));

      /**
       * @brief - Desctruction of the object.
       */
      ~JuliaRenderingOptions() = default;

      /**
       * @brief - Specialization of the base class to provide a nice viewpoint
       *          for Julia sets.
       * @return - a suited default rendering area to visualize Julia sets.
       */
      utils::Boxf
      getDefaultRenderingWindow() const noexcept override;

      /**
       * @brief - Specialization of the base class to provide an answer to whether
       *          the input point belongs to the Julia set or not.
       * @param p - the point to determine whether it belongs to the fractal.
       * @return - a value indicating the level of confidence that this point is
       *           part of the fractal.
       */
      float
      compute(const utils::Vector2f& p) const noexcept override;

      utils::Vector2f
      getConstant() const noexcept;

      void
      setConstant(const utils::Vector2f& constant) noexcept;

    private:

      /**
       * @brief - Used to retrieve the threshold above which a terms is known to
       *          be diverging. This allows to speed up the computations.
       * @return - a value above which the series for any point has to diverge.
       */
      static
      float
      getDivergenceThreshold() noexcept;

      /**
       * @brief - Used to retrieve a value that can be used to overshoot each
       *          computation for a point when it diverged. Computing some more
       *          steps usually allow to get larger `z` values and thus improve
       *          the effect of the smoothing algorithm.
       * @return - a value allowing to describe how many additional steps should
       *           be performed to reach acceptable performance of the smoothing
       *           algorithm.
       */
      static
      unsigned
      getSmoothingOvershoot() noexcept;

      /**
       * @brief - Provide a default exponent used in when computing the terms of the
       *          series. This value could be made configurable later on.
       * @return - the default exponent to attach to Julia sets.
       */
      static
      float
      getDefaultExponent() noexcept;

    private:

      /**
       * @brief - The constant represents an additional term that is added to the result
       *          of the exponentiation at each term of the series. This constant allows
       *          to provude a great variety of Julia set and is the characteristic of
       *          any set.
       */
      utils::Vector2f m_constant;
  };

  using JuliaRenderingOptionsShPtr = std::shared_ptr<JuliaRenderingOptions>;
}

# include "JuliaRenderingOptions.hxx"

#endif    /* JULIA_RENDERING_OPTIONS_HH */
