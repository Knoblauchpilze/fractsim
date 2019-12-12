
# include "RenderingStatus.hh"
# include <sdl_graphic/LinearLayout.hh>
# include <sdl_graphic/Button.hh>

namespace fractsim {

  RenderingStatus::RenderingStatus(const utils::Sizef& hint,
                                   sdl::core::SdlWidget* parent):
    sdl::core::SdlWidget(std::string("rendering_status"),
                         hint,
                         parent),

    m_propsLocker()
  {
    build();
  }

  void
  RenderingStatus::build() {
    // Set a focus policy which do not allow to hover or click on this widget.
    setFocusPolicy(sdl::core::FocusPolicy());

    // The rendering status is composed of a display allowing to display the
    // progress bar of the current computation, some indication of the palette
    // to use and a render button.
    sdl::graphic::LinearLayoutShPtr layout = std::make_shared<sdl::graphic::LinearLayout>(
      "rendering_status_layout",
      this,
      sdl::graphic::LinearLayout::Direction::Horizontal,
      getGlobalMargins(),
      getComponentMargins()
    );

    // And assign the layout to this widget.
    setLayout(layout);

    sdl::graphic::Button* render = new sdl::graphic::Button(
      getRenderButtonName(),
      "Render",
      "data/img/compute.bmp",
      "data/fonts/Goodtime.ttf",
      15u,
      this,
      utils::Sizef(),
      sdl::core::engine::Color::fromRGB(0.7031f, 0.7031f, 0.7031f)
    );
    if (render == nullptr) {
      error(
        std::string("Could not create rendering status"),
        std::string("Render button not allocated")
      );
    }

    sdl::graphic::ProgressBar* progress = new sdl::graphic::ProgressBar(
      getProgressBarName(),
      this
    );

    // Configure each element.
    render->setMaxSize(getRenderButtonMaxSize());

    // Add each element to the layout.
    layout->addItem(render);
    layout->addItem(progress);
  }

}
