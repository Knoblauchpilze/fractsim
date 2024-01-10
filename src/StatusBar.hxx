#ifndef    STATUS_BAR_HXX
# define   STATUS_BAR_HXX

# include "StatusBar.hh"
# include <sstream>
# include <iomanip>

namespace fractsim {

  inline
  void
  StatusBar::onMouseCoordsChanged(utils::Vector2f p) {
    // Transform the input position into a text.
    std::stringstream formatter;
    formatter << std::setprecision(3) << std::fixed;

    formatter << "x: "
              << p.x()
              << " "
              << "y: "
              << p.y();

    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Retrieve the rendering area label.
    sdl::graphic::LabelWidget* txt = getMouseCoordsLabel();
    txt->setText(formatter.str());
  }

  inline
  void
  StatusBar::onZoomChanged(utils::Vector2f zoom) {
    // Compute the mean zoom value by taking the average.
    const float avgZoom = (zoom.x() + zoom.y()) / 2.0f;

    std::stringstream zoomTxtWrapper;
    zoomTxtWrapper << "Zoom: ";
    zoomTxtWrapper << std::fixed << std::setprecision(0);
    zoomTxtWrapper << avgZoom;

    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Try to retrieve the label widget and set the text corresponding
    // to the new zoom level.
    sdl::graphic::LabelWidget* txt = getZoomLabel();
    txt->setText(zoomTxtWrapper.str());
  }

  inline
  void
  StatusBar::onRenderingAreaChanged(utils::Boxf area) {
    // Transform the input box into a text.
    std::stringstream formatter;
    formatter << std::setprecision(3) << std::fixed;

    formatter << "x: ["
              << area.getLeftBound()
              << ", "
              << area.getRightBound()
              << "] "
              << "y: ["
              << area.getBottomBound()
              << ", "
              << area.getTopBound()
              << "]";

    // Protect from concurrent accesses.
    const std::lock_guard guard(m_propsLocker);

    // Retrieve the rendering area label.
    sdl::graphic::LabelWidget* txt = getRenderingAreaLabel();
    txt->setText(formatter.str());
  }

  inline
  float
  StatusBar::getStatusMaxHeight() noexcept {
    return 30.0f;
  }

  inline
  const char*
  StatusBar::getInfoLabelFont() noexcept {
    return "data/fonts/Goodtime.ttf";
  }

  inline
  float
  StatusBar::getGlobalMargins() noexcept {
    return 2.0f;
  }

  inline
  float
  StatusBar::getComponentMargins() noexcept {
    return 7.0f;
  }

  inline
  const char*
  StatusBar::getMouseCoordsLabelName() noexcept {
    return "mouse_coords_label";
  }

  inline
  const char*
  StatusBar::getZoomLabelName() noexcept {
    return "zoom_label";
  }

  inline
  const char*
  StatusBar::getRenderingAreaLabelName() noexcept {
    return "rendering_area_label";
  }

  inline
  sdl::graphic::LabelWidget*
  StatusBar::getMouseCoordsLabel() {
    return getChildAs<sdl::graphic::LabelWidget>(getMouseCoordsLabelName());
  }

  inline
  sdl::graphic::LabelWidget*
  StatusBar::getZoomLabel() {
    return getChildAs<sdl::graphic::LabelWidget>(getZoomLabelName());
  }

  sdl::graphic::LabelWidget*
  StatusBar::getRenderingAreaLabel() {
    return getChildAs<sdl::graphic::LabelWidget>(getRenderingAreaLabelName());
  }

}

#endif    /* STATUS_BAR_HXX */
