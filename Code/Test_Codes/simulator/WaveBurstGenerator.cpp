long WaveBurstGenerator::next() {
  x += 1;
  dx += 1;
  if(dx >= u) {
    dx = 0;
    s = !s;
    long _min = s ? burstmin : lowmin;
    long _max = s ? burstmax : lowmax;
    u  = random(_min, _max);
    w1 = random(w1min, w1max);
    w2 = random(w2min, w2max);
  }
  return s ? long(A * sin(w1 * x) * cos(w2 * x) + B) : 0;
}  

