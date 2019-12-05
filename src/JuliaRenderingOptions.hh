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
       * @return - a value smaller than the accuracy if the point does *not* belong
       *           to the fractal.
       */
      unsigned
      compute(const utils::Vector2f& p) const noexcept override;

      utils::Vector2f
      getConstant() const noexcept;

      void
      setConstant(const utils::Vector2f& constant) noexcept;

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
