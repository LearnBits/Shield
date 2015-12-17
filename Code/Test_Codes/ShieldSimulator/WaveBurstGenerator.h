#ifndef WaveBurstGenerator_h
#define WaveBurstGenerator_h

#include <Arduino.h>

class WaveBurstGenerator {

  long A, B, w1min, w1max, w2min, w2max, lowmin, lowmax, burstmin, burstmax;
  long u, w1, w2, x, dx;
  boolean s;

  public:

    WaveBurstGenerator(long A, long B, long w1min, long w1max, long w2min, long w2max,
                       long lowmin, long lowmax, long burstmin, long burstmax) :
      A(A), B(B), w1min(w1min), w1max(w1max), w2min(w2min), w2max(w2max), 
      lowmin(lowmin), lowmax(lowmax), burstmin(burstmin), burstmax(burstmax),
      u(0), w1(0), w2(0), x(0), dx(0), s(false)
      {}

    long next();
};

#endif


