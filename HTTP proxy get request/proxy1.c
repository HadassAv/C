#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define BUFLEN 1000 //maximum response line size
#define BUFF 100

/**
 * Gets flagHost, flagPath, flagPort, flagRequest,  flagCpyPath,  flagTemp,  flagTemp2,  host,  path,  port,  request, cpyPath,  temp, temp2
 * Releases allotted allocations in case of failure.
 * @param flagHost, flagPath, flagPort,  flagRequest,  flagCpyPath,  flagTemp,  flagTemp2,  host,  path,  port,  request, cpyPath,  temp, temp2
 */
void freeMethod(int flagHost, int flagPath, int flagPort, int flagRequest, int flagCpyPath, int flagTemp, int flagTemp2, char *host, char *path, char *port, char *request, char *cpyPath, char *temp, char *temp2)
{
    if (flagHost == 1)
        free(host);
    if (flagPath == 1)
        free(path);
    if (flagPort == 1)
        free(port);
    if (flagRequest == 1)
        free(request);
    if (flagCpyPath == 1)
        free(cpyPath);
    if (flagTemp == 1)
        free(temp);
    if (flagTemp2 == 1)
        free(temp2);
    exit(EXIT_FAILURE);
}

/**
 * Gets name.
 * check if the name is file
 * @param name
 * @return 0 | 1 | -1
 */
int isFile(const char* name)
{
    DIR* directory = opendir(name);
    if(directory != NULL) //if is a directory
    {
        closedir(directory);
        return 0;
    }
    if(errno == ENOTDIR) //if it's a file
    {
        return 1;
    }
    return -1;
}

/**
 * Gets ptr.
 * check if malloc success or nr not.
 * @param ptr
 * @return needFree
 */
int errorMalloc(char *ptr)
{
    int needFree = 0;
    if(ptr == NULL)
    {
        perror("Malloc failed: \n");
        needFree = 1;
    }
    return needFree;
}

/**
 * Gets fd.
 * check if fopen success or nr not.
 * @param fd
 * @return 0 | 1
 */
int checkSuccessOpen(FILE *fd)
{
    int needFinish = 0;
    if (fd == NULL) //Success or not
    {
        perror("Failed: fopen\n");
        needFinish = 1;
    }
    return needFinish;
}

/**
 * Gets str.
 * print.
 * @param str
 */
void error(char* str)
{
    perror(str);
    printf("\n");
}

/**
 * Gets path.
 * Count the number of slashers ("/") in the path
 * @param path
 * @return count
 */
int counterNumSlash(char *path)
{
    int count = 0;
    char *token;
    token = strtok(path, "/"); //Will cut to the slash
    while (token != NULL)
    {
        count++; //Until the next word is NULL
        token = strtok(NULL, "/"); //Will cut to the slash
        if (token == NULL)
        {
            break;
        }
    }
    return count; //Will return the number of slashers
}

/**
 * Gets host, port and path, flagHost, flagPath, flagPort, flagRequest, port, request.
 * construct an HTTP request and print it to stdout
 * @param host, path, flagHost, flagPath, flagPort, flagRequest, port, request
 * @return fd
 */
