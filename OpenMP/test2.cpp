#include "omp.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main(int argc, char** argv)
{
	int nbthreads, tid;
	nbthreads = atoi(argv[1]);
	int x = 123;
	int a = 456;

	/*fork a team of thead with each thread having a private tid varialble*/
	#pragma omp parallel num_threads(nbthreads) private(tid,a)
	{

		// sleep(3);
		int tid = omp_get_thread_num();
		printf("thread %d has x = %d\n",tid,x);
		printf("Hello world from thread %d x = %d a = %d\n",tid,x,a);

		x = tid*10;		
	}
}