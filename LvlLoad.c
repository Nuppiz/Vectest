#include "Common.h"
#include "Game.h"
#include "Structs.h"

extern GameData_t Game;
extern Texture_t Textures[NUM_TEXTURES];

void levelLoader()
{
    FILE* level_file;
    char buffer[100];
    char filename[13];
    char c;
    int i = 0;
 
    float x, y;
    double angle;
    int radius, sprite_id;
    uint8_t control, ai_mode;
    id_t ai_target;
    
    /* build filename and load it */
    sprintf(filename, "LEVELS/level%d.txt", Game.Map.level_num);
    level_file = fopen(filename, "rb");
    
    if (level_file == NULL)
    {
        setVideoMode(TEXT_MODE);
        printf("Unable to open file: %s\n", filename);
        printf("Please check the file actually exists!\n");
        quit();
    }
    
    while ((c = fgetc(level_file)) != EOF)
    {
        if (c == '$')
        {
            fscanf(level_file, "%s", buffer);
            if (strcmp(buffer, "leveldim") == 0)
            {
                fscanf(level_file, "%d %d", &Game.Map.width, &Game.Map.height);
                Game.Map.collision = malloc((Game.Map.width * Game.Map.height) * sizeof(uint8_t));
                Game.Map.tiles = malloc((Game.Map.width * Game.Map.height) * sizeof(uint8_t));
            }
            else if (strcmp(buffer, "player") == 0)
            {
                fscanf(level_file, "%f %f %lf %d %d %d %d %d",
                    &x, &y, &angle, &radius, &control, &ai_mode, &ai_target, &sprite_id);
                Game.player_id = createObject(x, y, angle, radius, control, ai_mode, ai_target, &Textures[sprite_id]);
            }
            else if (strcmp(buffer, "dude") == 0)
            {
                fscanf(level_file, "%f %f %lf %d %d %d %d %d",
                    &x, &y, &angle, &radius, &control, &ai_mode, &ai_target, &sprite_id);
                createObject(x, y, angle, radius, control, ai_mode, ai_target, &Textures[sprite_id]);
            }
            else if (strcmp(buffer, "collisiondata") == 0)
            {
                while (i < Game.Map.width * Game.Map.height)
                {
                    if (c != '\n')
                    {
                        fscanf(level_file, "%d", &Game.Map.collision[i]);
                        i++;
                    }
                    else
                        putchar('\n');
                }
            }
            else if (strcmp(buffer, "tiledata") == 0)
            {
                i = 0;
                while ((c = fgetc(level_file)) != EOF)
                {
                    if (c != '\n')
                    {
                        fscanf(level_file, "%d", &Game.Map.tiles[i]);
                        i++;
                    }
                    else
                        putchar('\n');
                }
            }
        }
    }
    fclose(level_file);
}