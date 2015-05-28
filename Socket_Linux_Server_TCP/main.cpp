#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include <unistd.h>     //fork close...
#include<sys/socket.h>
#include<netinet/in.h>

#define DEFAULT_PORT 8888
#define MAXLINE 4096

using namespace std;

int main()
{
    int socket_fd, connect_fd;
    struct sockaddr_in server_addr;
    char buff[MAXLINE];
    int n;

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_fd == -1)
    {
        cerr<<"Failed to create socket!"<<endl;
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = DEFAULT_PORT;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //cout<<"IP ADDRESS: "<<INADDR_ANY<<endl;
    if(bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        cerr<<"Failed to bind socket!"<<endl;
        return 0;
    }

    if(listen(socket_fd,10) == -1)
    {
        cerr<<"Failed to listen socket!"<<endl;
        return 0;
    }

    cout<<"-----------------Waiting for Clients' Call.-----------------"<<endl;

    while(1){
    //阻塞直到有客户端连接，不然多浪费CPU资源。
        if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            cerr<<"Failed to accept socket."<<endl;;
            continue;
        }
        //接受客户端传过来的数据
        n = recv(connect_fd, buff, MAXLINE, 0);
        //向客户端发送回应数据
        if(!fork())
        { /*紫禁城*/
            if(send(connect_fd, "Hello,you are connected!\n", 26,0) == -1)
            perror("send error");
            close(connect_fd);
            exit(0);
        }
        buff[n] = '\0';
        cout<<"recv msg from client: "<<buff<<endl;
        close(connect_fd);
    }
    close(socket_fd);
    return 0;

}
