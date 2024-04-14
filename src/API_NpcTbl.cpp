#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "API_NpcTbl.h"

#include "mod_loader.h"
#include "cave_story.h"

// 5000 atm cause im crazy
#define MAX_TABLE_SIZE 5000

// Global variables
NPCFUNCTION gpEntityFuncTbl[MAX_TABLE_SIZE];
size_t entityFuncCount = 0;

void Replacement_ActNpChar(void)
{
	int i;
	int code_char;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if (gNPC[i].cond & 0x80)
		{
			code_char = gNPC[i].code_char;

			if (code_char <= 360)
				gpNpcFuncTbl[code_char](&gNPC[i]);
			else
				gpEntityFuncTbl[code_char - 361](&gNPC[i]);

			if (gNPC[i].shock)
				--gNPC[i].shock;
		}
	}
}

void Replacement_ChangeNpCharByEvent(int code_event, int code_char, int dir)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
		{
			gNPC[n].bits &= ~(NPC_SOLID_SOFT | NPC_IGNORE_TILE_44 | NPC_INVULNERABLE | NPC_IGNORE_SOLIDITY | NPC_BOUNCY | NPC_SHOOTABLE | NPC_SOLID_HARD | NPC_REAR_AND_TOP_DONT_HURT | NPC_SHOW_DAMAGE);	// Clear these flags
			gNPC[n].code_char = code_char;
			gNPC[n].bits |= (*gNpcTable)[gNPC[n].code_char].bits;
			gNPC[n].exp = (*gNpcTable)[gNPC[n].code_char].exp;
			SetUniqueParameter(&gNPC[n]);
			gNPC[n].cond |= 0x80;
			gNPC[n].act_no = 0;
			gNPC[n].act_wait = 0;
			gNPC[n].count1 = 0;
			gNPC[n].count2 = 0;
			gNPC[n].ani_no = 0;
			gNPC[n].ani_wait = 0;
			gNPC[n].xm = 0;
			gNPC[n].ym = 0;

			if (dir == 5)
			{
				// Another empty case that has to exist for the same assembly to be generated
			}
			else if (dir == 4)
			{
				if (gNPC[n].x < gMC.x)
					gNPC[n].direct = 2;
				else
					gNPC[n].direct = 0;
			}
			else
			{
				gNPC[n].direct = dir;
			}

			if (code_char <= 360)
				gpNpcFuncTbl[code_char](&gNPC[n]);
			else
				gpEntityFuncTbl[code_char - 361](&gNPC[n]);
		}
	}
}

void Replacement_ChangeCheckableNpCharByEvent(int code_event, int code_char, int dir)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if (!(gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
		{
			gNPC[n].bits &= ~(NPC_SOLID_SOFT | NPC_IGNORE_TILE_44 | NPC_INVULNERABLE | NPC_IGNORE_SOLIDITY | NPC_BOUNCY | NPC_SHOOTABLE | NPC_SOLID_HARD | NPC_REAR_AND_TOP_DONT_HURT | NPC_SHOW_DAMAGE);	// Clear these flags
			gNPC[n].bits |= NPC_INTERACTABLE;
			gNPC[n].code_char = code_char;
			gNPC[n].bits |= (*gNpcTable)[gNPC[n].code_char].bits;
			gNPC[n].exp = (*gNpcTable)[gNPC[n].code_char].exp;
			SetUniqueParameter(&gNPC[n]);
			gNPC[n].cond |= 0x80;
			gNPC[n].act_no = 0;
			gNPC[n].act_wait = 0;
			gNPC[n].count1 = 0;
			gNPC[n].count2 = 0;
			gNPC[n].ani_no = 0;
			gNPC[n].ani_wait = 0;
			gNPC[n].xm = 0;
			gNPC[n].ym = 0;

			if (dir == 5)
			{
				// Another empty case that has to exist for the same assembly to be generated
			}
			else if (dir == 4)
			{
				if (gNPC[n].x < gMC.x)
					gNPC[n].direct = 2;
				else
					gNPC[n].direct = 0;
			}
			else
			{
				gNPC[n].direct = (signed char)dir;
			}

			if (code_char <= 360)
				gpNpcFuncTbl[code_char](&gNPC[n]);
			else
				gpEntityFuncTbl[code_char - 361](&gNPC[n]);
		}
	}
}

// Function to add a new ActEntity function to the table
void AutPI_AddEntity(NPCFUNCTION func) {
    if (entityFuncCount < MAX_TABLE_SIZE) {
        // Add the new function to the end of the array
        gpEntityFuncTbl[entityFuncCount++] = func;
    }
    else {
        fprintf(stderr, "Maximum NPC count reached. Cannot add more functions.\n");
    }
}