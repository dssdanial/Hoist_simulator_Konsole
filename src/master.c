/**
 * Spawns a child process to execute a given program.
 *
 * @param program The path to the program to be executed.
 * @param arg_list Null-terminated list of arguments to be passed to the program.
 * @return The PID of the spawned process on success, 1 on error.
 */


#include "utility.h"

int spawn(const char * program, char * arg_list[]) {
    pid_t child_pid = fork();
    
    switch(child_pid) {
        case -1:  // an error occurred
            perror("Error while forking...");  // Print error message if fork fails
            return 1;
        case 0:  // this is the child process
            if(execvp (program, arg_list) == -1) {
                perror("Exec failed");  // Print error message if execvp fails
                return 1;
            }
            break;  // unnecessary because of return, but good to have for readability
        default:  // this is the parent process
            return child_pid;  // Return the child process ID to the parent
    }
}

/**
 * Main function. Creates a log file, defines command line arguments, 
 * spawns multiple processes, and waits for them to complete.
 * 
 * @return 0 on successful execution.
 */
int main() {
  createLogFile("./mas.txt","w+");
  logData("mas.txt","w+");

  /* Defines the command line's arguments for different process */
  char * arg_list_command[] = { "/usr/bin/konsole", "-e", "./bin/command", NULL };
  char * arg_list_inspection[] = { "/usr/bin/konsole", "-e", "./bin/inspection", NULL };
  char * arg_list_motorx[] = { "/usr/bin/konsole", "-e", "./bin/motorx", NULL };
  char * arg_list_motorz[] = { "/usr/bin/konsole", "-e", "./bin/motorz", NULL };
  char * arg_list_est[] = { "/usr/bin/konsole", "-e", "./bin/output", NULL };
  char * arg_list_wdg[] = { "/usr/bin/konsole", "-e", "./bin/monitor", NULL };
 
  /* Creates different processes */
  pid_t pid_cmd = spawn("/usr/bin/konsole", arg_list_command);
  pid_t pid_insp = spawn("/usr/bin/konsole", arg_list_inspection);
  pid_t pid_mox = spawn("/usr/bin/konsole", arg_list_motorx);
  pid_t pid_moz = spawn("/usr/bin/konsole", arg_list_motorz);
  pid_t pid_est = spawn("/usr/bin/konsole", arg_list_est);
  pid_t pid_wdg = spawn("/usr/bin/konsole", arg_list_wdg);

  int pids[] = {pid_cmd, pid_insp, pid_mox, pid_moz, pid_est, pid_wdg};
  int statuses[sizeof(pids)/sizeof(pids[0])];
  size_t num_pids = sizeof(pids)/sizeof(pids[0]);

  for(size_t i = 0; i < num_pids; ++i) {
      waitpid(pids[i], &statuses[i], 0);
  }

  return 0;
}
