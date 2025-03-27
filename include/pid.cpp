
class PID {
  public:
    unsigned long lastTime;
    unsigned long now;
    double lastError;
    double _kp, _ki, _kd;
    double _min, _max;
    double errorSum = 0;

    PID(double kp, double ki, double kd, double min, double max) {
      _kp = kp;
      _ki = ki;
      _kd = kd;
      _min = min;
      _max = max;
    }

    double compute(double actual, double desired, unsigned long now) {
      unsigned long dT = now - lastTime;
      double error = desired - actual;
      double errorInt = 0.5 * (error + lastError) * dT;
      errorSum += errorInt;
      double errorRate = (error - lastError) / dT; 

      lastError = error;
      lastTime = now;
      return range(_kp * error + _ki * errorSum + _kd * errorRate);
    }

    double range(double val) {
      if (val >= _max) {
        return _max;
      } else if (val <= _min) {
        return _min;
      } else return val;
    }
};