
# include "ColorPalette.hh"
# include <random>

namespace fractsim {

  void
  ColorPalette::generate() {
    // Clear any existing palette.
    m_colors.clear();

    // Use the dedicated method based on the generation mode.
    switch (m_mode) {
      case Mode::GoldenAngle:
        generateGoldenAngle();
        break;
      case Mode::Random:
        generateRandom();
        break;
      default:
        error(
          std::string("Cannot generate color palette"),
          std::string("Unknown generation mode ") + std::to_string(static_cast<int>(m_mode))
        );
        break;
    }
  }

  void
  ColorPalette::generateGoldenAngle() {
    // See this article for further details:
    // https://martin.ankerl.com/2009/12/09/how-to-create-random-colors-programmatically/
    // Basically we will use `HSV` color space to produce evenly distributed hues to use
    // to generate colors. Once this is done we complete the colors using the internal
    // saturation and value provided.

    // Generate each color.
    std::random_device support;
    std::mt19937 rng(support());
    std::uniform_real_distribution<> rndDist(0.0f, 1.0f);

    float h = rndDist(rng);
    float step = getGenerationPattern();

    for (unsigned id = 0u ; id < m_size ; ++id) {
      // Generate the color using the current hue.
      sdl::core::engine::Color c = sdl::core::engine::Color::fromHSV(h, m_s, m_v);
      debug("Generated color " + std::to_string(id) + ", c: " + c.toString() + " (from h: " + std::to_string(h) + ", s: " + std::to_string(m_s) + ", v: " + std::to_string(m_v) + ")");
      m_colors.push_back(c);

      // Move on to the next color by using the generation pattern.
      h = std::fmod(h + step, 1.0f);
    }
  }

  void
  ColorPalette::generateRandom() {
    // This solution was originally found here:
    // https://stackoverflow.com/questions/1484506/random-color-generator
    // Is is supposed to create radiant colors ideally spaced for minimum
    // clustering.
    // And apparently comes from there:  Adam Cole, 2011-Sept-14
    // http://mjijackson.com/2008/02/rgb-to-hsl-and-rgb-to-hsv-color-model-conversion-algorithms-in-javascript

    // Generate each color.
    for (unsigned id = 0u ; id < m_size ; ++id) {
      float r, g, b;
      float perc = 1.0f * id / m_size;

      float i = std::floor(perc * 6.0f);
      float f = perc * 6.0f - i;
      float q = 1.0f - f;

      int det = static_cast<int>(std::round(std::fmod(i, 6.0f)));

      switch (det) {
        case 0:
          r = 1.0f; g = f;    b = 0.0f;
          break;
        case 1:
          r = q;    g = 1.0f; b = 0.0f;
          break;
        case 2:
          r = 0.0f; g = 1.0f; b = f;
          break;
        case 3:
          r = 0.0f; g = q;    b = 1.0f;
          break;
        case 4:
          r = f;    g = 0.0f; b = 1.0f;
          break;
        case 5:
        default:
          r = 1.0f; g = 0.0f; b = q;
          break;
      }

      sdl::core::engine::Color c = sdl::core::engine::Color::fromRGB(r, g, b);
      debug("Generated color " + std::to_string(id) + ", c: " + c.toString());
      m_colors.push_back(c);
    }
  }

}