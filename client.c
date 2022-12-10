#include <arpa/inet.h>
#include <netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include <string.h>

#define SERVER_PORT 80 // port implicit la http
// Conectarea la website
int conectare_website(char* adresa){
   int client_socket = -1;
   struct sockaddr_in6 server_addr;
   int rezultat;

   client_socket = socket(AF_INET6,SOCK_STREAM,IPPROTO_TCP);
   if(client_socket == -1){
    perror("socket()"); // eroare la creare socket;
    return -1;
   }
   server_addr.sin6_family=AF_INET6;
   printf("Socket creat\n");
   inet_pton(AF_INET6,adresa,&server_addr.sin6_addr);//Conexiunea la adresa ipv6
   server_addr.sin6_port = htons(SERVER_PORT);
   rezultat = connect(client_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(rezultat== -1){
        perror("connect()");
        close(client_socket);
        return -1;
    }else{
        printf("Conexiune realizata\n");
        }   
return client_socket;
}
//Trimitere mesaj HTTP

void trimitere_mesajhttp(int socket){
    char mesaj[] = "GET / HTTP/1.0\r\n\r\n ";
    if(send(socket,mesaj,sizeof mesaj,0)== -1){
        perror("Mesajul catre server web nu a fost trimis");
    }else{
        puts("Mesajul catre serverul web a fost trimis! ");
    }
}
//Primire pagina web
void primire_pagina(int intrare,int iesire){
    FILE *fisier=fopen("index.html","w");
    int primit;
    char buffer[1024];
    do{
        primit = recv(intrare,buffer,sizeof buffer,0);
         if ( primit> 0 ){
            fprintf(fisier,"%.*s", primit,buffer);
	     send(iesire,buffer,primit,0);
         printf("%.*s",primit,buffer); }
         else if(primit == 0)
        printf("Remote side has closed the connection\n");
         else
            perror("recv(sock_fd, recvbuffer, sizeof recvbuffer, 0)");
    }while(primit>0);
    fclose(fisier);
   
}

// Inchidere socket client
void close_socket_client(int socket){
    int var;
    var = close(socket);
    if(var==0){
printf("Connection close\n");
    }
    if(var==-1){
 perror("close(sock_fd)\n");
    }
}
