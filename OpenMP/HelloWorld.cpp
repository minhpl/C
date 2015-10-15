#include "omp.h"
#include <stdio.h>
#include <stdlib.h>

main(int argc,char** argv)
{
	int nbthreads, tid;
	nbthreads = atoi(argv[1]);

	/*fork a team of threads with each thread having a private tid variable*/
	#pragma omp parallel if(nbthreads >= 4) num_threads(nbthreads)
	{
		tid = omp_get_thread_num();
		printf("Hello world from thread %d\n",tid);
	}

}