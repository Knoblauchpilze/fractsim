#ifndef    GMP_VECTOR2_HH
# define   GMP_VECTOR2_HH

# include <gmp.h>
# include <memory>
# include <maths_utils/Vector2.hh>

namespace fractsim {

  class GMPVector2 {
    public:

      /**
       * @brief - Create a `null` vector with arbitrary precision.
       */
      GMPVector2();

      /**
       * @brief - Create a vector with arbitrary precision from the input value.
       * @param vec - the input vector to use to update internal values for this
       *              element.
       */
      GMPVector2(const utils::Vector2f& vec);

      /**
       * @brief - Desctruction of the object.
       */
      ~GMPVector2();

      /**
       * @brief - Assign the internal description of this object with the values
       *          from the input vector.
       *          Note that this method assumes that the internal gmp floats are
       *          already assigned.
       * @param vec - the object to use to assign internal values.
       */
      void
      setFromVector(const utils::Vector2f& vec);

    private:

      /**
       * @brief - Reset or create the internal values to default numbers.
       */
      void
      reset();

      /**
       * @brief - Reset the values of the internal properties.
       */
      void
      clear();

    private:

      /**
       * @brief - The `x` coordinate of this vector.
       */
      mpf_t m_x;

      /**
       * @brief - The `y` coordinate of this vector.
       */
      mpf_t m_y;
  };

  using GMPVector2ShPtr = std::shared_ptr<GMPVector2>;
}

# include "GMPVector2.hxx"

#endif    /* GMP_VECTOR2_HH */
