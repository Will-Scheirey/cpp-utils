/*
Copyright (c) 2020 Chan Jer Shyan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
	Code copied and edited/added to from https://gist.github.com/acidleaf/8957147
*/

#ifndef __Vec2_H__
#define __Vec2_H__

#define _USE_MATH_DEFINES
#include <math.h>

class Vec2 {
public:
	float x, y;
	Vec2() :x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}
	Vec2(const Vec2& v) : x(v.x), y(v.y) {}
	
	Vec2& operator=(const Vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	
	Vec2 operator+(Vec2& v) {
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator-(Vec2& v) {
		return Vec2(x - v.x, y - v.y);
	}
	
	inline Vec2& operator+=(Vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	inline Vec2& operator-=(Vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	Vec2 operator+(float s) {
		return Vec2(x + s, y + s);
	}
	Vec2 operator-(float s) {
		return Vec2(x - s, y - s);
	}
	Vec2 operator*(float s) {
		return Vec2(x * s, y * s);
	}
	Vec2 operator/(float s) {
		return Vec2(x / s, y / s);
	}
	
	
	Vec2& operator+=(float s) {
		x += s;
		y += s;
		return *this;
	}
	Vec2& operator-=(float s) {
		x -= s;
		y -= s;
		return *this;
	}
	Vec2& operator*=(float s) {
		x *= s;
		y *= s;
		return *this;
	}
	Vec2& operator/=(float s) {
		x /= s;
		y /= s;
		return *this;
	}
	
	void set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	
	void rotate(float deg) {
		float theta = deg / 180.0f * (float)M_PI;
		float c = (float)cos(theta);
		float s = (float)sin(theta);
		float tx = x * c - y * s;
		float ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	void rotateRad(float theta) {
		float c = (float)cos(theta);
		float s = (float)sin(theta);
		float tx = x * c - y * s;
		float ty = x * s + y * c;
		x = tx;
		y = ty;
	}
	
	Vec2& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0f / length());
		return *this;
	}

	Vec2 normalized() {
		if (length() == 0) return *this;
		return *this / (length());
	}
	
	float dist(Vec2 v) const {
		Vec2 d(v.x - x, v.y - y);
		return d.length();
	}
	float length() const {
		return (float)sqrt((double)x * x + (double)y * y);
	}

	float angle() {
		return (float)atan2f(y, x);
	}

	void truncate(float length) {
		float angle = (float)atan2f(y, x);
		x = length * (float)cos(angle);
		y = length * (float)sin(angle);
	}
	
	Vec2 ortho() const {
		return Vec2(y, -x);
	}

	Vec2 copy() {
		return Vec2(x, y);
	}

	static float dot(Vec2 v1, Vec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static float cross(Vec2 v1, Vec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}
	static Vec2 center(Vec2 v1, Vec2 v2) {
		return Vec2((v1.x + v2.x) * 0.5f, (v1.y + v2.y) * 0.5f);
	}
	static float angle_between_points(Vec2 v1, Vec2 v2) {
		float angle;
		if (v1.x == v2.x) 
			angle = 0.5f * (float)M_PI;
		else
			angle = (float)atan2((double)v2.y - v1.y, (double)v1.x - v2.x);

		return angle;
	}

	static Vec2 rotate(Vec2 v, float deg) {
		float theta = deg / 180.0f * (float)M_PI;
		float c = (float)cos(theta);
		float s = (float)sin(theta);
		float tx = v.x * c - v.y * s;
		float ty = v.x * s + v.y * c;
		return Vec2(tx, ty);
	}
};


#endif
