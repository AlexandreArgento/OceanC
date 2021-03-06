#include "server.h"
 
int listener_d;
ocean *oc;

void handle_shutdown_server(int sig) {
	//Closes the socket in case of use of Ctrl-C
	if (listener_d) 
		close(listener_d);
	fprintf(stderr, "Socket closed!\n");
	exit(0);
} //handle_shutdown_server

void *thread_modif_wind(void* a){
	char command[50];
	while(1){
		scanf("%s",command);
		if(strncasecmp("w_speed",command,7)==0){
			change_str_wind(oc->wind,command[9]);
		}
		if(strncasecmp("w_direction",command,11)==0){
			change_dir_wind(oc->wind, strtodir(command[13]));
		}
	}
}

int main(int argc, char *argv[]) {
	//Runs the code handle_shutdown_server if Ctrl-C is used
	if (catch_signal(SIGINT, handle_shutdown_server) == -1) 
		error("Cannot set the interrupt handler");
	
	//Initializing the ocean
	ocean *oc = initialize_ocean(10,10,3,1);
	
	//Port we will be using
	int port = 6543;
	
	//Open 
	listener_d = open_socket(); 
	
	//Bind
	bind_to_port(listener_d, port); 
	
	//Listen
	listen_socket(listener_d,8);
	
	/*Variable definition: 
	  client_addr ---- client address
	  address_size --- size of the address
	  buf ------------ buffer where the server will read messages from the clients */
	struct sockaddr_storage client_addr;
	unsigned int address_size = sizeof(client_addr);
	puts("Waiting for connection...");
	char buf_cli[255];
	char buf_serv[255];
	
	pthread_t thread_wind;
	pthread_t thread_movement;

	if(pthread_create(&thread_wind, NULL, thread_modif_wind, NULL)==-1)
		error("Cannot join thread_wind");
	//initialisation de l'océan via le fichier 
	
	//thread lecture console serveur
	//thread deplacement des bateaux
	/*
	if (pthread_join(thread_matched_function, &result) == -1)
		error("Cannot join the thread for the matched function");
	if (pthread_join(thread_matched_function, &result) == -1)
		error("Cannot join the thread for the matched function");
	*/
	
	//Now we should loop the wait and fork a process for every incoming client
	while(1){
		int connect_d = accept_connection(listener_d,client_addr,address_size);
		//close_socket(port);
		//thread connection client
		
		//A supprimer
		if(!fork()) {
			//We are in the child process here
			//Le serveur doit prendre en charge le bateau qui arrive
					//Ajouter à l'annuaire
					//Ajouter à la carte
			close(listener_d); //The child will only need the connect_d socket generated by accept()
			if(say(connect_d,"\nWelcome in OceanC, a C-based ocean simulator\nThe options you can use are the following:\n\n1- Modify the variables of your boat (speed, direction) ------ Use one of the following\n\t------------------------------------------------------ 'modif_speed newSpeed'\n\t------------------------------------------------------ 'modif_dir newDirection'\n2- Display a message to other boats -------------------------- 'say yourMessage'\n3- Visualize the current ocean ------------------------------- 'visualize'\n\n") != -1)	
				{
				read_in(connect_d,buf_cli,sizeof(buf_cli));
				if(funcmatch(buf_cli,2)){
					close(connect_d);
					exit(0);	 
				} 
			}	//if(say(...
			close(connect_d);
			exit(0);
		} //if(!fork)
		//A supprimer
		
	} //while(1)
} //main()

//Mutex sur la carte?? annuaire??



