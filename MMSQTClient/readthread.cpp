#include "readthread.h"

ReadThread::ReadThread(MMSConnection* connection, QObject *parent) :
    QThread(parent)
{
    m_connection = connection;
}

void ReadThread::run(){
    SOCKET connection_socket = m_connection->getSocket();
    SOCKET highest_socket;

    fd_set sockets_to_read;
    FD_ZERO(&sockets_to_read);
    while(m_connection->isConnected()){
        FD_SET(connection_socket, &sockets_to_read);
        highest_socket = connection_socket + 1;

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int ports_waiting = select(highest_socket, &sockets_to_read, NULL, NULL, &timeout);
        if(ports_waiting > 0){
            if(FD_ISSET(connection_socket, &sockets_to_read)){
                m_connection->handleRead();
            }
        }
    }
}
