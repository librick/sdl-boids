#include "Vec2.h"
#include <cstdlib>
#include <math.h>
#include <iostream>

float randFloat(float a, float b) {
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

Vec2 Vec2::createVector(float x, float y) {
    return { x: x, y: y };
}

Vec2 Vec2::copy(Vec2 a) {
    return { x: a.x, y: a.y };
}

Vec2 Vec2::random() {
    return Vec2 {
        x: randFloat(-1.0f, 1.0f),
        y: randFloat(-1.0f, 1.0f),
    };
}

Vec2 Vec2::scale(float f) {
    return Vec2 {
        x: this->x * f,
        y: this->y * f
    };
}

Vec2 Vec2::sub(Vec2 a, Vec2 b) {
    return { x: a.x - b.x, y: a.y - b.y };
}

void Vec2::add(Vec2* v) {
    this->x += v->x;
    this->y += v->y;
}

void Vec2::sub(Vec2* v) {
    this->x -= v->x;
    this->y -= v->y;
}

void Vec2::mult(float f) {
    this->x *= f;
    this->y *= f;
}

Vec2* Vec2::div(float f) {
    this->x /= f;
    this->y /= f;
    return this;
}

void Vec2::set(float x, float y) {
    this->x = x;
    this->y = y;
}

void Vec2::setMag(float f) {
    float theta = atan(this->y/this->x);
    this->x = cos(theta)*f;
    this->y = sin(theta)*f;
}

float Vec2::mag() {
    float x = this->x;
    float y = this->y;
    float magSq = x*x + y*y;
    return sqrt(magSq);
}

float Vec2::magSq() {
    float x = this->x;
    float y = this->y;
    return x*x + y*y;
}

Vec2* Vec2::limit(float max) {
    float mSq = this->magSq();
    if (mSq > max * max) {
        this->div(sqrt(mSq))
            ->mult(max);
    }
    return this;
}

float Vec2::dist(Vec2 a, Vec2 b) {
    float ax = a.x;
    float ay = a.y;
    float bx = b.x;
    float by = b.y;
    return sqrt(
        (by-ay)*(by-ay) +
        (bx-ax)*(bx-ax)
    );
}

Vec2* Vec2::normalize() {
    float len = this->mag();
    // Multiply by recipricol rather than call div()
    // to avoid duplicate zero check.
    if (len != 0) this->mult(1 / len);
    return this;
}

