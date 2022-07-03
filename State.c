#include "Init.h"
#include "Input.h"
#include "Update.h"
#include "Draw.h"
#include "Exit.h"
#include "Enums.h"

/* Game states and stack manager */

State* Stack[NUM_STATES];
int state_count = 0;
int stack_top;

State States[NUM_STATES] = {
{
    0,
    titleInit,
    titleInput,
    titleUpdate,
    titleDraw,
    titleExit
},
{
    0,
    gameInit,
    gameInput,
    gameUpdate,
    gameDraw,
    gameExit
},
{
    0,
    pauseInit,
    pauseInput,
    pauseUpdate,
    pauseDraw,
    pauseExit
}
};

void pushToStack(int state_index)
{
    if (States[state_index].flags & STATE_IS_ACTIVE == 0)
    {
        state_count += 1;
        stack_top = state_count - 1;

        States[state_index].init();
        Stack[stack_top] = &States[state_index];
        Stack[stack_top]->flags |= STATE_IS_ACTIVE;
    }
}

void popFromStack()
{
    if (state_count > 0)
    {
        States[stack_top].exit();
        Stack[stack_top]->flags &= ~ STATE_IS_ACTIVE;

        Stack[stack_top] = NULL;
        state_count -= 1;
        stack_top = state_count - 1;
    }
}