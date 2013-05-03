#include "BaseType.h"

Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::Green(0.0f , 1.0f, 0.0f, 1.0f);
Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);

void Matrix::inverse()
{
    inverse(*this);
}

void Matrix::inverse(const Matrix &m)
{
    float factor[6];
    Matrix cof;
    cof.transpose(*this);

    factor[0] = cof.c3 * cof.d4 - cof.c4 * cof.d3;
    factor[1] = cof.c2 * cof.d4 - cof.c4 * cof.d2;
    factor[2] = cof.c2 * cof.d3 - cof.c3 * cof.d2;
    factor[3] = cof.c1 * cof.d4 - cof.c4 * cof.d1;
    factor[4] = cof.c3 * cof.d1 - cof.c1 * cof.d3;
    factor[5] = cof.c1 * cof.d2 - cof.c2 * cof.d1;

    a1	 = cof.b2 * factor[0] - cof.b3 * factor[1] + cof.b4 * factor[2];
    a2  = -cof.b1 * factor[0] + cof.b3 * factor[3] + cof.b4 * factor[4];
    a3  = cof.b1 * factor[1] - cof.b2 * factor[3] + cof.b4 * factor[5];
    a4  = -cof.b1 * factor[2] - cof.b2 * factor[4] - cof.b3 * factor[5];

    b1  = -cof.a2 * factor[0] + cof.a3 * factor[1] - cof.a4 * factor[2];
    b2  = cof.a1 * factor[0] - cof.a3 * factor[3] - cof.a4 * factor[4];
    b3  = -cof.a1 * factor[1] + cof.a2 * factor[3] - cof.a4 * factor[5];
    b4  = cof.a1 * factor[2] + cof.a2 * factor[4] + cof.a3 * factor[5];

    factor[0] = cof.a3 * cof.b4 - cof.a4 * cof.b3;
    factor[1] = cof.a2 * cof.b4 - cof.a4 * cof.b2;
    factor[2] = cof.a2 * cof.b3 - cof.a3 * cof.b2;
    factor[3] = cof.a1 * cof.b4 - cof.a4 * cof.b1;
    factor[4] = cof.a3 * cof.b1 - cof.a1 * cof.b3;
    factor[5] = cof.a1 * cof.b2 - cof.a2 * cof.b1;

    c1	 = factor[0] * cof.d2 - factor[1] * cof.d3 + factor[2] * cof.d4;
    c2  = -factor[0] * cof.d1 + factor[3] * cof.d3 + factor[4] * cof.d4;
    c3  = factor[1] * cof.d1 - factor[3] * cof.d2 + factor[5] * cof.d4;
    c4  = -factor[2] * cof.d1 - factor[4] * cof.d2 - factor[5] * cof.d3;

    d1  = -factor[0] * cof.c2 + factor[1] * cof.c3 - factor[2] * cof.c4;
    d2  = factor[0] * cof.c1 - factor[3] * cof.c3 - factor[4] * cof.c4;
    d3  = -factor[1] * cof.c1 + factor[3] * cof.c2 - factor[5] * cof.c4;
    d4  = factor[2] * cof.c1 + factor[4] * cof.c2 + factor[5] * cof.c3;

    float inv_det = 1.0f / (a1 * cof.a1 + a2 * cof.a2 + a3 * cof.a3 + a4 * cof.a4);
    *this *= inv_det;
    return ;
}
