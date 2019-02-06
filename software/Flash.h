#ifndef Flash_h
#define Flash_h

class Flash {
  public:
    byte counter = 0;
    unsigned long duration;
    Flash(unsigned long dur, void (*callback)(bool)) :
      duration{dur}, _callback{callback} {}
    bool run() {
      if (counter == 0) return false;
      if (_state == 0) {
        _callback(false);
        _state = 1;
        _time = millis();
      } else if (_state == 1 && (millis() - _time) > duration) {
        _callback(true);
        _state = 2;
        _time = millis();
      } else if (_state == 2 && (millis() - _time) > duration) {
        _callback(false);
        _state = 3;
        _time = millis();
      } else if (_state == 3 && (millis() - _time) > duration) {
        _state = 0;
        counter--;
      }
      return true;
    }
  private:
    byte _state = 0;
    unsigned long _time = 0;
    void (*_callback)(bool);
};

#endif
