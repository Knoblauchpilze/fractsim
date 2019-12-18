
# include "StatusBar.hh"
# include <sdl_graphic/LinearLayout.hh>

namespace fractsim {

  StatusBar::StatusBar(const utils::Sizef& hint,
                       sdl::core::SdlWidget* parent):
    sdl::core::SdlWidget(std::string("status_bar"),
                         hint,
                         parent),

    m_propsLocker()
  {
    build();
  }

  void
  StatusBar::build() {
    // Set a focus policy which do not allow to hover or click on this widget.
    setFocusPolicy(sdl::core::FocusPolicy());

    // The status bar is composed of a display allowing to display the current
    // coordinates of the mouse cursor in real world frame along with a label
    // displaying the total rendering area.
    sdl::graphic::LinearLayoutShPtr layout = std::make_shared<sdl::graphic::LinearLayout>(
      "status_bar_layout",
      this,
      sdl::graphic::LinearLayout::Direction::Horizontal,
      getGlobalMargins(),
      getComponentMargins()
    );

    // And assign the layout to this widget.
    setLayout(layout);

    sdl::graphic::LabelWidget* mouseCoords = new sdl::graphic::LabelWidget(
      getMouseCoordsLabelName(),
      "x: 0.0 y: 0.0",
      getInfoLabelFont(),
      15u,
      sdl::graphic::LabelWidget::HorizontalAlignment::Left,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      this,
      sdl::core::engine::Color::NamedColor::Gray
    );
    if (mouseCoords == nullptr) {
      error(
        std::string("Could not create status bar"),
        std::string("Mouse coordinates label not allocated")
      );
    }

    sdl::graphic::LabelWidget* zoom = new sdl::graphic::LabelWidget(
      getZoomLabelName(),
      "Zoom: 1",
      getInfoLabelFont(),
      15u,
      sdl::graphic::LabelWidget::HorizontalAlignment::Center,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      this,
      sdl::core::engine::Color::fromRGB(1.0f, 0.75, 0.25f)
    );
    if (zoom == nullptr) {
      error(
        std::string("Could not create status bar"),
        std::string("Zoom label not allocated")
      );
    }

    sdl::graphic::LabelWidget* renderingArea = new sdl::graphic::LabelWidget(
      getRenderingAreaLabelName(),
      "x: [0, 0] y: [0, 0]",
      getInfoLabelFont(),
      15u,
      sdl::graphic::LabelWidget::HorizontalAlignment::Center,
      sdl::graphic::LabelWidget::VerticalAlignment::Center,
      this,
      sdl::core::engine::Color::NamedColor::Gray
    );
    if (renderingArea == nullptr) {
      error(
        std::string("Could not create status bar"),
        std::string("Rendering area label not allocated")
      );
    }

    // Configure each element.
    mouseCoords->setFocusPolicy(sdl::core::FocusPolicy());
    zoom->setFocusPolicy(sdl::core::FocusPolicy());
    renderingArea->setFocusPolicy(sdl::core::FocusPolicy());

    // Add each element to the layout.
    layout->addItem(mouseCoords);
    layout->addItem(zoom);
    layout->addItem(renderingArea);
  }

}
