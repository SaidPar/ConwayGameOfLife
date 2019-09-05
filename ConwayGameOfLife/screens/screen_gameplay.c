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
static bool* pLastIteration = NULL;

static bool inSimulation = false;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
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

	pLastIteration = (bool*)malloc(numSquareRows * numSquareCols * sizeof(bool));
	//if ((bool *) NULL == pLastIteration)
		//log error

	inSimulation = false;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
	// Seeding Life
	//
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && false == inSimulation)
	{
		mousePoint = GetMousePosition();
		int row = mousePoint.y / SQUARE_SIZE;
		int col = mousePoint.x / SQUARE_SIZE;
		*(pDrawArray + row * numSquareCols + col) = true;
	}


	// Simulating Life
	//
	if (inSimulation)
	{
		// Exit Simulation
		//
		if (IsKeyPressed(KEY_ENTER))
		{
			finishScreen = true;
			inSimulation = false;
		}
		else
		{
			if (framesCounter % 15 == 0)
			{
				SimulateLife();
			}
		}
	}
	framesCounter++;

	// Start Simulation
	//
	if (IsKeyPressed(KEY_ENTER) && false == inSimulation)
		inSimulation = true;
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

void SimulateLife(void)
{
	bool* pTmp = pLastIteration;
	pLastIteration = pDrawArray;
	pDrawArray = pTmp;

	for (int row = 0; row < numSquareRows; row++)
	{
		for (int col = 0; col < numSquareCols; col++)
		{
			int neighborCount = GetNeighborCount(pLastIteration, row, col);

			bool* pCurrent = (pLastIteration + row * numSquareCols + col);
			bool* pCurrentDraw = (pDrawArray + row * numSquareCols + col);
			if (*pCurrent == true)
			{
				// if alive
				switch (neighborCount)
				{
				case 0:
				case 1:
					// cell dies
					*pCurrentDraw = false;
					break;

				case 2:
				case 3:
					// cell lives to next generation
					*pCurrentDraw = true;
					break;

				case 4:
				default:
					// cell dies from overpopulation
					*pCurrentDraw = false;
					break;
				}
			} // end if alive
			else
			{
				// if dead
				switch (neighborCount)
				{
				case 3:
					// becomes alive as if by reproduction
					*pCurrentDraw = true;
					break;

				default:
					*pCurrentDraw = *pCurrent;
				}
			} // end if dead
		} // end for cols
	} // end for rows
}

int GetNeighborCount(bool* pSquareArray, int row, int col)
{
	int neighborCount = 0;

	// East Neighbor
	//
	if (col > 0)
	{
		bool* pNeighbor = (pSquareArray + row * numSquareCols + (col - 1));
		if (*pNeighbor == true)
			neighborCount++;
	}

	// NorthEast Neighbor
	//
	if (col > 0 && row > 0)
	{
		bool* pNeighbor = (pSquareArray + (row - 1) * numSquareCols + (col - 1));
		if (*pNeighbor == true)
			neighborCount++;
	}

	// North Neighbor
	//
	if (row > 0)
	{
		bool* pNeighbor = (pSquareArray + (row - 1) * numSquareCols + col);
		if (*pNeighbor == true)
			neighborCount++;
	}

	// NorthWest Neighbor
	//
	if (row > 0 && col < numSquareCols)
	{
		bool* pNeighbor = (pSquareArray + (row - 1) * numSquareCols + (col + 1));
		if (*pNeighbor == true)
			neighborCount++;
	}

	// West Neighbor
	//
	if (col < numSquareCols)
	{
		bool* pNeighbor = (pSquareArray + row * numSquareCols + (col + 1));
		if (*pNeighbor == true)
			neighborCount++;
	}

	// SouthWest Neighbor
	//
	if (row < numSquareRows && col < numSquareCols)
	{
		bool* pNeighbor = (pSquareArray + (row + 1) * numSquareCols + (col + 1));
		if (*pNeighbor == true)
			neighborCount++;
	}
	
	// South Neighbor
	//
	if (row < numSquareRows)
	{
		bool* pNeighbor = (pSquareArray + (row + 1) * numSquareCols + col);
		if (*pNeighbor == true)
			neighborCount++;
	}

	// SouthEast Neighbor
	//
	if (row < numSquareRows && col > 0)
	{
		bool* pNeighbor = (pSquareArray + (row + 1) * numSquareCols + (col - 1));
		if (*pNeighbor == true)
			neighborCount++;
	}

	return neighborCount;
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
	free(pDrawArray);
	free(pLastIteration);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}