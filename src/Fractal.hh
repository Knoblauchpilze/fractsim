#ifndef    FRACTAL_HH
# define   FRACTAL_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include <maths_utils/Vector2.hh>
# include <maths_utils/Size.hh>
# include <maths_utils/Box.hh>

namespace fractsim {

  class Fractal: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new fractal with no associated cache yet and with the
       *          specified canvas size and rendering area.
       *          The rendering area represents an area in world coordinate which
       *          is mapped onto the canvas size.
       * @param canvas - the canvas size associated to the fractal. Used internally
       *               to compute the size of the cache needed.
       * @param area - the rendering area represented by this fractal.
       */
      Fractal(const utils::Sizef& canvas,
              const utils::Boxf& area);

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

      /**
       * @brief - Used to perform a resize of the internal cache to match the new input
       *          area. This will invalidate any existing cache.
       * @param canvas - the new canvas' size assigned to this fractal.
       */
      void
      resize(const utils::Sizef& canvas);

      /**
       * @brief - Assigns a new rendering area for this fractal. The area is supposed to
       *          be a real world coordinate representation that is mapped onto the internal
       *          canvas size.
       * @param area - the rendering area associated to this fractal.
       */
      void
      setRenderingArea(const utils::Boxf& area);

    private:

      /**
       * @brief - Used to retrieve a value that can be used as a threshold when comparing
       *          whether a coordinate lies inside the rendering area defined for this
       *          fractal.
       * @return - a value that can be used as a threshold.
       */
      static
      float
      getToleranceForCells() noexcept;

      /**
       * @brief - Fills the internal cache with invalid value.
       */
      void
      clear();

      /**
       * @brief - Uses the internal attributes to determine the size of the area covered by
       *          a single pixel. Basically computes a ratio between the real world size of
       *          the rendering area and the canvas' size.
       */
      void
      computeCellDelta();

      /**
       * @brief - Used to compute the cell to associate to the input coordinate. This coordinate
       *          should belong to the rendering area for this fractal otherwise an error will be
       *          raised.
       *          In case the input coordinate is inside the rendering area, the closest cell in
       *          the internal cache is returned.
       * @param p - the real world coordinate for which the cell should be determined.
       * @return - the coordinate of the cell corresponding to the input position.
       */
      utils::Vector2i
      computeCellFromCoords(const utils::Vector2f& p);

      /**
       * @brief - Used to try to assign the `confidence` to the value described by the `cell` of
       *          the internal cache. Potential failures include not allocated cache or out of
       *          bound cells.
       * @param cell - the cell to set.
       * @param value - the value to assign to the cell.
       */
      void
      setOrThrow(const utils::Vector2i& cell,
                 float value);

    private:

      /**
       * @brief - The size of the canvas associated to this fractal.
       */
      utils::Sizef m_canvas;

      /**
       * @brief - Holds the dimensions of the internal cache. This value is computed from
       *          the canvas' size and allows to make easy association between the size of
       *          the canvas and the number of values registered in the cache.
       */
      utils::Vector2i m_dims;

      /**
       * @brief - The area assigned to this fractal. This allows to determine which part of
       *          the internal cache corresponds to which position in the real world.
       */
      utils::Boxf m_area;

      /**
       * @brief - Represents the delta covered by each pixel of the canvas. This value is
       *          computed from the canvas' size and the rendering area.
       */
      utils::Vector2f m_cellDelta;

      /**
       * @brief - Holds the data associated to the fractal.
       */
      std::vector<float> m_data;
  };

  using FractalShPtr = std::shared_ptr<Fractal>;
}

# include "Fractal.hxx"

#endif    /* FRACTAL_HH */
