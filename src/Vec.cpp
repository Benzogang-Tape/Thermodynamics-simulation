#include "Vec.hpp"

Vec::Vec (double x_, double y_, double z_):
		x (x_),
		y (y_),
		z (z_)
{}

void Vec::SetPos (double x_, double y_, double z_) {
	x = x_;
	y = y_;
	z = z_;
}

Vec Vec::operator-() const {
	return Vec (-x, -y, -z);
}

Vec operator+ (const Vec& vec1, const Vec& vec2) {
	return Vec (vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

Vec operator- (const Vec& vec1, const Vec& vec2) {
	return Vec (vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

Vec operator* (const Vec& vec, const double scalar) {
	return Vec (vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vec operator* (const double scalar, const Vec& vec) {
	return vec * scalar;
}

Vec operator/ (const Vec& vec, const double scalar) {
	return Vec (vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

Vec operator+= (Vec& vec1, const Vec& vec2) {
	vec1.x += vec2.x;
	vec1.y += vec2.y;
	vec1.z += vec2.z;
	return vec1;
}

Vec operator-= (Vec& vec1, const Vec& vec2) {
	vec1.x -= vec2.x;
	vec1.y -= vec2.y;
	vec1.z -= vec2.z;
	return vec1;
}

Vec operator*= (Vec& vec, const double scalar) {
	vec.x *= scalar;
	vec.y *= scalar;
	vec.z *= scalar;
	return vec;
}

Vec operator/= (Vec& vec, const double scalar) {
	vec.x /= scalar;
	vec.y /= scalar;
	vec.z /= scalar;
	return vec;
}

double operator, (const Vec& vec1, const Vec& vec2) {
	return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

Vec operator^ (const Vec& vec1, const Vec& vec2) {
	return Vec (vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
}

double Vec::GetLen() const {
	return std::sqrt (x * x + y * y + z * z);
}

void Vec::Normalize () {
	*this /= GetLen ();
}

Vec Vec::operator! () const {
	Vec ret = *this;
	ret.Normalize();
	return ret;
}

Vec operator* (const Vec& vec1, const Vec& vec2) {
	return Vec (vec1.y * vec2.z - vec1.z * vec2.y,
				vec1.z * vec2.x - vec1.x * vec2.z,
				vec1.x * vec2.y - vec1.y * vec2.x);
}

void Vec::RotateAroundZ (const double angle) {
	double sin_ = std::sin (angle);
	double cos_ = std::cos (angle);

	double new_x = x * cos_ - y * sin_;
	double new_y = x * sin_ + y * cos_;

	SetPos (new_x, new_y, z);
}


Ray::Ray (const Vec& start_, const Vec& dir_):
		start (start_),
		dir (dir_)
{}

double Distance (Ray ray, Vec point) {
	return ((ray.start - point) * !(ray.dir)).GetLen();
}

double GetRandAngle () {
	return std::rand() / (RAND_MAX / (M_PI * 2));
}