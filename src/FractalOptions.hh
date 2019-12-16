#ifndef    FRACTAL_OPTIONS_HH
# define   FRACTAL_OPTIONS_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include <sdl_engine/Gradient.hh>

namespace fractsim {

  class FractalOptions: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new set of options to render a fractal. Only
       *          describes very general options such as the accuracy to
       *          use to consider that a point is part of a fractal and
       *          some visual options like the gradient to use to represent
       *          the fractal.
       *          The `wrapping` argument allows to make the palette repeat
       *          after some fixed number of iterations. Basically one big
       *          problem of setting a fixed palette is that when larger
       *          values are provided for the accuracy, the low zooms level
       *          display only a single color because the accuracy is so high.
       *          One way to work around this is to provide palette wrapping.
       *          Basically it will tell the number of iterations needed to
       *          apply a full palette. If this value is equal to the input
       *          `accuracy` it changes nothing compared to the base idea.
       *          But if the value is smaller (say `accuracy / 2`) it means
       *          that over the course of the `accuracy` the palette will be
       *          applied twice. This means that we will actually see more
       *          iterations of the palette at low zoom levels. Note that in
       *          this case a color is no longer unique and cannot be used to
       *          determine the convergence speed of a point. But most of the
       *          time it is not what we want anyway so we can live with that.
       * @param accuracy - an integer representing the number of iterations
       *                   to compute to consider that the point is part of
       *                   the fractal.
       *                   Note that in general the more we zoom in the more
       *                   accuracy is needed to keep a sufficient level of
       *                   detail.
       * @param wrapping - the number of iterations needed to wrap the palette
       *                   around. Basically the number of convergence steps
       *                   required to loop through all the colors of the input
       *                   palette.
       * @param palette - the gradient to use to color the regions of the
       *                  rendering window which don't belong to the fractal.
       */
      FractalOptions(unsigned accuracy,
                     unsigned wrapping,
                     sdl::core::engine::GradientShPtr palette);

      /**
       * @brief - Desctruction of the object.
       */
      ~FractalOptions() = default;

      unsigned
      getAccuracy() const noexcept;

      void
      setAccuracy(unsigned acc) noexcept;

      sdl::core::engine::GradientShPtr
      getPalette() const noexcept;

      /**
       * @brief - Assigns a new palette to this options. If the input argument is
       *          `null` an error is raised.
       * @param palette - the new palette to use to display the fractal.
       */
      void
      setPalette(sdl::core::engine::GradientShPtr palette);

      /**
       * @brief - Used to retrieve a default rendering window allowing to visualize
       *          correctly the data associated to the fractal.
       *          Should be specialized by inheriting classes to better fit their
       *          constraints.
       * @return - a box representing the default rendering area allowing to have a
       *           nice viewpoint on the fractal.
       */
      virtual utils::Boxf
      getDefaultRenderingWindow() const noexcept = 0;

      /**
       * @brief - Interface method allowing each inheriting class to define its own
       *          computation semantic. Basically utilizes the options defined by
       *          this object and the additional ones provided by the class to check
       *          whether the input point belongs to the fractal or not.
       * @param p - the point to determine whether it belongs to the fractal.
       * @return - a value representing the confidence that this point belongs to
       *           the fractal given the degree of accuracy provided by the internal
       *           value (i.e. `getAccuracy()`).
       */
      virtual float
      compute(const utils::Vector2f& p) const noexcept = 0;

    protected:

      /**
       * @brief - Used to retrieve a default suitable accuracy to compute
       *          the associated fractal.
       * @return - a suitale value to compute the fractal.
       */
      static
      unsigned
      getDefaultAccuracy() noexcept;

      /**
       * @brief - Used to retrieve a default palette defining nice coloring
       *          to represent a fractal.
       * @return - a default palette to use in case none is provided.
       */
      static
      sdl::core::engine::GradientShPtr
      getDefaultPalette() noexcept;

      /**
       * @brief - Used to retrieve a default suitable palette wrapping count. More
       *          details about the precise role of the wrapping can be found in the
       *          constructor documentation.
       * @return - a valid palette wrapping count.
       */
      static
      unsigned
      getDefaultPaletteWrapping() noexcept;

      /**
       * @brief - Used to retrieve the palette wrapping applied to this object.
       * @return - the current palette wrapping.
       */
      unsigned
      getPaletteWrapping() const noexcept;

      /**
       * @brief - Can be used by inheriting classes to perform some palette wrapping on
       *          the input element. The value is supposed to be in the range `[0; acc]`
       *          and is wrapped so that a value in the range `[0; 1]` is produced with
       *          an adequate wrapping.
       * @param val - the value to wrap.
       * @return - a wrapped value of the input iterations count.
       */
      float
      performWrapping(float val) const noexcept;

    private:

      /**
       * @brief - The accuracy to use to compute the fractal. This accuracy represents
       *          the maximum number of temrs of the series defining a fractal to compute
       *          before concluding that the point belongs to the fractal. The larger the
       *          value the longer it will take to compute each point but the finer the
       *          details will be.
       */
      unsigned m_accuracy;

      /**
       * @brief- The number of steps to loop through all the colors of the palette used
       *         by this object. If this value is smaller than the accuracy it allows to
       *         display some more iterations of the palette at low zoom levels and thus
       *         allow for higher accuracy to be prettier in these cases.
       */
      unsigned m_wrapping;

      /**
       * @brief - The palette to use to render visually the fractal. Usually the points
       *          belonging to the fractal are rendered in black and the points which are
       *          escaping are colored based on the number of iterations it took to exit
       *          the prescribed radius. This allows to give visual information about the
       *          speed at which a series escape to infinity.
       */
       sdl::core::engine::GradientShPtr m_palette;
  };

  using FractalOptionsShPtr = std::shared_ptr<FractalOptions>;
}

# include "FractalOptions.hxx"

#endif    /* FRACTAL_OPTIONS_HH */
