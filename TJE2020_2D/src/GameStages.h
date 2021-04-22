#ifndef GAME_STAGES_H
#define GAME_STAGES_H

#include "stage.h"
#include "Dino.h"

class IntroStage : public Stage {
    public:
        Image* intro_font;

    public:
        void init(){
            intro_font = Image::Get("data/bitmap-font-white.tga"); //load bitmap-font image

        };

        void render(Image* framebuffer){
            framebuffer->fill(Color::BLACK);
            framebuffer->drawText("Party Dino :)", 0, 0, *intro_font);
        };

        void update(double dt){

        };

        void deinit(){

        };
};

class PlayStage : public Stage {
    public:
        Dino* player;

    public:
        void init(){
            std::cout<<"Play stage init"<<std::endl;
            player = new Dino(Vector2(20,20)); 
        };

        void render(Image* framebuffer){
            framebuffer->fill(Color::BLACK);
            player->render(framebuffer);
        };

        void update(double dt){
            player->update(dt);
        };

        void deinit(){

        };
};



#endif