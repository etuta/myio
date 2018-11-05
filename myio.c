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
#define BYTES_PER_ITERATION 10

void usage(char *argv[]);

struct
file_info{
  int fileD;
  int buf_offset;
  int buf_size;
  char buf[20];
  void *fp;
  int bytes_read;
  int bytes_written;
  int file_offset;
  int user_buf_offset;
};
// create the entire struct in the heap to avoid other functions taking the stack area and inputting other information on that same stack. Overlap of data.
//myopen
struct file_info*
myopen(char* filePath, int flags){
  struct file_info *ptStruct= malloc(sizeof(struct file_info));
  ptStruct->buf_size = 20;
  ptStruct->buf_offset = 0;
  ptStruct->bytes_read = 0;
  ptStruct->bytes_written = 0;

  //open file
  ptStruct->fileD = open(filePath,flags,0666);
  if(ptStruct->fileD == -1){
    printf("\n");
    return ptStruct;
  }
    printf("Open file %s\n", filePath);
    printf("\n");
  return ptStruct;
}

//myclose
struct file_info*
myclose(struct file_info *file_close){
  // close file
  if(close(file_close->fileD)==-1){
    printf("\n");
    return file_close;
  }
  //free memory
  printf("CLose File\n");
  free(file_close);
  printf("\n");
  return file_close;
}

//myread
int
myread (struct file_info *file_read,char* buffer, size_t count){
  int check;
  file_read->bytes_read = 0;
  file_read->user_buf_offset = 0;
  if(file_read->buf_offset == file_read->buf_size){
    file_read->buf_offset = 0;
  }
  //Overflow case - Section 1
  if(file_read->buf_offset + count > file_read->buf_size){
    // content left in the buffer
    if(file_read->buf_offset > 0){
      int left = file_read->buf_size - file_read->buf_offset;
      //printf("left = %d\n",left);
      memcpy(buffer,file_read->buf + file_read->buf_offset,left);
      file_read->user_buf_offset += left;
      //printf("Left content put in user content = %s\n", buffer);
      count = count - left;
      file_read->bytes_read += left;
      file_read->buf_offset = 0;
    }
    //Section 2
    while(count > file_read->buf_size){
        check = read(file_read->fileD,file_read->buf,file_read->buf_size);
        //printf("in my buf during overflow = %s \n", file_read->buf);
        if(check == -1){
          printf("\n");
          return -1;
        }
        file_read->bytes_read += check;
        if(check < file_read -> buf_size){
          printf("reached end of file \n");
          memcpy(buffer+file_read->user_buf_offset,file_read->buf + file_read->buf_offset,check);
          file_read->user_buf_offset += check;
          return file_read->bytes_read;
        }

      memcpy(buffer+file_read->user_buf_offset,file_read->buf + file_read->buf_offset,file_read->buf_size);
      file_read->user_buf_offset += file_read->buf_size;
        //printf("in user buf during overflow = %s \n", buffer);
      count = count - file_read->buf_size;

        //printf("out buf offset in while loop = %d \n",file_read->buf_offset);
        //printf("\n");
    }
  }
  //Section 3
  if(file_read->buf_offset == 0){
    check = read(file_read->fileD,file_read->buf,file_read->buf_size);
    //printf("in my buf in section 3 = %s \n", file_read->buf);
    //printf("check after read = %d\n", check);
    if(check == -1){
      //perror("write");
      printf("\n");
      return -1;
    }
    else if(check < count){
      printf("reached end of file \n");
      memcpy(buffer + file_read->user_buf_offset,file_read->buf + file_read->buf_offset,check);
      printf("in user buf in section 3 = %s \n", buffer);
      file_read->user_buf_offset += check;
      file_read->bytes_read += check;
      file_read->buf_offset += check;
      return file_read->bytes_read;
    }
    /*else{
      memcpy(buffer,file_read->buf + file_read->buf_offset,count);
      file_read->bytes_read += count;
    }*/
  }
    //printf("buf offset right before memcpy = %d\n",file_read->buf_offset);
    //printf("------------in user buf in section 3 = %s \n", buffer);
    //printf("COUNT = %d \n", count);
    //printf("user_buf_offset %d \n", file_read->user_buf_offset);
  memcpy(buffer + file_read->user_buf_offset,file_read->buf + file_read->buf_offset,count);
    //printf("User buf offset = %d\n",file_read->user_buf_offset);
  file_read->user_buf_offset += count;
    //printf("in user buf in section 3 = %s \n", buffer);
  file_read->buf_offset += count;
  file_read->bytes_read += count;
  file_read->file_offset += file_read->bytes_read;

  //printf("\n");
  return file_read->bytes_read;
}

