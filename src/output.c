#include "./utility.h"

float speedValue = 0;
char speedLabel[2];
float Vx = 0.0;
float Vz = 0.0;
float Xs, Xe = 0.0;
float Zs, Ze  = 0.0;
bool stop_reset_event_control = false;

int fdSpeed, fdLocation; 

char * mSpeed = "/tmp/mSpeed";
char * mLocation = "/tmp/mLocation"; 

void sig_killhandler(int signo);
void sig_resetHandler(int signo);
void sig_stopHandler(int signo);

int main(int argc, char const *argv[])
{
    char speed[80];
    char *token;

    fd_set fds;
    struct timeval tv;
    
    mkfifo(mSpeed, 0666); 
    mkfifo(mLocation, 0666); 

    signal(SIGINT, sig_killhandler);
    signal(SIGUSR1, sig_resetHandler);
    signal(SIGUSR2, sig_stopHandler);

    createLogFile("./est.txt","w+");

    while(1){
        fdSpeed = open(mSpeed,O_RDONLY|O_NONBLOCK); 
        if (fdSpeed<0){
            printf("Could not open the 'mSpeed' pipe; errno=%d\n", errno);
            exit(1); 
        } 

        FD_ZERO(&fds); 
        FD_SET(fdSpeed, &fds);

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        select(fdSpeed + 1, &fds, NULL, NULL, &tv);

        int numBytes = read(fdSpeed, speed, 80);
        close(fdSpeed);

        if(numBytes){
            token = strtok(speed, ",");
            strcpy(speedLabel, token);
            
            token = strtok(NULL, ",");
            speedValue = atoi(token);
        }

        if(strcmp(speedLabel, "X")==0)
        {
            Vx = speedValue;
        }

        Xs = poseUpdate(Xs, Vx);

        if(Xs>40 || Xs<0){
            kill(getPID("./est.txt"), SIGUSR2);
            kill(getPID("./mox.txt"), SIGUSR2);
            kill(getPID("./moz.txt"), SIGUSR2);
        }
        
        Xe = Xs + getRandomError(-1, 1, Xs);

        if(strcmp(speedLabel, "Z")==0)
        {
            Vz = speedValue;
        }

        Zs = poseUpdate(Zs, Vz);

        if(Zs>10 || Zs<0){
            kill(getPID("./est.txt"), SIGUSR2);
            kill(getPID("./mox.txt"), SIGUSR2);
            kill(getPID("./moz.txt"), SIGUSR2);
        }

        Ze = Zs + getRandomError(-1, 1, Zs);

        printf("Vx:%f, Vz:%f, (X_Sensor,Z_Sensor)=(%f,%f), (X_Estimate, Z_Estimate)=(%f,%f)\n", Vx, Vz, Xs,Zs,Xe,Ze);

        sendPositionMessage(fdLocation, mLocation, Xe, Ze);

        logData("est.txt", "w+");
    }
    return 0; 
}
void sig_stopHandler(int signo){
    if(signo == SIGUSR2){
        if(!stop_reset_event_control){
            speedValue = 0.0;
            Vx = 0;
            Vz = 0;
            printf("*** Stop Mode *** Vx:%f, Vz:%f, (X_Sensor,Z_Sensor)=(%f,%f), (X_Estimate, Z_Estimate)=(%f,%f)\n", Vx, Vz, Xs,Zs,Xs,Zs);
        }else{
            printf("\n\nThe Reset Mode is active and Stop Mode is not allowed!\n\n");
        }
    }
}



void sig_resetHandler(int signo){
    if(signo == SIGUSR1){
        stop_reset_event_control = true;
        speedValue = 0.0;
        Vx = -1;
        Vz = -1;
        
        while(Xs>0.0 || Zs>=0){
            if(Xs>=0)
                Xs = poseUpdate(Xs, Vx);
            if(Zs>=0)    
                 Zs = poseUpdate(Zs, Vz);
            printf("*** Reset Mode *** Vx:%f, Vz:%f, (X_Sensor,Z_Sensor)=(%f,%f), (X_Estimate, Z_Estimate)=(%f,%f)\n", Vx, Vz, Xs,Zs,Xs,Zs);
            sendPositionMessage(fdLocation, mLocation, Xs, Zs);
            usleep(500000);
        }
        stop_reset_event_control = false;
        Xs = 0; Xe = 0; Zs = 0; Ze = 0;Vx = 0; Vz = 0;
    }
}

void sig_killhandler(int signo){
    if (signo == SIGINT){
        printf("I Received SIGINT Signal!\n");
        close(fdSpeed);
        close(fdLocation);
        kill(getpid(), SIGKILL);
    }
}