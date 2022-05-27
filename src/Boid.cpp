#include "Boid.h"
#include <vector>
#include <math.h>
#include "Constants.cpp"
#include "Vec2.h"
#include <SDL2/SDL.h>

Boid::Boid(float x, float y) {
    this->acceleration = Vec2::createVector(0, 0);
    this->velocity = Vec2::random();
    this->position = Vec2::createVector(x, y);
    this->initPos = Vec2::copy(this->position);
    this->maxSpeed = 3;
    this->maxForce = 0.05;
    this->width = BABY_WIDTH;
    this->height = BABY_HEIGHT;
}

void Boid::flock(
    std::vector<Boid*> otherBoids,
    float desiredSeparation,
    float alignRad,
    float coRad
) {
    Vec2 sep = this->separate(otherBoids, desiredSeparation);
    Vec2 ali = this->align(otherBoids, alignRad);
    Vec2 coh = this->cohesion(otherBoids, coRad);
    // Arbitrarily weight these forces
    sep.mult(1.5);
    ali.mult(1.0);
    coh.mult(1.0);
    this->applyForce(&sep);
    this->applyForce(&ali);
    this->applyForce(&coh);
}

void Boid::applyForce(Vec2* force) {
    this->acceleration.add(force);
}

void Boid::update() {
    this->position.add(&this->velocity);
    this->velocity.add(&this->acceleration);
    this->acceleration.mult(0);
}

void Boid::borders() {
  if (this->position.x < -this->width) this->position.x = WINDOW_WIDTH + this->width;
  if (this->position.y < -this->height) this->position.y = WINDOW_HEIGHT + this->height;
  if (this->position.x > WINDOW_WIDTH + this->width) this->position.x = -this->width;
  if (this->position.y > WINDOW_HEIGHT + this->height) this->position.y = -this->height;
}

Vec2 Boid::separate(std::vector<Boid*> otherBoids, float desiredSeparation) {
  Vec2 steer = Vec2::createVector(0, 0);
  int count = 0;
  // For every boid in the system, check if it's too close
  for (long unsigned int i = 0; i < otherBoids.size(); i++) {
    Boid* other = otherBoids[i];
    float d = Vec2::dist(this->position, other->position);
    // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
    if (d > 0 && d < desiredSeparation) {
      // Calculate vector pointing away from neighbor
      Vec2 diff = Vec2::sub(this->position, other->position);
      diff.normalize();
      diff.div(d); // Weight by distance
      steer.add(&diff);
      count++; // Keep track of how many
    }
  }
  // Average, divide by how many
  if (count > 0) {
    steer.div(count);
  }

  // As long as the vector is greater than 0
  if (steer.mag() > 0) {
    steer.normalize();
    steer.mult(this->maxSpeed);
    steer.sub(&(this->velocity));
    steer.limit(this->maxForce);
  }
  return steer;
};

Vec2 Boid::align(std::vector<Boid*> otherBoids, float neighborDist) {
    Vec2 sum = Vec2::createVector(0, 0);
    int count = 0;
    for (int i = 0; i < (int)otherBoids.size(); i++) {
        Boid* other = otherBoids[i];
        float d = Vec2::dist(this->position, other->position);
        if(d > 0 && d < neighborDist) {
            sum.add(&(other->velocity));
            count++;
        }
    }
    if(count > 0) {
        sum.div(count);
        sum.normalize();
        sum.mult(this->maxSpeed);
        Vec2 steer = Vec2::sub(sum, this->velocity);
        steer.limit(this->maxForce);
        return steer;
    }
    return Vec2::createVector(0, 0);
}

Vec2 Boid::cohesion(std::vector<Boid*> otherBoids, float neighborDist) {
    Vec2 sum = Vec2::createVector(0, 0);
    int count = 0;
    for (int i = 0; i < (int)otherBoids.size(); i++) {
        Vec2 otherPos = otherBoids[i]->position;
        float d = Vec2::dist(this->position, otherPos);
        if(d > 0 && d < neighborDist) {
            sum.add(&otherPos);
            count++;
        }
    }
    if(count > 0) {
        sum.div(count);
        return this->seek(sum);
    }
    return Vec2::createVector(0, 0);
}

Vec2 Boid::seek(Vec2 target) {
    Vec2 desired = Vec2::sub(target, this->position);
    desired.normalize();
    desired.mult(this->maxSpeed);
    Vec2 steer = Vec2::sub(desired, this->velocity);
    steer.limit(this->maxForce);
    return steer;
}

void Boid::render (
    std::vector<Boid*> boids,
    SDL_Renderer* renderer,
    SDL_Texture* tex,
    float desiredSep,
    float alignmentDist,
    float cohesionDist,
    float maxSpeed,
    bool showVelocityLines
) {
    SDL_Rect srcRect = { x: 0, y: 0, w: BABY_WIDTH, h: BABY_HEIGHT };
    for (std::vector<Boid*>::iterator it = boids.begin(); it != boids.end(); it++)
    {
        Boid* boid = *it;
        boid->maxSpeed = maxSpeed;
        boid->borders();
        boid->flock(boids, desiredSep, alignmentDist, cohesionDist);
        boid->update();

        SDL_Rect rect = {
            x: int(boid->position.x),
            y: int(boid->position.y),
            w: BABY_WIDTH,
            h: BABY_HEIGHT
        };
        double angle = atan(boid->velocity.y / boid->velocity.x) * 180/3.1415 + 90;
        SDL_RenderCopyEx(renderer, tex, &srcRect, &rect, angle, NULL, SDL_FLIP_NONE);

        if(showVelocityLines) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            int babyPosX = boid->position.x + BABY_WIDTH / 2;
            int babyPosY = boid->position.y + BABY_HEIGHT / 2;
            SDL_RenderDrawLine(
                renderer,
                babyPosX,
                babyPosY,
                babyPosX + BABY_WIDTH*(boid->velocity.x),
                babyPosY + BABY_HEIGHT*(boid->velocity.y)
            );
        }
    }
}