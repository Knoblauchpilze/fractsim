#ifndef    GMP_BOX_HH
# define   GMP_BOX_HH

# include <gmp.h>
# include <memory>
# include <maths_utils/Box.hh>

namespace fractsim {

  class GMPBox {
    public:

      /**
       * @brief - Create an empty area with arbitrary precision.
       */
      GMPBox();

      /**
       * @brief - Create an area with arbitrary precision from the input box.
       * @param box - the input area to use to update internal values for this
       *              box.
       */
      GMPBox(const utils::Boxf& box);

      /**
       * @brief - Desctruction of the object.
       */
      ~GMPBox();

      /**
       * @brief - Assign the internal description of this object with the values
       *          from the input box.
       *          Note that this method assumes that the internal gmp floats are
       *          already assigned.
       * @param box - the object to use to assign internal values.
       */
      void
      setFromBox(const utils::Boxf& box);

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
       * @brief - The coordinate of the center along the `x` axis.
       */
      mpf_t m_x;

      /**
       * @brief - The coordinate of the center along the `y` axis.
       */
      mpf_t m_y;

      /**
       * @brief - The width of the box.
       */
      mpf_t m_w;

      /**
       * @brief - The height of the box.
       */
      mpf_t m_h;
  };

  using GMPBoxShPtr = std::shared_ptr<GMPBox>;
}

# include "GMPBox.hxx"

#endif    /* GMP_BOX_HH */
