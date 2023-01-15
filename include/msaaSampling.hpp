#pragma once

#include <maths.hpp>

static const Int2D msaa1[1] = {Int2D()};

static const Int2D msaa4[4] = {
    Int2D(-2,-6), Int2D(6,-2),
    Int2D(-6,2), Int2D(2,6),
};

static const Int2D msaa9[9] = {
    Int2D( 1,-3), Int2D(-1, 3), Int2D( 5, 1),
    Int2D(-3,-5), Int2D(-5, 5), Int2D(-7,-1),
    Int2D( 3, 7), Int2D( 7,-7), Int2D( 0, 0),
};

static const Int2D msaa16[16] = {
    Int2D( 1, 1), Int2D(-1,-3), Int2D(-3, 2), Int2D( 4,-1),
    Int2D(-5,-2), Int2D( 2, 5), Int2D( 5, 3), Int2D( 3,-5),
    Int2D(-2, 6), Int2D( 0,-7), Int2D(-4,-6), Int2D(-6, 4),
    Int2D(-8, 0), Int2D( 7,-4), Int2D( 6, 7), Int2D(-7,-8),
};

static const Int2D* const samplePoints[4] {
    msaa1,
    msaa4,
    msaa9,
    msaa16,
};