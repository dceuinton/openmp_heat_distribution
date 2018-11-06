#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <vector>
#include <time.h>

// Include the headers for the project
#include "arrayff.hxx"
#include "draw.hxx"

using namespace std;

// int* delegateRows(int total, int threads) {
//   int *ret = new int[threads];

//   int ppi = (int)(total / threads);
//   int remainder = total - (threads *  ppi);

//   for (int i = 0; i < threads; i++) {
//     ret[i] = ppi;
//   }

//   if (remainder != 0) {
//     for (int i = 0; i < remainder; i++) {
//       ret[i] = ret[i] + 1;
//     }
//   }

//   if (threads > 1) {
//     ret[0] = ret[0] - 1;
//     ret[threads - 1] = ret[threads - 1] - 1;
//   }
//   return ret;
// }

// void updateCell(Array<float, 2> &h, Array<float, 2> &g, int y, int x) {
//   g(y, x) = 0.25 * (h(y, x-1) + h(y, x+1) + h(y+1, x) + h(y-1, x));
// }

// int sumConverged(int *nums, int size) {
//   int ret = 0;
//   for (int i = 0; i < size; i++) {
//     ret += nums[i];
//   }
//   return ret;
// }

// void printArray(Array<float, 2>& a, int yDim, int xDim) {
// 	for (int y = 0; y < yDim; ++y) {
// 		for (int x = 0; x < xDim; ++x) {
// 			printf("%.1f ", a(y, x));
// 		}
// 		printf("\n");
// 	}
// }

int main(int argc, char* argv[]) {

  double start, end, init, parallel;

  start = omp_get_wtime();
  
	int npix = 100;

  if (argc == 1) {
    printf("Usage: ./main <grid size> <(optional)number of threads>\nDefault grid size 100x100\n");
  } else {
  	npix = atoi(argv[1]);
  }

  const float tol = 0.00001;
  const int npixx = npix;
  const int npixy = npix;
  const int ntotal = npixx * npixy;
  int nthreads = omp_get_max_threads();

  if (argc == 3) {
    nthreads = atoi(argv[2]);
  }

  omp_set_num_threads(nthreads);
 
  Array<float, 2> h(npixy, npixy), g(npixy, npixx);

  const int nrequired = npixx * npixy;
  const int max_iterations = 1000000;

  int iterations = 0;
  int nconverged[nthreads];
  int converged = 0;

  for (int i = 0; i < nthreads; i++) {
    nconverged[i] = 0;
  }

  fix_boundaries2(h);
  dump_array<float, 2>(h, "pl80.fit");

  	init = omp_get_wtime();

	do {
		
		  #pragma omp parallel for
	  	  for (int y = 1; y < npixy - 1; ++y) {
	  	  	for (int x = 1; x < npixx - 1; ++x) {
	  	  		g(y, x) = 0.25 * (h(y, x-1) + h(y, x+1) + h(y+1, x) + h(y-1, x));
	  	  	}
	  	  }
		  

		  fix_boundaries2(g);
		  converged = 0;

		  #pragma omp parallel 
		  {
		  	int id = omp_get_thread_num();
			int local_converged = 0;
		  	nconverged[id] = 0;
		  	#pragma omp for 
		  	for (int y = 0; y < npixy; ++y) {
		  		for (int x = 0; x < npixx; ++x) {
		  		    float dif = fabs(g(y,x) - h(y, x));
				    if (dif < tol) {++local_converged;}
				    h(y,x) = g(y,x);
		  		}
		  	}
			#pragma omp critical 
			converged += local_converged;
		  }

		  ++iterations;
		  
	} while (converged < ntotal && iterations < max_iterations);

	parallel = omp_get_wtime();

	dump_array<float, 2>(h, "pl81.fit");

	printf("\nFinished process using %d threads with %d iterations\n",nthreads, iterations);

	end = omp_get_wtime();

	printf("[%f, %f, %f, %f]\n\n", end - start, init-start, parallel-init, end - parallel);
}
