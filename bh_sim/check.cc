#include "geostate.h"
#include "rtweekend.h"

int main() {
    GeoState s{8.0, 1.1, 0.0, 0.0, 0.3, 0.5};   // r, theta, phi, t, pr, ptheta
    double E = 1.0, Lz = 2.0, a = 0.7, M = 1.0, h = 0.01;

    // // check dpr/dl
    // GeoState sp = s; sp.r += h;
    // GeoState sm = s; sm.r -= h;
    // double num_r = -(hamiltonian(sp,E,Lz,a,M) - hamiltonian(sm,E,Lz,a,M)) / (2*h);
    // double ana_r = deriv(s,E,Lz,a,M).pr;
    // std::cout << "dpr:  " << ana_r << "  " << num_r << "\n";

    // // check dptheta/dl
    // GeoState tp = s; tp.theta += h;
    // GeoState tm = s; tm.theta -= h;
    // double num_t = -(hamiltonian(tp,E,Lz,a,M) - hamiltonian(tm,E,Lz,a,M)) / (2*h);
    // double ana_t = deriv(s,E,Lz,a,M).ptheta;
    // std::cout << "dpth: " << ana_t << "  " << num_t << "\n";

    GeoState d = deriv(s, E, Lz, a, M);
    std::cout << "deriv: dr=" << d.r << " dth=" << d.theta 
            << " dpr=" << d.pr << " dpth=" << d.ptheta << "\n";

    for (int i = 0; i < 10; i++) {
        s = rk4(s, h, E, Lz, a, M);
        std::cout << i << ": r=" << s.r << " th=" << s.theta 
                << " pr=" << s.pr << " H=" << hamiltonian(s,E,Lz,a,M) << "\n";
    }
}