FILE* createFolders(char *host, char *path, int flagHost, int flagPath, int flagPort, int flagRequest, char *port, char *request)
{
    FILE* fd;
    char *temp, *temp2, *token, *cpyPath, *v;
    int count, needFree, flagOpenW, flagCpyPath = 0, flagTemp = 0, flagTemp2 = 0;

    mkdir(host, 0777); //Create a host folder
    if (strcmp(path, "/") == 0) //If there is no path
    {
        temp = (char*)malloc(sizeof(char)*(strlen(host)+13));
        needFree = errorMalloc(temp); //Check if the assignment was successful
        if (needFree == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, 0, 0, host, path, port, request, NULL, NULL, NULL);
        }
        flagTemp = 1;
        strcpy(temp, host);
        strcat(temp, "/index.html");
        fd = fopen(temp, "w+"); //Generate a file with an index.html extension
        flagOpenW = checkSuccessOpen(fd); //success or not
        if (flagOpenW == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, flagTemp, 0, host, path, port, request, NULL, temp, NULL);
        }
        free(temp);
    }
    else //If there is a path
    {
        cpyPath = (char*) malloc(sizeof(char)* strlen(path)+1);
        needFree = errorMalloc(cpyPath); //Check if the assignment was successful
        if (needFree == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, 0, 0, host, path, port, request, NULL, NULL, NULL);
        }
        flagCpyPath = 1;
        strcpy(cpyPath, path);
        count = counterNumSlash(path); //Send to a method that calculates the number "\"
        if (count == 1) //If there is one slash
        {
            temp = (char*)malloc(sizeof(char)*(strlen(host)+strlen(cpyPath)+1));
            needFree = errorMalloc(temp); //Check if the assignment was successful
            if (needFree == 1)
            {
                freeMethod(flagHost, flagPath, flagPort, flagRequest, flagCpyPath, 0, 0, host, path, port, request, cpyPath, NULL, NULL);
            }
            strcpy(temp, host);
            strcat(temp, "/");
            strcat(temp, path);
            fd = fopen(temp, "w+"); //A file is opened in the name of the path
            flagOpenW = checkSuccessOpen(fd); //Check if the opening was successful
            if (flagOpenW == 1)
            {
                freeMethod(flagHost, flagPath, flagPort, flagRequest, flagCpyPath, 0, 0, host, path, port, request, cpyPath, NULL, NULL);
            }
        }
        else //If there is more than one slash
        {
            temp = (char*)malloc(sizeof(char)*(strlen(host)+strlen(cpyPath)+2));
            needFree = errorMalloc(temp); //Check if the assignment was successful
            if (needFree == 1)
            {
                freeMethod(flagHost, flagPath, flagPort, flagRequest, flagCpyPath, 0, 0, host, path, port, request, cpyPath, NULL, NULL);
            }
            flagTemp = 1;
            temp2 = (char*)malloc(sizeof(char)*(strlen(host)+strlen(cpyPath)+2));
            needFree = errorMalloc(temp2); //Check if the assignment was successful
            if (needFree == 1)
            {
                freeMethod(flagHost, flagPath, flagPort, flagRequest, flagCpyPath, flagTemp, 0,host, path, port, request, cpyPath, temp, NULL);
            }
            flagTemp2 = 1;
            strcpy(temp, host);
            mkdir(temp, 0777); //create folder of host

            token = strtok(cpyPath, "/"); //cut the path
            strcat(temp, "/");
            strcat(temp, token);
            mkdir(temp, 0777); //We will create a folder according to the path each time
            while (token != NULL) //We will unpack the path
            {
                token = strtok(NULL, "/");
                if (token == NULL)
                {
                    break;
                }
                count--; //We will subtract from the number of slashes
                if (count == 1) //If we're in the last slash
                {
                    strcpy(temp2, temp);
                    v = token;
                    break;
                }
                strcpy(temp2, temp);
                strcat(temp2, "/");
                strcat(temp2, token); //We will chain the continuation of the path
                mkdir(temp2, 0777);
                strcpy(temp, temp2); //We will continue to the next section of the path
            }
            strcat(temp2, "/");
            strcat(temp2, v);
            fd = fopen(temp2, "w+"); //A file is opened for writing and reading according to the path we received
            flagOpenW = checkSuccessOpen(fd); //Check if he was able to open
            if (flagOpenW == 1)
            {
                freeMethod(flagHost, flagPath, flagPort, flagRequest, flagCpyPath, flagTemp, flagTemp2, host, path, port, request, cpyPath, temp, temp);
            }
        }
        free(temp);
        free(temp2);
        free(cpyPath);
    }
    return fd; //Will return the file he created
}

/**
 * Gets host, port and path, flagHost, flagPath, flagPort,, flagRequest, port, request.
 * construct an HTTP request and print it to stdout
 * @param host, path, flagHost, flagPath, flagPort, flagRequest, port
 */
