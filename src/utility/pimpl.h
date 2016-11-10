#ifndef PIMPL_H_
#define PIMPL_H_

#include <memory>

template<typename T>
class pimpl {
 public:
  pimpl();
  template<typename ...Args> pimpl(Args&& ...);
  ~pimpl();

  T* operator->();
  const T* operator->() const;
  T& operator*();

 private:
  std::unique_ptr<T> m;
};

#endif // PIMPL_H_
