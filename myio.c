#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// write the text program and make sure it complies.

// return a struct - file-discriptor, pointer to the buffer,
// buffer offset (to keep track of where in the buffer we are).
struct
file_info{
  int fileD;
  int buf_offset;
  int buf_size;
  char buf[4096];
  void* fp;
};
// create the entire struct in the heap to avoid other functions taking the stack area and inputting other information on that same stack. Overlap of data.

struct file_info*
myopen(char* filePath, int flags, mode_t mode){
  struct file_info *ptStruct= malloc(sizeof(struct file_info));
  //  printf(" our buffer pointer open = %p \n", ptStruct->buf);
  ptStruct->buf_size = 4096;
  ptStruct->buf_offset = 0;
  //open file
  ptStruct->fileD = open(filePath,flags,mode);
  //printf("file discriptor %d \n",ptStruct->fileD );
  if(ptStruct->fileD == -1){
    //printf("Cannot open file: %s \n", filePath);
    perror("open");
    //change name
    // provide same facility as open
    //return some value that indicates an error
  }
    // allocate memory
    //printf("This seems to be working\n");
    //printf("Supposedly created the buffer area\n");
  // return a pointer to the newly created struct -> save it in the variable
  // then send it to myread to read it.
  return ptStruct;
}


struct file_info*
myclose(struct file_info *file_close){
  // close file
  //printf("so here?\n");
  //printf("file discriptor %d \n",file_close->fileD);
  if(close(file_close->fileD) == -1){
  //  printf("hm,......\n");
    perror("close");
  }
    //free memory
    //printf("This is, I guess, FREEDOM! \n");
    free(file_close);
  return file_close;
}


struct file_info*
myread (struct file_info *file_read,char* buffer, size_t count){
  //printf(" our buffer pointer 1 = %p \n", file_read->buf);
  if(file_read->buf_offset + count>4096){
    int left = 4096 - file_read->buf_offset;
    memcpy(buffer,file_read->buf + file_read->buf_offset,left);

    count = count - left;
    //printf("new count = %ld \n", count);
    //printf(" our buffer pointer 2 = %p \n", file_read->buf);
    file_read->buf_offset = 0;
    //int w = 0;
    while(count>4096){
        read(file_read->fileD,file_read->buf,4096);
        memcpy(buffer,file_read->buf + file_read->buf_offset,4096);
        count = count - 4096;
        //w++;
      //  printf("w %d\n", w);
    }
  }
  if(file_read->buf_offset == 0){
    // read
    // and how are we going to enter the buffer
    //printf(" our buffer pointer 3 = %p \n", file_read->buf);
    //printf("This is, I guess, reading \n");
    //printf("file discriptor read %d \n",file_read->fileD);
    read(file_read->fileD,file_read->buf,4096);
  }
  // read from the buffer memory and read x bytes from there
  memcpy(buffer,file_read->buf + file_read->buf_offset,count);

  //where in the buf should it read from
  file_read->buf_offset += count;
  //printf("This is the buf-offset = %d \n", file_read->buf_offset);
  //overflow cases -> read from the remaining bytes in buf and then
  // call read again for the next 4096
  return file_read;
}

int
main(int argc, char* argv[]){
  struct file_info *p;
  char buffer[20000];
  if(argc < 2){
    printf("Usage: ./myio <File Path> \n");
    exit(4);
  }
  char* filename;
  filename = argv[1];
  p = myopen(filename,O_CREAT|O_RDWR|O_TRUNC,0666);
  //printf("file discriptor in main %d \n",p->fileD);
  myread(p,buffer,10);
  myread(p,buffer,10);
  myread(p,buffer,10000);
  myclose(p);
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
