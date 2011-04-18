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
** Date Created: 2008/12/10
** Last Modified: 2008/12/12
** network.c
** Function definitions for the connect 4 game.
*/
#include "network.h"

BOOL AcceptClient(SOCKET listenSocket,
                  SOCKET **clientSockets,
                  struct sockaddr **clientinfo,
                  int *currentClients,
                  int *maxClients)
{
    struct sockaddr *newclientinfo;
    SOCKET *newClientSockets;

    if((*currentClients) >= (*maxClients))
    {
        newClientSockets = (SOCKET*)calloc((*maxClients) * 2, sizeof(SOCKET));
        if(newClientSockets)
        {
            memcpy(newClientSockets, (*clientSockets),
                   (*maxClients) * sizeof(SOCKET));
            free((*clientSockets));
            (*clientSockets) = newClientSockets;

            if((*clientinfo))
            {
                newclientinfo = (struct sockaddr*)calloc((*maxClients) * 2,
                                                   sizeof(struct sockaddr));

                if(newclientinfo)
                {
                    memcpy(newclientinfo, (*clientinfo),
                           (*maxClients) * sizeof(struct sockaddr));
                    free((*clientinfo));
                    (*clientinfo) = newclientinfo;
                }
                else
                {
                    free((*clientinfo));
                    (*clientinfo) = NULL;
                }
            }

            (*maxClients) *= 2;
        }
    }

    if((*currentClients) < (*maxClients))
    {
        if((*clientinfo))
        {
            int addrlen = sizeof(struct sockaddr);
            (*clientSockets)[(*currentClients)] = accept(listenSocket,
                                    &(*clientinfo)[(*currentClients)],
                                                             &addrlen);
        }
        else
        {
            (*clientSockets)[(*currentClients)] = accept(listenSocket,
                                                         NULL, NULL);
        }

        if((*clientSockets)[(*currentClients)] != INVALID_SOCKET)
        {
            (*currentClients)++;
            return TRUE;
        }
    }

    return FALSE;
}

int KillClient(SOCKET *clientSockets,
               int *currentClients,
               int clientToKill)
{
    closesocket(clientSockets[clientToKill]);

    while(clientToKill < (*currentClients))
    {
        clientSockets[clientToKill] = clientSockets[clientToKill + 1];
        ++clientToKill;
    }

    clientSockets[clientToKill] = INVALID_SOCKET;
    return --(*currentClients);
}

int sendall(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 on failure, 0 on success
} 