char* createHttpRequest(char *host, char *path, int flagHost, int flagPath, int flagPort, int flagRequest, char *port)
{
    char *httpRequest;
    int needFree = 0;

    if (strcmp(path, "/") == 0) //If there is no path
    {
        httpRequest = (char*) malloc(sizeof(char)*strlen(host)+50); //Allocation of the appropriate size
        needFree = errorMalloc(httpRequest); //Check if successful
        if (needFree == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
        }
        sprintf(httpRequest, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", host); //Creates an HTTP request
        printf("HTTP request =\n%s\nLEN = %d\n", httpRequest, (int)strlen(httpRequest)); //Prints as requested
    }
    else
    {
        httpRequest = (char*) malloc(sizeof(char)*(strlen(host)+strlen(path)+50)); //Allocation of the appropriate size
        needFree = errorMalloc(httpRequest); //Check if successful
        if (needFree == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
        }
        sprintf(httpRequest, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", path, host); //Creates an HTTP request
        printf("HTTP request =\n%s\nLEN = %d\n", httpRequest, (int)strlen(httpRequest)); //Prints as requested
    }
    return httpRequest;
}

/**
 * Gets host, port and path, flagHost, flagPath, flagPort, What the method do:
 * construct an HTTP request and print it to stdout
 * Connect to the server
 * Send the HTTP request to the server
 * Receive an HTTP response
 * Save the file locally
 * Display the response on the screen
 * Print the length of the response
 * @param host, port, path, flagHost, flagPath, flagPort
 */
void connectToServer(char *host, char *port, char *path, int flagHost, int flagPath, int flagPort)
{
    int rc, sockfd, flagRequest = 0, needFree = 0;
    char rbuf[BUFLEN] = {0};
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *back, *request;
    FILE *fd;

    back = createHttpRequest(host, path, flagHost, flagPath, flagPort, flagRequest, port); //Create an HTTP request
    request = (char*) malloc(sizeof(char)*strlen(back)+1);
    needFree = errorMalloc(request); //Check if the assignment was successful
    if (needFree == 1)
    {
        freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, request, NULL, NULL, NULL);
    }
    flagRequest = 1;
    strcpy(request, back);
    free(back);

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //Creating a socket
    if (sockfd < 0) //Check if successful
    {
        error("socket failed\n");
        freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, 0, 0, host, path, port, request, NULL, NULL, NULL);
    }
    //Connect to server
    server = gethostbyname(host); //Sending the server
    if (server == NULL) //Check if successful
    {
        fprintf(stderr, "ERROR, no such host\n");
        freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, 0, 0, host, path, port,request, NULL, NULL, NULL);
    }
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(port)); //Sending the port

    rc = connect(sockfd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)); //Creating a connection
    if (rc < 0) //Check if successful
    {
        error("connect failed: \n");
        freeMethod(flagHost, flagPath, flagPort, flagRequest, 0, 0, 0, host, path, port, request, NULL, NULL, NULL);
    }
    write(sockfd, request, strlen(request)+1); //Writing the request to socket
    free(request);
    read(sockfd, rbuf, 15); //Reading the first 15 characters into the rbuf
    int size;
    if (strcmp(rbuf, "HTTP/1.0 200 OK") == 0) //Check that the message is correct and starts with: HTTP/1.0 200 OK
    {
        fd = createFolders(host, path, flagHost, flagPath, flagPort, flagRequest, port, request); //We will create folders
        char *headers = "\r\n\r\n";
        char *patp = headers;
        size = 0;
        while ((rc = read(sockfd, rbuf, 999)) > 0) //Called from the socket into the buffer
        {
            for (int i = 0; i < rc; i++) //put into file without headers
            {
                if (*patp == 0)
                {
                    fwrite(rbuf + i, 1, rc - i, fd);
                    break;
                }
                else if (rbuf[i] == *patp) ++patp;
                else patp = headers;
            }
            rbuf[rc] = '\0';
            printf("HTTP/1.0 200 OK");
            printf("%s", rbuf); //We will print all the response we received
            size+= strlen(rbuf);
        }
        printf("\n Total response bytes: %d\n", size+15);
        fclose(fd);
    }
    else //If the message is not correct we will just print to the screen
    {
        printf("%s", rbuf);
        size = 0;
        while ((rc = read(sockfd, rbuf, 999)) > 0) //We will read the whole response
        {
            rbuf[rc] = '\0';
            printf("%s", rbuf); //We will print all the response we received
            size+= strlen(rbuf);
        }
        printf("\n Total response bytes: %d\n", size);
    }
    if (flagPath == 1)
        free(path);
    free(host);
    close(sockfd);
    exit(EXIT_SUCCESS);
}

/**
 * Gets host, port and path, flagHost, flagPath, flagPort.
 * Check if the file appears in the local filesystem according to host and path.
 * if yes -
 * construct HTTP response with the requested file
 * else -
 * Connect to the server method
 * @param host, port, path, flagHost, flagPath, flagPort
 */
