#include "BaseType.h"

Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::Green(0.0f , 1.0f, 0.0f, 1.0f);
Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);

Vector &Vector::operator*=(const Matrix &m) {
	*this = *this * m;
	return *this;
}

void Matrix::inverse()
{
    inverse(*this);
}

void Matrix::inverse(const Matrix &m)
{
    float factor[6];
    Matrix cof;
    cof.transpose(*this);

    factor[0] = cof(2,2) * cof(3,3) - cof(2,3) * cof(3,2);
    factor[1] = cof(2,1) * cof(3,3) - cof(2,3) * cof(3,1);
    factor[2] = cof(2,1) * cof(3,2) - cof(2,2) * cof(3,1);
    factor[3] = cof(2,0) * cof(3,3) - cof(2,3) * cof(3,0);
    factor[4] = cof(2,2) * cof(3,0) - cof(2,0) * cof(3,2);
    factor[5] = cof(2,0) * cof(3,1) - cof(2,1) * cof(3,0);

    _data[0][0]	 = cof(1,1) * factor[0] - cof(1,2) * factor[1] + cof(1,3) * factor[2];
    _data[0][1]  = -cof(1,0) * factor[0] + cof(1,2) * factor[3] + cof(1,3) * factor[4];
    _data[0][2]  = cof(1,0) * factor[1] - cof(1,1) * factor[3] + cof(1,3) * factor[5];
    _data[0][3]  = -cof(1,0) * factor[2] - cof(1,1) * factor[4] - cof(1,2) * factor[5];

    _data[1][0]  = -cof(0,1) * factor[0] + cof(0,2) * factor[1] - cof(0,3) * factor[2];
    _data[1][1]  = cof(0,0) * factor[0] - cof(0,2) * factor[3] - cof(0,3) * factor[4];
    _data[1][2]  = -cof(0,0) * factor[1] + cof(0,1) * factor[3] - cof(0,3) * factor[5];
    _data[1][3]  = cof(0,0) * factor[2] + cof(0,1) * factor[4] + cof(0,2) * factor[5];

    factor[0] = cof(0,2) * cof(1,3) - cof(0,3) * cof(1,2);
    factor[1] = cof(0,1) * cof(1,3) - cof(0,3) * cof(1,1);
    factor[2] = cof(0,1) * cof(1,2) - cof(0,2) * cof(1,1);
    factor[3] = cof(0,0) * cof(1,3) - cof(0,3) * cof(1,0);
    factor[4] = cof(0,2) * cof(1,0) - cof(0,0) * cof(1,2);
    factor[5] = cof(0,0) * cof(1,1) - cof(0,1) * cof(1,0);

    _data[2][0]	 = factor[0] * cof(3,1) - factor[1] * cof(3,2) + factor[2] * cof(3,3);
    _data[2][1]  = -factor[0] * cof(3,0) + factor[3] * cof(3,2) + factor[4] * cof(3,3);
    _data[2][2]  = factor[1] * cof(3,0) - factor[3] * cof(3,1) + factor[5] * cof(3,3);
    _data[2][3]  = -factor[2] * cof(3,0) - factor[4] * cof(3,1) - factor[5] * cof(3,2);

    _data[3][0]  = -factor[0] * cof(2,1) + factor[1] * cof(2,2) - factor[2] * cof(2,3);
    _data[3][1]  = factor[0] * cof(2,0) - factor[3] * cof(2,2) - factor[4] * cof(2,3);
    _data[3][2]  = -factor[1] * cof(2,0) + factor[3] * cof(2,1) - factor[5] * cof(2,3);
    _data[3][3]  = factor[2] * cof(2,0) + factor[4] * cof(2,1) + factor[5] * cof(2,2);

    float inv_det = 1.0f / (_data[0][0] * cof(0,0) +
    		_data[0][1] * cof(0,1) +
    		_data[0][2] * cof(0,2) +
    		_data[0][3] * cof(0,3));
    *this *= inv_det;
    return ;
}
