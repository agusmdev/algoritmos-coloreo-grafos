static unsigned int s = 1;

unsigned int pseudorandom(unsigned int seed) {
  seed += s << 4;
  seed *= 1103515245;
  seed += 12345;
  seed = (unsigned int) (seed / 65536) % 2048;

  return seed;
}

void set_seed(unsigned int seed)
{
    s = seed;
}