#include "Vector2.h"
#include <cmath>

Vector2::Vector2() :
	x_(0.0f),
	y_(0.0f) {
}

Vector2::Vector2(float x, float y) :
	x_(x),
	y_(y) {
}

Vector2::Vector2(const Vector2& v) = default;

Vector2::Vector2(float e) :
	x_(e),
	y_(e) {
}

Vector2::~Vector2() = default;

Vector2 Vector2::operator+(const Vector2& v) const {
	return Vector2(x_ + v.x_, y_ + v.y_);
}

Vector2 Vector2::operator-(const Vector2& v) const {
	return Vector2(x_ - v.x_, y_ - v.y_);
}

Vector2 Vector2::operator*(float e) const {
	return Vector2(x_ * e, y_ * e);
}

Vector2 Vector2::operator/(float e) const {
	return Vector2(x_ / e, y_ / e);
}

Vector2 Vector2::operator/(Vector2 const& v) const {
	return Vector2(x_ / v.x_, y_ / v.y_);
}

Vector2 Vector2::operator*(const Vector2& v) const {
	return Vector2(x_ * v.x_, y_ * v.y_);
}

bool Vector2::operator==(const Vector2& v) const {
	return (x_ == v.x_ && y_ == v.y_);
}

bool Vector2::operator!=(const Vector2& v) const {
	return (x_ != v.x_ || y_ != v.y_);
}

Vector2 Vector2::operator+=(const Vector2& v) {
	x_ += v.x_;
	y_ += v.y_;
	return *this;
}

Vector2 Vector2::operator*=(const Vector2& v) {
	x_ *= v.x_;
	y_ *= v.y_;
	return *this;
}

Vector2 Vector2::operator*=(float e) {
	x_ *= e;
	y_ *= e;
	return *this;
}

float Vector2::magnitude() const {
	return sqrtf(x_ * x_ + y_ * y_);
}

Vector2& Vector2::normalize() {
	x_ = x_ / magnitude();
	y_ = y_ / magnitude();
	return *this;
}

Vector2 Vector2::normalized() const {
	return (*this) / magnitude();
}

float Vector2::getX() const {
	return x_;
}

float Vector2::getY() const {
	return y_;
}

void Vector2::setX(float x) {
	x_ = x;
}

void Vector2::setY(float y) {
	y_ = y;
}

void Vector2::set(const Vector2& v) {
	x_ = v.x_;
	y_ = v.y_;
}

void Vector2::set(float x, float y) {
	x_ = x;
	y_ = y;
}

const Vector2 Vector2::ZERO(0.0f, 0.0f);
const Vector2 Vector2::RIGHT(1.0f, 0.0f);
const Vector2 Vector2::LEFT(-1.0f, 0.0f);
const Vector2 Vector2::UP(0.0f, 1.0f);
const Vector2 Vector2::DOWN(0.0f, -1.0f);
const Vector2 Vector2::UNIT(1.0f, 1.0f);
