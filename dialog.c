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
** dialog.c
** Function definitions for the connect 4 game.
*/
#include "dialog.h"

INT_PTR CALLBACK DlgProcAddPlayer(HWND hwnd,
                                  UINT message,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    int red, green, blue;
    BOOL success = TRUE, last = TRUE;
    PLAYER newPlayer[1];
    switch(message)
    {
        case WM_INITDIALOG:
            return 1;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDCANCEL:
                    DestroyWindow(hwnd);
                    break;

                case IDC_BTNADDPLAYER:
                    memset(newPlayer, 0, sizeof(PLAYER));
                    red = (int)GetDlgItemInt(hwnd,
                                             IDC_COLOURRED,
                                             &last,
                                             FALSE);
                    success = success & last;

                    green = (int)GetDlgItemInt(hwnd,
                                               IDC_COLOURGREEN,
                                               &last,
                                               FALSE);
                    success = success & last;

                    blue = (int)GetDlgItemInt(hwnd,
                                              IDC_COLOURBLUE,
                                              &last,
                                              FALSE);
                    success = success & last;

                    if(!GetDlgItemText(hwnd, IDC_NAME, newPlayer->name, 20))
                    {
                        MessageBox(hwnd,
                                   "Please enter a name.",
                                   "Value Error",
                                   MB_OK | MB_ICONWARNING);
                    }
                    else if(!success ||
                            (red > 255) || (red < 0) ||
                            (green > 255) || (green < 0) ||
                            (blue > 255) || (blue < 0))
                    {
                        MessageBox(hwnd,
                                   "Colour values must be between 255 and 0",
                                   "Value Error",
                                   MB_OK | MB_ICONWARNING);
                    }
                    else
                    {
                        newPlayer->colour = RGB(red, green, blue);
                        newPlayer->isPlaying = TRUE;
                        newPlayer->isLocal = TRUE;
                        if(IsDlgButtonChecked(hwnd, IDC_CHKAI) == BST_CHECKED)
                        {
                            newPlayer->isAI = TRUE;
                        }
                        else
                        {
                            newPlayer->isAI = FALSE;
                        }
                        SendMessage(GetWindow(hwnd, GW_OWNER), ADDPLAYER,
                                    (WPARAM)newPlayer, 0);
                        DestroyWindow(hwnd);
                    }
                    break;
            }
            return 1;

        default:
            return 0;
    }
}

INT_PTR CALLBACK DlgProcJoin(HWND hwnd,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam)
{
    char ip[16], port[6];
    switch(message)
    {
        case WM_INITDIALOG:
            return 1;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDCANCEL:
                    DestroyWindow(hwnd);
                    break;

                case IDC_BTNCONNECT:
                    GetDlgItemText(hwnd, IDC_IPADDRESS, ip, 16);
                    GetDlgItemText(hwnd, IDC_CONNECTPORT, port, 6);
                    SendMessage(GetWindow(hwnd, GW_OWNER), JOINGAME,
                                (WPARAM)ip, (LPARAM)port);
                    DestroyWindow(hwnd);
                    break;
            }
            return 1;

        default:
            return 0;
    }
}

INT_PTR CALLBACK DlgProcHost(HWND hwnd,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam)
{
    char port[6];
    switch(message)
    {
        case WM_INITDIALOG:
            return 1;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDCANCEL:
                    DestroyWindow(hwnd);
                    break;

                case IDC_BTNHOST:
                    GetDlgItemText(hwnd, IDC_HOSTPORT, port, 6);
                    SendMessage(GetWindow(hwnd, GW_OWNER), HOSTGAME,
                                (WPARAM)port, 0);
                    DestroyWindow(hwnd);
                    break;
            }
            return 1;

        default:
            return 0;
    }
}
