#include "Common.h"
#include "Game.h"
#include "Structs.h"
#include "Loadgfx.h"

/* Level data and entity loader */

extern GameData_t Game;
extern Texture_t* Textures;
Entity_t Entities[32];
Tile_t TileSet[100];
extern int texture_count;

const char* entity_type_strings [NUM_ENTITYTYPES] =
{
    "Door",
    "Button",
    "Key",
};

void loadTileset(char* filename)
{
    FILE* tileset;
    char c, symbol, tex_id;
    char name[50];

    tileset = fopen(filename, "rb");

    if (tileset == NULL)
        tileset = fopen(DEFAULT_TILESET, "rb");

    ASSERT(tileset != NULL);

    while ((c = fgetc(tileset)) != EOF)
    {
        if (c == '$')
        {
            symbol = fgetc(tileset) - 32;
            fscanf(tileset, "%s", name);
            tex_id = loadTexture(name);
            TileSet[symbol].texture_id = tex_id;

            while((c = fgetc(tileset)) != '\n' && c != EOF)
            {
                switch (c)
                {
                case 'D': Textures[tex_id].material_type = MAT_DEFAULT; continue;
                case 'G': Textures[tex_id].material_type = MAT_GRASS; continue;
                case 'O': TileSet[symbol].obstacle = 1; continue;
                case 'B': TileSet[symbol].block_bullets = 1; continue;
                case 'E': TileSet[symbol].is_entity = 1; continue;
                
                default:  continue;
                }
            }
        }
    }
    fclose(tileset);
}

int entityTypeCheck(char* entity_name)
{
    int entity_type_index;

    for (entity_type_index = 0; entity_type_index < NUM_ENTITYTYPES; entity_type_index++)
    {
        if (strcmp(entity_name, entity_type_strings[entity_type_index]) == 0)
            return entity_type_index;
    }
    return RETURN_ERROR;
}

void entityHandler(FILE* level_file, int entity_type)
{
    // because it would look messy to put all of this into the main loader function

    int entity_id, ent_x, ent_y, state, tilemap_location; // common variables
    int locked, key; // door variables
    int target; // switch/button variables
    Entity_t* ent; // shorter name of the current entity for more compact code

    fscanf(level_file, "%d %d %d %d", &entity_id, &ent_x, &ent_y, &state);

    tilemap_location = ent_y * Game.Map.width + ent_x;

    Game.Map.tilemap[tilemap_location].is_entity = 1;
    Game.Map.tilemap[tilemap_location].entity_value = entity_id;

    ent = &Entities[entity_id];

    ent->x = ent_x;
    ent->y = ent_y;
    ent->state = state;
    ent->type = entity_type;

    switch(entity_type)
    {
    case ENT_DOOR: fscanf(level_file, "%d %d", &locked, &key),
                ent->data.door.locked = locked,
                ent->data.door.key = key;
                break;
    case ENT_BUTTON: fscanf(level_file, "%d", &target),
                ent->data.button.target = target;
                break;
    }
}

void levelLoader()
{
    // general variables
    FILE* level_file;
    char buffer[100];
    char c;
    int i;

    // tileset variables
    int tileset_found = FALSE;
    char tileset_file[20] = DEFAULT_TILESET;
    char texture_filename[20];

    // actor variables
    int x, y;
    double angle;
    int radius, control, ai_mode, ai_timer;
    id_t ai_target, texture_id;

    // entity variables
    char entity_name [20];
    int entity_type;

    //int key_type; // key variable (red, blue, yellow, skeleton?)

    level_file = fopen("LEVELS/tiletest.txt", "r");
    
    if (level_file == NULL)
    {
        setVideoMode(TEXT_MODE);
        printf("Unable to open file: tiletest.txt");
        printf("Please check the file actually exists!\n");
        quit();
    }

    while ((c = fgetc(level_file)) != EOF)
    {
        if (c == '$')
        {
            fscanf(level_file, "%s", buffer);
            if (strcmp(buffer, "tileset") == 0)
            {
                fscanf(level_file, "%s", tileset_file);
                tileset_found = TRUE;
                loadTileset(tileset_file);
            }
            else if (strcmp(buffer, "leveldim") == 0)
            {
                fscanf(level_file, "%d %d", &Game.Map.width, &Game.Map.height);
                Game.Map.tilemap = malloc((Game.Map.width * Game.Map.height) * sizeof(Tile_t));
            }
            else if (strcmp(buffer, "tilemap") == 0)
            {
                i = 0;
                while ((c = fgetc(level_file)) != EOF && i < Game.Map.width * Game.Map.height)
                {
                    if (c != '\n')
                    {
                        Game.Map.tilemap[i] = TileSet[c - 32];
                        i++;
                    }
                }
                // remove once a proper key system is in place
                Game.Map.tilemap[87].is_entity = 0;
                Game.Map.tilemap[87].entity_value = TILE_KEY_RED;
            }
            else if (strcmp(buffer, "player") == 0)
            {
                fscanf(level_file, "%d %d %lf %d %d %s",
                    &x, &y, &angle, &radius, &control, texture_filename);
                Game.player_id = createObject((float)x, (float)y, angle, radius, control, 0, 0, 0, texture_filename);
            }
            else if (strcmp(buffer, "dude") == 0)
            {
                fscanf(level_file, "%d %d %lf %d %d %d %d %u %s",
                    &x, &y, &angle, &radius, &control, &ai_mode, &ai_timer, &ai_target, texture_filename);
                createObject((float)x, (float)y, angle, radius, control, ai_mode, ai_timer, ai_target, texture_filename);
            }
            else if (strcmp(buffer, "entity") == 0)
            {
                fscanf(level_file, "%s", entity_name);
                entity_type = entityTypeCheck(entity_name);
                if (entity_type == RETURN_ERROR)
                {
                    // replace later with just exit to main menu
                    setVideoMode(TEXT_MODE);
                    printf("Level load error: invalid entity type.\n");
                    printf("Please check the level file!\n");
                    quit();
                }
                entityHandler(level_file, entity_type);
            }
        }
    }
    if (tileset_found == FALSE)
    {
        loadTileset(tileset_file);
    }
    fclose(level_file);
}