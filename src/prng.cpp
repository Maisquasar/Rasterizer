#include <time.h>
#include <cmath>
#include <prng.hpp>

PRNG::PRNG()
{
    s1 = time(NULL);
    s2 = s1 ^ 0x54a7f18c;
    s3 = s1 & 0x77;
    for (int i = 0; i < 5; i++) getRandomNumber();
}

double PRNG::getRandomNumber() {
	s1 = (171 * s1) % 30269;
	s2 = (172 * s2) % 30307;
	s3 = (170 * s3) % 30323;
	return std::fmod((s1 / 30269.0 + s2 / 30307.0 + s3 / 30323.0), 1.0);
}