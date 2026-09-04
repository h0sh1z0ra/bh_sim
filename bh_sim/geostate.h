#ifndef GEOSTATE_H
#define GEOSTATE_H

#include <cmath>
#include <cassert>
#include <atomic>

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
    // Step counter
    std::atomic<long long> total_steps{0};
    std::atomic<long long> maxxed_rays{0};
    std::atomic<double > max_H{0.0};

    GeoState deriv(const GeoState& s, double E, double Lz, double a, double M) {
        // Convenience
        double r = s.r, theta = s.theta, p_r = s.pr, p_theta = s.ptheta, L_z = Lz;
        
        // // Efficiency
        // double sth  = sin(theta), cth = cos(theta);
        // double sth2 = sth*sth, cth2 = cth*cth;
        // double r2   = r*r, a2 = a*a;
        // double Lz2  = Lz*Lz, pr2 = p_r*p_r;
        // double a2r2 = (a2+r2)*(a2+r2);

        // Changed format to reduce computation times
        double x0 = sin(theta);
        double x1 = x0*x0;
        double x2 = 2*r;
        double x3 = -M*x2;
        double x4 = a*a;
        double x5 = r*r;
        double x6 = x4 + x5;
        double x7 = x3 + x6;
        double x8 = x7*x7;
        double x9 = cos(theta);
        double x10 = x4*(x9*x9) + x5;
        double x11 = 1.0/(x10*x10);
        double x12 = M - r;
        double x13 = L_z*L_z;
        double x14 = x10*x13;
        double x15 = x13*(-x10 - x3);
        double x16 = r*x7;
        double x17 = p_r*p_r;
        double x18 = -x12;
        double x19 = x1*x8;
        double x20 = x10*x15;
        double x21 = p_theta*p_theta;
        double x22 = L_z*M*a;
        double x23 = x1*x4;
        double x24 = E*(E*(x23*x7 - x6*x6) + 4*r*x22);
        double x25 = x1*x10;
        double x26 = x1*x1*x4;
        double x27 = x26*x7;

        // // Big Greeks
        // double Sigma = r2 + a2 *cth*cth;
        // double Delta = r2 - 2*M*r + a2;
        // double A     = (r2+a2)*(r2+a2) - a2*Delta*sth2;
        // double Sigma2 = Sigma*Sigma;
        // double Delta2 = Delta*Delta;

        // // Inverse metrics
        // double gtt     = -A/(Sigma*Delta);
        // double gtphi   = -(2*M*a*r)/(Sigma*Delta);
        // double gphiphi = (Delta - (a2)*(sth*sth))/(Sigma*Delta*sth*sth);

        // Big greeks and metrics
        double Sigma = x10;               // cse already computed it!
        double Delta = x7;                // and this
        double A = (r*r + a*a)*(r*r + a*a) - a*a*Delta*x1;
        double gtt     = -A/(Sigma*Delta);
        double gtphi   = -(2*M*a*r)/(Sigma*Delta);
        double gphiphi = (Delta - a*a*x1)/(Sigma*Delta*x1);

        double dr_dl      = (Delta/Sigma) * p_r;
        double dth_dl     = (1/Sigma) * p_theta;

        // double dpr_dl     = (-E*r*(E*(a2*(-2*M*r + a2 + r2)*sth2 - a2r2) + 4*L_z*M*a*r)*(-2*M*r + a2 + r2)*sth2 
        //                   + E*(M - r)*(E*(a2*(-2*M*r + a2 + r2)*sth2 - a2r2) 
        //                   + 4*L_z*M*a*r)*(a2*cth2 + r2)*sth2 + E*(E*(a2*(-M + r)*sth2 - 2*r*(a2 + r2)) 
        //                   + 2*L_z*M*a)*(a2*cth2 + r2)*(-2*M*r + a2 + r2)*sth2 
        //                   + Lz2*r*(-2*M*r + a2 + r2)*(2*M*r - a2*cth2 - r2) 
        //                   - Lz2*(M - r)*(a2*cth2 + r2)*(-2*M*r + a2 + r2) 
        //                   - Lz2*(M - r)*(a2*cth2 + r2)*(2*M*r - a2*cth2 - r2) 
        //                   + pr2*(-M + r)*(a2*cth2 + r2)*Delta2*sth2 
        //                   + r*(pr2*(-Delta) - p_theta*p_theta)*Delta2*sth2)/(Sigma2*Delta2*sth2);

        // double dptheta_dl = (E*E*a2*(a2*cth2 + r2)*(-2*M*r + a2 + r2)*sth2*sth2 
        //                   + E*a2*(E*(a2*(-2*M*r + a2 + r2)*sth2 - a2r2) 
        //                   + 4*L_z*M*a*r)*sth2*sth2 - Lz2*a2*(a2*cth2 + r2)*sth2 - Lz2*a2*(2*M*r - a2*cth2 - r2)*sth2 
        //                   + Lz2*(a2*cth2 + r2)*(2*M*r - a2*cth2 - r2) 
        //                   + a2*(pr2*(-2*M*r + a2 + r2) + p_theta*p_theta)*(-2*M*r + a2 
        //                   + r2)*sth2*sth2)*cth/(Sigma2*(-2*M*r + a2 + r2)*sth2*sth);

        double dpr_dl = -x11*(E*x25*x7*(E*(x18*x23 - x2*x6) + 2*x22) + r*x19*(-x17*x7 - x21) 
                      - x1*x16*x24 + x10*x17*x18*x19 - x12*x14*x7 - x12*x20 + x12*x24*x25 + x15*x16)/(x1*x8);

        double dptheta_dl = -x11*x9*(E*E*x10*x27 - x14*x23 - x15*x23 + x20 + x24*x26 + x27*(x17*x7 + x21))/(x0*x0*x0*x7);  

        double dt_dl      = -gtt*E + gtphi*Lz;
        double dphi_dl    = -gtphi*E + gphiphi*Lz;

        GeoState d;
        d.r      = dr_dl;       // NOT RADIUS!!!
        d.theta  = dth_dl;
        d.phi    = dphi_dl;
        d.t      = dt_dl;       
        d.pr     = dpr_dl;     // Signs are flipped in the sympy solver
        d.ptheta = dptheta_dl;

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
        // Shrink step size near horizon and increase further out
        GeoState d = deriv(s, ray.E, ray.Lz, spin, M);
        
        // Dynamic step size for polar axis too
        double dh_r  = std::abs(d.r) > 1e-12 ? 0.05 * s.r / std::abs(d.r) : 1e9;
        double dh_th = std::abs(d.theta) > 1e-12 ? 0.05 / std::abs(d.theta) : 1e9;

        double dh    = std::min({dh_r, dh_th, h * std::max(0.05, (s.r - r_horizon) / (2.0 * M))}); // 0.05 is the step floor; xM is how aggressively the step grows with dist

        // Step and increment counters
        s = rk4(s, dh, ray.E, ray.Lz, spin, M);
        total_steps++;
        // if (i % 200 == 0) {
        //       std::cout << i << ": r=" << s.r 
        //           << " H=" << abs(hamiltonian(s, ray.E, ray.Lz, spin, M)) << "\n";
        // }
        
        // 1.01 avoids BL coordinates blowing up
        if (std::isnan(s.theta) || std::isnan(s.r)) return color(1,0,0);
        if (s.r < r_horizon * 1.01 || s.r < 0) {
            return color(0,0,0); // Fell into BH
        }
        if (s.r > 100.0) { // escpae radius
            // return sky_color(s.theta, s.phi);
            return color(0,1,1);
        }
    }   
    maxxed_rays++;
    return color(1,0,0);  // red if it runs out of steps
    
}

#endif