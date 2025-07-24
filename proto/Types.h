#pragma once
#include <SDL_stdinc.h>
#include <vector>

struct Vector3
{
    double x, y, z;
};

inline constexpr double length(const Vector3 &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
inline constexpr Vector3 normalize(const Vector3 &v)
{
    const double l = length(v);
    if (l == 0.0f)
        return v;
    return Vector3{v.x / l, v.y / l, v.z / l};
}
inline constexpr double dot(const Vector3 &a, const Vector3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline constexpr Vector3 cross(const Vector3 &a, const Vector3 &b)
{
    return Vector3{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
inline constexpr Vector3 operator+(const Vector3 &a, const Vector3 &b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
inline constexpr Vector3 operator-(const Vector3 &a, const Vector3 &b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
inline constexpr Vector3 operator*(const Vector3 &a, const Vector3 &b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

inline constexpr Vector3 operator*(const Vector3 &v, const double s)
{
    return {v.x * s, v.y * s, v.z * s};
}
inline constexpr Vector3 operator*(const double s, const Vector3 &v)
{
    return v * s;
}
inline constexpr Vector3 operator/(const Vector3 &a, const Vector3 &b)
{
    return {a.x / b.x, a.y / b.y, a.z / b.z};
}
inline constexpr Vector3 operator/(const Vector3 &v, const double s)
{
    return {v.x / s, v.y / s, v.z / s};
}

struct Ray
{
    Vector3 origin;
    Vector3 direction;

    Ray(const Vector3 &o, const Vector3 &d)
        : origin{o}
        , direction{normalize(d)}
    {}
};

struct Bounds3
{
    Vector3 min;
    Vector3 max;

    inline constexpr Vector3 center() const { return (min + max) * 0.5f; }
    inline constexpr Vector3 extents() const { return max - min; }
    inline constexpr bool contains(Vector3 &p) const
    {
        return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y && p.z >= min.z
               && p.z <= max.z;
    }
};

struct Color
{
    double r, g, b, a;

    Color(const double r, const double g, const double b)
        : r{r}
        , g{g}
        , b{b}
        , a{1.0}
    {}
    Color(const double r, const double g, const double b, const double a)
        : r{r}
        , g{g}
        , b{b}
        , a{a}
    {}
    explicit Color(const double val)
        : r{val}
        , g{val}
        , b{val}
        , a{1.0}
    {}

    inline Uint32 convert() const
    {
        auto red = static_cast<unsigned char>(r * 255.0);
        auto green = static_cast<unsigned char>(g * 255.0);
        auto blue = static_cast<unsigned char>(b * 255.0);
        auto alpha = static_cast<unsigned char>(a * 255.0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 pixelColor = (alpha << 24) | (blue << 16) | (green << 8) | red;
#else
        Uint32 pixelColor = (blue << 0) | (green << 8) | (red << 16) | (alpha << 24);
#endif

        return pixelColor;
    }

    Color operator*(const double val) const
    {
        return {r * val, g * val, b * val, a};
    }
};

struct Intersection
{
    bool hit{false};
    Vector3 position{INFINITY, INFINITY, INFINITY};
    Vector3 normal{INFINITY, INFINITY, INFINITY};
    double distance{INFINITY};
    double u{INFINITY};
    double v{INFINITY};
};

class Primitive
{
public:
    virtual ~Primitive() = default;
    virtual Intersection intersect(const Ray &r) const = 0;
    virtual bool intersectP(const Ray &r) const = 0;
    // virtual Bounds3 bounds() const = 0;
};

class Aggregate : public Primitive
{
public:
    std::vector<std::shared_ptr<Primitive>> primitives;

    bool intersectP(const Ray &r) const override
    {
        for (const auto &prim : primitives)
        {
            if (prim->intersectP(r))
                return true;
        }
        return false;
    }

    Intersection intersect(const Ray &r) const override
    {
        Intersection result;
        for (const auto &prim : primitives) {
            Intersection i = prim->intersect(r);
            if (i.hit && i.distance < result.distance)
                result = i;
        }
        return result;
    }
};

class Sphere : public Primitive
{
public:
    Vector3 center;
    double radius;

    Sphere(const Vector3 &center, const double radius)
        : center{center}
        , radius{radius}
    {}

    bool intersectP(const Ray &r) const override
    {
        const Vector3 A = r.origin - center;
        const Vector3 D = r.direction;

        const double a = dot(D, D);
        const double b = 2.0 * dot(A, D);
        const double c = dot(A, A) - radius * radius;
        const double discriminant = b * b - 4 * a * c;

        return discriminant >= 0;
    }

    Intersection intersect(const Ray &r) const override
    {
        const Vector3 A = r.origin - center;
        const Vector3 D = r.direction;

        const double a = dot(D, D);
        const double b = 2.0 * dot(A, D);
        const double c = dot(A, A) - radius * radius;
        const double discriminant = b * b - 4 * a * c;

        Intersection result;

        if (discriminant < 0) {
            result.hit = false;
            return result;
        }

        result.hit = true;
        result.distance = (-b - sqrt(discriminant)) / 2.0 * a;
        result.position = r.origin + r.direction * result.distance;
        result.normal = normalize(result.position - center);
        result.u = 0.5 + atan2(result.normal.z, result.normal.x) / (2.0 * M_PI);
        result.v = 0.5 - asin(result.normal.y) / M_PI;

        return result;
    }
};

class Texture
{
public:
    Color sample(double u, double v) const;
};

class CheckerboardTexture : public Texture
{
public:
    int rows;
    int cols;
    bool showCenter;

    CheckerboardTexture()
        : rows(10)
        , cols(10)
        , showCenter(true)
    {}
    CheckerboardTexture(const int rows, const int cols, const bool centerCircle)
        : rows(rows)
        , cols(cols)
        , showCenter(centerCircle)
    {}

    Color sample(const double u, const double v) const
    {
        if (showCenter) {
            const double cu = u - 0.5;
            const double cv = v - 0.5;

            // Center Circle
            constexpr double radius = 0.01;
            if ((sqrt(cu * cu + cv * cv) < radius))
                return {1.0, 0.0, 0.0};

            // Center lines
            constexpr double thickness = 0.005;
            if (abs(cu) < thickness || abs(cv) < thickness)
                return {0.7, 0.7, 0.0};
        }

        bool white = static_cast<int>(u * static_cast<double>(cols)) % 2 == 0;
        white = static_cast<int>(v * static_cast<double>(rows)) % 2 == white;

        double val = white ? 1.0 : 0.0;
        return {val, val, val};
    }
};