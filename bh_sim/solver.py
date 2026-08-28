import sympy as sp

# variables
r, th, a, M, E, Lz, pr, pth = sp.symbols('r theta a M E L_z p_r p_theta', real=True)

# capital greeks
Sigma = r**2 + a**2*sp.cos(th)**2
Delta = r**2 - 2*M*r + a**2

# inverse metric components
A      = (r**2 + a**2)**2 - a**2*Delta*sp.sin(th)**2
g_tt   = -A/(Sigma*Delta)
g_tphi = -2*M*a*r/(Sigma*Delta)
g_phph = (Delta - a**2*sp.sin(th)**2)/(Sigma*Delta*sp.sin(th)**2)
g_rr   = Delta/Sigma
g_thth = 1/Sigma

# Finding Hamiltonian
H = sp.Rational(1,2)*(g_tt*E**2 - 2*g_tphi*E*Lz + g_phph*Lz**2
                      + g_rr*pr**2 + g_thth*pth**2)

dH_dr  = sp.simplify(sp.diff(H, r))
dH_dth = sp.simplify(sp.diff(H, th))

# print(sp.cxxcode(dH_dr))
print(sp.cxxcode(dH_dth))