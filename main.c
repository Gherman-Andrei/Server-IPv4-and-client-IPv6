#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include "server.c"
#include "client.c"

#define COMMAND "14#"
#define PORT 22114
#define ADRESA "2001:1430:a:50::50"
// Adresa IPv6 2001:1430:a:50::50 - axu.tm

int main(){
    char buffer[1024];
    //Deschidere socket 
    int server_socket;
    int primit;
    server_socket = deschidere_socket_server();
    //Bind intre socket si port
    bind_server(server_socket,PORT);
    //Listen
    ascultare(server_socket);
        
    while(1){
        //Acceptare conexiune
        int accept = acceptare_conexiune(server_socket);
        if(accept == 0){
            continue;  
        }
        do{
            primit = recv(accept,buffer,sizeof buffer,0);
            if(!verificare(buffer, COMMAND)){
                puts("Comanda neimplementata");
                send(accept,"Comanda neimplementata\n",100,0);
                continue;
            }else if (primit>0){
                puts("Comanda implementata");
                int client = conectare_website(ADRESA); // Conectare website
                trimitere_mesajhttp(client);//Trimitere mesaj http pentru cerere pagina web
                primire_pagina(client,accept);//Primire pagina web si salvare intr-un fisier index.html
                close_socket_client(client);
           }
        }while(primit>0);  
        close(accept);
        puts("Client deconectat");      
    } 
        close(server_socket);
        puts("Socket server inchis");
        return 0;
}
