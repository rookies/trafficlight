#ifndef Flash_hh
#define Flash_hh

/**
 * Flashes lights in the following pattern:
 *         ____    ____    ____        ON
 * ________|  |____|  |____|  |_______ OFF
 *    T1    T2  T2  T2  T2  T2   T1
*/
class Flash {
  public:
    byte counter = 0;
    unsigned long duration1, duration2;
    Flash(unsigned long T1, unsigned long T2, void (*callback)(bool)) :
      duration1{T1}, duration2{T2}, _callback{callback} {}
    bool run() {
      switch (_state) {
        case 0:
          /* Wait until counter > 0, don't change lights. */
          if (counter > 0) {
            /* To state 1, turn lights off. */
            _state = 1;
            _time = millis();
            _callback(false);
          } else {
            return false;
          }
          break;
        case 1:
          /* Wait duration 1. */
          if ((millis() - _time) > duration1) {
            /* To state 2, turn lights on. */
            _state = 2;
            _time = millis();
            _callback(true);
          }
          break;
        case 2:
          /* Check counter or wait duration 2. */
          if (counter == 0) {
            /* To state 4, turn lights off. */
            _state = 4;
            _time = millis();
            _callback(false);
          } else if ((millis() - _time) > duration2) {
            /* To state 3, turn lights off, decrement counter. */
            _state = 3;
            _time = millis();
            _callback(false);
            counter--;
          }
          break;
        case 3:
          /* Wait duration 2. */
          if ((millis() - _time) > duration2) {
            /* To state 2, turn lights on. */
            _state = 2;
            _time = millis();
            _callback(true);
          }
          break;
        case 4:
          /* Wait duration 1. */
          if ((millis() - _time) > duration1) {
            /* To state 0. */
            _state = 0;
          }
          break;
        default:
          _state = 0;
      }
      return true;
    }
    void reset() {
      _state = 0;
    }
  private:
    byte _state = 0;
    unsigned long _time = 0;
    void (*_callback)(bool);
};

#endif
