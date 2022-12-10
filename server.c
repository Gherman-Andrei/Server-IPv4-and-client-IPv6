#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

//Functie pentru deschiderea socket
int deschidere_socket_server(){
	int socketdeschis ;
	const int pornit = 1;
	
	//Deschidere socket server
	//AF_INET - IPv4
	//SOCK_STREAM - conectare TCP

	socketdeschis = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(socketdeschis == -1){
		perror("Nu a fost deschis socketul!");
		return 0;
	}
	puts("Socket deschis");

	//Reutilizare port
	if(setsockopt(socketdeschis,SOL_SOCKET,SO_REUSEPORT,&pornit,sizeof(pornit))<0){
		perror("Eroare la reutilizarea portului");
		return 0;
	}
	return socketdeschis;
}
// Sfarsitul functiei

//Functie pentru BIND SERVER SOCKET

void bind_server(int socket , int port ){
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;//IPV4
	server_addr.sin_port = htons(port);//port
	server_addr.sin_addr.s_addr = INADDR_ANY; //selectare automata a interfacei
	
	if(bind(socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		perror("Bind fail");
		exit(1);
	}else{
		printf("Bind creeat pe portul:%d\n",port);
	}
}//Sfarsit functie bind
 
//Functia LISTEN

void ascultare(int socket){
	//listen(socket pe care asculta , numarul maxim de clienti in coada de asteptare )
	//Daca coada de asteptare este umpluta , se primeste "Connection refused"
	//folosim variabila coada pentru setarea unui numar maxim de clienti in asteptare
	//Folosesc index din documentatie , 10 
	if(listen(socket,10)== -1){
		perror("Eroare la asteptarea clientilor");
	}else{
		puts("\n Asteptare clienti");
	}
}//Sfarsit funtie LISTEN

//Functie pentru accept

int acceptare_conexiune(int socket){
	struct sockaddr_in client_addr;
	socklen_t addr_size; 
	addr_size = sizeof(client_addr);//size of addr_size

	int acceptare_socket = accept(socket,(struct sockaddr*)&client_addr,&addr_size);
	if(acceptare_socket == -1){
		perror("Conexiunea NU a fost acceptata");
		send(acceptare_socket,"Conexiunea NU a fost acceptata!\n",100,0);
		// functia send trimite 
		return 0;
	}else{
		puts("Conexiune acceptata");
		return acceptare_socket;
	}

}

//Functie pentru verificare comanda

int verificare(char *buffer, char *command){

	if(buffer[0] == command[0] && buffer[1]==command[1] && buffer[2]=='#'){
		return 1;
	}else{
		return 0;
	}
}
