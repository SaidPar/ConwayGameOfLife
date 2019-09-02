/**********************************************************************************************
*
*   raylib - Standard Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2019 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------

static const int screenWidth = 800;
static const int screenHeight = 450;

static int framesCounter;
static int finishScreen;

static Vector2 offset = { 0 };
static Vector2 mousePoint = { 0.0f, 0.0f };
static Vector2 squareVector = { SQUARE_SIZE, SQUARE_SIZE };

static int numSquareCols = 0;
static int numSquareRows = 0;
static bool* pDrawArray = NULL;


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
	offset.x = screenWidth % SQUARE_SIZE;
	offset.y = screenHeight % SQUARE_SIZE;

	// Allocate draw array
	numSquareCols = screenWidth / SQUARE_SIZE + 1;
	numSquareRows = screenHeight / SQUARE_SIZE + 1;
	pDrawArray = (bool*)malloc(numSquareRows * numSquareCols * sizeof(bool));
	//if ((bool *) NULL == pDrawArray)
		//log error
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		mousePoint = GetMousePosition();
		int row = mousePoint.y / SQUARE_SIZE;
		int col = mousePoint.x / SQUARE_SIZE;
		*(pDrawArray + row * numSquareCols + col) = true;
	}
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

	// Draw grid lines
	for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
	{
		DrawLineV(	(Vector2) { SQUARE_SIZE * i + offset.x / 2, offset.y / 2 }, 
					(Vector2) { SQUARE_SIZE * i + offset.x / 2, screenHeight - offset.y / 2 }, 
					LIGHTGRAY);
	}

	for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
	{
		DrawLineV(	(Vector2) { offset.x / 2, SQUARE_SIZE * i + offset.y / 2 }, 
					(Vector2) { screenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2 }, 
					LIGHTGRAY);
	}
	
	for (int row = 0; row < numSquareRows; row++)
	{
		for (int col = 0; col < numSquareCols; col++)
		{
			if (*(pDrawArray + row * numSquareCols + col) == true)
			{
				Vector2 squarePos = { SQUARE_SIZE * col + offset.x / 2, SQUARE_SIZE * row + offset.y / 2 };
				DrawRectangleV(squarePos, squareVector, WHITE);
			}
		}
	}
		
	
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
	free(pDrawArray);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}