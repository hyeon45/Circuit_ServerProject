#include <stdio.h>
#include <stdlib.h>
#include "filetobuf.h"

char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr) return NULL;

    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, 1, length, fptr);
    fclose(fptr);
    buf[length] = '\0';
    return buf;
}
