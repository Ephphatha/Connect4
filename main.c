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
** Date Created: 2008/11/17
** Last Modified: 2008/12/12
** main.c
** A connect 4 game.
*******************************************************************************
** CHANGELOG:                                                                **
** Date:      Notes:                                                         **
** 2008/11/17 -Created the file and wrote a todolist.                        **
** 2008/11/18 -Implemented the gamestate, added paint logic. Player pieces   **
**             are just ellipses with no padding. No game logic as of yet.   **
** 2008/11/19 -Removed a few magic numbers.                                  **
**            -Now paints with space between pieces and a border.            **
**            -Input handled correctly.                                      **
**            -GameState updates, no win conditions programmed yet though.   **
**            -Now detects a win and displays a messagebox.                  **
**            -ClearGame() function added.                                   **
** 2008/11/20 -Created a menu and join game dialog. Doesn't actually do      **
**             anything yet, but at least it looks nice. Base DialogProc is  **
**             implemented.                                                  **
** 2008/11/21 -Reworked the input handling functions to enable me to check   **
**             input against what was expected. Should be useful over network**
**             games if a packet is lost or duplicated.                      **
**            -On that topic, now ready to do networking. Just need to       **
**             implement support for sockets and all the jazz that goes with.**
** 2008/11/24 -About ready to start coding the network support. Tester code  **
**             for initialising sockets in there somewhere, now I just need  **
**             to work on making it useful...                                **
** 2008/12/02 -I think I have successfully initalised the client side socket,**
**             just need to work on the server side code and then I can test **
**             sending data.                                                 **
**            -I think the serverside code is done. Currently no protection  **
**             against connecting to multiple hosts, and no support for      **
**             multiple clients...                                           **
** 2008/12/06 -Player struct created, handling of players changed.           **
**            -Console window added for debugging.                           **
**            -The client side code is complete save for sending data. Server**
**             side code just needs to accept() connections and then I can   **
**             work on data transfer.                                        **
** 2008/12/07 -Trialing sending and recieving data. Don't be suprised if this**
**             breaks something, but it is 3am...                            **
**            -4AM UPDATE! Ding ding ding! Not working. The server is        **
**             erroring with the WSANOTSOCKET error (or words to that effect)**
**             The client still appears to be recieving data, but it's       **
**             either mangled or not really arriving, just the server program**
**             is causing the client to hang.                                **
**            -Deleted the code I was using to test. I was sending a char    **
**             array I had stuffed ints into by bit shifting them... Wasn't  **
**             exactly the best solution. Now there's a proper union set up. **
** 2008/12/08 -Fixed the socket error I was getting. I think I was           **
**             overwriting a valid socket with an improper call to accept(). **
**             need to work on how I send and receive data, it appears to be **
**             getting malformed.                                            **
** 2008/12/09 -Still not receiving data properly. Fixed thanks to IceKarma of**
**             #xkcd. turns out my packet was 17 bytes long and the last byte**
**             was being truncated.                                          **
** 2008/12/10 -Implemented the AddPlayer dialog. Added an AddPlayerFromStruct**
**             function. Should make things easier.                          **
** 2008/12/11 -Client socket array should now handle a client in the middle  **
**             of the array closing his connection.                          **
** 2008/12/12 -Renamed a few functions and constants.                        **
*******************************************************************************
** BUGLIST:                                                                  **
** ID:     Description:                                                      **
** BUG 001 FIXED 2008/11/19                                                  **
**         -Clicking left of or above the grid is being detected as a click  **
**          in the first column/row.                                         **
** BUG 002 FIXED 2008/11/19                                                  **
**         -Win isn't being reported until a run is longer than 4 units.     **
** BUG 003 FIXED 2008/12/06                                                  **
**         -Stack corruption when adding the second player.                  **
** BUG 004 FIXED 2008/12/08                                                  **
**         -Client sockets aren't being created properly when the host       **
**          accept()s them. Error 10038, WSANOTSOCK                          **
** BUG 005 FIXED 2008/12/08                                                  **
**         -Y co-ordinate is not being sent correctly.                       **
*******************************************************************************
** TODOLIST: Search for TODO for more items.                                 **
** TODO: Customisable player pieces. Icons or Bitmaps?                       **
** TODO: Resizable gamestate.                                                **
** TODO: Modify the client code to detect a serverside close and kill the    **
**       receive timer.                                                      **
** TODO: Modify the client and server code to acknowledge connections, make  **
**       the client wait till the server acknowledges a connection. Graceful **
**       abortion if it doesn't receive reciprocation in appropriate time.   **
**       Send a list of players to the client, including themselves. Client  **
**       should free their player list and use this one instead.             **
** TODO: Modify the packet struct to allow the gamestate to be sent when the **
**       client joins a game in progress. (Including players)                **
** TODO: Move the network code to seperate files.                            **
** TODO: AI.                                                                 **
** ************************************************************************* **
** DONE: Modify network code to only send packets when a network connection  **
**       exists.                                                             **
** DONE: Modify server code to restructure the array when a client           **
**       disconnects and kill the timer if neccessary.                       **
** DONE: Networking. Default to port 52045.                                  **
**       Previous idea was shit. Now just using a non blocking call to recv()**
**       for each client connection.                                         **
** DONE: Seperate functions into files.                                      **
** DONE: Paint logic. Add a buffer zone around the game grid, between pieces.**
**       There should be gridWidth - 1 spacers between pieces horizontally,  **
**       gridHeight - 1 spacers vertically. Perhaps draw circles smaller than**
**       they should be?                                                     **
** DONE: Handle input.                                                       **
** DONE: Collision detection.                                                **
** DONE: Game logic.                                                         **
*******************************************************************************
*/
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN //Excludes winsock.h from windows.h (and more...)
#endif

