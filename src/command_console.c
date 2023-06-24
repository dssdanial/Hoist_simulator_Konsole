#include "./../include/command_utilities.h"
#include "./utility.h"

int fdx, fdz; 

void sig_killhandler(int signo);

int main(int argc, char const *argv[])
{
    
    char * mxCommand = "/tmp/mxCommand"; 
    mkfifo(mxCommand, 0666); 

    
    char * mzCommand = "/tmp/mzCommand"; 
    mkfifo(mzCommand, 0666); 

    
    createLogFile("./cmd.txt","w+");
    signal(SIGINT, sig_killhandler);

   
    int first_resize = TRUE;
    init_console_ui();
    while(TRUE)
	{	
       
        int cmd = getch();
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

                
                if(check_button_pressed(vx_decr_btn, &event)) {
                   
                    sendMessage(fdx, mxCommand, "VX-");
                    
                    logData("cmd.txt", "w+");

                    mvprintw(LINES - 1, 1, "Horizontal Speed Decreased");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }

                
                else if(check_button_pressed(vx_incr_btn, &event)) {
                    
                    sendMessage(fdx, mxCommand, "VX+");
                    
                    logData("cmd.txt", "w+");

                    mvprintw(LINES - 1, 1, "Horizontal Speed Increased");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }

                
                else if(check_button_pressed(vx_stp_button, &event)) {
                    
                    sendMessage(fdx, mxCommand, "VXS");
                    
                    logData("cmd.txt", "w+");

                    mvprintw(LINES - 1, 1, "Horizontal Motor Stopped");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }

                
                else if(check_button_pressed(vz_decr_btn, &event)) {
                    
                    sendMessage(fdz, mzCommand, "VZ-");
                    
                    logData("cmd.txt", "w+");

                    mvprintw(LINES - 1, 1, "Vertical Speed Decreased");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }

                
                else if(check_button_pressed(vz_incr_btn, &event)) {
                    
                    sendMessage(fdz, mzCommand, "VZ+");
                    
                    logData("cmd.txt", "w+");

                    mvprintw(LINES - 1, 1, "Vertical Speed Increased");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }

                
                else if(check_button_pressed(vz_stp_button, &event)) {
                    
                    sendMessage(fdz, mzCommand, "VZS");
                    
                    logData("cmd.txt", "w+");
                    
                    mvprintw(LINES - 1, 1, "Vertical Motor Stopped");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }
            }
        }

        refresh();
	}

    
    endwin();
    return 0;
}


void sig_killhandler(int signo){
    if (signo == SIGINT){
        printf("I Received SIGINT Signal!\n");
        
        close(fdx);
        close(fdz);
        kill(getpid(), SIGKILL);
    }
}

