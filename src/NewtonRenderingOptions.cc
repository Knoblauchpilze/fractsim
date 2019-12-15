
# include "NewtonRenderingOptions.hh"

namespace fractsim {

  NewtonRenderingOptions::NewtonRenderingOptions(const std::vector<Coefficient>& coeffs):
    FractalOptions(getDefaultAccuracy(), getDefaultPalette()),

    m_propsLocker(),

    m_coefficients(),
    m_derivative(),

    m_maxDegree(),
    m_roots()
  {
    initialize(coeffs);
  }

  unsigned
  NewtonRenderingOptions::compute(const utils::Vector2f& c) const noexcept {
    // We want to iterate the series of Newton for the polynom defined
    // through the coefficients of this object by the following expression:
    // z(n+1) = z(n) - p(z) / p'(z)
    //
    // Where the `p(z)` expression is the value of the polynom for the
    // current term of the series and `p'(z)` corresponds to the derivative.
    // By the end of the iteration we should reach one of the roots of the
    // polynom.
    // At this point we have to assign a value to the polynom which is the
    // index of the root.

    // Iterate the function
    unsigned acc = getAccuracy();
    unsigned conv = getConvergenceDuration();
    float thresh = getConvergenceThreshold();
    unsigned terms = 0u;
    std::complex<float> cur(c.x(), c.y());
    std::complex<float> p, pp;

    bool optimum = false;
    unsigned close = 0u;


    while (terms < acc && !optimum && close <= conv) {
      evaluate(cur, p, pp);
      optimum = std::norm(pp) < getNullThreshold();

      if (!optimum) {
        std::complex<float> tmp = p / pp;
        cur -= tmp;

        if (std::norm(tmp) <= thresh) {
          ++close;
        }
        else {
          close = 0u;
        }
      }

      // TODO: Improve mechanism for coloring: add some sort of convergence measurement.
      ++terms;
    }

    // Determine the confidence for this point by scanning the roots found
    // so far and checking if any is close enough of the value we reached
    // for this series.
    // To make sure that people can call the computing method in parallel
    // we will protect this critical section behind a locker.
    Guard guard(m_propsLocker);

    unsigned idRoot = 0u;
    bool found = false;

    while (idRoot < m_roots.size() && !found) {
      found = (
        utils::fuzzyEqual(cur.real(), m_roots[idRoot].real(), getRootEqualityThreshold()) &&
        utils::fuzzyEqual(cur.imag(), m_roots[idRoot].imag(), getRootEqualityThreshold())
      );

      ++idRoot;
    }

    // In any case, we want the value to be assigned to the palette entry
    // matching the index of the root in the internal array. This can be
    // done by setting a value of `rootStep * accuracy` where the `rootStep`
    // is just the number of roots that this polynom can have.
    //
    // If we found a root matching the value reached by the series, assign
    // this value and otherwise add the root to the list and set the value.
    // Without loss of generality we can assign the root and then use the
    // `idRoot` as a valid value.
    if (!found) {
      m_roots.push_back(cur);
    }

    float perc = std::round(acc * idRoot / m_maxDegree);

    return static_cast<int>(perc);
  }

  void
  NewtonRenderingOptions::initialize(const std::vector<Coefficient>& coeffs) {
    // Assign coefficients.
    setCoefficients(coeffs);

    // Assign a palette from the expected number of roots. Given that `C` is algebraically
    // closed we know that if the polynom has degree `n`, all `n` roots will be in `C` and
    // can be reached by the method.

    // Determine max degree.
    m_maxDegree = std::numeric_limits<float>::lowest();
    for (unsigned id = 0u ; id < m_coefficients.size() ; ++id) {
      if (m_maxDegree < m_coefficients[id].degree) {
        m_maxDegree = m_coefficients[id].degree;
      }
    }

    // Populate the palette.
    sdl::core::engine::GradientShPtr gradient = std::make_shared<sdl::core::engine::Gradient>(
      std::string("newton_rendering_gradient"),
      sdl::core::engine::gradient::Mode::Linear
    );

    // Handle trivial case of a polynom with no degree.
    if (m_maxDegree < getNullThreshold()) {
      gradient->setColorAt(0.0f, sdl::core::engine::Color::NamedColor::Red);

      setPalette(gradient);

      return;
    }

    // Assume integer degrees for now.
    float step = 1.0f / m_maxDegree;

    // Populate the palette with random values.
    gradient->setColorAt(0.0f, sdl::core::engine::Color::NamedColor::Black);

    float p = step;
    while (p < 1.0f) {
      sdl::core::engine::Color c = generateRandomColor(p, 1.0f);
      gradient->setColorAt(p, c);
      p += step;
    }

    gradient->setColorAt(1.0f, sdl::core::engine::Color::NamedColor::White);

    setPalette(gradient);
  }

  void
  NewtonRenderingOptions::computeDerivative() {
    m_derivative.clear();

    // Apply the derivative to each coefficient.
    for (unsigned id = 0u ; id < m_coefficients.size() ; ++id) {
      const InternalCoefficient& a = m_coefficients[id];

      // For now only handle cases where the degree is at least `1` in the derivative.
      if (a.degree > 1.0f) {
        m_derivative.push_back(InternalCoefficient{a.degree - 1.0f, a.degree * a.coeff});
      }
    }
  }

  void
  NewtonRenderingOptions::evaluate(const std::complex<float>& x,
                                   std::complex<float>& p,
                                   std::complex<float>& pp) const noexcept
  {
    // Compute the value of the polynom.
    p = std::complex<float>(0.0f, 0.0f);
    pp = std::complex<float>(0.0f, 0.0f);

    for (unsigned id = 0u ; id < m_coefficients.size() ; ++id) {
      p += (m_coefficients[id].coeff * std::pow(x, m_coefficients[id].degree));
    }

    for (unsigned id = 0u ; id < m_derivative.size() ; ++id) {
      pp += (m_derivative[id].coeff * std::pow(x, m_derivative[id].degree));
    }
  }

}
