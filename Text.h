#ifndef TEXT
#define TEXT

void loadFont();
void drawSymbol(int x, int y, int i, uint8_t color);
int  drawText(int x, int y, char* string, uint8_t color);
int  drawTextClipped(int x, int y, char* string, uint8_t color);

#endif/* TEXT */
