/* Thomas Grove
 * 0597569
 * 
 * CIS*3090 Parallel Assignment 2
 * - Where is Parallaldo?
 *
 */
#include "wp.h";


//global
Queue *image_queue;
Queue *parallaldo_queue;
int numtargets, numparallaldos;
int numthreads;
char *imagedir, *parallaldodir;
Image * nextImage;

/*
args: wp  aldodir  imagedir threads
# aldodir is the directory populated with parallaldo templates
# imagedir is the directory populated with images to search in
# threads is the total number of threads to utilize (including the main thread)

*/
int main(int argc, char **argv){
	double time;

	//start clock
	StartTime();

	if(argc != 4){
		printf("Usage \"wp  aldodir  imagedir #threads\"\n");
		return 0;
	}
	parallaldodir = argv[1];
	imagedir = argv[2];
	sscanf (argv[3], "%d", &numthreads);

//	numthreads = 16;

	/* Preparation */
	image_queue = newQueue();
	parallaldo_queue = newQueue();
	
	/* Read in Image Files */
	ReadImageFiles();
//	printf("Number of Files: %d\n", image_queue->size);

	/* Read in Parallaldo Files */
	ReadParallaldoFiles();
//	printf("Number of Parallaldos: %d\n", parallaldo_queue->size);  

	/* Search */
//	printf("Searching\n");
	while ( image_queue->size > 0){
		nextImage = (Image*) deleteNextNode(image_queue);
		searchImage(nextImage);
	}
	
	//endclock
	time = EndTime();

	printf("%lf Seconds\n", time);
	return 0;
}

 
/* Read Image Files */
void ReadImageFiles(){
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	FILE *fp;
	char *filename;
	char *folder;
	char *buffer;
	LARGE_INTEGER filesize;
	Image *img;

//	printf("Reading Image Files\n");
	
	/* Allocating buffers for folder */
	folder = (char*) calloc(strlen(imagedir)+50 , sizeof(char));
	strcpy(folder,imagedir);
	strcat(folder, "/*.img");

	/* Loop through all the files in the provided folder */
	if((hFind = FindFirstFile(folder, &FindFileData)) != INVALID_HANDLE_VALUE){
		do{
//			printf("%s\n", FindFileData.cFileName);

			filename =(char*) calloc(100, sizeof(char));
			sprintf(filename, "./%s/%s", imagedir, FindFileData.cFileName);
			
			/* Open File */
			fp = fopen(filename,"r");
			if( !fp ){
				printf("Couldnt open file %s\n", filename);
			}else{
				/*File Size*/
				filesize.LowPart = FindFileData.nFileSizeLow;
				filesize.HighPart = FindFileData.nFileSizeHigh;
//				printf("  %s   %ld bytes\n", FindFileData.cFileName, filesize.QuadPart);

				/* Allocate space */
				buffer = (char*) calloc(filesize.QuadPart +1, sizeof(char));
				img = (Image*)calloc(1, sizeof(Image));
				img->filename  = (char*)calloc(strlen(FindFileData.cFileName)+1, sizeof(char));
				strcpy( img->filename, FindFileData.cFileName );

				/* Read data in */
				fread(buffer, filesize.QuadPart, 1, fp);
				parseImageFile(buffer, img);
				addToQueue(image_queue,img);

				/* Closing File */
				free(buffer);
				fclose(fp);
			}

		}while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	else {
		printf("Invalid File Handle!\n");
	}
	
}

/**
 * Parse Raw File Buffer
 * - gets row & columns
 * - formates rows
 */
void parseImageFile(char *buff, Image *img){
	char *token;
	int i, r, c;

	token = strtok(buff, "\n");
	/*Rows & Cols*/
	sscanf(token, "%d %d", &r, &c);
//	printf("%d %d\n", r, c);

	/* Allocate space for data in image struct */
	img->rows = r;
	img->cols = c;
	img->data = (char**)calloc(r, sizeof(char*) );


	/*Parse Rows*/
	for (i = 0; i < r; i++){
		token = strtok(NULL, "\n");
		img->data[i] = (char*)calloc(c+1, sizeof(char));
		strcpy(img->data[i], token);
		
	}
		
}

/* Read Parallaldo Files */
void ReadParallaldoFiles(){
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	FILE *fp;
	char *filename;
	char *folder;
	char *buffer;
	LARGE_INTEGER filesize;
	Parallaldo *pw;

//	printf("Reading Parallaldo Files\n");
	
	/* Allocating buffers for folder */
	folder = (char*) calloc(strlen(imagedir)+50 , sizeof(char));
	strcpy(folder, parallaldodir);
	strcat(folder, "/*.txt");

	/* Loop through all the files in the provided folder */
	if((hFind = FindFirstFile(folder, &FindFileData)) != INVALID_HANDLE_VALUE){
		do{
//			printf("%s\n", FindFileData.cFileName);

			filename =(char*) calloc(100, sizeof(char));
			sprintf(filename, "./%s/%s", parallaldodir, FindFileData.cFileName);
			
			/* Open File */
			fp = fopen(filename,"r");
			if( !fp ){
				printf("Couldnt open file %s\n", filename);
			}else{
				/*File Size*/
				filesize.LowPart = FindFileData.nFileSizeLow;
				filesize.HighPart = FindFileData.nFileSizeHigh;
//				printf("  %s   %ld bytes\n", FindFileData.cFileName, filesize.QuadPart);

				/* Allocate space */
				buffer = (char*) calloc(filesize.QuadPart +1, sizeof(char));
				pw = (Parallaldo*)calloc(1, sizeof(Parallaldo));
				pw->filename = (char*)calloc(strlen(FindFileData.cFileName)+1, sizeof(char));
				strcpy( pw->filename, FindFileData.cFileName );

				/* Read data in */
				fread(buffer, filesize.QuadPart, 1, fp);
				parseParallaldoFile(buffer, pw);
				addToQueue(parallaldo_queue,pw);

				/* Closing File */
				free(buffer);
				fclose(fp);
			}

		}while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	
}

/**
 * Parse Raw File Buffer
 * - gets row & columns
 * - formates rows
 */
void parseParallaldoFile(char *buff, Parallaldo *pw){
	char *token;
	int i, r, c;

	token = strtok(buff, "\n");
	/*Rows & Cols*/
	sscanf(token, "%d %d", &r, &c);
//	printf("%d %d\n", r, c);

	/* Allocate space for data in image struct */
	pw->rotations[0].rot = 0;
	pw->rotations[0].rows = r;
	pw->rotations[0].cols = c;
	pw->rotations[0].data = (char**)calloc(r, sizeof(char*));

	/*Parse Rows */
//	printf("\n\n\n ROTATION 0\n");
	for (i = 0; i < r; i++){
		token = strtok(NULL, "\n");
		pw->rotations[0].data[i] = (char*)calloc(c+1, sizeof(char));
		strcpy(pw->rotations[0].data[i], token);
//		printf("||%s||%d\n",pw->rotations[0].data[i],i);
		
	}
	/* Calculate other rotations */
	calculateRotations(pw);
	
}

/**
 * Calculate Rotation for provided Parallaldo struct
 */
void calculateRotations(Parallaldo *pw){
	int i;
	int r,c;
	char tmp;
	/* rotations 90*, 180*, 270* */
	for(i = 1; i < 4; i++){
//		printf("\n\nROTATION %d\n", i);
		/* Dimensions */
		pw->rotations[i].rot = i*90;
		pw->rotations[i].rows = pw->rotations[i-1].cols;
		pw->rotations[i].cols = pw->rotations[i-1].rows;

		/* malloc space */
		pw->rotations[i].data = (char**)calloc(pw->rotations[i].rows, sizeof(char*));

		/* rows = col, col = rows */
		for(r = 0; r < pw->rotations[i].rows; r++ ){
			pw->rotations[i].data[r] = (char*)calloc(pw->rotations[i].cols +1, sizeof(char));
			for(c = 0; c < pw->rotations[i].cols; c++ ){
				
				pw->rotations[i].data[r][c] = pw->rotations[i-1].data[pw->rotations[i].cols-c-1][r];
			
			}//end c
//			printf("||%s||%d\n",pw->rotations[i].data[r],r);
		}//end r

	}//end i


}


void searchImage(Image *img){
	pthread_t *threads;
	ThreadData *td;
	int i;

//	printf("Searching Image %s\n", img->filename);

	/* Allocate Threads */
	threads = (pthread_t *)calloc(numthreads, sizeof(pthread_t) );

	for (i = 0; i < numthreads; i++){
		td = (ThreadData *)calloc(1, sizeof(ThreadData));
		td->img = img;
		td->index = i;
		pthread_create ( &threads[i] , NULL, searchWorker, (void*) td);
			
	}
		
	/* Join Threads (barrier) */
	for (i = 0; i < numthreads; i++) {
		pthread_join( threads[i] , NULL);
	}
}

void * searchWorker( void *td ){
	Image *img;
	int para, rot, r, c, i, rr;
	int index, nummyrows, myblockstart, myblockend;
	char *strcheck, *strcheck2;
	int checkcol, checkcol2;
	int found;

	Parallaldo *temp_para;
	P_Rotation temp_rot;

	/* Unpackagin Data */
	img = ((ThreadData*)td)->img;
	index = ((ThreadData*)td)->index;
	

	/* Calculate size of file block I will work on */
	nummyrows = (int)(img->rows /(numthreads));

	/* my position */
	myblockstart = index * nummyrows;	

	myblockend =  myblockstart + nummyrows;

	/* if I am the last thread, i'll get the remainder of the rows */
	if(index == numthreads-1)
		myblockend += img->rows % (numthreads);

	if(numthreads == 1)
		myblockend = img->rows;

//	printf("Thread %d out of %d, mystart %d, myend %d\n", index+1, numthreads, myblockstart, myblockend);	
	
	//select next parallaldo
	for(para = 0; para < parallaldo_queue->size; para++) {
		temp_para = (Parallaldo*) lookAtNode(parallaldo_queue, para);
		//printf("parallaldo %s\n", temp_para->filename);
		//select rotation
		for(rot = 0; rot < 4; rot++ ){
			temp_rot = temp_para->rotations[rot];
//			printf("rotation %d\n",temp_rot.rot);
			//select next row from block;
			for (r = myblockstart; r < myblockend; r++){	
				//printf("Row %d\n", r);
				//check if top row of parallaldo is found in image's current row
				strcheck = NULL;
				strcheck = strstr(img->data[r], temp_rot.data[0]);
				found = 0;
				while ( strcheck != NULL ){
					//check rest of parallaldo vs current location in image
					checkcol = strcheck - img->data[r];
//					printf("check row: %d col: %d\n",r, checkcol);
					strcheck2 = NULL;
					for(c = 1; c < temp_rot.rows && ((r + c) < img->rows); c++){
						strcheck2 = strstr(img->data[r+c] + checkcol, temp_rot.data[c]);
						
						/* if row does not match OR if match and does not line, */
						if(strcheck2 == NULL ||  (strcheck2 - img->data[r+c]) != checkcol ){
							//not found, go to next row in image
							break;
						}
						
						if (c == temp_rot.rows-1){
							found = 1;

							if(rot == 0){ /* Rotation 0 */
								printf("$%s %s (%d,%d,%d)\n", temp_para->filename, img->filename, r+1, checkcol+1, temp_rot.rot );
							}else if(rot == 1){ /* Rotation 90 */
								printf("$%s %s (%d,%d,%d)\n", temp_para->filename, img->filename, r+1, checkcol + temp_rot.cols, temp_rot.rot );
							}else if(rot == 2){ /* Rotation 180 */
								printf("$%s %s (%d,%d,%d)\n", temp_para->filename, img->filename, r + temp_rot.rows, checkcol + temp_rot.cols, temp_rot.rot );
							}else{ /* Rotation 270 */
								printf("$%s %s (%d,%d,%d)\n", temp_para->filename, img->filename, r + temp_rot.rows, checkcol+1 , temp_rot.rot );
							}
							found = 1;
						}
					
					}//end for

					
					/* if not found, check rest of row */
					if( !found && img->cols - checkcol+2 > temp_rot.cols ){
						/* not found and space left in row to check */
						strcheck = strstr( (img->data[r] + checkcol +2), temp_rot.data[0]);
						checkcol = strcheck - img->data[r];
					}else{
						/* notfound and no space left in row to check */
						strcheck = NULL;
					}

				}//end while strcheck !null
			
				
				if( r + temp_rot.rows > img->rows){
					//not enough space to find
					//printf("not enough space\n");

					break;
				}
			}//end row

		} //end parallaldos rotations

	}//end parallaldo

	return td;
}




// helper function, don't call this directly
static double TimeHelper( int reset )
{
	static double start = 0.0;
	if ( reset ) {
		start = omp_get_wtime();
		return 0.0;
	}
	return (double)(omp_get_wtime()-start);
}

// StartTime resets timer
// EndTime returns no. of wal lclock seconds elapsed since StartTime
void StartTime()
{
	TimeHelper( 1 ); // reset timer
}

double EndTime()
{
	return TimeHelper( 0 );
}