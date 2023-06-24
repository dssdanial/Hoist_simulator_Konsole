#include "./../include/inspection_utilities.h"
#include "./utility.h"

int fdLocation; 

void sig_killhandler(int signo);

int main(int argc, char const *argv[])
{
    int first_resize = TRUE;
    float ee_x, ee_y;

    char location[80];
    char *token;

    char * mLocation = "/tmp/mLocation"; 
    mkfifo(mLocation, 0666); 
  
    struct timeval tv;
    fd_set fds;

    createLogFile("./insp.txt","w+");

    signal(SIGINT, sig_killhandler);

    // Initialize User Interface 
    init_console_ui();

    // Infinite loop
    while(TRUE)
	{	
        fdLocation = open(mLocation,O_RDONLY, O_NONBLOCK);
        /*Error Checking*/ 
        if (fdLocation<0){
            printf("Could not open the 'fdLocation' pipe; errno=%d\n", errno);
            
        }  

        FD_ZERO(&fds); 
        FD_SET(fdLocation, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        select(fdLocation + 1, &fds, NULL, NULL, &tv);

        /*Reading the position data*/
        read(fdLocation, location, 80);
        close(fdLocation);

        /*Extracting the current x position*/ 
        token = strtok(location, ",");
        if(token!=NULL)
            ee_x = atof(token);

        /*Extracting the current z position*/ 
        token = strtok(NULL, ",");
        if(token!=NULL)
             ee_y = atof(token);

        // Get mouse/resize commands in non-blocking mode...
        int cmd = getch();

        // If user resizes screen, re-draw UI
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }
        else if(cmd == KEY_MOUSE) {

            if(getmouse(&event) == OK) {

                if(check_button_pressed(stp_button, &event)) {
                    mvprintw(LINES - 1, 1, "STP button pressed");
                    /*Logging the data*/
                    logData("insp.txt", "w+");
                    /*Send the 'SIGUSR2 signal' to the target process*/
                    kill(getPID("./est.txt"), SIGUSR2);
                    kill(getPID("./mox.txt"), SIGUSR2);
                    kill(getPID("./moz.txt"), SIGUSR2);
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }

                // RESET button pressed
                else if(check_button_pressed(rst_button, &event)) {
                    mvprintw(LINES - 1, 1, "RST button pressed");
                    /*Logging the data*/
                    logData("insp.txt", "w+");
                    kill(getPID("./est.txt"), SIGUSR1);
                    kill(getPID("./mox.txt"), SIGUSR1);
                    kill(getPID("./moz.txt"), SIGUSR1);
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }
            }
        }
        
        // To be commented in final version...
        switch (cmd)
        {
            case KEY_LEFT:
                ee_x--;
                break;
            case KEY_RIGHT:
                ee_x++;
                break;
            case KEY_UP:
                ee_y--;
                break;
            case KEY_DOWN:
                ee_y++;
                break;
            default:
                break;
        }
        
        // Update UI
        update_console_ui(&ee_x, &ee_y);
	}

    // Terminate
    endwin();
    return 0;
}


void sig_killhandler(int signo){
    if (signo == SIGINT){
        printf("I Received SIGINT Signal!\n");
        /*Close the resource*/
        close(fdLocation);
        /*Kill yourself!*/
        kill(getpid(), SIGKILL);
    }
}