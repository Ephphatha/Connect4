/*
 *  The MIT License
 *
 *  Copyright 2008 Andrew James <ephphatha@thelettereph.com>.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
/* Andrew James
** Date Created: 2008/11/21
** Last Modified: 2008/11/21
** game.c
** Function definitions for the connect 4 game.
*/
#include "game.h"

BOOL ClearGame(short** gameState, int gridWidth, int gridHeight,
               short* activePlayer)
{
    for(int i = 0; i < gridWidth; i++)
    {
        for(int j = 0; j < gridHeight; j++)
        {
            gameState[i][j] = 0;
        }
    }
    *activePlayer = 0;
    return TRUE;
}

int CheckRuns(short** gameState, int gridWidth, int gridHeight, int x, int y)
{
    int horizontalRun = 1, verticalRun = 1, ldiagonalRun = 1, rdiagonalRun = 1;
    int i = x, j = y;
    while((--i >= 0) && (gameState[i][y] == gameState[x][y]))
    {
        horizontalRun++;
    }
    i = x;
    while((++i < gridWidth) && (gameState[i][y] == gameState[x][y]))
    {
        horizontalRun++;
    }
    i = x;

    while((--j >= 0) && (gameState[x][j] == gameState[x][y]))
    {
        verticalRun++;
    }
    j = y;
    while((++j < gridHeight) && (gameState[x][j] == gameState[x][y]))
    {
        verticalRun++;
    }
    j = y;

    while((--i >= 0) && (--j >= 0) && (gameState[i][j] == gameState[x][y]))
    {
        rdiagonalRun++;
    }
    i = x;
    j = y;
    while((++i < gridWidth) && (++j < gridHeight) &&
          (gameState[i][j] == gameState[x][y]))
    {
        rdiagonalRun++;
    }
    i = x;
    j = y;

    while((--i >= 0) && (++j < gridHeight) &&
          (gameState[i][j] == gameState[x][y]))
    {
        ldiagonalRun++;
    }
    i = x;
    j = y;
    while((++i < gridWidth) && (--j >= 0) &&
          (gameState[i][j] == gameState[x][y]))
    {
        ldiagonalRun++;
    }

    return max(max(horizontalRun, verticalRun),
               max(ldiagonalRun, rdiagonalRun));
}

int DoInput(short** gameState, int gridWidth, int gridHeight,
            int column, short activePlayer)
{
    int i = 0;

    if(column >= gridWidth)
    {
        return -1;
    }

    while(!gameState[column][i] && (++i < gridHeight))
    {
    }

    if(--i >= 0)
    {
        gameState[column][i] = activePlayer;
        return i;
    }
    else
    {
        return -1;
    }
}
