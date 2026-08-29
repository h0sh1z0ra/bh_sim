#ifndef GEOSTATE_H
#define GEOSTATE_H

#include <cmath>
#include <cassert>

#include "rtweekend.h"
#include "sky.h"

using namespace std;

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
        double sth  = sin(theta), cth = cos(theta);
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

        double sth  = sin(theta), cth = cos(theta);
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
        return H; // Should remain constant throughout RK4 steps
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

        // cout << "rk4: h=" << h << " k1.r=" << k1.r 
        //       << " y.r=" << y.r << " result.r=" << result.r << "\n";
        return result;
    }

struct RayInit {
    GeoState state;
    double E, Lz;
};

RayInit init_ray(double r_cam, double th_cam, double ph_cam,  // camera pos
                 double n_r, double n_th, double n_ph, double spin, double M) { // direction
    /* 1. Metric components at camera position */
    double r = r_cam, th = th_cam;

    double sth  = sin(th), cth = cos(th);
    double sth2 = sth*sth, cth2 = cth*cth;
    double r2   = r*r, a2 = spin*spin;

    // Big Greeks
    double Sigma = r2 + a2 *cth*cth, Delta = r2 - 2*M*r + a2;
    
    // Lowered metrics
    double g_rr     =  Sigma/Delta;
    double g_thth   =  Sigma;
    double g_tt     = -(1 - 2*M*r/Sigma);
    double g_tphi   = -(2*M*spin*r*sth2)/Sigma;
    double g_phiphi =  (r2 + a2 + 2*M*a2*r*sth2/Sigma)*sth2;

    /* 2. Contravariant momentum from tetrad */
    double pth = n_th / sqrt(Sigma);
    double pr  = sqrt(Delta/Sigma) * n_r;

    // Coupled
    double denom = sqrt(g_phiphi - g_tphi*g_tphi/g_tt);
    double pphi = n_ph / denom;
    double pt   = 1.0/std::sqrt(-g_tt) - (g_tphi/g_tt) * pphi;

    /* 3. Lower indices */
    double p_t   = g_tt*pt + g_tphi*pphi;
    double p_phi = g_tphi*pt + g_phiphi*pphi;
    double p_r   = g_rr*pr;
    double p_th  = g_thth*pth;

    double E = -p_t, Lz = p_phi;

    /* 4. Return */
    RayInit out;
    out.state = GeoState{r_cam, th_cam, ph_cam, 0.0, p_r, p_th};
    out.E = E;
    out.Lz = Lz;
    return out;
}


void cartToBL(const point3& p, double& r, double& th, double& ph) {
    r = p.length();
    th = acos(p.z()/r);
    ph = atan2(p.y(), p.x());
}

void cartToSph(const point3& dir, double& th, double& ph, double& n_r, double& n_th, double& n_ph) {
    double sth = sin(th), cth = cos(th);
    double sph = sin(ph), cph = cos(ph);

    // Spherical units
    vec3 rhat  ( sth*cph, sth*sph, cth );
    vec3 thhat ( cth*cph, cth*sph, -sth );
    vec3 phhat ( -sph, cph, 0 );

    n_r  = dot(dir, rhat);
    n_th = dot(dir, thhat);
    n_ph = dot(dir, phhat);

    double mag2 = n_r*n_r + n_th*n_th + n_ph*n_ph;
    // std::cout << "th=" << th << " ph=" << ph
    //           << " n=(" << dir.x() << "," << dir.y() << "," << dir.z() << ")"
    //           << " -> (" << n_r << "," << n_th << "," << n_ph << ") mag2=" << mag2 << "\n";
    assert(abs(mag2 - 1.0) < 1e-9);
}

color march(const RayInit& ray, double spin, double M) {
    GeoState s = ray.state;
    double r_horizon = M + std::sqrt(M*M - spin*spin);  // Outer horizon
    double h = 0.05;
    int max_steps = 100000;

    for (int i = 0; i < max_steps; i++) {
        // Shrink step size near horizon
        double dh = h * std::min(1.0, (s.r - r_horizon) / (10.0 * M));
        dh = std::max(dh, 1e-5);          // floor so it doesn't stall

        // Step
        s = rk4(s, dh, ray.E, ray.Lz, spin, M);
        
        // 1.01 avoids BL coordinates blowing up
        if (s.r < r_horizon * 1.01 || s.r < 0 || std::isnan(s.r)) 
            return color(0,0,0); // Fell into BH
        if (s.r > 200.0) {
            return sky_color(s.theta, s.phi);
        }
    }
    return color(1,0,0);  // red if it runs out of steps
}

#endif