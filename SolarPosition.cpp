#include "SolarPosition.h"

#include "Globals.h"

#include <cmath>

SolarPosition::SolarPosition(void) {
}

SolarPosition::~SolarPosition(void) {
}

void SolarPosition::FindPosition(const Julian& j, Eci& eci) {

    const double mjd = j.FromJan1_12h_1900();
    const double year = 1900 + mjd / 365.25;
    const double T = (mjd + Delta_ET(year) / Globals::SEC_PER_DAY()) / 36525.0;
    const double M = Globals::Deg2Rad(Modulus(358.47583 + Modulus(35999.04975 * T, 360.0)
            - (0.000150 + 0.0000033 * T) * T * T, 360.0));
    const double L = Globals::Deg2Rad(Modulus(279.69668 + Modulus(36000.76892 * T, 360.0)
            + 0.0003025 * T*T, 360.0));
    const double e = 0.01675104 - (0.0000418 + 0.000000126 * T) * T;
    const double C = Globals::Deg2Rad((1.919460 - (0.004789 + 0.000014 * T) * T) * sin(M)
            + (0.020094 - 0.000100 * T) * sin(2 * M) + 0.000293 * sin(3 * M));
    const double O = Globals::Deg2Rad(Modulus(259.18 - 1934.142 * T, 360.0));
    const double Lsa = Modulus(L + C - Globals::Deg2Rad(0.00569 - 0.00479 * sin(O)), Globals::TWOPI());
    const double nu = Modulus(M + C, Globals::TWOPI());
    double R = 1.0000002 * (1 - e * e) / (1 + e * cos(nu));
    const double eps = Globals::Deg2Rad(23.452294 - (0.0130125 + (0.00000164 -
            0.000000503 * T) * T) * T + 0.00256 * cos(O));
    R = R * Globals::AU();

    Vector solar_position = Vector(R * cos(Lsa),
            R * sin(Lsa) * cos(eps),
            R * sin(Lsa) * sin(eps),
            R);

    eci = Eci(j, solar_position);
}

double SolarPosition::Modulus(double arg1, double arg2) const {

    double result = arg1 - arg2 * floor(arg1 / arg2);
    if (result < 0.0)
        result += arg2;
    return result;
}

double SolarPosition::Delta_ET(double year) const {

    return 26.465 + 0.747622 * (year - 1950) + 1.886913 * sin(Globals::TWOPI() * (year - 1975) / 33);
}
