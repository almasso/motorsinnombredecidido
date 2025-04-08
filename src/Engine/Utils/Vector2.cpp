#include "Vector2.h"
#include <cmath>
#include <sol/state.hpp>

Vector2::Vector2() :
	_x(0.0f),
	_y(0.0f) {
}

Vector2::Vector2(float x, float y) :
	_x(x),
	_y(y) {
}

Vector2::Vector2(const Vector2& v) = default;

Vector2::Vector2(float e) :
	_x(e),
	_y(e) {
}

Vector2::~Vector2() = default;

Vector2 Vector2::operator+(const Vector2& v) const {
	return {_x + v._x, _y + v._y};
}

Vector2 Vector2::operator-(const Vector2& v) const {
	return {_x - v._x, _y - v._y};
}

Vector2 Vector2::operator*(float e) const {
	return {_x * e, _y * e};
}

Vector2 Vector2::operator/(float e) const {
	return {_x / e, _y / e};
}

Vector2 Vector2::operator/(Vector2 const& v) const {
	return {_x / v._x, _y / v._y};
}

Vector2 Vector2::operator*(const Vector2& v) const {
	return Vector2(_x * v._x, _y * v._y);
}

bool Vector2::operator==(const Vector2& v) const {
	return (_x == v._x && _y == v._y);
}

bool Vector2::operator!=(const Vector2& v) const {
	return (_x != v._x || _y != v._y);
}

Vector2 Vector2::operator+=(const Vector2& v) {
	_x += v._x;
	_y += v._y;
	return *this;
}

Vector2 Vector2::operator*=(const Vector2& v) {
	_x *= v._x;
	_y *= v._y;
	return *this;
}

Vector2 Vector2::operator*=(float e) {
	_x *= e;
	_y *= e;
	return *this;
}

float Vector2::magnitude() const {
	return sqrtf(_x * _x + _y * _y);
}

Vector2& Vector2::normalize() {
	_x = _x / magnitude();
	_y = _y / magnitude();
	return *this;
}

Vector2 Vector2::normalized() const {
	return (*this) / magnitude();
}

float Vector2::getX() const {
	return _x;
}

float Vector2::getY() const {
	return _y;
}

void Vector2::setX(float x) {
	_x = x;
}

void Vector2::setY(float y) {
	_y = y;
}

void Vector2::set(const Vector2& v) {
	_x = v._x;
	_y = v._y;
}

void Vector2::set(float x, float y) {
	_x = x;
	_y = y;
}

float Vector2::distanceTo(const Vector2 &v) const {
	return ((*this) - v).magnitude();
}

float Vector2::distanceToManhattan(const Vector2 &v) const {
    return std::abs(_x - v._x) + std::abs(_y - v._y);
}

void Vector2::RegisterToLua(sol::state& lua) {
	sol::usertype<Vector2> type = lua.new_usertype<Vector2>("Vector2",
		sol::constructors<Vector2(), Vector2(float), Vector2(float, float)>());
	type["x"] = sol::property(&Vector2::getX, &Vector2::setX);
	type["y"] = sol::property(&Vector2::getY, &Vector2::setY);
	type["magnitude"] = &Vector2::magnitude;
	type["normalized"] = &Vector2::normalized;
	type["normalize"] = &Vector2::normalize;
}

Vector2::operator std::array<float, 2>() {
	return {_x, _y};
}

const Vector2 Vector2::ZERO(0.0f, 0.0f);
const Vector2 Vector2::RIGHT(1.0f, 0.0f);
const Vector2 Vector2::LEFT(-1.0f, 0.0f);
const Vector2 Vector2::UP(0.0f, 1.0f);
const Vector2 Vector2::DOWN(0.0f, -1.0f);
const Vector2 Vector2::UNIT(1.0f, 1.0f);
