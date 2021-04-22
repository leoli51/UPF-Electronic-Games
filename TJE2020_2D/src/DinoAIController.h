#ifndef DINO_AI_CONTROLLER_H
#define DINO_AI_CONTROLLER_H

#define REACH_THRESHOLD 1

#include "Dino.h"
#include "framework.h"

class DinoAIController {

    public:
        Dino* dino;
        float max_wait_time;
        float wait_time;
        bool getting_there;
        Vector2 target;
        float move_speed = 10; 
    
    public: 
        DinoAIController(Dino* d, float max_wait = 5){
            dino = d;
            max_wait_time = max_wait;
            wait_time = random(max_wait);
            target.set(random(100), random(100));
        };

        void setControlledDino(Dino* d){
            dino = d;
        };

        void update(float dt){
            if (getting_there){
                dino->move((target - dino->sprite->position).normalize() * dt * move_speed);
                getting_there = dino->sprite->position.distance(target) > REACH_THRESHOLD;
            }
            else 
                wait_time -= dt;

            if (wait_time <= 0){
                getting_there = true;
                wait_time = random(max_wait_time);
                target.set(random(100), random(100));
            }
        };
};

#endif