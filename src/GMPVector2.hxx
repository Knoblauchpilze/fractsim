#ifndef    GMP_VECTOR2_HXX
# define   GMP_VECTOR2_HXX

# include "GMPVector2.hh"

namespace fractsim {

  inline
  GMPVector2::GMPVector2():
    m_x(),
    m_y()
  {
    reset();
    setFromVector(utils::Vector2f());
  }

  inline
  GMPVector2::GMPVector2(const utils::Vector2f& vec):
    m_x(),
    m_y()
  {
    reset();
    setFromVector(vec);
  }

  inline
  GMPVector2::~GMPVector2() {
    clear();
  }

  inline
  void
  GMPVector2::setFromVector(const utils::Vector2f& vec) {
    clear();
    reset();

    mpf_set_d(m_x, vec.x());
    mpf_set_d(m_y, vec.y());
  }

  inline
  void
  GMPVector2::reset() {
    mpf_init(m_x);
    mpf_init(m_y);
  }

  inline
  void
  GMPVector2::clear() {
    mpf_clear(m_x);
    mpf_clear(m_y);
  }

}

#endif    /* GMP_VECTOR2_HXX */