void checkIfFileExists(char *host, char *port, char *path, int flagHost, int flagPath, int flagPort)
{
    char *fullPath;
    FILE *fd;
    char buffer[BUFLEN]; //The size of buffer is 1000
    int needFree, res = 0, num;
    char str[BUFF];

    if (strcmp(path, "/") == 0) //If there is no path
    {
        fullPath = (char*) malloc(sizeof(char)*strlen(host)+12); //Assignment the size of the host+12
        needFree = errorMalloc(fullPath); //Check if the assignment was successful
        if (needFree == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, NULL, NULL, NULL,NULL);
        }
        strcpy(fullPath, host);
        strcat(fullPath, "/index.html"); //Will be threaded to shift the index.html
    }
    else
    {
        fullPath = (char*) malloc(sizeof(char)*(strlen(host)+strlen(path))+2); //Assignment the size of the host and path
        needFree = errorMalloc(fullPath); //Check if the assignment was successful
        if (needFree == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
        }
        strcpy(fullPath, host);
        strcat(fullPath, "/");
        strcat(fullPath, path); //We will link to the fullPath the host along with the path
    }
    num = isFile(fullPath);
    if (access(fullPath, F_OK) == 0 && num == 1) //The file appears in the local filesystem according to host and path
    {
        fd = fopen(fullPath, "r"); //Open the existing file for reading
        needFree = checkSuccessOpen(fd);
        if (needFree == 1)
        {
            free(fullPath);
            freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
        }
        printf("File is given from local filesystem\n"); //We will print a message that the file is found
        fseek(fd, 0L, SEEK_END);
        res = ftell(fd); //Calculating the size of the file
        sprintf(str, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n", res);
        printf("%s", str); //Will print some characters in the file
        fclose(fd);
        fd = fopen(fullPath, "r"); //Open the existing file for reading
        needFree = checkSuccessOpen(fd);
        if (needFree == 1)
        {
            free(fullPath);
            freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
        }
        while (fgets(buffer, BUFLEN, fd) != NULL) //We will print everything that is written in the file
        {
            printf("%s", buffer); //We will print what we read
        }
        printf("\n Total response bytes: %d\n", (int)(strlen(str)+res)); //Will print a final number of characters (what came out + 37)
        fclose(fd); //Close file
        free(host); //We will release the resources we allocated before the end
        free(fullPath);
        if (flagPath == 1)
            free(path);
        if (flagPort == 1)
            free(port);
        exit(EXIT_SUCCESS); //We'll finish successfully
    }
    else //If not - need to connect to server
    {
        free(fullPath);
        connectToServer(host, port, path, flagHost, flagPath, flagPort); //Send to method that do it
    }
}

/**
 * Gets url and divide it to: host, port and path.
 * if there is no port put 80.
 * if there is no path put /.
 * @param url
 */
void divideToHostPortPath(char* url)
{
    char *token, *host, *path = "/", *port = "80", *found;
    int firstTime = 0, flagHost = 0, flagPath = 0, flagPort = 0, needFree;

    token = strtok(url, "/"); //cut the protocol - http
    while (token != NULL) //Divides the URL into a host and a path
    {
        if(firstTime != 1) //Will be distributed to the host for the first time
        {
            token = strtok(NULL, "/"); //cut the host from the url
            if (token == NULL) //if there is no host - print Usage
            {
                perror("ERROR: no such host\n");
                freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
                exit(EXIT_FAILURE);
            }
            host = (char*)malloc(sizeof(char)*strlen(token)+1); //else - allocate place to host
            needFree = errorMalloc(host); //check if malloc succeed
            if (needFree == 1)
            {
                freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
            }
            flagHost = 1;
            strcpy(host, token); //copy token to host
        }
        firstTime = 1; //Will no longer go into the If of the first time that has already cut the host
        token = strtok(NULL, "\n"); //Will cut to the end of the line. That is, the whole path
        if (token == NULL) //If the token is equal to NULL it will end the loop
        {
            break;
        }
        path = (char*)malloc(sizeof(char)*strlen(token)+1); //Allocation to path
        needFree = errorMalloc(path); //check if malloc succeed
        if (needFree == 1)
        {
            freeMethod(flagHost, flagPath, flagPort, 0, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
        }
        flagPath = 1;
        strcpy(path, token);
    }

    found = strchr(host, ':'); //There is a port
    if (found != NULL)
    {
        token = strtok(host, ":"); //If the host contains: We would like to split the port
        host = (char*)realloc(host, sizeof(char)*(strlen(token)+1));
        strcpy(host, token); //The host will be without the port
        token = strtok(NULL, "\n"); //Puts the port
        if (token != NULL) //If there is indeed a port
        {
            port = (char*) malloc(sizeof(char)*strlen(token)+1); //Allocation the size of the port
            needFree = errorMalloc(port); //check if malloc succeed
            if (needFree == 1)
            {
                freeMethod(flagHost, flagPath, flagPort,  0, 0, 0, 0, host, path, port, NULL, NULL, NULL, NULL);
            }
            flagPort = 1;
            strcpy(port, token);
        }
    }
    checkIfFileExists(host, port, path, flagHost, flagPath, flagPort); //Send to method that check if the file appears in the local filesystem according to host and path
}

int main(int argc, char *argv[])
{
    char *url = argv[1]; //We got a URL from the user

    if (argc != 2) //In case no URL is received we will print an appropriate message
    {
        printf("Usage: proxy <URL> \n");
        exit(EXIT_FAILURE);
    }
    if (url[strlen(url)-1] == '/') //If the path ends in slash we will lower it
    {
        url[strlen(url)-1] = '\0';
    }
    divideToHostPortPath(url); //We will send the URL to the method that breaks it down into host, port and path
    return 0;
}
