#ifndef    GMP_BOX_HXX
# define   GMP_BOX_HXX

# include "GMPBox.hh"

namespace fractsim {

  inline
  GMPBox::GMPBox():
    m_x(),
    m_y(),

    m_w(),
    m_h()
  {
    reset();
    setFromBox(utils::Boxf());
  }

  inline
  GMPBox::GMPBox(const utils::Boxf& box):
    m_x(),
    m_y(),

    m_w(),
    m_h()
  {
    reset();
    setFromBox(box);
  }

  inline
  GMPBox::~GMPBox() {
    // Clear the internal values.
    clear();
  }

  inline
  void
  GMPBox::setFromBox(const utils::Boxf& box) {
    clear();
    reset();

    mpf_set_d(m_x, box.x());
    mpf_set_d(m_x, box.y());

    mpf_set_d(m_w, box.w());
    mpf_set_d(m_h, box.h());
  }

  inline
  void
  GMPBox::reset() {
    mpf_init(m_x);
    mpf_init(m_y);

    mpf_init(m_w);
    mpf_init(m_h);
  }

  inline
  void
  GMPBox::clear() {
    mpf_clear(m_x);
    mpf_clear(m_y);

    mpf_clear(m_w);
    mpf_clear(m_h);
  }

}

#endif    /* GMP_BOX_HXX */
