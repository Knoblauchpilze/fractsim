#ifndef    NEWTON_RENDERING_OPTIONS_HH
# define   NEWTON_RENDERING_OPTIONS_HH

# include <memory>
# include <vector>
# include <maths_utils/Vector2.hh>
# include "FractalOptions.hh"

namespace fractsim {

  class NewtonRenderingOptions: public FractalOptions {
    public:

      /**
       * @brief - Convenience structure defining a polynom coefficient.
       */
      struct Coefficient {
        float degree;
        utils::Vector2f coeff;
      };

    public:

      /**
       * @brief - Create a new set of options to render a newton polynomial
       *          fractal. The input vector represents the list of coefficient
       *          of the polynom.
       * @param coeffs - the list of the coefficient defining the polynom.
       */
      NewtonRenderingOptions(const std::vector<Coefficient>& coeffs);

      /**
       * @brief - Desctruction of the object.
       */
      ~NewtonRenderingOptions() = default;

      /**
       * @brief - Specialization of the base class to provide a nice viewpoint
       *          for Newton polynomial fractals.
       * @return - a suited default rendering area to visualize Newton polynomial
       *           fractals.
       */
      utils::Boxf
      getDefaultRenderingWindow() const noexcept override;

      /**
       * @brief - Specialization of the base class to provide an answer to whether
       *          the input point belongs to the Newton polynomial fractal or not.
       * @param p - the point to determine whether it belongs to the fractal.
       * @return - a value smaller than the accuracy if the point does *not* belong
       *           to the fractal.
       */
      unsigned
      compute(const utils::Vector2f& p) const noexcept override;

      const std::vector<Coefficient>&
      getCoefficients() const noexcept;

      void
      setCoefficients(const std::vector<Coefficient>& coeffs) noexcept;

    private:

      /**
       * @brief - This array is used to represent the coefficients defining the polynom
       *          attached to this fractal. Each polynom generates a distinct fractal as
       *          its zero are different.
       *          We allow for a sparse representation of the coefficients by defining
       *          their degree.
       */
      std::vector<Coefficient> m_coefficients;
  };

  using NewtonRenderingOptionsShPtr = std::shared_ptr<NewtonRenderingOptions>;
}

# include "NewtonRenderingOptions.hxx"

#endif    /* NEWTON_RENDERING_OPTIONS_HH */
