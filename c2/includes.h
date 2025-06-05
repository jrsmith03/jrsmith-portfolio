#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#include "tiny-aes/aes.h"

#define MAX_PASSWORD_LEN 64
#define MAX_USERNAME_LEN 32
#define OUTPUT_BUFFER 4096
#define READ_LENGTH 32

void auth_with_server(char** argv, int sockfd, struct AES_ctx ctx, char* key, char* iv, char* response);
int read_key_file(char* key, char* iv);