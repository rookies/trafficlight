#ifndef ModeSwitcher_h
#define ModeSwitcher_h

template <typename T, byte N>
struct ModeSwitcher {
  T mode = 0;
  T next() {
    mode = (mode + 1) % N;
    return mode;
  }
};

#endif
