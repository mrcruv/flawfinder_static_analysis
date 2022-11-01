﻿// fixed code fragment:

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <strsafe.h>

#define FUNC1_BUFFER_SIZE 1024
#define MAIN_BUFFER_SIZE 10

void func1() {	
    char buffer[FUNC1_BUFFER_SIZE]; // flawfinder: ignore
    int ret;
    HRESULT hr;
    
    ret = printf("Please enter your user id :"); // flawfinder: ignore
    
    if (ret < 0) exit(EXIT_FAILURE); // actually the condition to be verified would be 'ret < strnlen("Please enter your user id :", 28)'
    
    if (fgets(buffer, FUNC1_BUFFER_SIZE, stdin) == NULL) exit(EXIT_FAILURE);
    
    if (!isalpha(buffer[0])) {
        char errormsg[1044]; // flawfinder: ignore
        
        hr = StringCbCopyA(errormsg, FUNC1_BUFFER_SIZE, buffer);
        
        if (FAILED(hr)) exit(EXIT_FAILURE);
        
        hr = StringCbCatA(errormsg, 1044, " is not  a valid ID");
        
        if (FAILED(hr)) exit(EXIT_FAILURE);
 	}
}

void func2(int f2d) {
    char *buf2;
    size_t len;
    int ret;

    ret = read(f2d, &len, sizeof(len)); // flawfinder: ignore
    
    if (ret == -1) {
    	perror("error on first read (func2)");
		exit(EXIT_FAILURE); // errno is set
	}
	
    buf2 = calloc(len + 1, sizeof(char));
    
    if (buf2 == NULL) exit(EXIT_FAILURE);
    
    ret = read(f2d, buf2, len); // flawfinder: ignore
    
    if (ret == -1) {
    	perror("error on second read (func2)");
		exit(EXIT_FAILURE);
	}
	
    buf2[len] = '\0';
}

void func3(int f3d){
    char *buf3;
    int len, ret;
        
    ret = read(f3d, &len, sizeof(len)); // flawfinder: ignore
    
    if (ret == -1) {
    	perror("error on first read (func3)");
		exit(EXIT_FAILURE);
	}

    if (len > 8000) {
        fprintf(stderr, "error on too large length"); // flawfinder: ignore
        exit(EXIT_FAILURE);
    }

    buf3 = (char *)calloc(len, sizeof(char));
    
    if (buf3 == NULL) exit(EXIT_FAILURE);
    
    ret = read(f3d, buf3, len); // flawfinder: ignore
    
    free(buf3);
    buf3 = NULL;
    
    if (ret == -1) {
    	perror("error on second read (func3)");
		exit(EXIT_FAILURE);
	}
}

void main() {
    char *foo = "fooooooooooooooooooooooooooooooooooooooooooooooooooo";
    char *buffer = (char *)calloc(MAIN_BUFFER_SIZE, sizeof(char));
    HRESULT hr;
    
    if (buffer == NULL) exit(EXIT_FAILURE);
		
	hr = StringCbCopyA(buffer, MAIN_BUFFER_SIZE, foo);
	
	free(buffer);
	buffer = NULL;

    if (FAILED(hr) && hr != STRSAFE_E_INSUFFICIENT_BUFFER) exit(EXIT_FAILURE);
    
    func1();
    func3(strnlen(foo, 53));
    exit(EXIT_SUCCESS);
}