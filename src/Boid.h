#pragma once
#include <vector>
#include "Vec2.h"
#include <SDL2/SDL.h>

class Boid {
    public:
        static void render(
            std::vector<Boid*> boids,
            SDL_Renderer* renderer,
            SDL_Texture* tex,
            float desiredSep,
            float alignmentDist,
            float cohesionDist,
            float maxSpeed,
            bool showVelocityLines);
        Vec2 initPos;
        Vec2 position;
        Vec2 velocity;
        Vec2 acceleration;
        float maxForce;
        float maxSpeed;
        float width;
        float height;
        Boid(float x, float y);
        void update();
        void flock(std::vector<Boid*> otherBoids, float desiredSeparation, float alignRad, float coRad);
        void borders();
    private:
        Vec2 separate(std::vector<Boid*> otherBoids, float desiredSeparation);
        Vec2 align(std::vector<Boid*> otherBoids, float neighborDist);
        Vec2 cohesion(std::vector<Boid*> otherBoids, float neighborDist);
        Vec2 seek(Vec2 target);
        void applyForce(Vec2* force);
};