/*int
mywrite (struct file_info *file_write, char* buffer, size_t count){
  int check;
  printf("initial buffer offset in write %d \n", file_write->buf_offset);
  //int w = lseek(file_write->fileD, count, SEEK_SET);
  //printf("the W %d\n", w);
  file_write->bytes_written += count;
  printf("buffer in start of write = %s \n", buffer);

  if(file_write->buf_offset + count > file_write->buf_size){
    int left = file_write->buf_size - file_write->buf_offset;
    memcpy(buffer,file_write->buf + file_write->buf_offset,left);
    count = count - left;
    //printf("new count = %ld \n", count);
    //printf(" our buffer pointer 2 = %p \n", file_read->buf);
    file_write->buf_offset = 0;
    //int w = 0;
    while(count>file_write->buf_size){
      check = write(file_write->fileD,buffer,file_write->buf_size);
      if(check == -1){
          //perror("write");
        printf("\n");
        return -1;
      }
      memcpy(buffer,file_write->buf + file_write->buf_offset,file_write->buf_size);
      count = count - file_write->buf_size;
      //w++;
      //  printf("w %d\n", w);
    }
  }

  if(file_write->buf_offset == 0){
    // write
    // and how are we going to enter the buffer
    //printf(" our buffer pointer 3 = %p \n", file_read->buf);
    //printf("This is, I guess, reading \n");
    //printf("file discriptor read %d \n",file_read->fileD);
    printf("buffer in between write = %s \n", buffer);
    printf("file D in write %d \n",file_write->fileD);
    printf("buffer in write = %s \n", buffer);
    write(file_write->fileD,buffer,count);
    printf("our buf struct %s \n", file_write->buf);
    file_write->buf_offset = count;
    //printf("buffer in write = %s \n", buffer);
  }
  // read from the buffer memory and read x bytes from there
  //memcpy(buffer,file_write->buf + file_write->buf_offset,count);
  //printf("buffer in write = %s \n", buffer);
  //where in the buf should it read from
  //file_write->buf_offset += count;
  //w = lseek(file_write->fileD, file_write->buf_offset, SEEK_SET);
  //printf("the W %d\n", w);

  printf("buffer offset in write %d \n", file_write->buf_offset);
  printf("count %d \n", count);

  printf("M = %d \n",m);
  //overflow cases -> read from the remaining bytes in buf and then
  // call read again for the next 4096
  return file_write->bytes_written;
}*/

int
myseek(struct file_info *file_seek, int offset, int whence){
  if(whence == 0){
    if(file_seek->file_offset == 0){
      lseek(file_seek->fileD, offset, whence);
      file_seek->file_offset = offset;
    }
  }
  else if(whence == 1){
    printf("offset/count = %d \n", offset);
    printf("buffer offset in myseek %d \n", file_seek->buf_offset);
    file_seek->buf_offset = offset + file_seek->buf_offset;//current_location;
  }
  else{
    return -1;
  }
  return file_seek->file_offset;
}


int
main (int argc, char* argv[]){
  struct file_info *p;
  int n;
  char buffer[100];
  char* filename = argv[1];
  p = myopen(filename, O_RDONLY);
  if(p->fileD < 0) {
      perror("open");
      exit(3);
  }
  n = read(p->fileD, buffer, 20);
  //n = myread(p,buffer,20);
  printf("************************\n");
  printf("Read output = %d \n",n);
  printf("content in user buffer = %s\n", buffer);
  printf("Bytes read = %d \n my buf_offset = %d \n",p->bytes_read,p->buf_offset);
  printf("Content in my buf = %s\n", p->buf);
  printf("***********************\n");
  printf("\n");
  //n = read(p->fileD, buffer, 46);
//  myseek(p,30,SEEK_SET);
  n = myread(p,buffer,25);
  printf("***********************\n");
  printf("my read output = %d \n",n);
  printf("content in user buffer = %s\n", buffer);
  printf("Bytes read = %d \n my buf_offset = %d \n",p->bytes_read,p->buf_offset);
  printf("Content in my buf = %s\n", p->buf);
  printf("***********************\n");
  printf("\n");

  n = myread(p,buffer,10);
  printf("***********************\n");
  printf("my read output = %d \n",n);
  printf("content in user buffer = %s\n", buffer);
  printf("Bytes read = %d \n my buf_offset = %d \n",p->bytes_read,p->buf_offset);
  printf("Content in my buf = %s\n", p->buf);
  printf("***********************\n");

  myclose(p);
}

