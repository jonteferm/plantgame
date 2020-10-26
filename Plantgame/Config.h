#define WIDTH 100
#define HEIGHT 100

#define WIDTHx2 (WIDTH*2)
#include "Node.h"

#define HEIGHTx2 (HEIGHT*2)
// use square map
#define N MIN(WIDTHx2,HEIGHTx2)
// store a 2D map in a 1D array
#define SIZE (N+2)*(N+2)

#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}  
// convert x,y to array index
#define IX(i,j) ((i)+(N+2)*(j))

#define X_MAX 100
#define X_STEP 1
#define Y_MAX 100
#define Y_STEP 1


