#pragma once

#include <vull/maths/Common.hh>
#include <vull/support/Array.hh>

namespace vull {

#define DEFINE_OP(op, ope)                                                                                             \
    constexpr Vec operator op(T rhs) const { return Vec(*this) ope rhs; }                                              \
    constexpr Vec operator op(const Vec &rhs) const { return Vec(*this) ope rhs; }                                     \
    constexpr Vec &operator ope(T rhs) {                                                                               \
        for (unsigned i = 0; i < L; i++) {                                                                             \
            m_elems[i] ope rhs;                                                                                        \
        }                                                                                                              \
        return *this;                                                                                                  \
    }                                                                                                                  \
    constexpr Vec &operator ope(const Vec &rhs) {                                                                      \
        for (unsigned i = 0; i < L; i++) {                                                                             \
            m_elems[i] ope rhs.m_elems[i];                                                                             \
        }                                                                                                              \
        return *this;                                                                                                  \
    }

template <typename T, unsigned L>
class Vec {
    Array<T, L> m_elems;

public:
    constexpr Vec() : Vec(T(0)) {}

    // Duplication construction, e.g. Vec3f(1.0f) == Vec3f(1.0f, 1.0f, 1.0f)
    constexpr Vec(T t) requires(L != 1);

    // Construction from all components e.g. Vec2f(1.0f, 2.0f) or Vec3f(1.0f, 2.0f, 3.0f)
    template <typename... Ts>
    constexpr Vec(Ts... ts) requires(sizeof...(Ts) == L) : m_elems{static_cast<T>(ts)...} {}

    // Construction from a smaller vector + remaining scalar components, e.g. Vec4f(Vec3f(), 1.0f)
    template <unsigned L1, typename... Ts>
    constexpr Vec(const Vec<T, L1> &vec, Ts... ts) requires(sizeof...(Ts) == L - L1);

    // Truncating construction from a larger vector, e.g. Vec3f(Vec4f())
    template <unsigned L1>
    constexpr Vec(const Vec<T, L1> &vec) requires(L1 > L);

    DEFINE_OP(+, +=)
    DEFINE_OP(-, -=)
    DEFINE_OP(*, *=)
    DEFINE_OP(/, /=)
    DEFINE_OP(%, %=)
    DEFINE_OP(&, &=)
    DEFINE_OP(|, |=)
    DEFINE_OP(^, ^=)
    DEFINE_OP(<<, <<=)
    DEFINE_OP(>>, >>=)

    void set_x(T x) requires(L >= 1) { m_elems[0] = x; }
    void set_y(T y) requires(L >= 2) { m_elems[1] = y; }
    void set_z(T z) requires(L >= 3) { m_elems[2] = z; }
    void set_w(T w) requires(L >= 4) { m_elems[3] = w; }

    constexpr T &operator[](unsigned elem) { return m_elems[elem]; }
    constexpr T operator[](unsigned elem) const { return m_elems[elem]; }
    constexpr T x() const requires(L >= 1) { return m_elems[0]; }
    constexpr T y() const requires(L >= 2) { return m_elems[1]; }
    constexpr T z() const requires(L >= 3) { return m_elems[2]; }
    constexpr T w() const requires(L >= 4) { return m_elems[3]; }
};
#undef DEFINE_OP

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;
using Vec2u = Vec<uint32_t, 2>;
using Vec3u = Vec<uint32_t, 3>;
using Vec4u = Vec<uint32_t, 4>;

template <typename T, unsigned L>
constexpr Vec<T, L>::Vec(T t) requires(L != 1) {
    for (unsigned i = 0; i < L; i++) {
        m_elems[i] = t;
    }
}

template <typename T, unsigned L>
template <unsigned L1, typename... Ts>
constexpr Vec<T, L>::Vec(const Vec<T, L1> &vec, Ts... ts) requires(sizeof...(Ts) == L - L1) {
    for (unsigned i = 0; i < L1; i++) {
        m_elems[i] = vec[i];
    }
    Array packed{static_cast<T>(ts)...};
    for (unsigned i = 0; i < sizeof...(Ts); i++) {
        m_elems[L1 + i] = packed[i];
    }
}

template <typename T, unsigned L>
template <unsigned L1>
constexpr Vec<T, L>::Vec(const Vec<T, L1> &vec) requires(L1 > L) {
    for (unsigned i = 0; i < L; i++) {
        m_elems[i] = vec[i];
    }
}

template <typename T, unsigned L>
constexpr Vec<T, L> abs(const Vec<T, L> &vec) {
    Vec<T, L> ret;
    for (unsigned i = 0; i < L; i++) {
        ret[i] = abs(vec[i]);
    }
    return ret;
}

template <typename T>
constexpr Vec<T, 3> cross(const Vec<T, 3> &lhs, const Vec<T, 3> &rhs) {
    return {
        lhs.y() * rhs.z() - lhs.z() * rhs.y(),
        lhs.z() * rhs.x() - lhs.x() * rhs.z(),
        lhs.x() * rhs.y() - lhs.y() * rhs.x(),
    };
}

template <typename T, unsigned L>
constexpr float distance(const Vec<T, L> &lhs, const Vec<T, L> &rhs) {
    return magnitude(rhs - lhs);
}

template <typename T, unsigned L>
constexpr float dot(const Vec<T, L> &lhs, const Vec<T, L> &rhs) {
    float ret = 0.0f;
    for (unsigned i = 0; i < L; i++) {
        ret += lhs[i] * rhs[i];
    }
    return ret;
}

template <typename T, unsigned L>
constexpr float magnitude(const Vec<T, L> &vec) {
    return sqrt(square_magnitude(vec));
}

template <typename T, unsigned L>
constexpr Vec<T, L> normalise(const Vec<T, L> &vec) {
    return vec / magnitude(vec);
}

template <typename T, unsigned L>
constexpr float square_magnitude(const Vec<T, L> &vec) {
    return dot(vec, vec);
}

} // namespace vull