/*
int
main (int argc, char* argv[]){
  struct file_info *p;
  int n;
  char buffer[100];
  char* filename = argv[1];
  p = myopen(filename, O_RDONLY);
  if(p->fileD < 0) {
      perror("open");
      exit(3);
  }
  n = read(p->fileD,buffer,25);
  //printf("***********************\n");
  printf("READ output = %d \n",n);
  printf("content in user buffer = %s\n", buffer);
  printf("Bytes read = %d \n my buf_offset = %d \n",p->bytes_read,p->buf_offset);
  printf("Content in my buf = %s\n", p->buf);
  printf("***********************\n");
  printf("\n");

  n = read(p->fileD,buffer,10);
  printf("***********************\n");
  printf("READ output = %d \n",n);
  printf("content in user buffer = %s\n", buffer);
  printf("Bytes read = %d \n my buf_offset = %d \n",p->bytes_read,p->buf_offset);
  printf("Content in my buf = %s\n", p->buf);
  printf("***********************\n");

  myclose(p);
}

int
main(int argc, char* argv[]){
  struct file_info *p;
  struct file_info *q;
  int n;
  if(argc < 2){
    printf("Usage: ./myio <File Path> \n");
    exit(1);
  }
  char * filename;
  char * destname;
  filename = argv[1];
  destname = argv[2];
  printf("filename = %s \n", filename);
  printf("destname = %s \n", destname);
  p = myopen(filename,O_RDONLY);
  if(p->fileD < 0) {
      perror("open");
      exit(3);
    }
  q = myopen(destname, O_CREAT | O_WRONLY);
  if(q->fileD < 0) {
      perror("open");
      exit(3);
    }
  printf("p = %d \n", p);
  printf("fileD - p = %d \n",p->fileD);
  printf("fileD - q = %d \n", q->fileD);
  printf("\n");
  char buffer[100];
  //n = myread(p,buffer,10);
  //printf("n1 %d \n", n);
  //printf("offset1 %d \n", p->buf_offset);
  //mywrite(p, buffer, 10);
  //printf("offset1 W %d \n", p->buf_offset);
  //n = myread(p,buffer,7);
  //printf("n2 %d\n", n);
  //printf("offset2 %d\n", p->buf_offset);
  n = myread(p,buffer, 70);
  int m = lseek(p->fileD, p->buf_offset,SEEK_SET);
  printf("buffer in main %s \n", buffer);
  printf("lseek = %d\n", m);
  printf("n3 %d\n", n);
  printf("offset3 %d\n", p->buf_offset);
  printf("buffer in main just before write %s \n", buffer);
  n = mywrite(q, buffer, 10);
  printf("n3 %d\n", n);
  printf("offset4 %d\n", q->buf_offset);
  //m = lseek(p->fileD, 10,SEEK_CUR);
  //printf("lseek = %d\n", m);
  //mywrite(p, buffer, 10);
  myclose(p);
  myclose(q);
  return 0;
}
*/
/*
int main(int argc, char *argv[])
{
    char *src_filename, *dst_filename;
    //int src_fd, dst_fd;
    int n;
    char buf[BYTES_PER_ITERATION];
    struct file_info *s;
    struct file_info *d;

    // check command-line args
    if(argc < 3) {
        usage(argv);
        exit(1);
    }
    src_filename = argv[1];
    dst_filename = argv[2];

    // open source and destination file
    s = myopen(src_filename, O_RDONLY);
    if(s->fileD < 0) {
        perror("open");
        exit(2);
    }
    d = myopen(dst_filename, O_CREAT | O_WRONLY);
    if(d->fileD < 0) {
        perror("open");
        exit(3);
    }

    // read from source into destination
    while((n = myread(s, buf, BYTES_PER_ITERATION)) > 0) {
        n = mywrite(d, buf, n);
        if(n < 0) {
            perror("write");
            exit(4);
        }
    }

    if(n < 0) {
        perror("read");
    }

    // clean up
    myclose(s);
    myclose(d);
}

void usage(char *argv[])
{
    printf("usage: %s src_filename dst_filename\n", argv[0]);
}

*/

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
