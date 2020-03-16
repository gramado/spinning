

//gcc -o spin spinning.c


#include <stdio.h>
#include <unistd.h>

#include <assert.h> 
#include <errno.h>
#include <stdlib.h>


#    define true 1
#    define false 0

#define ASSERT assert
  
struct __STDIO_FILE2 {
    int fd;
    int eof;
    int error;
    int mode;
    pid_t popen_child;
    char* buffer;
    size_t buffer_size;
    size_t buffer_index;
    int have_ungotten;
    char ungotten;
    char default_buffer[BUFSIZ];
};

typedef struct __STDIO_FILE2 FILE2;

FILE2 *____stdout;
    
    
    
int fflush2 ( FILE2 *stream)
{
    // FIXME: fflush(NULL) should flush all open output streams.
    ASSERT(stream);
    if (!stream->buffer_index)
        return 0;
        
    int rc = write(stream->fd, stream->buffer, stream->buffer_index);
    
    stream->buffer_index = 0;
    stream->error = 0;
    stream->eof = 0;
    stream->have_ungotten = false;
    stream->ungotten = 0;
    if (rc < 0) {
        stream->error = errno;
        return EOF;
    }
    return 0;
}

int fputc2 (int ch, FILE2 *stream)
{
    assert(stream);
    assert(stream->buffer_index < stream->buffer_size);
    stream->buffer[stream->buffer_index++] = ch;
    if (stream->buffer_index >= stream->buffer_size)
        fflush2(stream);
    else if (stream->mode == _IONBF || (stream->mode == _IOLBF && ch == '\n'))
        fflush2(stream);
    if (stream->eof || stream->error)
        return EOF;
    return ch;
}

int putc2(int ch, FILE2* stream)
{   
    return fputc2(ch, stream);
}


void __string( char *string )
{
	int Index;
    for ( Index=0; string[Index] != 0; Index++ )
        putc2 ( string[Index], ____stdout);
}


void advance_cursor (){
	
    static int pos=0;
  
    char cursor[4]={'/','-','\\','|'};
    
	printf ("%c\b", cursor[pos]);
    fflush (stdout);
    pos = (pos+1) % 4;
}


int main ( int argc, char *argv[]){
  
    int i=0;
    
    

    
    ____stdout = (FILE2 *) malloc(4096);
    
    
    ____stdout->fd = 1;  //stdout;
    ____stdout->buffer = ( char * )  malloc(4096); 
    ____stdout->buffer_index = 0;
    ____stdout->buffer_size = 4096;
    ____stdout->error = 0;
    ____stdout->eof = 0;
    ____stdout->have_ungotten = false;
    ____stdout->ungotten = 0;   
    ____stdout->mode |= _IOLBF;
    

    putc2 ('X', ____stdout);
    putc2 ('\n', ____stdout);

    __string("This is a string\n");

	for (i=0; i<100; i++)
	{
        advance_cursor ();
        usleep (100000);
    };


    printf ("\n");
    return 0;
}



