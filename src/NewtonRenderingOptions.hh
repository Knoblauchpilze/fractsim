#ifndef    NEWTON_RENDERING_OPTIONS_HH
# define   NEWTON_RENDERING_OPTIONS_HH

# include <memory>
# include <vector>
# include <complex>
# include <maths_utils/Vector2.hh>
# include <maths_utils/Box.hh>
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
       *          This function does not lock the properties mutex while computing
       *          the series *which means that we don't support concurrent changes
       *          to the coefficients while a computation is being performed*. It
       *          does handle the creation of roots in a thread-safe manner though
       *          and can be used concurrently.
       * @param c - the point to determine whether it belongs to the fractal.
       * @return - a value indicating the level of confidence that this point is
       *           part of the fractal.
       */
      float
      compute(const utils::Vector2f& c) const noexcept override;

      void
      setCoefficients(const std::vector<Coefficient>& coeffs) noexcept;

    private:

      /**
       * @brief - Interpreted value of the input coefficient so that it is faster
       *          when performing rendering.
       */
      struct InternalCoefficient {
        float degree;
        std::complex<float> coeff;
      };

      /**
       * @brief - Return a default threshold allowing to detect null derivative (or
       *          any kind of null value really) when iterating the Newton series.
       * @return - a value allowing to detect null elements.
       */
      static
      float
      getNullThreshold() noexcept;

      /**
       * @brief - Used to retrieve a value allowing to determine when two values are
       *          considered equal. This is helpful when one want to determine whether
       *          a value is identical to some existing root.
       * @return - a threshold to declare two values identical.
       */
      static
      float
      getRootEqualityThreshold() noexcept;

      /**
       * @brief - Used to return a value that can be used when two successive iterations
       *          of the Newton series are close enough to be considered converged. This
       *          usually means that the series has converged to the root it is supposed
       *          to reach and thus that there is no need to continue the computations.
       * @return - the threshold value allowing to determine when a series has converged.
       */
      static
      float
      getConvergenceThreshold() noexcept;

      /**
       * @brief - Allow to determine when a series is considered to have converged. This
       *          means that for a certain number of successive terms the series reached
       *          values close enough to each other.
       * @return - the number of `close enough` terms that a series should produce to be
       *           considered converged.
       */
      static
      unsigned
      getConvergenceDuration() noexcept;

      /**
       * @brief - Return a value representing the available interval from the range `[0; 1]`
       *          is available to represent the roots colors.
       * @return - a value in the range `[0; 1[` representing the interval available for the
       *           roots colors in the palette used by this object.
       */
      static
      float
      getRootGradientInterval() noexcept;

      /**
       * @brief - Return a value representing the separation interval between two consecutive
       *          roots colors section. Basically it means that two consecutive root will be
       *          separated by a useless area of this length in the output gradient.
       * @return - the separation between two roots areas.
       */
      static
      float
      getRootGradientSeparation() noexcept;

      /**
       * @brief - Used when building a new options object to initialie the coefficients
       *          the derivative but also the palette to reflect the possible roots of
       *          a polynom and be able to color it correctly.
       * @param coeffs - the coefficients of the polynom to initialize.
       */
      void
      initialize(const std::vector<Coefficient>& coeffs);

      /**
       * @brief - Used whenver the coefficients of the polynom are changed to update
       *          the expression of the derivative.
       *          Assumes that the locker is already acquired.
       */
      void
      computeDerivative();

      /**
       * @brief - Used to evaluate the polynom defined by the coefficients and derivative
       *          at the input point `x`. The locker is not assumed to be acquired and is
       *          *not* acquired by this method.
       * @param x - the value at which the polynom and its derivative should be evaluated.
       * @param p - output value storing the value of this polynom at `x`.
       * @param pp - output value storing the value of the derivative at `x`.
       */
      void
      evaluate(const std::complex<float>& x,
               std::complex<float>& p,
               std::complex<float>& pp) const noexcept;

      /**
       * @brief - Used to compute a valid position that can be used in the gradient defined
       *          for this object given the series converged to the `root`-th root within a
       *          number of iterations represented by `terms`.
       *          This method assumes that the locker is already locked.
       * @param root - the index of the root the series converged to.
       * @param terms - the number of temrs it took to consider that the series converged.
       * @return - a value that can be used in the internal gradient to retrieve the color
       *           associated to this object.
       */
      float
      getColorPosFromRoot(unsigned root,
                          unsigned terms) const;

    private:

      /**
       * @brief - A mutex protecting the internal data from concurrent accesses.
       */
      mutable std::mutex m_propsLocker;

      /**
       * @brief - This array is used to represent the coefficients defining the polynom
       *          attached to this fractal. Each polynom generates a distinct fractal as
       *          its zero are different.
       *          We allow for a sparse representation of the coefficients by defining
       *          their degree.
       */
      std::vector<InternalCoefficient> m_coefficients;

      /**
       * @brief - An evaluation of the derivative of the polynom. It is evaluated when
       *          the coefficients are changed and can be used for more efficient time
       *          to compute the orbit of a point.
       */
      std::vector<InternalCoefficient> m_derivative;

      /**
       * @brief - A value holding the maximum degree of any coefficients for the underlying
       *          polynom.
       */
      float m_maxDegree;

      /**
       * @brief - A vector holding the roots for the polynom associated to this fractal.
       *          The roots are cleared when needed and accumulated with each call to
       *          the `compute` method.
       */
      mutable std::vector<std::complex<float>> m_roots;
  };

  using NewtonRenderingOptionsShPtr = std::shared_ptr<NewtonRenderingOptions>;
}

# include "NewtonRenderingOptions.hxx"

#endif    /* NEWTON_RENDERING_OPTIONS_HH */
