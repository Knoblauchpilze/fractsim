
# include "Fractal.hh"

namespace fractsim {

  Fractal::Fractal():
    utils::CoreObject(std::string("fractal_proxy"))
  {
    setService(std::string("fractal"));
  }

  Fractal::~Fractal() {}

}
