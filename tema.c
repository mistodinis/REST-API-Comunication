#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#define HOST "3.8.116.10" // Hostul pe care lucram
#define PORT 8080	 // Portul pe care lucram



int main(int argc, char *argv[])
{	// trimit mesajul
    char *message;
    // primesc raspunsul
     char *response;
    // id socket
    int sockfd;
    // baza de date utilizatori
    char *users[20]; // neutilizat
    // parole
    char *parole[20]; // neutilizat
    // cati utilizatori sunt
    int counter_users = 0;
    // Cookie'uri
    char *cookies[2];
    
   // int counter_cookies = 0;
    // Token JWT
    char *jwt[2];

    int counter_jwt = 0;
    //Alocare memorie vectori
    for(int i = 0;i < 20;i++){
    	users[i] = calloc(30,sizeof(char));
    	parole[i] = calloc(30,sizeof(char));
    }
    //Alocare memorie cookies , jwt
    for(int i = 0;i < 2;i++){
    	cookies[i] = calloc(200,sizeof(char));
    	jwt[i] = calloc(500,sizeof(char));
    }

    char *aux = calloc(20, sizeof(char));
    //Tipul aplicatiei
	char* type = "application/json";
	// flag pentru a vedea daca este introdusa o comanda corect sau nu
	int corect;
    // Ciclul in care se desfasoara programul
	while(1){
		printf("-> ");

		corect = 1;
		//Citirea comenzii
		scanf("%s" , aux);
		//Verific daca aux are valoarea register
		if(strcmp("register" , aux) == 0){
			//Deschid conexiunea
			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			//Citest username si parola
			printf("username: ");
			scanf("%s" , users[counter_users]);

			printf("parola: ");
			scanf("%s" , parole[counter_users]);

			printf("\n");
			//Incrementez numarul de utilizator (Neutilizat)
			counter_users++;
			// Aloc memorie pentru stringul de tip json , si il parsez cu ajutorul compute
			char* data[1];

			for(int i =0 ; i < 1;i++)
				data[i] = malloc(100*sizeof(char));
			
			compute(data[0],"{\"username\":\"");
			compute(data[0], users[counter_users-1]);
			compute(data[0], "\",\"password\":\"");
			compute(data[0], parole[counter_users-1]);
			compute(data[0],"\"}");
			// Compun mesajul "mesage" cu ajutorul compute_post_request
			message = compute_post_request(HOST, "/api/v1/tema/auth/register", type, data, 1, NULL, 0);
			
			puts(message);
			//Comunic cu serverul si stochez raspunsul in "response"
			send_to_server(sockfd, message);
			
			response = receive_from_server(sockfd);
			
			char const s[2] = "\n";
			//Prelucrez outputul , in caz ca este in regula si in cazul in care utilizatorul exista
			if(response){
				char *tok = strtok(response, s);
		
				if(strstr(tok, "Bad")){
					compute(tok , "\nAlready existing this username");
				}
			
				puts(tok);
			}
			
			puts("");
			//Eliberez data
			free(*data);
			//Modific corect , inseamna ca aux a fost o comanda valida
			corect = 0;
			//Inchid conexiunea cu serverul
			close_connection(sockfd);

		}

		if(strcmp("login" , aux) == 0){
			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

			char* data[1];
			
			for(int i =0 ; i < 1;i++){
				data[i] = malloc(100*sizeof(char));
			}

			char *aux_user[1];
			for(int i =0;i < 2;i++){
				aux_user[i] = calloc(30 , sizeof(char));
			}

			printf("username: ");
			scanf("%s" , aux_user[0]);
			
			printf("parola: ");
			scanf("%s" , aux_user[1] );
			//printf("%s %s " , aux_user[0] , aux_user[1]);

			compute(data[0],"{\"username\":\"");
			compute(data[0], aux_user[0]);
			compute(data[0], "\",\"password\":\"");
			compute(data[0], aux_user[1]);
			compute(data[0],"\"}");

			message = compute_post_request(HOST , "/api/v1/tema/auth/login" , type , data , 1, NULL , 0);
			puts(message);
			
			send_to_server(sockfd,message);
			response= receive_from_server(sockfd);
 
			char *tok =  strtok(response , "\n");
			//Prelucrez outputul astfel incat sa afisez ce trebuie si sa obtin cookie'ul
			if(!strstr(tok, "Bad")){
				puts(tok);
			 	while(tok  != NULL ){
		
			 		tok = strtok(NULL,": ");

					if(strstr(tok , "connect")){
			 			cookies[0] = strtok(tok , ";" );
			 			
			 			//puts(cookies[0]);
			 			
			 			//counter_cookies++;
			 			
			 			break;
		 			}
			 	}
			}else{
				compute(tok, "\nUsername sau parola gresite!");
				
				puts(tok);
			}
			puts("");
			
			free(*data);
			free(*aux_user);
			
			corect = 0;
			
			close_connection(sockfd);
		}

		if(strcmp("enter_library" , aux) == 0){
			
			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

			message = compute_get_request(HOST, "/api/v1/tema/library/access" , NULL , cookies , 1 );
			
			puts(message);
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			
			char *tok = strtok(response , "\n");

			int flag = 0;
			// Prelucrez pentru a obtine jwt'ul si pentru a afisa outputul
			if(!strstr(tok,"Unauthorized")){
				puts(tok);

				 while(tok != NULL){
				 	tok = strtok(NULL,"{");
				 	
				 	if(strstr(tok,"token")){
				 		char *rez = strtok(tok, "\"");
				 		//puts(rez);
				 		while(rez != NULL){
				 			if(strlen(rez) > 100){
				 				strcpy(jwt[counter_jwt], rez);
				 				//puts(rez);
				 				//puts(jwt[counter_jwt]);

				 				counter_jwt++;

				 				flag = 1;

				 				break;

				 			}
				 			rez = strtok(NULL, "\"");
				 			//puts(rez);
				 		}
				 	}

				 	if(flag == 1)
				 		break;	
				
				}
			}else{
				compute(tok , "\nNu esti logat !");
				puts(tok);
			}
			puts("");
			corect = 0;
			close_connection(sockfd);

		}

		if(strcmp("get_books" , aux) == 0){

			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			//Folosirea unei functii compute_get_authorization pentru a putea folosi in input jwt'ul
			message = compute_get_authorization(HOST, "/api/v1/tema/library/books" , jwt , counter_jwt);
			puts(message);

			send_to_server(sockfd,message);
			response = receive_from_server(sockfd);

			char *tok = strtok(response, "\n");

			if(!strstr(tok, "Error")){
				puts(tok);
				while(tok != NULL){
					tok = strtok(NULL, "\n");

					if(strstr(tok , "[")){

						puts(tok);
						break;
					}
				}
			}else{
				compute(tok , "\nEroare la decodarea jwt'ului !");
				puts(tok);
			}

			puts("");
			
			corect = 0;
			
			close_connection(sockfd);
		}

		if(strcmp("add_book" , aux) == 0){
			
			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			//Alocare memorie pentru stringul de tip json si pentru variabila (char) pentru adaugare
			char* data[1];
			char* add = calloc(30,sizeof(char));
			for(int i =0 ; i < 1;i++){
				data[i] = malloc(150*sizeof(char));
			}
			//Adaugarea efectiva a datelor in stringul de tip json
			printf("title: ");
			scanf("%s" , add);
			compute(data[0], "{\"title\":\"");
			compute(data[0], add);

			printf("author: ");
			scanf("%s" , add);
			compute(data[0] , "\",\"author\":\"");
			compute(data[0] , add);

			printf("genre: ");
			scanf("%s", add);
			compute(data[0], "\",\"genre\":\"");
			compute(data[0], add);

			printf("page count: ");
			int j ; char str[10];
			scanf("%d" , &j);
			sprintf(str, "%d" , j);
			compute(data[0], "\",\"page_count\":");
			compute(data[0], str);

			printf("publisher: ");
			scanf("%s" , add);
			compute(data[0],",\"publisher\":\"");
			compute(data[0], add);
			compute(data[0], "\"}");

			 printf("\n");
			 //Folosirea unei alte functii pentru a asambla mesajul "message" input pentru server
			message = compute_post_addbook(HOST, "/api/v1/tema/library/books" , type , data , 1 , jwt , 1);
			puts(message);

			send_to_server(sockfd , message);
			response = receive_from_server(sockfd);
			
			char *tok = strtok(response, "\n");

			if(!strstr(tok , "Error")) {
			puts(tok);
			}else{
				if(strstr(tok, "Many")){


				compute(tok , "\nPrea multe cereri de creare , mai asteapta !");
				puts(tok);
				}else{
					compute(tok , "\nEroare la decodarea jwt'ului!");
					puts(tok);
				}
			}
			
			free(*data);
			
			corect = 0;
			
			close_connection(sockfd);

		}

		if(strcmp("get_book" , aux) == 0){

			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			//Citirea id'ului cartii respective
			printf("id= ");
			int id; scanf("%d" , &id);
			//Trecerea id'ului din int in string
			char str[12];
			sprintf(str, "%d" , id);
			//Asamblarea url'ului finall
			char url [80];
			strcpy(url , "/api/v1/tema/library/books/");
			compute(url, str);
			
			message = compute_get_authorization(HOST, url , jwt , 1);
			puts(message);

			send_to_server(sockfd,message);
			response = receive_from_server(sockfd);

			char *tok = strtok(response , "\n");

			if(strstr(tok, "500")){
				compute(tok , "\nEroare la decodarea jwt'ului !");
				
				puts(tok);
				//Folosirea lui goto , in cazul in care se afiseara eroare se sare peste urmatorul if
				goto jump;
			}
			if(!strstr(tok, "Found")){
				puts(tok);
				while(tok != NULL){
					tok = strtok(NULL, "\n");

					if(strstr(tok , "[")){

						puts(tok);
						break;
					}
				}
			}else{
				compute(tok , "\nNot book was found!");
				puts(tok);
			}
			puts("");
			jump: 
				puts("");

			corect = 0;	
			close_connection(sockfd);
		}

		if(strcmp("delete_book" , aux) == 0){
			//Acest if este aproape identic cu cel de deasupra , este modificata eticheta de la goto si unele mesaje in caz de eroare
			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

			printf("id= ");
			int id; scanf("%d" , &id);
			
			char str[12];
			sprintf(str, "%d" , id);

			char url [80];
			strcpy(url , "/api/v1/tema/library/books/");
			compute(url, str);
			message = delete_authorization(HOST, url , jwt  , 1);

			puts(message);

			send_to_server(sockfd,message);
			response = receive_from_server(sockfd);

			char *tok = strtok(response , "\n");

			if(strstr(tok, "500")){
				compute(tok , "\nNu puteti obtine detalii daca nu sunteti logat!");
				puts(tok);
				goto hop;
			}
			if(!strstr(tok, "Found")){
				puts(tok);
			
			}else{
				compute(tok , "\nNot book was deleted!");
				puts(tok);
			}
			puts("");
			hop:
				puts("");

			corect = 0;
			
			close_connection(sockfd);
		}

		if(strcmp("logout" , aux) == 0){

			sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

			message = compute_get_request(HOST, "/api/v1/tema/auth/logout" , NULL , cookies , 1 );
			puts(message);

			send_to_server(sockfd,message);
			response = receive_from_server(sockfd);

			char *tok = strtok(response, "\n");
			//Tratarea erorilor
			if(strstr(tok, "400")){
				compute(tok , "\nNu ai fost logat!");
				puts(tok);
			}else{
				puts(tok);
			}

			puts("");
			
			corect = 0;
			
			close_connection(sockfd);
		}
		// Se intra pe Help doar in cazul in care tastam "help" si sunt afisate toate comenzile posibile din cadrul comunicatiei
		if(strcmp("help", aux) == 0){
			printf("Comenzi posibile:\nregister - Inregistrare\nlogin - logare\nenter_library - Cerere acces la biblioteca\nget_books - Lista cu informatii sumare despre carti\nget_book - Informatii amanuntite despre o carte\nadd_book - Adaugarea unei carti\ndelete_book - Stergerea unei carti\nlogout - Delogare\nexit - Iesirea din Comunicatie\n ");
			
			corect = 0;
		}
		// Se iese din programul cu "exit"
		if(strcmp("exit" , aux) == 0){
			//close_connection(sockfd);
			exit(0);
		}
		// Daca cuvantul citit de la tastatura , aux, nu este nici unul din cele de deasupra , se afiseaza mesajul pentru a sti mai apoi sa se apeleze help pentru a obtine comenzile disponibile
		if(corect == 1){
			printf("Tasteaza \"help\" pentru mai multe informatii\n");
		}
	}


    return 0;
}
