# include <iostream>
# include <cstdlib>
# include <mpi.h>

int main( int nargs, char* argv[] )
{
	// On initialise le contexte MPI qui va s'occuper :
	//    1. Créer un communicateur global, COMM_WORLD qui permet de gérer
	//       et assurer la cohésion de l'ensemble des processus créés par MPI;
	//    2. d'attribuer à chaque processus un identifiant ( entier ) unique pour
	//       le communicateur COMM_WORLD
	//    3. etc...

	MPI_Init( &nargs, &argv );
	// Pour des raison préfère toujours cloner le communicateur global
	// MPI_COMM_WORLD qui gère l'ensemble des processus lancés par MPI.
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	// On interroge le communicateur global pour connaître le nombre de processus
	// qui ont été lancés par l'utilisateur :
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	// On interroge le communicateur global pour connaître l'identifiant qui
	// m'a été attribué ( en tant que processus ). Cet identifiant est compris
	// entre 0 et nbp-1 ( nbp étant le nombre de processus qui ont été lancés par
	// l'utilisateur )
	int rank;
	int tag;
	int recvbuf1, recvbuf2;
	int sendbuf1 = 0;
	int sendbuf2 = 1;
	MPI_Status status;
	int count;

	MPI_Comm_rank(globComm, &rank);
	if(rank == 0) {
		MPI_Ssend(&sendbuf1, count, MPI_INT, 2, tag, globComm);
		MPI_Recv(&recvbuf1, count, MPI_INT, 2, tag, globComm, &status);
	}
	else if (rank == 1) {
		MPI_Ssend(&sendbuf2, count, MPI_INT, 2, tag, globComm);
	}
	else if(rank == 2) {
		MPI_Recv(&recvbuf1, count, MPI_INT, MPI_ANY_SOURCE, tag, globComm,&status);
		MPI_Ssend(&sendbuf2, count, MPI_INT, 0, tag, globComm);
		MPI_Recv(&recvbuf2, count, MPI_INT, MPI_ANY_SOURCE, tag, globComm,&status);
		}

	// On peut maintenant commencer à écrire notre programme parallèle en utilisant les
	// services offerts par MPI.
	std::cout << "Hello World, I'm processus " << rank << " on " << nbp << " processes.\n";

	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
