#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/neutrino.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sched_aps.h>

#include <_pack1.h>

#define PORT 503

using namespace std;

char ip[20];

void procArgs(int, char**);

int main(int argc, char** argv){

	//APS----------------------------------------------------------------------

	int reto;

	sched_aps_create_parms creation_data;
	sched_aps_join_parms join_data;

	memset(&creation_data, 0, sizeof(creation_data));
	creation_data.budget_percent = 5;
	creation_data.critical_budget_ms = 0;
	creation_data.name = "Utilerias";

	reto = SchedCtl( SCHED_APS_CREATE_PARTITION, &creation_data,
	                sizeof(creation_data));
	if (reto != EOK)
		cout << "No se pudo crear la particion " << creation_data.name
			 << " : " <<  strerror(errno) << ' ' << errno << endl;

	else
		cout << "El ID de la nueva particion es: " << creation_data.id << endl;

	memset(&join_data, 0, sizeof(join_data));
	join_data.id = 2;
	join_data.pid = 0;
	join_data.tid = 0;

	reto = SchedCtl( SCHED_APS_JOIN_PARTITION, &join_data,
	                sizeof(join_data));

	if (reto != EOK)
		cout << "La aplicacion no pudo iniciar en la particion " << join_data.id
			 << " : " << strerror(errno) << ' ' << errno << endl;

	else
		cout << "La aplicacion esta corriendo en la particion "
			 << join_data.id << endl;

	//SOCKETS------------------------------------------------------------------

	int socketfd;

	struct sockaddr_in server;

	char mensaje[100];

	procArgs(argc, argv);

	if( ( socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){

       cout << "Error al inicializar nuevo socket." << endl;
       return 1;

    }

    server.sin_family =   AF_INET;
    server.sin_port = htons(PORT);

	if( connect(socketfd,
			(struct sockaddr *)&server, sizeof(struct sockaddr)) == -1 ){

		 cout << "Error de conexion con el servidor." << endl;
		 return 1;

    }

	//CICLO INFINITO PARA ENVIAR DATOS-----------------------------------------

	while(true){

			cout << "Mensaje: ";
			gets(mensaje);
			send(socketfd, mensaje , sizeof(mensaje), 0);

	}

	 return EXIT_SUCCESS;

}

void procArgs(int argc, char** argv){

	int c;
	bool a = false;

	while( (c = getopt(argc, argv, "p:")) != -1 ){

		switch(c){

			case 'p':
				strcpy(ip, optarg);
				a = true;
				break;

			default:
				break;

		}

	}

	if(!a)
		cout << "Parametros: " << endl << " -p ip del servidor" << endl;

}

#include <_packpop.h>
