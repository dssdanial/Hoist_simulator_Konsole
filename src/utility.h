#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>

void sendMessage(int fd, char *pipeName, char *message);
void sendVelocityMessage(int fd, char *pipeName, float velocity, char *mLabel);
void sendPositionMessage(int fd, char *pipeName, float positionX, float positionY);
float poseUpdate(float pose, float velocity);
float getRandomError(int lower, int upper, float data);
void sig_handler(int signo);
void createLogFile(char *fileName, char *mode);
void logData(char *fileName, char *mode);
bool checkTime(int hStart, int mStart, int sStart, int durationSeconds);
int getPID(char *logFileAddress);

/* Description: a function to send a message from one process to another through a named pipe. */
void sendMessage(int fd, char *pipeName, char *message) {
    // Opening the 'pipeName' pipe to write the message
    fd = open(pipeName, O_WRONLY);
    
    // Error Checking
    if (fd < 0) {
        printf("Could not open the %s pipe; errno=%d\n", pipeName, errno);
        exit(1);
    }
    
    // Writing the message
    write(fd, message, strlen(message) + 1);
    
    // Closing the pipe
    close(fd);
}

/* Description: a function to send the 'velocity message' of the 'motor x/z processes' to the 'estimator process'. */
void sendVelocityMessage(int fd, char *pipeName, float velocity, char *mLabel) {
    // Saving the label of the data
    char message[20];
    strcpy(message, mLabel);

    // Converting the float data into a string
    char data[10];
    gcvt(velocity, 2, data);

    // Concatenating the label and the value of the data
    strcat(message, data);

    // Printing the message into the motor x/z process's console
    printf("%s Speed:%s\n", mLabel, data);

    // Sending the velocity message to the target process
    sendMessage(fd, pipeName, message);
}

/* Description: a function to send the 'position message' of the 'estimator process' to the 'inspector process'. */
void sendPositionMessage(int fd, char *pipeName, float positionX, float positionY) {
    // Temporary variable to send the message
    char message[20];

    // Converting the float x-data into a string
    char x[10];
    gcvt(positionX, 6, x);

    // Converting the float z-data into a string
    char z[10];
    gcvt(positionY, 6, z);

    // Creating the complete position message by concatenation of strings
    strcat(message, x);
    strcat(message, ",");
    strcat(message, z);

    // Sending the position message to the target process
    sendMessage(fd, pipeName, message);
}

/* Description: a function to update the x/z robot's position based on the current position and velocity of the robot. */
float poseUpdate(float pose, float velocity) {
    pose = pose + velocity;
    return pose;
}

/* Description: a function to generate a random error in a range and add it to a given data. */
float getRandomError(int lower, int upper, float data) {
    srand(time(NULL));
    float error = ((rand() % (upper - lower + 1) + lower)) / 10.0;
    return error;
}

void createLogFile(char *fileName, char *mode) {
    // In a new run of the program, if the log file exists, remove the old log file and create a new one
    FILE *fp;
    if (!remove(fileName)) {
        printf("The old log file is deleted successfully!\n");
    }

    if (fp = fopen(fileName, mode)) {
        printf("The new log file is created successfully!\n");
        logData(fileName, mode);
    } else {
        printf("Could not create a log file!\n");
    }
}

/* Description: a function to record the essential information of a process into a logfile. */
void logData(char *fileName, char *mode) {
    FILE *fp;
    int pid = getpid();

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Opening the logfile and recording the current activity time of the process
    fp = fopen(fileName, mode);

    // Error Checking
    if (fp < 0) {
        printf("Could not open the %s file; errno=%d\n", fileName, errno);
        exit(1);
    }

    // Writing into the file
    fprintf(fp, "%d,%d,%d,%d\n", pid, tm.tm_hour, tm.tm_min, tm.tm_sec);

    // Closing the file
    fclose(fp);
}

/* Description: a function to check the activity status of a target process based on the last active time of the process and the given time limitation. */
bool checkTime(int hStart, int mStart, int sStart, int durationSeconds) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Comparing the last activity time of the process with a given time limitation
    bool status = (tm.tm_hour - hStart) * 3600 + (tm.tm_min - mStart) * 60 + (tm.tm_sec - sStart) > durationSeconds;

    if (status) {
        printf("Time Over\n");
        return true;
    }

    return false;
}

/* Description: a function to find the PID of the target process. */
int getPID(char *logFileAddress) {
    FILE *fp;
    char buffer[100];

    // Opening the logfile of the target process
    fp = fopen(logFileAddress, "r");

    // Error Checking
    if (fp < 0) {
        printf("Could not open the file; errno=%d\n", errno);
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);
    fread(buffer, 50, 1, fp);
    fclose(fp);

    // Finding the PID of the target process
    char *token;
    token = strtok(buffer, ",");
    int processPID = atoi(token);
    return processPID;
}
