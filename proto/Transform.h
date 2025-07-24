#pragma once

#include "proto.h"

struct Matrix4
{
    std::array<double, 16> m;

    // clang-format off
    inline constexpr Matrix4()
        : m{1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1}
    {}
    // clang-format on

    // Static helper: create translation matrix
    static constexpr Matrix4 translation(double tx, double ty, double tz) {
        Matrix4 mat{};
        mat.m[12] = tx;
        mat.m[13] = ty;
        mat.m[14] = tz;
        return mat;
    }

    // Static helper: create scaling matrix
    static constexpr Matrix4 scaling(double sx, double sy, double sz) {
        Matrix4 mat{};
        mat.m[0] = sx;
        mat.m[5] = sy;
        mat.m[10] = sz;
        return mat;
    }

    // Static helper: create rotation matrix around Z axis
    static Matrix4 rotationZ(double radians) {
        Matrix4 mat{};
        double c = std::cos(radians);
        double s = std::sin(radians);
        mat.m[0] = c;  mat.m[4] = -s;
        mat.m[1] = s;  mat.m[5] = c;
        mat.m[10] = 1.f;
        mat.m[15] = 1.f;
        return mat;
    }

    static Matrix4 rotationX(double radians) {
        Matrix4 mat{};
        double c = std::cos(radians);
        double s = std::sin(radians);
        mat.m[0] = 1.f;
        mat.m[5] = c;  mat.m[6] = -s;
        mat.m[9] = s;  mat.m[10] = c;
        mat.m[15] = 1.f;
        return mat;
    }

    static Matrix4 rotationY(double radians) {
        Matrix4 mat{};
        double c = std::cos(radians);
        double s = std::sin(radians);
        mat.m[0] = c;  mat.m[2] = s;
        mat.m[5] = 1.f;
        mat.m[8] = -s; mat.m[10] = c;
        mat.m[15] = 1.f;
        return mat;
    }
};



class Transform
{
public:
    Matrix4 m, mInv;

    explicit Transform(const Matrix4 &mat)
    {
        m = mat;
    }

    inline Ray applyRay(const Ray &r) const {
        return {
            applyPoint(r.origin),
            applyVector(r.direction)  // no translation
        };
    }

    inline Vector3 applyPoint(const Vector3 &p) const {
        const auto &mat = m.m;
        double x = p.x, y = p.y, z = p.z;

        double xp = mat[0]*x + mat[1]*y + mat[2]*z + mat[3];
        double yp = mat[4]*x + mat[5]*y + mat[6]*z + mat[7];
        double zp = mat[8]*x + mat[9]*y + mat[10]*z + mat[11];
        double wp = mat[12]*x + mat[13]*y + mat[14]*z + mat[15];

        if (wp == 1.0 || wp == 0.0)
            return {xp, yp, zp};
        return {xp/wp, yp/wp, zp/wp};
    }

    inline Vector3 applyVector(const Vector3 &v) const {
        const auto &mat = m.m;
        double x = v.x, y = v.y, z = v.z;

        return {
            mat[0] * x + mat[1] * y + mat[2] * z,
            mat[4] * x + mat[5] * y + mat[6] * z,
            mat[8] * x + mat[9] * y + mat[10] * z
        };
    }


};

