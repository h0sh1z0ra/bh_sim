#ifndef GEOSTATE_H
#define GEOSTATE_H

#include <cmath>
#include "rtweekend.h"

struct GeoState {
    double r, theta, phi, t;        // position
    double pr, ptheta;              // momenta to integrate

    GeoState() : r(0), theta(0), phi(0), t(0), pr(0), ptheta(0) {}

    GeoState(double r_, double th_, double ph_, double t_, double pr_, double pth_)
        : r(r_), theta(th_), phi(ph_), t(t_), pr(pr_), ptheta(pth_) {}
};

    GeoState deriv(const GeoState& s, double E, double Lz, double a, double M) {
        // Convenience
        double r = s.r, theta = s.theta, p_r = s.pr, p_theta = s.ptheta, L_z = Lz;


        // Efficiency
        double sth  = std::sin(theta), cth = std::cos(theta);
        double sth2 = sth*sth, cth2 = cth*cth;
        double r2   = r*r, a2 = a*a;
        double Lz2  = Lz*Lz, pr2 = p_r*p_r;
        double a2r2 = (a2+r2)*(a2+r2);

        // Big Greeks
        double Sigma = r2 + a2 *cth*cth;
        double Delta = r2 - 2*M*r + a2;
        double A     = (r2+a2)*(r2+a2) - a2*Delta*sth2;
        double Sigma2 = Sigma*Sigma;
        double Delta2 = Delta*Delta;

        // Inverse metrics
        double gtt     = -A/(Sigma*Delta);
        double gtphi   = -(2*M*a*r)/(Sigma*Delta);
        double gphiphi = (Delta - (a2)*(sth*sth))/(Sigma*Delta*sth*sth);

        double dr_dl      = (Delta/Sigma) * p_r;
        double dth_dl     = (1/Sigma) * p_theta;

        double dpr_dl     = (-E*r*(E*(a2*(-2*M*r + a2 + r2)*sth2 - a2r2) + 4*L_z*M*a*r)*(-2*M*r + a2 + r2)*sth2 
                          + E*(M - r)*(E*(a2*(-2*M*r + a2 + r2)*sth2 - a2r2) 
                          + 4*L_z*M*a*r)*(a2*cth2 + r2)*sth2 + E*(E*(a2*(-M + r)*sth2 - 2*r*(a2 + r2)) 
                          + 2*L_z*M*a)*(a2*cth2 + r2)*(-2*M*r + a2 + r2)*sth2 
                          + Lz2*r*(-2*M*r + a2 + r2)*(2*M*r - a2*cth2 - r2) 
                          - Lz2*(M - r)*(a2*cth2 + r2)*(-2*M*r + a2 + r2) 
                          - Lz2*(M - r)*(a2*cth2 + r2)*(2*M*r - a2*cth2 - r2) 
                          + pr2*(-M + r)*(a2*cth2 + r2)*Delta2*sth2 
                          + r*(pr2*(-Delta) - p_theta*p_theta)*Delta2*sth2)/(Sigma2*Delta2*sth2);

        double dptheta_dl = (E*E*a2*(a2*cth2 + r2)*(-2*M*r + a2 + r2)*sth2*sth2 
                          + E*a2*(E*(a2*(-2*M*r + a2 + r2)*sth2 - a2r2) 
                          + 4*L_z*M*a*r)*sth2*sth2 - Lz2*a2*(a2*cth2 + r2)*sth2 - Lz2*a2*(2*M*r - a2*cth2 - r2)*sth2 
                          + Lz2*(a2*cth2 + r2)*(2*M*r - a2*cth2 - r2) 
                          + a2*(pr2*(-2*M*r + a2 + r2) + p_theta*p_theta)*(-2*M*r + a2 
                          + r2)*sth2*sth2)*cth/(Sigma2*(-2*M*r + a2 + r2)*sth2*sth);

        double dt_dl      = -gtt*E + gtphi*Lz;
        double dphi_dl    = -gtphi*E + gphiphi*Lz;

        GeoState d;
        d.r      = dr_dl;       // NOT RADIUS!!!
        d.theta  = dth_dl;
        d.phi    = dphi_dl;
        d.t      = dt_dl;       
        d.pr     = -dpr_dl;     // Flip signs since they're flipped for some reason idk
        d.ptheta = -dptheta_dl;

        return d;
    }

    double hamiltonian(const GeoState& s, double E, double Lz, double spin, double M) {
        double r = s.r, theta = s.theta, p_r = s.pr, p_theta = s.ptheta, L_z = Lz;

        double sth  = std::sin(theta), cth = std::cos(theta);
        double sth2 = sth*sth, cth2 = cth*cth;
        double r2   = r*r;
        double a2 = spin*spin;

        // Big Greeks
        double Sigma = r2 + a2 *cth*cth;
        double Delta = r2 - 2*M*r + a2;

        double A     = (r2+a2)*(r2+a2) - a2*Delta*sth2;

        // Inverse metrics
        double grr     = Delta/Sigma;
        double gthth   = 1 / Sigma;
        double gtt     = -A/(Sigma*Delta);
        double gtphi   = -(2*M*spin*r)/(Sigma*Delta);
        double gphiphi = (Delta - (a2)*(sth*sth))/(Sigma*Delta*sth*sth);

        auto H = 0.5*(gtt*E*E - 2*gtphi*E*Lz + gphiphi*Lz*Lz + grr*p_r*p_r + gthth*p_theta*p_theta);
        return H;
    }

    // Operators for RK4
    GeoState operator+(const GeoState& a, const GeoState& b) {
        return GeoState(a.r+b.r, a.theta+b.theta, a.phi+b.phi, 
                        a.t+b.t, a.pr+b.pr, a.ptheta+b.ptheta);
    }

    GeoState operator*(const GeoState& s, double h) {
        return GeoState(s.r*h, s.theta*h, s.phi*h, s.t*h, s.pr*h, s.ptheta*h);
    }

    GeoState operator*(double h, const GeoState& s) { 
        return s*h; 
    }

    GeoState rk4(const GeoState& y, double h, double E, double Lz, double a, double M) {
        GeoState k1 = deriv(y, E, Lz, a, M);
        GeoState k2 = deriv(y + k1*(h/2), E, Lz, a, M);
        GeoState k3 = deriv(y + k2*(h/2), E, Lz, a, M);
        GeoState k4 = deriv(y + k3*h, E, Lz, a, M);

        GeoState result =  y + (k1 + k2*2 + k3*2 + k4) * (h/6);

        std::cout << "rk4: h=" << h << " k1.r=" << k1.r 
              << " y.r=" << y.r << " result.r=" << result.r << "\n";
        return result;
    }

#endif