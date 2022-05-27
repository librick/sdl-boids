#pragma once
struct Vec2 {
    float x;
    float y;

    static Vec2 createVector(float x, float y);
    static Vec2 random();
    static Vec2 copy(Vec2 a);
    static float dist(Vec2 a, Vec2 b);
    static Vec2 sub(Vec2 a, Vec2 b);

    Vec2 scale(float f);
    void add(Vec2* v);
    void sub(Vec2* v);
    void mult(float f);
    Vec2* div(float f);
    void set(float x, float y);
    float mag();
    float magSq();
    void setMag(float f);
    Vec2* limit(float f);
    Vec2* normalize();
};
