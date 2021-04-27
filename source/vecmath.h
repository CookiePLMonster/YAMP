#pragma once

namespace vecmath_data {

template<typename T>
struct Tuple2
{
	T x, y;
};

}

namespace vecmath {

template<typename T>
struct Tuple2 : vecmath_data::Tuple2<T>
{};

template<typename T>
struct Vector2 : Tuple2<T>
{};

}