#include "major2.h"

// Shell User Settings
char* username;
char cwd[PATH_MAX];
char hostname[HOST_NAME_MAX];
time_t start_time;

// Pipe
int pipes[MAX_PIPES][2];

void sigint_handler(int signum) {
    //printf("Received SIGINT in process %d.\n", getpid());
    printf("\n");
}

int initialize_shell() {
    // Set the start time of the shell
    start_time = time(NULL);
    // set shell current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return EXIT_FAILURE;
    }

    // set the shell username
    username = getlogin();
    if (username == NULL) {
        username = getenv("USER");
    }

    // set the shell hostname
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("gethostname");
        return EXIT_FAILURE;
    }

    // used for ^C signal
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char** argv){
    // shell initialization
    if (initialize_shell() == EXIT_FAILURE) {
        fprintf(stderr, "Failed to initialize shell.\n");
        exit(EXIT_FAILURE);
    }
    if(isatty(STDIN_FILENO)){
        // Interactive Mode
        while(1){
            // print the shell prompt
            printf("%s@%s:%s$ ",username,hostname,cwd);
            char input[MAX_BUFFER];
            if (fgets(input, sizeof(input), stdin) == NULL) {
                // handle input error 
                continue;
            }
            size_t input_len = strlen(input);
            if(input_len >= 512){perror("Input Too Large"); continue;}
            if(input_len <= 0) {continue;}
            if(strcmp(input,"\n") == 0){continue;}
            // Remove the trailing newline character
            input[strcspn(input, "\n")] = '\0';
            printf("%s\n", input);
            parse_input(input);
        }
    }else{
        // shell is non-interactive mode
    }
}

int parse_input(char* input){
    const char outer_delimiters[] = ";"; // command line split
    // Parse Input
    char* token;
    char* outer_saveptr = NULL;

    token = strtok_r(input, outer_delimiters, &outer_saveptr);
    
    while (token != NULL) {
        process_command(token);
        token = strtok_r(NULL, outer_delimiters,&outer_saveptr);
    }
    return EXIT_SUCCESS;
}

int process_command(char* input){
    const char pipe_delimiters[] = "|"; // pipe split

    // Parse Input
    char* token;
    char* pipe_saveptr = NULL;
    token = strtok_r(input, pipe_delimiters, &pipe_saveptr);
    int pipeNum = 0;
    while (token != NULL) {
        if(isStrWhiteSpace(token) != TRUE){
            strtrim(&token);
            process_handler(token,pipeNum);
            pipeNum++;
        }
        
        token = strtok_r(NULL, pipe_delimiters,&pipe_saveptr);
    }
    return EXIT_SUCCESS;
}

int process_handler(char* input,int pipeNum){
    // TODO: setup rest of pipe stuff
    if(pipe(pipes[pipeNum]) == EXIT_FAILURE){
        perror("pipe");
        return EXIT_FAILURE;
    }
    
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        //child process
        // ArgList Memory Allocation
        char** argList =(char**)malloc(MAX_ARGS*sizeof(char*));
        if(argList == NULL){
            perror("taskList allocation error");
            exit(EXIT_FAILURE);
        }

        // Parsing Command Line
        char** tokens = tokenize(input," ");
        if(tokens == NULL){
            perror("Tokenize Error");
            exit(EXIT_FAILURE);
        }
        
        //redirection handling
        int cmd_count = 0;
        for (int i = 0; tokens[i] != NULL; i++)
        {
            if(strcmp(tokens[i],">") == 0){  // Forward Redirection
                
                if(tokens[i+1] == NULL){ // Checking Filename
                    perror("syntax error");
                    free_tokens(i,tokens);
                    exit(EXIT_FAILURE);
                };
                
                // setup the forward redirection
                char* filename = tokens[i+1];
                int file = open(filename,O_WRONLY|O_CREAT,0777);
                if (file < 0) {
                    perror("Error opening file");
                    free_tokens(i,tokens);
                    exit(EXIT_FAILURE);
                }else{
                    // Redirect stdout to the file
                    dup2(file,STDOUT_FILENO);
                    close(file);
                    free(tokens[i]); // free the redirection operator
                    free(tokens[i+1]);  // free the filename arg
                    i++;
                    continue;
                }
            }
            else if(strcmp(tokens[i],"<") == 0){ // Backward Redirection
                //TODO: Setup back redirection
            }else{
                argList[cmd_count] = strdup(tokens[i]);
                cmd_count++;
            }
            
            free(tokens[i]);
        }
        free(tokens); // free the token array

        argList[cmd_count] = NULL; // Null Terminate Arg Array

        // if process/command failed execution
        if(exec_cmd(argList) != EXIT_SUCCESS){
            perror("command execution error");
            exit(EXIT_FAILURE);
        };

    }
    else
    {
        // Parent process
        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        }
        return status;
    }
    //should never really hit this 
    return EXIT_SUCCESS;
}

/*
* Add a new built-in cd command that accepts one optional argument, a 
* directory path, and changes the current working directory to that directory. If no 
* argument is passed, the command will change the current working directory to 
* the user’s HOME directory. You may need to invoke the chdir() system call.
*
* change directory command that will change based off of
* the location at which the user is depending on call
* will go to HOME if parameter is not called
*/
void exec_cd(char* command)
{
    //if there is no input, then automatically call "HOME" directory
    if(command == NULL)
    {
        command == getenv("HOME");
    }

    //if calling [input] with exec_cd() fails, then return an error message 
    if(chdir(command) != 0)
    { perror("cd failed as directory does not exist"); }

    if (chdir(command) == 0 )
    {
        chdir(command);
    }

}


/**
 * @brief Execute a command.
 *
 * This function is responsible for executing various commands based on the provided
 * command name and arguments.
 * @param argv The array of arguments for the command.
 * @return 0 on success.
 */
int exec_cmd(char *const *argv)
{   
    // if argument is null
    if(argv[0] == NULL){
        return EXIT_FAILURE;
    }

    if(strcmp(argv[0],"cd") == 0){printf("cd command Here!\n");}
    else if(strcmp(argv[0],"path") == 0){printf("path command Here!\n");}
    else if(strcmp(argv[0],"myhistory") == 0){printf("myhistory command Here!\n");}
    else if(strcmp(argv[0],"exit") == 0 || strcmp(argv[0],"quit") == 0 ){printf("exitting...!\n"); kill(getppid(),SIGKILL);exit(EXIT_SUCCESS);}
    else if(execvp(argv[0], argv) != EXIT_SUCCESS){return EXIT_FAILURE;};
    return EXIT_SUCCESS;
}
