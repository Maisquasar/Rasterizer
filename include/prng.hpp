#pragma once

class PRNG
{
    private:
    unsigned int s1, s2, s3;
    public:
    // Mersenne twister
    PRNG();
    double getRandomNumber();
};