#define WINVER 0x0501       //Minimum required version of windows to run the
#define _WIN32_WINNT 0x0501 //compiled program is XP.
//#define _WIN32_IE 0x0400 //Uncomment if needed for INITCOMMONCONTROLSEX

#include <windows.h>
#include <stdio.h>  //Needed for printf, sprintf
#include <stdlib.h> //Needed for malloc, free. (skipped by lean and mean)

#include <commctrl.h>
#include <math.h>

//For console output (used for debugging)
#ifdef _DEBUG
#include <fcntl.h>
#include <io.h>
#endif

//Needed for network support
#include <Winsock2.h>
#include <Ws2tcpip.h>

//Local includes
#include "game.h"
#include "dialog.h"
#include "network.h"
#include "player.h"
#include "resource.h"

#define ACCEPTTIMER 2
#define RECEIVETIMER 4

#pragma pack(push,1)
typedef struct _c4packet
{
    struct
    {
        int len;
        char type;
    } header;

    union
    {
        struct
        {
            short player;
            int x;
            int y;
        } input;

        PLAYER player;
    } body;
} C4PACKET;
#pragma pack(pop)

//Declared only to stop DevC++ complaining. Is actually a function from stdio.h
#ifdef _DEBUG
FILE *_fdopen(int handle, const char *mode);
#endif

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hGlobalInstance = NULL;

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nFunsterStil)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;
    INITCOMMONCONTROLSEX iccx;

    hGlobalInstance = hThisInstance;

    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_INTERNET_CLASSES;
    InitCommonControlsEx(&iccx); //TODO: Check if this fails, disable dialogs

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "WindowsApp";
    wincl.lpfnWndProc = WndProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if(!RegisterClassEx(&wincl))
    {
        return 0;
    }

    hwnd = CreateWindowEx(
           0,
           "WindowsApp",
           "Connect 4",
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           640,
           480,
           HWND_DESKTOP,
           LoadMenu(hThisInstance, MAKEINTRESOURCE(IDR_MENU)),
           hThisInstance,
           NULL
           );

    ShowWindow(hwnd, nFunsterStil);

    while(GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return (int)messages.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //Gamestate specific variables
    static short **gameState = NULL;
    static int gridWidth = 7, gridHeight = 6;
    static PLAYER *players = NULL;
    static short totalPlayers = 0, maxPlayers = 0, activePlayer = 0;

    //Variables dealing with painting
    HDC hdc = NULL, hdcMem = NULL;
    HBITMAP bitmap = NULL;
    HBRUSH brush = NULL, initialBrush = NULL;
    PAINTSTRUCT ps;

    //Variables dealing with both painting and input
    static RECT clientRect;
    static int diameter = 0, spacing = 0, border = 0;

    //Variables dealing with input
    POINT mouseClick = {0, 0};
    static HWND hwndDialog = NULL;
    int filledRow = 0;

    //Text buffer to be used in various places
    static char* buffer = NULL;

    //Network specific variables.
    //For winsock initialisation
    static WSADATA wsaData;
    static BOOL wsaSucceeded = FALSE;
    //Common to server and client code
    int status = 0;
    static struct addrinfo *servinfo = NULL, hints; //May not need to be static
    C4PACKET packet;
    int bytesSent = 0, bytesReceived = 0;
    //Specific to client code
    static SOCKET connectSocket = INVALID_SOCKET;
    //Specific to server code
    static SOCKET listenSocket = INVALID_SOCKET;
    static SOCKET *clientSockets = NULL;
    static struct sockaddr *clientinfo = NULL;
    static int currentClients = 0, maxClients = 0;

    //Debug specific variables.
    #ifdef _DEBUG
    int hConHandle;
    long lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;
    #endif

    switch(message)
    {
        case WM_CREATE:
            #ifdef _DEBUG
            // allocate a console for this app
            AllocConsole();

            // set the screen buffer to be big enough to let us scroll text
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                       &coninfo);
            coninfo.dwSize.Y = 500;
            SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
                                       coninfo.dwSize);

            // redirect unbuffered STDOUT to the console
            lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
            hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
            fp = _fdopen(hConHandle, "w");
            *stdout = *fp;

            setvbuf(stdout, NULL, _IONBF, 0);
            #endif

            if((gameState = (short**)calloc(gridWidth, sizeof(short*))))
            {   //Double bagging my assignments to get gcc to stop warning me
                //about it. Fucking compiler thinks that I might be using this
                //as a comparison and therefore I need to be coddled...
                for(int i = 0; i < gridWidth; i++)
                {
                    if(!(gameState[i] = (short*)calloc(gridHeight,
                                                       sizeof(short))))
                    {
                        for(int j = 0; j < i; j++)
                        {
                            free(gameState[j]);
                        }
                        free(gameState);
                        MessageBox(hwnd, "Couldn't create the gamestate.",
                                   "Out Of Memory Error",
                                   MB_OK | MB_ICONWARNING);
                        PostQuitMessage(-1);
                        return 0;
                    }
                }

                #ifdef _DEBUG
                printf("Gamestate initialised.\n");
                #endif
            }
            else
            {
                MessageBox(hwnd, "Couldn't create the gamestate.",
                           "Out Of Memory Error",
                           MB_OK | MB_ICONWARNING);
                PostQuitMessage(-1);
                return 0;
            }

            if((players = (PLAYER*)calloc(4, sizeof(PLAYER))))
            {
                #ifdef _DEBUG
                printf("Player array initialised.\n");
                #endif

                maxPlayers = 4;
            }
            else
            {
                MessageBox(NULL, "Couldn't create the player array.",
                           "Out Of Memory Error", MB_OK | MB_ICONWARNING);

                for(int i = 0; i < gridWidth; i++)
                {
                    free(gameState[i]);
                }
                free(gameState);
                PostQuitMessage(-1);
                return 0;
            }

            if(!AddPlayer(&players, &totalPlayers, &maxPlayers,
                          "Andrew", 0x0000FF00, TRUE, TRUE, FALSE))
            {
                #ifdef _DEBUG
                printf("Couldn't add the player.");
                #endif
            }
            #ifdef _DEBUG
            else
            {
                printf("Added player %s, total player count is %d\n",
                       players[0].name, totalPlayers);
            }
            #endif

            if(!AddPlayer(&players, &totalPlayers, &maxPlayers,
                          "Buddy", 0x0000FFFF, TRUE, TRUE, FALSE))
            {
                #ifdef _DEBUG
                printf("Couldn't add the player.");
                #endif
            }
            #ifdef _DEBUG
            else
            {
                printf("Added player %s, total player count is %d\n",
                       players[1].name, totalPlayers);
            }
            #endif

            GetClientRect(hwnd, &clientRect);
            diameter = min(clientRect.right / (gridWidth + 1),
                           clientRect.bottom / (gridHeight + 1));
            spacing = diameter * 0.1;
            border = diameter * 0.5;

            if(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
            {
                #ifdef _DEBUG
                printf("Winsock initialised.\n");
                #endif

                if((LOBYTE(wsaData.wVersion) != 2) ||
                   (HIBYTE(wsaData.wVersion) != 2))
                {
                    #ifdef _DEBUG
                    printf("Wrong winsock version installed.\n");
                    printf("Requested 2.2, recieved %d.%d.\n",
                           LOBYTE(wsaData.wVersion),
                           HIBYTE(wsaData.wVersion));
                    #endif

                    WSACleanup();
                }
                else
                {
                    #ifdef _DEBUG
                    printf("Correct version of winsock installed.\n");
                    #endif

                    wsaSucceeded = TRUE;
                    if((clientSockets = (SOCKET*)calloc(2, sizeof(SOCKET))))
                    {
                        #ifdef _DEBUG
                        printf("Allocated memory for client sockets.\n");
                        #endif
                        maxClients = 2;

                        if((clientinfo = (struct sockaddr*)calloc(2,
                                        sizeof(struct sockaddr))))
                        {
                            #ifdef _DEBUG
                            printf("Allocated memory for the clientinfo ");
                            printf("struct.\n");
                            #endif
                        }
                        #ifdef _DEBUG
                        else
                        {
                            printf("Unable to allocate memory for the client");
                            printf("info struct, continuing regardless.\n");
                        }
                        #endif
                    }
                    #ifdef _DEBUG
                    else
                    {
                        //TODO: Grey out host game option.
                        printf("Unable to allocate memory for the clients.\n");
                    }
                    #endif
                }
            }
            #ifdef _DEBUG
            else
            {
                //TODO: Grey out menu options dealing with network support.
                printf("WSAStartup failed.\n");
            }
            #endif
            return 0;

        case WM_SIZE:
            GetClientRect(hwnd, &clientRect);
            diameter = min(clientRect.right / (gridWidth + 1),
                           clientRect.bottom / (gridHeight + 1));
            spacing = diameter * 0.1;
            border = diameter * 0.5;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_LBUTTONDOWN:
            if(players[activePlayer].isLocal && !players[activePlayer].isAI)
            {
                mouseClick.x = floor((double)(LOWORD(lParam) - border)
                                     / diameter);
                mouseClick.y = floor((double)(HIWORD(lParam) - border)
                                     / diameter);

                if((mouseClick.x >= 0) && (mouseClick.x < gridWidth) &&
                   (mouseClick.y >= 0) && (mouseClick.y < gridHeight))
                {
                    PostMessage(hwnd, INPUT, (WPARAM)mouseClick.x,
                                (LPARAM)(-1));
                }

                #ifdef _DEBUG
                printf("Mouseclick registered, x: %ld, y: %ld.\n",
                        mouseClick.x, mouseClick.y);
                #endif

                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;

        case WM_COMMAND:
            switch(wParam)
            {
                case IDM_EXIT:
                    PostMessage(hwnd, WM_DESTROY, 0, 0);
                    break;

                case IDM_ADDPLAYER:
                    if(!IsWindow(hwndDialog))
                    {
                        hwndDialog = CreateDialog(hGlobalInstance,
                                                  MAKEINTRESOURCE(
                                                            IDD_ADDPLAYER),
                                                  hwnd,
                                                  (DLGPROC)DlgProcAddPlayer);
                        ShowWindow(hwndDialog, SW_SHOW);
                    }
                    break;

                case IDM_CONNECT:
                    if(!IsWindow(hwndDialog))
                    {
                        hwndDialog = CreateDialog(hGlobalInstance,
                                                  MAKEINTRESOURCE(IDD_CONNECT),
                                                  hwnd,
                                                  (DLGPROC)DlgProcJoin);
                        ShowWindow(hwndDialog, SW_SHOW);
                    }
                    break;

                case IDM_HOSTGAME:
                    if(!IsWindow(hwndDialog))
                    {
                        hwndDialog = CreateDialog(hGlobalInstance,
                                                  MAKEINTRESOURCE(IDD_HOST),
                                                  hwnd,
                                                  (DLGPROC)DlgProcHost);
                        ShowWindow(hwndDialog, SW_SHOW);
                    }
                    break;
            }
            return 0;

        case WM_TIMER:
            if(wParam == ACCEPTTIMER)
            {
                if(AcceptClient(listenSocket, &clientSockets, &clientinfo,
                                &currentClients, &maxClients))
                {
                    SetTimer(hwnd, RECEIVETIMER, 100, NULL);
                }
            }
            else if(wParam == RECEIVETIMER)
            {
                if(connectSocket != INVALID_SOCKET)
                {
                    bytesReceived = recv(connectSocket, (char*)&packet,
                                         sizeof(C4PACKET), 0);

                    if(bytesReceived == 0)
                    {
                        #ifdef _DEBUG
                        printf("Connection closed.\n");
                        #endif
                        closesocket(connectSocket);
                        connectSocket = INVALID_SOCKET;
                    }
                    #ifdef _DEBUG
                    else if(bytesReceived == SOCKET_ERROR)
                    {
                        printf("recv failed on client with error: %d\n",
                               WSAGetLastError());
                    }
                    #endif
                }
                else if(currentClients)
                {
                    for(int i = 0; i < currentClients; i++)
                    {
                        bytesReceived = recv(clientSockets[i], (char*)&packet,
                                             sizeof(C4PACKET), 0);

                        if(bytesReceived > 0)
                        {
                            #ifdef _DEBUG
                            printf("Packet recieved from client %d.\n", i);
                            #endif
                            //TODO: Rewrite this function to PostMessage(INPUT)
                            //as soon as the packet arrives from each client.
                            i = currentClients;
                        }
                        else if(bytesReceived == 0)
                        {
                            #ifdef _DEBUG
                            printf("Client %d connection closed.\n", i);
                            #endif
                            if(!KillClient(clientSockets, &currentClients, i))
                            {
                                KillTimer(hwnd, RECEIVETIMER);
                            }
                        }
                        #ifdef _DEBUG
                        else if(bytesReceived == SOCKET_ERROR)
                        {
                            printf("recv failed on client %d with error: %d\n",
                                   i, WSAGetLastError());
                        }
                        #endif
                    }
                }

                if(bytesReceived > 0)
                {
                    #ifdef _DEBUG
                    printf("Bytes recieved: %d, expected: %d.\n",
                           bytesReceived, packet.header.len);
                    #endif

                    if(bytesReceived == packet.header.len)
                    {
                        #ifdef _DEBUG
                        printf("Received packet contents:\n");
                        printf("header.type: %c\n", packet.header.type);
                        printf("header.len: %d\n", packet.header.len);
                        printf("body.input.player: %d\n",
                               packet.body.input.player);
                        printf("body.input.x: %d\n",
                               packet.body.input.x);
                        printf("body.input.y: %d\n",
                               packet.body.input.y);
                        #endif

                        PostMessage(hwnd, INPUT,
                                    (WPARAM)packet.body.input.x,
                                    (LPARAM)packet.body.input.y);
                    }
                }
            }
            return 0;

        case ADDPLAYER:
            AddPlayerFromStruct(&players, &totalPlayers, &maxPlayers,
                                (PLAYER*)wParam);
            return 0;

        case CLEARGAME:
            ClearGame(gameState, gridWidth, gridHeight, &activePlayer);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case JOINGAME:
            if(wsaSucceeded)
            {
                memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;

                if((status = getaddrinfo((char*)wParam, (char*)lParam,
                                            &hints, &servinfo)) == 0)
                {
                    if(listenSocket != INVALID_SOCKET)
                    {   //For the moment you can't host a game and be a client.
                        #ifdef _DEBUG
                        printf("Closing old listen socket.\n");
                        #endif
                        closesocket(listenSocket);
                        listenSocket = INVALID_SOCKET;
                    }

                    if(connectSocket != INVALID_SOCKET)
                    {
                        #ifdef _DEBUG
                        printf("Closing old connect socket.\n");
                        #endif
                        closesocket(connectSocket);
                        connectSocket = INVALID_SOCKET;
                    }

                    connectSocket = socket(servinfo->ai_family,
                                           servinfo->ai_socktype,
                                           servinfo->ai_protocol);

                    if(connectSocket == INVALID_SOCKET)
                    {
                        #ifdef _DEBUG
                        printf("socket() failed: %d\n", WSAGetLastError());
                        printf("Freeing addressinfo.\n");
                        #endif
                        freeaddrinfo(servinfo);
                    }
                    else
                    {
                        if(connect(connectSocket, servinfo->ai_addr,
                                   servinfo->ai_addrlen) == SOCKET_ERROR)
                        {
                            #ifdef _DEBUG
                            printf("connect() failed: %d\n",
                                   WSAGetLastError());
                            printf("Closing connect socket.\n");
                            #endif
                            closesocket(connectSocket);
                            connectSocket = INVALID_SOCKET;
                            #ifdef _DEBUG
                            printf("Freeing addressinfo.\n");
                            #endif
                            freeaddrinfo(servinfo);
                        }
                        else
                        {
                            #ifdef _DEBUG
                            printf("Connected on IP: %s, Port: %s\n",
                                   (char*)wParam, (char*)lParam);
                            #endif
                            u_long mode = 1;
                            ioctlsocket(connectSocket, FIONBIO, &mode);
                            SetTimer(hwnd, RECEIVETIMER, 1000, NULL);
                        }
                    }
                }
                else
                {
                    #ifdef _DEBUG
                    printf("getaddrinfo failed: %d\n", status);
                    #endif
                }
            }
            return 0;

        case HOSTGAME:
            if(wsaSucceeded)
            {
                memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;
                hints.ai_flags = AI_PASSIVE;

                if((status = getaddrinfo(NULL, (char*)wParam,
                                         &hints, &servinfo)) == 0)
                {
                    if(connectSocket != INVALID_SOCKET)
                    {   //Can't be a client and host a game. May change the
                        //behaviour at a later date.
                        #ifdef _DEBUG
                        printf("Closing old connect socket.\n");
                        #endif
                        closesocket(connectSocket);
                        connectSocket = INVALID_SOCKET;
                    }

                    if(listenSocket != INVALID_SOCKET)
                    {
                        #ifdef _DEBUG
                        printf("Closing old listen socket.\n");
                        #endif
                        closesocket(listenSocket);
                        listenSocket = INVALID_SOCKET;
                    }

                    listenSocket = socket(servinfo->ai_family,
                                          servinfo->ai_socktype,
                                          servinfo->ai_protocol);

                    if(listenSocket == INVALID_SOCKET)
                    {
                        #ifdef _DEBUG
                        printf("socket() failed: %d\n", WSAGetLastError());
                        #endif
                        freeaddrinfo(servinfo);
                    }
                    else
                    {
                        if(bind(listenSocket, servinfo->ai_addr,
                                servinfo->ai_addrlen) != 0)
                        {
                            #ifdef _DEBUG
                            printf("bind() failed: %d\n", WSAGetLastError());
                            printf("Closing the listensocket.\n");
                            #endif
                            closesocket(listenSocket);
                            listenSocket = INVALID_SOCKET;
                            #ifdef _DEBUG
                            printf("Freeing addressinfo.\n");
                            #endif
                            freeaddrinfo(servinfo);
                        }
                        else
                        {
                            #ifdef _DEBUG
                            printf("Bound to port: %s\n", (char*)wParam);
                            #endif
                            if(listen(listenSocket, 5) == SOCKET_ERROR)
                            {
                                #ifdef _DEBUG
                                printf("listen() failed: %d\n",
                                       WSAGetLastError());
                                printf("Closing the listensocket.\n");
                                #endif

                                closesocket(listenSocket);
                                listenSocket = INVALID_SOCKET;

                                #ifdef _DEBUG
                                printf("Freeing addressinfo.\n");
                                #endif

                                freeaddrinfo(servinfo);
                            }
                            else
                            {
                                #ifdef _DEBUG
                                printf("Listening on port: %s\n",
                                       (char*)wParam);
                                #endif

                                u_long mode = 1;
                                ioctlsocket(listenSocket, FIONBIO, &mode);
                                SetTimer(hwnd, ACCEPTTIMER, 5000, NULL);
                            }
                        }
                    }
                }
                #ifdef _DEBUG
                else
                {
                    printf("getaddrinfo failed: %d\n", status);
                }
                #endif
            }
            return 0;

        case INPUT:
            filledRow = DoInput(gameState, gridWidth, gridHeight,
                                (int)wParam, activePlayer + 1);

            if(!(((int)lParam == -1) && (filledRow == (int)lParam)) && 
                (((int)lParam == -1) || (filledRow == (int)lParam)))
            {
                if(((int)lParam == -1) && ((connectSocket != INVALID_SOCKET) ||
                                            currentClients))
                {
                    packet.header.type = 'I';
                    packet.header.len = sizeof(packet.header)
                                      + sizeof(packet.body.input);
                    packet.body.input.player = activePlayer;
                    packet.body.input.x = (int)wParam;
                    packet.body.input.y = filledRow;

                    #ifdef _DEBUG
                    printf("Sending packet contents:\n");
                    printf("header.type: %c\n", packet.header.type);
                    printf("header.len: %d\n", packet.header.len);
                    printf("body.input.player: %d\n",
                           packet.body.input.player);
                    printf("body.input.x: %d\n",
                           packet.body.input.x);
                    printf("body.input.y: %d\n",
                           packet.body.input.y);
                    #endif

                    if(connectSocket != INVALID_SOCKET)
                    {
                        #ifdef _DEBUG
                        printf("Attempting to send to host.\n");
                        #endif

                        bytesSent = send(connectSocket, (char*)&packet,
                                         packet.header.len, 0);

                        #ifdef _DEBUG
                        if(bytesSent == SOCKET_ERROR)
                        {
                            printf("send() failed with error: %d\n",
                                   WSAGetLastError());
                        }
                        else
                        {
                            printf("Sent %d of %d bytes to host.\n",
                                   bytesSent, packet.header.len);
                        }
                        #endif
                    }
                    else if(currentClients)
                    {
                        for(int i = 0; i < currentClients; i++)
                        {
                            #ifdef _DEBUG
                            printf("Attempting to send to client %d.\n", i);
                            #endif

                            bytesSent = send(clientSockets[i], (char*)&packet,
                                             packet.header.len, 0);

                            #ifdef _DEBUG
                            if(bytesSent == SOCKET_ERROR)
                            {
                                printf("send() failed with error: %d\n",
                                       WSAGetLastError());
                            }
                            else
                            {
                                printf("Sent %d of %d bytes to client.\n",
                                       bytesSent, packet.header.len);
                            }
                            #endif
                        }
                    }
                }

                if(CheckRuns(gameState, gridWidth, gridHeight,
                             (int)wParam, filledRow) >= 4)
                {
                    buffer = (char*)calloc(64, sizeof(char));
                    sprintf(buffer, "%s wins!", players[activePlayer].name);
                    MessageBox(hwnd, buffer, "Congratulations!", MB_OK);
                    free(buffer);
                    buffer = NULL;
                    PostMessage(hwnd, CLEARGAME, 0, 0);
                }
                else
                {
                    NextPlayer(players, totalPlayers, &activePlayer);

                    #ifdef _DEBUG
                    printf("%s to move. Player %d out of %d.\n",
                            players[activePlayer].name,
                            activePlayer + 1, totalPlayers);
                    #endif

                }
            }
            else
            {
                #ifdef _DEBUG
                printf("GameState out of sync. ");
                printf("Expected to fill row %d, actually filled row %d.\n",
                       (int)lParam, filledRow);
                #endif
                //Request or resend gamestate to clients.
            }
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            hdcMem = CreateCompatibleDC(hdc);
            bitmap = CreateCompatibleBitmap(hdc, clientRect.right,
                                            clientRect.bottom);
            SelectObject(hdcMem, bitmap);

            brush = CreateSolidBrush(0x00FFFFFF);
            FillRect(hdcMem, &clientRect, brush);
            DeleteObject(brush);

            for(int i = 0; i < gridWidth; i++)
            {
                for(int j = 0; j < gridHeight; j++)
                {
                    if(gameState[i][j])
                    {
                        brush = CreateSolidBrush(players[gameState[i][j]-1]
                                                        .colour);
                    }
                    else
                    {
                        brush = CreateSolidBrush(0x00FFFFFF);
                    }
                    int x = border + (i * diameter) + (spacing * 0.5);
                    int y = border + (j * diameter) + (spacing * 0.5);
                    initialBrush = (HBRUSH)SelectObject(hdcMem, brush);
                    Ellipse(hdcMem, x, y,
                            x + diameter - spacing,
                            y + diameter - spacing);
                    SelectObject(hdcMem, initialBrush);
                    DeleteObject(brush);
                }
            }

            BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom,
                   hdcMem, 0, 0, SRCCOPY);

            DeleteDC(hdcMem);
            DeleteObject(bitmap);
            EndPaint(hwnd, &ps);
			return 0;

        case WM_ERASEBKGND:
            return 1;

        case WM_DESTROY:
            for(int i = 0; i < gridWidth; i++)
            {
                free(gameState[i]);
            }
            free(gameState);

            free(players);

            if(buffer)
            {
                free(buffer);
            }

            freeaddrinfo(servinfo); //Free the linked-list

            if(listenSocket != INVALID_SOCKET)
            {
                closesocket(listenSocket);
            }

            if(connectSocket != INVALID_SOCKET)
            {
                closesocket(connectSocket);
            }

            if(wsaSucceeded)
            {
                WSACleanup();
            }

            if(clientSockets)
            {
                free(clientSockets);
            }

            if(clientinfo)
            {
                free(clientinfo);
            }

            KillTimer(hwnd, ACCEPTTIMER);
            KillTimer(hwnd, RECEIVETIMER);

            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
