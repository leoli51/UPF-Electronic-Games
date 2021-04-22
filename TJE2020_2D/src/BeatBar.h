#ifndef BEAT_BAR_H
#define BEAT_BAR_H

#include "image.h"
#include "framework.h"

#define MUSIC_BPM 124 // You little beauty - FISHER
#define BEAT_PERIOD 60.0f / 124.0f



class Beat {
    public :
        float position;
        int type;
    
    public:
        void update();
}

class BeatBar {
    public:
        float time_to_next_beat;
    
    public:
        BeatBar(){
            time_to_next_beat = BEAT_PERIOD;
        };

        void update(float dt){
            time_to_next_beat -= dt;
            if (time_to_next_beat <= 0){
                time_to_next_beat = BEAT_PERIOD;
                //spawn beat 
            }
        };

        void render(Image* framebuffer){
            // draw three lines 
            framebuffer->drawLine(0, framebuffer->height - 11, framebuffer->width, framebuffer->height - 11, Color::WHITE);
            framebuffer->drawLine(0, framebuffer->height - 6, framebuffer->width, framebuffer->height - 6, Color::WHITE);
            framebuffer->drawLine(0, framebuffer->height - 1, framebuffer->width, framebuffer->height - 1, Color::WHITE);

            framebuffer->drawLine(16, framebuffer->height - 11, 16, framebuffer->height, Color::WHITE);
        };
};

#endif