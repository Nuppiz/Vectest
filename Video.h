#ifndef VIDEO_H
#define VIDEO_H

#include "Structs.h"

extern uint8_t *VGA;

void setVideoMode(uint8_t mode);
void loadPalette(char* filename, Palette_t* pal);
void setPalette_VGA(Palette_t* pal);
void render();

#endif /* VIDEO_H */
