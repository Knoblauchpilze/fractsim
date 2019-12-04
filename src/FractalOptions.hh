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
       * @param accuracy - an integer representing the number of iterations
       *                   to compute to consider that the point is part of
       *                   the fractal.
       *                   Note that in general the more we zoom in the more
       *                   accuracy is needed to keep a sufficient level of
       *                   detail.
       * @param palette - the gradient to use to color the regions of the
       *                  rendering window which don't belong to the fractal.
       */
      FractalOptions(unsigned accuracy,
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
