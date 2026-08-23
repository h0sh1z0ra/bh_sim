#ifndef VEC3_H
#define VEC3_H

class vec3 {
    public:
        double e[3];  // fixed size 3 array, of doubles (floats; same as Python's)

        vec3() : e{0,0,0} {}   // default constructor, if no arguments are passed in
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}  // constructor if arguments are given

        // Accessing; returned as vector.x()
        // Const type, so accessing doesn't change shi
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        // Mathematical operators
        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; } // just for accessing
        double& operator[](int i) {return e[i]; }   // modifies vector

        // += and *= operations
        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;   // this = self, but pointer; *this dereferences           
        }                   // giving you the actual object itself

        vec3& operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        // Magnitude
        double length() const {
            return std::sqrt(length_squared());
        }
        
        // Magnitude squared
        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
};

// for clarity (holy shit), alias point3 for vec3
using point3 = vec3;

// Vector utility functions
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0]+v.e[0], u.e[1]+v.e[1], u.e[2]+v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0]-v.e[0], u.e[1]-v.e[1], u.e[2]-v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return v * t; // calls the other * operator above
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3&u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3&u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

#endif