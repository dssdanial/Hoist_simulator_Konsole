#include "./utility.h"

void sig_killhandler(int signo);

int main(int argc, char const *argv[])
{

  FILE *fp;
  char *token;
  char buffer[100];
  int processPID;
  int lastActivityHour;
  int lastActivityMin;
  int lastActivitySec;
  bool processActivityStatus;
  int inactiveProcessCounter;
  char processLogAddress[5][20] = {"./mox.txt", "./moz.txt", "./est.txt", "./cmd.txt", "./insp.txt"};
  char processPIDList[5][20] = {"", "", "", "", ""};
  
  while(1){
    while (inactiveProcessCounter<5){
      fp = fopen(processLogAddress[inactiveProcessCounter], "r");
      /*Error Checking*/ 
      if (fp<0){
          printf("Could not open the 'mxCommand' pipe; errno=%d\n", errno);
          exit(1); 
      } 

      fseek(fp, 0, SEEK_SET);
      fread(buffer, 50, 1, fp);
      fclose(fp);

      token = strtok(buffer, ",");
      strcpy(processPIDList[inactiveProcessCounter], token);
      processPID = atoi(token);

      token = strtok(NULL,",");
      lastActivityHour = atoi(token);

      token = strtok(NULL,",");
      lastActivityMin = atoi(token);

      token = strtok(NULL,",");
      lastActivitySec = atoi(token);
      
      if(inactiveProcessCounter!=2){ 
          printf("The process %d last activity time is(H,M,S):%d:%d:%d \n",processPID, lastActivityHour, lastActivityMin, lastActivitySec);

          printf("Check Activity Status in last 60 seconds...\n");
          processActivityStatus = checkTime(lastActivityHour, lastActivityMin, lastActivitySec, 60);

          if(processActivityStatus){
            printf("The process %d was inactive for more than 60 seconds... :(\n\n", processPID);
            inactiveProcessCounter = inactiveProcessCounter + 1;
            if(inactiveProcessCounter ==5){  /*If all the processes were inactive in the last 60 seconds*/
              printf("Killing all the process :|\n");
              for(int i=0;i<5;i++){
                /*Send the 'kill signal' to all the target process*/
                printf("Killing process %s\n", processPIDList[i]);
                kill(atoi(processPIDList[i]), SIGINT);
              }
              kill(getpid(), SIGINT);
            }
          }else{
            /*if atleast one process was active in the last 60 seconds, ignore the cheking process */
            printf("Good! The process %d is active :)\n\n", processPID);
            inactiveProcessCounter = 0;
            break;
          }
      }else{
        inactiveProcessCounter = inactiveProcessCounter + 1;
      }
    }
  }
    return 0; 
}


void sig_killhandler(int signo){
    if (signo == SIGINT){
        printf("I Received SIGINT Signal!\n");
        kill(getpid(), SIGKILL);
    }
}
