#include "./utility.h"
char command[10];
float velocity = 0.0;
int fdCommand, fdSpeed; 
void sig_killhandler(int signo);
void sig_resetHandler(int signo);
void sig_stopHandler(int signo);

  
int main(int argc, char const *argv[])
{
    
    char * mxCommand = "/tmp/mxCommand";
    mkfifo(mxCommand, 0666);

    
    char * mSpeed = "/tmp/mSpeed"; 
    mkfifo(mSpeed, 0666);
    signal(SIGINT, sig_killhandler);
    signal(SIGUSR1,sig_resetHandler);
    signal(SIGUSR2, sig_stopHandler);
    createLogFile("./mox.txt","w+");
    
    while(1){
        
        fdCommand = open(mxCommand,O_RDONLY); 
       
        if (fdCommand<0){
            printf("Could not open the 'mxCommand' pipe; errno=%d\n", errno);
            exit(1); 
        } 
        
        read(fdCommand, command, 10);
        
        close(fdCommand);

       
        if(strcmp(command,"VX-") == 0)
        {
           
            velocity = velocity - 1.0;
            
            if(velocity < -2.0)
            {
                velocity = -2.0;
                printf("MAX Speed!\n");
            }
        }

        
        if(strcmp(command,"VX+") == 0)
        {
            
            velocity = velocity + 1.0;
            
            if(velocity > 2.0)
            {
                velocity = 2.0;
                printf("MAX Speed!\n");
            }
        }

        
        if(strcmp(command,"VXS") == 0)
        {
            velocity = 0.0;    
            printf("Stopped!\n");
        }

        
        sendVelocityMessage(fdSpeed, mSpeed, velocity, "X,"); 
        
        logData("mox.txt", "w+");
    }
    return 0; 
}

void sig_killhandler(int signo){
    if (signo == SIGINT){
        printf("I Received SIGINT Signal!\n");
        close(fdCommand);
        close(fdSpeed);
        kill(getpid(), SIGKILL);
    }
}

void sig_resetHandler(int signo){
    if(signo == SIGUSR1){
        velocity = 0.0;
        strcpy(command, "VXS");
        printf("*** Reset Mode ***\n");
    }
}

void sig_stopHandler(int signo){
    if(signo == SIGUSR2){
        velocity = 0.0;
        strcpy(command, "VXS");
        printf("*** Stop Mode ***\n");
    } 
}

