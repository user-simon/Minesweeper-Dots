#pragma once

typedef unsigned int uint;

static uint get_random(uint max)
{
	static std::random_device rd;
	static std::mt19937 gen = std::mt19937(rd());
	static std::uniform_int_distribution<uint> dist;

	return dist(gen) % max;
}

template<typename T>
T _abs(T arg)
{
	if (arg < 0)
		return -arg;
	return arg;
}

template<typename T>
struct vec2_t
{
	T x, y;

	static const vec2_t<T> ZERO;

	constexpr vec2_t(T x, T y) : x(x), y(y) {}
	constexpr vec2_t() : vec2_t(0, 0) {}
	constexpr vec2_t(T v) : vec2_t(v, v) {}

	vec2_t<T> abs()
	{
		return { _abs(x), _abs(y) };
	}

	operator bool()
	{
		return x || y;
	}

	template<typename A>
	operator vec2_t<A>()
	{
		return vec2_t<A>((A)x, (A)y);
	}

	template<typename A>
	operator sf::Vector2<A>()
	{
		return sf::Vector2<A>(x, y);
	}
};

// hash specialization for use in ex. hash maps

namespace std
{
	template <typename T> struct hash<vec2_t<T>>
	{
		size_t operator()(const vec2_t<T>& a) const
		{
			return hash<uint64_t>()((uint64_t)a.x << sizeof(int32_t) | a.y);
		}
	};
}

template<typename T>
const vec2_t<T> vec2_t<T>::ZERO = { 0, 0 };

template<>
vec2_t<uint> vec2_t<uint>::abs()
{
	return *this;
}

// operators

template<typename T>
bool operator==(vec2_t<T> a, vec2_t<T> b)
{
	return a.x == b.x && a.y == b.y;
}

template<typename T>
bool operator!=(vec2_t<T> a, vec2_t<T> b)
{
	return !(a == b);
}

template<typename T, typename A>
vec2_t<T> operator* (vec2_t<T> a, A b)
{
	return { (T)(a.x * b), (T)(a.y * b) };
}

template<typename T, typename A>
vec2_t<T>& operator*=(vec2_t<T>& a, A b)
{
	a = a * b; return a;
}

template<typename T, typename A>
vec2_t<T> operator/ (vec2_t<T> a, A b)
{
	return a * (1.f / b);
}

template<typename T, typename A>
vec2_t<T>& operator/=(vec2_t<T>& a, A b)
{
	a = a / b; return a;
}

template<typename T, typename A>
vec2_t<T> operator+ (vec2_t<T> a, vec2_t<A>b)
{
	return { a.x + b.x, a.y + b.y };
}

template<typename T, typename A>
vec2_t<T>& operator+=(vec2_t<T>& a, vec2_t<A>b)
{
	a = a + b; return a;
}

template<typename T, typename A>
vec2_t<T> operator- (vec2_t<T> a, vec2_t<A>b)
{
	return a + (b * -1.f);
}

template<typename T>
vec2_t<uint> operator- (vec2_t<T> a, vec2_t<uint>b)
{
	return a + (vec2_t<int>(b.x, b.y) * -1);
}

template<typename T, typename A>
vec2_t<T>& operator-=(vec2_t<T>& a, vec2_t<A>b)
{
	a = a - b; return a;
}

typedef vec2_t<uint> vec2u;
typedef vec2_t<int> vec2i;
typedef vec2_t<float> vec2f;
