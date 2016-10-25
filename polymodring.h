#include <gmp.h>
#include <gmpxx.h>
#include <vector>
#include <functional>
#include <initializer_list>
#include <iostream>

#include "numbers.h"
#include "polyring.h"
#include "alg.h"

#pragma once

template <typename T>
class polymod {
	private:
		poly<T> base, value;
		
	public:
		polymod();
		explicit polymod(poly<T> value);
		polymod(poly<T> base, poly<T> value);
		polymod(const polymod<T> &other);
		polymod(const polymod<T> &other, poly<T> value);
		
		poly<T> get_base() const;
		poly<T> get_value() const;
		
		polymod<T> &operator=(poly<T> value);
		polymod<T> &operator=(const polymod<T> &other);
		
		bool operator==(const polymod<T> &other) const;
		bool operator!=(const polymod<T> &other) const;
		
		polymod<T> &operator+=(const polymod<T> &other);
		polymod<T> &operator-=(const polymod<T> &other);
		polymod<T> &operator*=(const polymod<T> &other);
		polymod<T> &operator/=(const polymod<T> &other);
		
		polymod<T> operator+(const polymod<T> &other) const;
		polymod<T> operator-(const polymod<T> &other) const;
		polymod<T> operator*(const polymod<T> &other) const;
		polymod<T> operator/(const polymod<T> &other) const;
		
		polymod<T> operator-() const;
		polymod<T> inv() const;
		
		operator poly<T>();
};

template <typename T>
std::function<polymod<T>(poly<T>)> to_mod(poly<T> base);

template <typename T>
polymod<T> zero(const polymod<T> &reference) {
	return polymod<T>(reference.get_base(), zero<poly<T>>());
}

template <typename T>
polymod<T> one(const polymod<T> &reference) {
	return polymod<T>(reference.get_base(), one<poly<T>>());
}

template <typename T>
polymod<T> from_int(int n, const polymod<T> &reference) {
	return polymod<T>(reference.get_base(), from_int<poly<T>>());
}

template <typename T>
polymod<T>::polymod() {
	this->base = zero<poly<T>>();
	this->value = zero<poly<T>>();
}

template <typename T>
polymod<T>::polymod(poly<T> value) {
	this->base = zero<poly<T>>();
	this->value = value;
}

template <typename T>
polymod<T>::polymod(poly<T> base, poly<T> value) {
	this->base = base;
	this->value = value.divide(base).remainder;
}

template <typename T>
polymod<T>::polymod(const polymod<T> &other) {
	this->base = other.base;
	this->value = other.value;
}

template <typename T>
polymod<T>::polymod(const polymod<T> &other, poly<T> value) {
	this->base = other.base;
	this->value = value.divide(other.base).remainder;
}

template <typename T>
poly<T> polymod<T>::get_base() const {
	return this->base;
}

template <typename T>
poly<T> polymod<T>::get_value() const {
	return this->value;
}

template <typename T>
polymod<T> &polymod<T>::operator=(poly<T> value) {
	this->value = value.divide(this->base).remainder;
	return *this;
}

template <typename T>
polymod<T> &polymod<T>::operator=(const polymod<T> &other) {
	this->base = other.base;
	this->value = other.value;
	return *this;
}

template <typename T>
bool polymod<T>::operator==(const polymod<T> &other) const {
	return (this->value == other.value);
}

template <typename T>
bool polymod<T>::operator!=(const polymod<T> &other) const {
	return (this->value != other.value);
}

template <typename T>
polymod<T> &polymod<T>::operator+=(const polymod<T> &other) {
	this->value += other.value;
	this->base = (this->base == 0) ? other.base : this->base;
	this->value = this->value.divide(this->base).remainder;
	return *this;
}

template <typename T>
polymod<T> &polymod<T>::operator-=(const polymod<T> &other) {
	this->value -= other.value;
	this->base = (this->base == 0) ? other.base : this->base;
	this->value = this->value.divide(this->base).remainder;
	return *this;
}

template <typename T>
polymod<T> &polymod<T>::operator*=(const polymod<T> &other) {
	this->value *= other.value;
	this->base = (this->base == 0) ? other.base : this->base;
	this->value = this->value.divide(this->base).remainder;
	return *this;
}

template <typename T>
polymod<T> &polymod<T>::operator/=(const polymod<T> &other) {
	return (*this) *= other.inv();
}

template <typename T>
polymod<T> polymod<T>::operator+(const polymod<T> &other) const {
	return polymod<T>(*this) += other;
}

template <typename T>
polymod<T> polymod<T>::operator-(const polymod<T> &other) const {
	return polymod<T>(*this) -= other;
}

template <typename T>
polymod<T> polymod<T>::operator*(const polymod<T> &other) const {
	return polymod<T>(*this) *= other;
}

template <typename T>
polymod<T> polymod<T>::operator/(const polymod<T> &other) const {
	return polymod<T>(*this) /= other;
}

template <typename T>
polymod<T> polymod<T>::operator-() const {
	return polymod<T>(this->base, -this->value);
}

template <typename T>
polymod<T> polymod<T>::inv() const {
	std::tuple<poly<T>, poly<T>, poly<T>> gcd = extended_gcd(this->value, this->base);
	return polymod<T>(this->base, std::get<0>(gcd));
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const polymod<T> &m) {
	return os << m.get_value();
}

template <typename T>
polymod<T>::operator poly<T>() {
	return this->value;
}

template <typename T>
polymod<T> make_polymod(poly<T> base, poly<T> value) {
	return polymod<T>(base, value);
}

template <typename T>
std::function<polymod<T>(poly<T>)> to_mod(poly<T> base) {
	return std::bind(make_polymod, base, std::placeholders::_1);
}