#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// write the text program and make sure it complies.

// return a struct - file-discriptor, pointer to the buffer,
// buffer offset (to keep track of where in the buffer we are).
struct
file_info{
  int fileD;
  int buf_offset;
  char buf[4096];
  int *fp;
};
// create the entire struct in the heap to avoid other functions taking the stack area and inputting other information on that same stack. Overlap of data.
//myopen
struct file_info*
myopen(char* filePath, int flags, mode_t mode){
  int n;
  struct file_info *ptStruct = malloc(sizeof(struct file_info));
  //open file
  ptStruct->fileD = open(filePath,O_CREAT|O_RDWR|O_WRONLY|O_RDONLY|O_TRUNC,0666);
  if(ptStruct->fileD==-1){
    printf("Cannot open file: %s \n", filePath);
    //change name
    // provide same facility as open
    //return some value that indicates an error
    exit(1);
  }
  //else{
    // allocate memory
    printf("This seems to be working\n");

		if (ptStruct->fp == 0){
			printf("ERROR: Out of memory\n");
      //ptStruct->buf+=4096;
			exit(2);
		}
    else{
      ptStruct->buf_offset=0;
      printf("Supposedly created the buffer area\n");
    }
  // return a pointer to the newly created struct -> save it in the variable
  // then send it to myread to read it.
  //}
  return ptStruct;

}

//myclose
struct file_info*
myclose(struct file_info *file_close){
  // close file
  if(close(file_close->fileD)==-1){
    perror("close");
    exit(3);
  }
  else{
    //free memory
    free(file_close->fp);
  }
  return file_close;
}

//myread
struct file_info*
myread (struct file_info *file_read, size_t count){
  read(file_read->fileD,file_read->buf[file_read->buf_offset], count);
  file_read->buf_offset +=count;
  return file_read;
}
// read from the buffer memory and read x bytes from there
// - where is the buffer and how are we going to enter the buffer
// read
//


int
main(int argc, char* argv[]){
  struct file_info *p;
  if(argc<2){
    printf("Usage: ./myio <File Path> \n");
    exit(4);
  }
  char * filename;
  filename = argv[1];
  p = myopen(filename,O_CREAT|O_RDWR|O_TRUNC,0666);
  char buffer[10];
  myread(p,buffer, 10);
  //myread(10);
  myclose(p->fileD);
  return 0;
}




// close file

//mywrite
// open file
// allocate memory
// write in the buffer memory
// write
//free memory
//close file

//myseek

//myflush
// write memory
// free moemory
