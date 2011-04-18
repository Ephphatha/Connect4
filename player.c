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
** Date Created: 2008/12/06
** Last Modified: 2008/12/10
** player.c
** Function definitions for the connect 4 game.
*/
#include "player.h"

BOOL AddPlayer(PLAYER **players,
               short *totalPlayers,
               short *maxPlayers,
               char* name,
               COLORREF colour,
               BOOL isLocal,
               BOOL isPlaying,
               BOOL isAI)
{
    PLAYER newPlayer[1];
    memset(newPlayer, 0, sizeof(PLAYER));
    strncpy(newPlayer->name, name, 19);
    newPlayer->name[19] = '\0';
    newPlayer->colour = colour;
    newPlayer->isAI = isAI;
    newPlayer->isLocal = isLocal;
    newPlayer->isPlaying = isPlaying;

    if(AddPlayerFromStruct(players, totalPlayers, maxPlayers, newPlayer))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL AddPlayerFromStruct(PLAYER **players,
                         short *totalPlayers,
                         short *maxPlayers,
                         PLAYER *newPlayer)
{
    if(*totalPlayers >= *maxPlayers)
    {
        short newMaxPlayers = *maxPlayers * 2;
        PLAYER *newPlayerArray = (PLAYER*)calloc(newMaxPlayers,
                                                 sizeof(PLAYER));

        if(newPlayerArray)
        {
            memcpy(newPlayerArray, *players, *totalPlayers * sizeof(PLAYER));
            free(*players);
            *players = newPlayerArray;
            *maxPlayers = newMaxPlayers;
        }
    }

    if(*totalPlayers < *maxPlayers)
    {
        memcpy(&(*players)[*totalPlayers], newPlayer, sizeof(PLAYER));
        (*totalPlayers)++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int NextPlayer(PLAYER *players,
               short totalPlayers,
               short *activePlayer)
{
    int validPlayers = 0;
    int playerToMove = (*activePlayer);
    for(int i = 0; i < totalPlayers; i++)
    {
        if(players[i].isPlaying)
        {
            validPlayers++;
        }
    }

    if(validPlayers)
    {
        do
        {
            if(++playerToMove >= totalPlayers)
            {
                playerToMove = 0;
            }
        }
        while(!players[playerToMove].isPlaying);
    }

    (*activePlayer) = playerToMove;

    return validPlayers;
}
