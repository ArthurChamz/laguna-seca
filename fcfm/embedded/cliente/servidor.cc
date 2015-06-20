#include <iostream>
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

int main(int argc, char** argv){

	//APS----------------------------------------------------------------------

	int reto;

	sched_aps_create_parms creation_data;
	sched_aps_join_parms join_data;

	memset(&creation_data, 0, sizeof(creation_data));

	creation_data.budget_percent = 10;
	creation_data.critical_budget_ms = 0;
	creation_data.name = "Redes";

	reto = SchedCtl(SCHED_APS_CREATE_PARTITION, &creation_data,
			sizeof(creation_data));

	if (reto != EOK)
	   cout << "No se pudo crear la particion " << creation_data.name
			<< " : " <<  strerror(errno) << ' ' << errno << endl;

	else
	   cout << "El ID de la nueva particion es: " << creation_data.id << endl;

	memset(&join_data, 0, sizeof(join_data));

	join_data.id = 1;
	join_data.pid = 0;
	join_data.tid = 0;

	reto = SchedCtl( SCHED_APS_JOIN_PARTITION, &join_data,
					sizeof(join_data));

	if (reto != EOK)
	   cout << "La aplicacion no pudo iniciar en la particion "
			<< join_data.id << " : " << strerror(errno) << ' ' << errno << endl;

	else
	   cout << "La aplicacion esta corriendo en la particion "
	   	   	<< join_data.id << endl;

	//SOCKETS------------------------------------------------------------------

	int n;
	int sin_size;
	int socketfd1, socketfd2;

	struct sockaddr_in server;
	struct sockaddr_in client;

	char mensaje[100];

	if( ( socketfd1 = socket(AF_INET, SOCK_STREAM, 0) ) ==  -1 ){

		cout << "No se pudo inicializar el socket." << endl;
		return 1;

	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

	if( bind(socketfd1,
			(struct sockaddr*)&server, sizeof(struct sockaddr) ) == -1 ){

		cout << "Error al reservar puerto." << endl;
		return 1;

	 }

	if( listen(socketfd1, 100) == -1 ){

		 cout << "Error en listen." << endl;
		 return 1;

	}

	sin_size = sizeof(struct sockaddr_in);

	//CICLO INFINITO PARA RECIBIR DATOS----------------------------------------

	while(true){

		cout << "Estableciendo conexion con un cliente." << endl;

		if( (socketfd2 = accept( socketfd1,
				(struct sockaddr*)&client, (socklen_t*) &sin_size)) == -1){

		  cout << "Error an accept." << endl;
		  return 1;;

		}

		cout << "Conexion exitosa." << endl;

		while(true){

			if( ( n =  recv( socketfd2, mensaje, sizeof(mensaje)  ,0)) == -1)
				   cout << "Error en recv." << endl;

			cout << "Mensaje: " << mensaje << endl;

		}

	}

	return EXIT_SUCCESS;

}

#include <_packpop.h>
