/* Thomas Grove
 * 0597569
 * 
 * CIS*3090 Parallel Assignment 2
 * - Where is Parallaldo?
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <windows.h>
#include <pthread.h>
#include <omp.h>
#include <string.h>

#include "Queue.h"

/* Holds Parsed Image Data 
 * - Number of rows & cols
 * - filename
 * - 2D array of rows * cols
 */
typedef struct image_pt{
	char *filename;
	int rows, cols;
	/* data[rows][cols] */
	char **data;
}Image;

/* Holds information for each parallado rotation
 * - number of rows & columns it takes up, data(0s & 1s)
 */
typedef struct p_rotation_pt{
	int rot;
	int rows, cols;
	/* data[rows][cols] */
	char **data;
}P_Rotation;

/* Holds information for each parallado
 * - Filename, four rotations of parallado
 */
typedef struct parallaldo_pt{
	char *filename;
	P_Rotation rotations[4];
}Parallaldo;

/* Used to send serach worker threads the initial data they need to work on */
typedef struct thread_data_pt{
	int index;
	Image *img;
}ThreadData;

/* Image File methods */
void ReadImageFiles(void);
void parseImageFile(char *buff, Image *img);

/* Parallaldo methods */
void ReadParallaldoFiles(void);
void parseParallaldoFile(char *buff, Parallaldo *pw);
void calculateRotations(Parallaldo *pw);

/* Search methods */
void searchImage(Image *img);
void * searchWorker( void *td );

/* Wallclock.h */
static double TimeHelper( int reset );
void StartTime();
double EndTime();
