#include "geostate.h"
#include "rtweekend.h"

int main() {
    // GeoState s{8.0, 1.1, 0.0, 0.0, 0.3, 0.5};   // r, theta, phi, t, pr, ptheta
    // double E = 1.0, Lz = 2.0, a = 0.7, M = 1.0, h = 0.01;

    /* Check dpr/dl */
    // GeoState sp = s; sp.r += h;
    // GeoState sm = s; sm.r -= h;
    // double num_r = -(hamiltonian(sp,E,Lz,a,M) - hamiltonian(sm,E,Lz,a,M)) / (2*h);
    // double ana_r = deriv(s,E,Lz,a,M).pr;
    // std::cout << "dpr:  " << ana_r << "  " << num_r << "\n";

   /* Check dptheta/dl */
    // GeoState tp = s; tp.theta += h;
    // GeoState tm = s; tm.theta -= h;
    // double num_t = -(hamiltonian(tp,E,Lz,a,M) - hamiltonian(tm,E,Lz,a,M)) / (2*h);
    // double ana_t = deriv(s,E,Lz,a,M).ptheta;
    // std::cout << "dpth: " << ana_t << "  " << num_t << "\n";

    /* Check step works */
    // GeoState d = deriv(s, E, Lz, a, M);
    // std::cout << "deriv: dr=" << d.r << " dth=" << d.theta 
    //         << " dpr=" << d.pr << " dpth=" << d.ptheta << "\n";

    // for (int i = 0; i < 10; i++) {
    //     s = rk4(s, h, E, Lz, a, M);
    //     std::cout << i << ": r=" << s.r << " th=" << s.theta 
    //             << " pr=" << s.pr << " H=" << hamiltonian(s,E,Lz,a,M) << "\n";
    // }

    /* Check H verifies to ~0 at camera r = 50*/
    // double spin = 0.7;
    // double M = 1.0;
    // double nr = -0.7, nth = 0.5, nph = 0.5;
    // double mag = std::sqrt(nr*nr + nth*nth + nph*nph);
    // nr /= mag; nth /= mag; nph /= mag;

    // RayInit ray = init_ray(50.0, 1.4, 0.0, 
    //                        nr, nth, nph, spin, M);

    // std::cout << "H = " << hamiltonian(ray.state, ray.E, ray.Lz, spin, M) << "\n";
    // std::cout << "E = " << ray.E << "  Lz = " << ray.Lz << "\n";

    /* Testing ray */
    double a = 0.0, M = 1.0;

    // Fire ray from r = 50 toward hole with some impact paramter
    double b = 5.0;
    double nr = -std::sqrt(1 - (b/50.0)*(5/50.0));
    double nph = b/50.0;
    double nth = 0.0;

    RayInit ray = init_ray(50.0, pi/2, 0.0, nr, nth, nph, a, M);
    std::cout << "H = " << hamiltonian(ray.state, ray.E, ray.Lz, a, M) << "\n";

    GeoState s = ray.state;
    double h = 0.01;
    for (int i = 0; i < 100000; i++) {
        s = rk4(s, h, ray.E, ray.Lz, a, M);
        if (i % 100 == 0) std::cout << i << ": r=" << s.r << " H=" << hamiltonian(s, ray.E, ray.Lz, a, M) << "\n";
        if (s.r < 2.02) { std::cout << "CAPTURED at step " << i << "\n"; break; }
        if (s.r > 100)  { std::cout << "ESCAPED at step " << i << "\n"; break; }
    }
}