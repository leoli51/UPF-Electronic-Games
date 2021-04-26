#ifndef BEAT_BAR_H
#define BEAT_BAR_H

#include "image.h"
#include "framework.h"
#include "input.h"

#define MUSIC_BPM 120 // You little beauty - FISHER
#define BEAT_PERIOD 60.0f / MUSIC_BPM

#define UP_BEAT 1
#define DOWN_BEAT 2
#define DOUBLE_BEAT 3

typedef struct {
    float position;
    int type;
} Beat;

Beat randomBeat(){
    Beat b;
    b.position = 160;
    b.type = rand() % 3 + 1;
    return b;
};

class BeatBar {
    public:
        int hitted;
        int missed;
        float beat_speed;
        float time_to_next_beat;
        std::vector<Beat> beats;
        PlayerController* player;

    public:
        BeatBar(PlayerController* player_controller = NULL){
            time_to_next_beat = BEAT_PERIOD;
            beats = std::vector<Beat>();
            beat_speed = 25;
            hitted = missed = 0;
            player = player_controller;
        };


        /**
         * Returns true if there is a hit and false otherwise
         */
        bool update(float dt){
            time_to_next_beat -= dt;
            if (time_to_next_beat <= 0){
                time_to_next_beat = BEAT_PERIOD;
                //spawn beat 
                beats.push_back(randomBeat());
            }
            // update all beats
            int beat_on_line_index = -1;
            for (int i = 0; i < beats.size(); i++) {
                beats.at(i).position -= dt * beat_speed;  

                // which beat is now on the beat line
                if (12 <= beats.at(i).position && beats.at(i).position <= 18)
                    beat_on_line_index = i;

                if (beats.at(i).position < -4) {
                    beats.erase(beats.begin() + i);
                    i--;
                    missed++;
                    player->decreaseInfluenceRadius();
                }                 
            }

            if (beat_on_line_index != -1){
                int pressed = 0;
                pressed |= Input::wasKeyPressed(SDL_SCANCODE_K) ? 1 : 0;
                pressed |= Input::wasKeyPressed(SDL_SCANCODE_M) ? 2 : 0;

                if (beats.at(beat_on_line_index).type == pressed){
                    hitted++;
                    beats.erase(beats.begin() + beat_on_line_index);
                    player->increaseInfluenceRadius();
                    return true;
                }
                
                //std::cout<<"hits: "<<hitted<<" miss: "<<missed<<std::endl;
            }
            return false;
        };

        void render(Image* framebuffer){
            // draw three lines 
            framebuffer->drawLine(0, framebuffer->height - 11, framebuffer->width, framebuffer->height - 11, Color::WHITE);
            framebuffer->drawLine(0, framebuffer->height - 6, framebuffer->width, framebuffer->height - 6, Color::WHITE);
            framebuffer->drawLine(0, framebuffer->height - 1, framebuffer->width, framebuffer->height - 1, Color::WHITE);

            // vertical line
            framebuffer->drawLine(16, framebuffer->height - 11, 16, framebuffer->height, Color::WHITE);

            // draw beats 
            Beat tmp;
            for (int i = 0; i < beats.size(); i++) {
                tmp = beats.at(i);
                
                if (tmp.type & UP_BEAT)
                    framebuffer->drawRectangle(tmp.position, framebuffer->height - 10, 4, 4, Color::BRIGHT_TEAL);
                if (tmp.type & DOWN_BEAT)
                    framebuffer->drawRectangle(tmp.position, framebuffer->height - 5, 4, 4, Color::ELECTRIC_CRIMSON);
            }
        };
};

#endif