#ifndef    FRACTAL_HH
# define   FRACTAL_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include <maths_utils/Vector2.hh>

namespace fractsim {

  class Fractal: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new fractal with no associated cache yet.
       */
      Fractal();

      /**
       * @brief - Destruction of the objec: should release the internal cached tiles
       *          used to represent the fractal.
       */
      ~Fractal();

      /**
       * @brief - Used to assign the input point with the specified confidence. The
       *          confidence represents a value in the range `[0; 1]` (if this is not
       *          the case it is clamped) where `0` indicates that the point cannot
       *          diverge quicker from being stable and `1` meaning that at the level
       *          of confidence specified we cannot say that the point diverges. It is
       *          not a very good indication if the accuracy used during the computation
       *          is low but it gets better the higher this value is.
       *          The input coordinate is interpreted internally in order to assign the
       *          closest pixel with this value.
       * @param p - the coordinate for which the confidence should be updated.
       * @param confidence - a value in the range `[0; 1]` indicating the level of
       *                     confidence we have for this point belonging to the fractal.
       */
      void
      assignValueForCoord(const utils::Vector2f& p,
                          float confidence);

    private:
  };

  using FractalShPtr = std::shared_ptr<Fractal>;
}

# include "Fractal.hxx"

#endif    /* FRACTAL_HH */
