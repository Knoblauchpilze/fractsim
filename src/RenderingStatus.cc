
# include "RenderingStatus.hh"
# include <sdl_graphic/LinearLayout.hh>
# include <sdl_graphic/Button.hh>
# include <sdl_graphic/LabelWidget.hh>

namespace fractsim {

  RenderingStatus::RenderingStatus(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::core::SdlWidget(std::string("rendering_status"),
                         hint,
                         parent,
                         sdl::core::engine::Color::NamedColor::Silver),

    m_locker()
  {
    build();
  }

  void
  RenderingStatus::build() {
    // Set a focus policy which do not allow to hover or click on this widget.
    setFocusPolicy(sdl::core::FocusPolicy());

    // The rendering status is composed of a display allowing to display the
    // current zoom level, the progress bar of the current computation, some
    // indication of the palette to use and a render button.
    sdl::graphic::LinearLayoutShPtr layout = std::make_shared<sdl::graphic::LinearLayout>(
      "rendering_status_layout",
      this,
      sdl::graphic::LinearLayout::Direction::Horizontal
    );

    // And assign the layout to this widget.
    setLayout(layout);

    sdl::graphic::Button* render = new sdl::graphic::Button(
      "rendering_status_render",
      "Render",
      "data/img/compute.bmp",
      "data/fonts/Goodtime.ttf",
      15u,
      this
    );
    if (render == nullptr) {
      error(
        std::string("Could not create rendering status"),
        std::string("Render button not allocated")
      );
    }

    render->setMaxSize(getRenderButtonMaxSize());

    layout->addItem(render);
  }

}
