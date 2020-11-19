myvar = 0;
for (i = 1; i < numtasks, ++i) {
	task = i;
	MPI_Request req;
	MPI_ISend(&myvar, ..., ..., task, &req);
	MPI_Wait(req);
	myvar = myvar + 2;
	// Do some works
	...
}