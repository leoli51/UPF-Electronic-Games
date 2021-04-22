#ifndef CAMERA_H
#define CAMERA_H

#include "framework.h"
#include "image.h"

class Camera : public Image {
    public:
        Area world_viewport;
    
    public:

        void renderSprite(Sprite* sprite){
            Area sprite_area(sprite->position.x, sprite->position.y, sprite->size.x, sprite->size.y);
            Area intersection = intersection_rect(world_viewport, sprite_area);
            if (intersection == NULL_AREA) 
                return;
            else 
                drawImage(sprite->image, sprite->position.x, sprite->position.y, intersection);
        };

};

#endif 