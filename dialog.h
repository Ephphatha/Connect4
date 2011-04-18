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
** Last Modified: 2008/12/12
** dialog.h
** Function declarations for the connect 4 game.
*/
#ifndef __dialog
#define __dialog
#include <windows.h>
#include "player.h"
#include "resource.h"

//Values for messages the application uses.
#define ADDPLAYER 0x8042
#define CLEARGAME 0x8084
#define HOSTGAME 0x80C6
#define JOINGAME 0x8108
#define INPUT 0x814A

INT_PTR CALLBACK DlgProcAddPlayer(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK DlgProcJoin(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK DlgProcHost(HWND, UINT, WPARAM, LPARAM);

#endif
