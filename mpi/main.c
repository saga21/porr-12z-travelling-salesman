#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#define NEXT_NODE (node+1)%node_count
#define PREV_NODE (node+node_count-1)%node_count

//tutaj przesyłam tablicę jednowymiarową, aby przesłać dwuwymiarową można zrobić tak: http://stackoverflow.com/questions/5901476/sending-and-receiving-2d-array-over-mpi
//instalację mpi wziąłem z http://www.mpich.org/ jest też wersja na windowsa :)
//nie wiem ja kna windzie ale na debianie kompilacja:  mpicc main.c  -o main
//uruchamianie mpiexec -n 4 ./main
//musiałem odpalić jeszcze coś ale postępowałme wg komunikatów które wyskoczyły
// C8GX-FT9WTCLT 
int main(int argc, char **argv)
{
	int i,node, node_count,sum = 0,flag;
	int *cities_array, *recv_data;
	void *buffer;
	MPI_Request send_req, recv_req;
	MPI_Status status;

	buffer = (int*)malloc(10 * sizeof(int) + MPI_BSEND_OVERHEAD); //(towns_count * sizeof(int));
	cities_array = (int*)malloc(10 * sizeof(int)); //(towns_count * sizeof(int));
	recv_data = (int*)malloc(10 * sizeof(int)); //(towns_count * sizeof(int));


	//init wymagany
	MPI_Init(&argc,&argv);

	//mówimy że ten bufor ma być użyty podczas wysyłania nieblokującego, uwaga - wielkość bufora w bajtach
	//aby był użyteczny musimy mu zapewnić tyle miejsca ile będzie wysłanych danych + MPI_BSEND_OVERHEAD - nie wiemy kiedy send się skończy
	// przykład: http://mpi.deino.net/mpi_functions/MPI_Bsend.html
	MPI_Buffer_attach(buffer, 10 * sizeof(int) + MPI_BSEND_OVERHEAD);


	//pobierz id i ilość zadań
	MPI_Comm_rank(MPI_COMM_WORLD, &node);
	MPI_Comm_size(MPI_COMM_WORLD, &node_count);

	for(i = 0; i < 10; ++i){
		sum += node;
		cities_array[i] = sum;
	}

	srand(node);
	sleep(rand()%5);


	//wyślij nieblokująco. Myślę, że przed wysłaniem drugiej transzy trzeba sprawdzić czy poprzedni send się skończył
	//analogicznie do sprawdzenia czy coś przyszło
	MPI_Ibsend((void*)cities_array,10 ,MPI_INT,NEXT_NODE,0,MPI_COMM_WORLD,&send_req);

	MPI_Irecv((void*)recv_data, 10, MPI_INT, PREV_NODE,0,MPI_COMM_WORLD,&recv_req);
	do
	{	
		//sprawdź status wysyłania/odbierania
		MPI_Test(&recv_req, &flag, &status);
		sleep(rand()%5);
	} while (!flag);


	 
	printf("Hello World from Node %d, next node is: %d, prev: %d.    Received: %d, %d, %d\n",node,NEXT_NODE,PREV_NODE,recv_data[0],recv_data[3],recv_data[9]);


	free(cities_array);
	free(recv_data);

	//finito
	MPI_Finalize();

	return 0;
}