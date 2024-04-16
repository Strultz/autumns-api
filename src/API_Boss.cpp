#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "API_Boss.h"

#include "mod_loader.h"
#include "cave_story.h"

BOSSFUNCTION gpBossAPIFuncTbl[MAX_BOSS_TABLE_SIZE];
size_t bossFuncCount = 0;

void Replacement_ActBossChar()
{
	int code_char;
	int bos;

	if (!(gBoss[0].cond & 0x80))
		return;

	code_char = gBoss[0].code_char;

	if (code_char < 10)
		gpBossFuncTbl[code_char]();
	else
		gpBossAPIFuncTbl[code_char - 10]();

	for (bos = 0; bos < BOSS_MAX; ++bos)
		if (gBoss[bos].shock)
			--gBoss[bos].shock;
}

// Function to add a new ActEntity function to the table
void AutPI_AddBoss(BOSSFUNCTION func, char* author, char* name) {
    if (bossFuncCount < MAX_BOSS_TABLE_SIZE) {
        // Add the new function to the end of the array
        gpBossAPIFuncTbl[bossFuncCount++] = func;
        printf("Added BOSS '%s:%s' to boss function table with ID %d.\n", author, name, bossFuncCount + 9);
    }
    else {
        fprintf(stderr, "Maximum BOSS count reached. Cannot add more functions.\n");
    }
}