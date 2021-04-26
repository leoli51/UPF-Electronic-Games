#ifndef DINO_AI_CONTROLLER_H
#define DINO_AI_CONTROLLER_H

#define REACH_THRESHOLD 1
#define MAX_X 120
#define MAX_Y 109

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
            target.set(random(MAX_X), random(MAX_Y));
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
                target.set(random(MAX_X), random(MAX_Y));
            }
        };
};

class AiDinoManager {

    public:
        std::vector<DinoAIController*> controllers;

        AiDinoManager(int dinos){
            for (int i = 0; i < dinos; i++)
                spawnDino();
        };

        ~AiDinoManager(){
            for (int i = 0; i < controllers.size(); i++){
                DinoAIController* controller = controllers.at(i);
                delete controller->dino;
                delete controller;
            }
            controllers.clear();
        };

        void spawnDino(){
            Dino* dino = new Dino(Vector2(random(MAX_X), random(MAX_Y)), "data/green_dino.tga", false);
            DinoAIController* controller = new DinoAIController(dino);

            controllers.push_back(controller); 
        };

        void update(float dt){
            for (int i = 0; i < controllers.size(); i++){
                controllers.at(i)->update(dt);
                controllers.at(i)->dino->update(dt);
            };
        };


        void render(Image* framebuffer){
            std::sort(controllers.begin(), controllers.end(), [](const DinoAIController* a, const DinoAIController* b){return a->dino->sprite->position.y < b->dino->sprite->position.y;});

            for (int i = 0; i < controllers.size(); i++)
                controllers.at(i)->dino->render(framebuffer);
        };

        Dino* closestDinoTo(Vector2 pos){
            if (controllers.empty()) return NULL;

            Dino* closest = controllers.at(0)->dino;
            float min_distance = closest->sprite->position.distance(pos);
            for (int i = 0; i < controllers.size(); i++){
                float dst = controllers.at(i)->dino->sprite->position.distance(pos);
                if (dst < min_distance){
                    closest = controllers.at(i)->dino;
                    min_distance = dst;
                }
            }
            return closest;
        };

        void addDino(Dino* dino){
            DinoAIController* controller = new DinoAIController(dino);
            dino->sprite->setImage("data/green_dino.tga");
            dino->sprite->setSheetProperties(24, 1);
            dino->sprite->setAnimation(0,1,.4f);
            controllers.push_back(controller); 
        };

        void removeDino(Dino* dino){
            for (int i = 0; i < controllers.size(); i++)
                if (dino == controllers.at(i)->dino){
                    DinoAIController* tmp = controllers.at(i);
                    controllers.erase(controllers.begin() + i);
                    delete tmp;
                }
                
        };

};

#endif