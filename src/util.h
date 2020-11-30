#pragma once

typedef unsigned int UINT;

static UINT get_random(UINT max)
{
	static std::default_random_engine generator(std::time(NULL));
	static std::uniform_int_distribution<UINT> distribution;

	return distribution(generator) % max;
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

	static const vec2_t<T> zero;

	constexpr vec2_t(T x = 0, T y = 0) : x(x), y(y) {}

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
const vec2_t<T> vec2_t<T>::zero = { 0, 0 };

template<>
vec2_t<UINT> vec2_t<UINT>::abs()
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
vec2_t<UINT> operator- (vec2_t<T> a, vec2_t<UINT>b)
{
	return a + (vec2_t<int>(b.x, b.y) * -1);
}

template<typename T, typename A>
vec2_t<T>& operator-=(vec2_t<T>& a, vec2_t<A>b)
{
	a = a - b; return a;
}

typedef vec2_t<UINT> VEC2U;
typedef vec2_t<int> VEC2I;
typedef vec2_t<float> VEC2F;
