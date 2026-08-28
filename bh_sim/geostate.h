#ifndef GEOSTATE_H
#define GEOSTATE_H

#include <cmath>

struct GeoState {
    double r, theta, phi, t;        // position
    double pr, ptheta;              // momenta to integrate

    GeoState deriv(const GeoState& s, double E, double Lz, double a, double M) {
        double r = s.r, theta = s.theta, p_r = s.pr, p_theta = s.ptheta, L_z = Lz;

        double Sigma = r*r + a*a *cos(theta)*cos(theta);
        double Delta = r*r - 2*M*r + a*a;

        double dr_dl      = (Delta/Sigma) * pr;
        double dth_dl     = (1/Sigma) * s.ptheta;

        double dpr_dl     = (-E*r*(E*(std::pow(a, 2)*(-2*M*r + std::pow(a, 2) + std::pow(r, 2))*std::pow(std::sin(theta), 2) 
                          - std::pow(std::pow(a, 2) + std::pow(r, 2), 2)) + 4*L_z*M*a*r)*(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2))*std::pow(std::sin(theta), 2) + E*(M - r)*(E*(std::pow(a, 2)*(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2))*std::pow(std::sin(theta), 2) - std::pow(std::pow(a, 2) + std::pow(r, 2), 2)) 
                          + 4*L_z*M*a*r)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) + std::pow(r, 2))*std::pow(std::sin(theta), 2) 
                          + E*(E*(std::pow(a, 2)*(-M + r)*std::pow(std::sin(theta), 2) - 2*r*(std::pow(a, 2) + std::pow(r, 2))) 
                          + 2*L_z*M*a)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) + std::pow(r, 2))*(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2))*std::pow(std::sin(theta), 2) + std::pow(L_z, 2)*r*(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2))*(2*M*r - std::pow(a, 2)*std::pow(std::cos(theta), 2) - std::pow(r, 2)) 
                          - std::pow(L_z, 2)*(M - r)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          + std::pow(r, 2))*(-2*M*r + std::pow(a, 2) + std::pow(r, 2)) 
                          - std::pow(L_z, 2)*(M - r)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          + std::pow(r, 2))*(2*M*r - std::pow(a, 2)*std::pow(std::cos(theta), 2) - std::pow(r, 2)) 
                          + std::pow(p_r, 2)*(-M + r)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          + std::pow(r, 2))*std::pow(-2*M*r + std::pow(a, 2) + std::pow(r, 2), 2)*std::pow(std::sin(theta), 2) 
                          + r*(std::pow(p_r, 2)*(2*M*r - std::pow(a, 2) - std::pow(r, 2)) - std::pow(p_theta, 2))*std::pow(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2), 2)*std::pow(std::sin(theta), 2))/(std::pow(std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          + std::pow(r, 2), 2)*std::pow(-2*M*r + std::pow(a, 2) + std::pow(r, 2), 2)*std::pow(std::sin(theta), 2));

        double dptheta_dl = (std::pow(E, 2)*std::pow(a, 2)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) + std::pow(r, 2))*(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2))*std::pow(std::sin(theta), 4) + E*std::pow(a, 2)*(E*(std::pow(a, 2)*(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2))*std::pow(std::sin(theta), 2) - std::pow(std::pow(a, 2) + std::pow(r, 2), 2)) 
                          + 4*L_z*M*a*r)*std::pow(std::sin(theta), 4) - std::pow(L_z, 2)*std::pow(a, 2)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          + std::pow(r, 2))*std::pow(std::sin(theta), 2) - std::pow(L_z, 2)*std::pow(a, 2)*(2*M*r - std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          - std::pow(r, 2))*std::pow(std::sin(theta), 2) + std::pow(L_z, 2)*(std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          + std::pow(r, 2))*(2*M*r - std::pow(a, 2)*std::pow(std::cos(theta), 2) - std::pow(r, 2)) 
                          + std::pow(a, 2)*(std::pow(p_r, 2)*(-2*M*r + std::pow(a, 2) + std::pow(r, 2)) + std::pow(p_theta, 2))*(-2*M*r + std::pow(a, 2) 
                          + std::pow(r, 2))*std::pow(std::sin(theta), 4))*std::cos(theta)/(std::pow(std::pow(a, 2)*std::pow(std::cos(theta), 2) 
                          + std::pow(r, 2), 2)*(-2*M*r + std::pow(a, 2) + std::pow(r, 2))*std::pow(std::sin(theta), 3));

        double dt_dl      = 
        double dphi_dl    = 

    }
};