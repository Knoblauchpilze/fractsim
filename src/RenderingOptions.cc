
# include "RenderingOptions.hh"

namespace fractsim {

  RenderingOptions::RenderingOptions(const utils::Boxf& window):
    utils::CoreObject("rendering_options"),

    m_initial(window),
    m_area()
  {
    reset();
  }

  void
  RenderingOptions::zoom(const utils::Vector2f& center,
                         float factor)
  {
    // We want to keep the center at the same position compared to the current
    // zoom level while reducing (or increasing) the dimensions of the window
    // by `factor`.

    // Compute the offset of the desired `center` compared to the current area.
    float toLeft = center.x() - m_area.getLeftBound();
    float toRight = m_area.getRightBound() - center.x();
    float toBottom = center.y() - m_area.getBottomBound();
    float toUp = m_area.getTopBound() - center.y();

    // Each dimension should be changed by a factor as provided in input.
    toLeft /= factor;
    toRight /= factor;
    toBottom /= factor;
    toUp /= factor;

    // Compute the new extreme points.
    float minX = center.x() - toLeft;
    float maxX = center.x() + toRight;
    float minY = center.y() - toBottom;
    float maxY = center.y() + toUp;

    // Compute the new area.
    utils::Boxf newArea = utils::Boxf(
      (minX + maxX) / 2.0f,
      (minY + maxY) / 2.0f,
      maxX - minX,
      maxY - minY
    );

    log("Changed area from " + m_area.toString() + " to " + newArea.toString() + " (center: " + center.toString() + ", f: " + std::to_string(factor) + ")");

    // Assign it to the internal area.
    m_area = newArea;
  }

}
