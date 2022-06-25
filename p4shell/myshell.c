#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));
}

char **parse_line(char *line) {
    int buffer_size = 514;
    char **parsed_arr = malloc(sizeof(char*) * buffer_size);
    char *token;
    token = strtok(line, " \t\r\n");
    int index = 0;
while(token!= NULL) {
    parsed_arr[index] = token;
    index++;
    token = strtok(NULL, " \t\r\n");
}
parsed_arr[index]=NULL;
return parsed_arr;
}

char **multiple_commands(char* line) {
    int buffer_size = 514;
    char **parsed_arr = malloc(sizeof(char*) * buffer_size);
    char *token;
    token = strtok(line, ";");
    int index = 0;
        while(token != NULL) {
            parsed_arr[index] = token;
            index++;
            token = strtok(NULL, ";");
        } if (index == 0) {
            parsed_arr[0] = line;
            parsed_arr[1] = NULL;
        } else {
            parsed_arr[index] = NULL;
        }
    return parsed_arr;
}

int built_in(char **parsed_arr) {
    if (strcmp(parsed_arr[0], "exit") == 0) {
            exit(0);
        }
     else if (strcmp(parsed_arr[0], "cd") == 0) {
        if (parsed_arr[1] == NULL) {
            int num = chdir(getenv("HOME"));
            if (num != 0) {
                char error_message[30] = "An error has occurred\n";
                write(STDOUT_FILENO, error_message, strlen(error_message));
            }
        } else if (parsed_arr[2] != NULL) {
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
        }
        else {
            int num = chdir(parsed_arr[1]);
            if (num != 0) {
                char error_message[30] = "An error has occurred\n";
                write(STDOUT_FILENO, error_message, strlen(error_message));
            }
        }
    } else if (strcmp(parsed_arr[0], "pwd") == 0) {
            char cwd[512];
            getcwd(cwd, sizeof(cwd));
            myPrint(cwd);
            myPrint("\n");
        } else {
        return 1;
    }
    return 0;
}

int init_process(char **argv) {
    int i = built_in(argv);
    if (i == 0) {
        return 1;
    }
    pid_t pid = fork();
    int status;
    if (pid == 0) {
        int exec = execvp(argv[0], argv);
        if (exec == -1) {
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
        } 
        } else if (pid < 0) {
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
        } else {
            waitpid(pid, &status, WUNTRACED);
       }
    return 1;
}

int batchcmd(char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    char cmd_buff[514];
    char* pinput;
    while((pinput = fgets(cmd_buff, 514, fp))!= NULL) {
        /*if (strlen(pinput) == 512 && pinput[513] != '\n') {
        while (strlen(pinput) == 512 && pinput[513] != '\n') {
                   myPrint(fgets(cmd_buff, 514, fp));
                }
                char error_message[30] = "An error has occurred\n";
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    continue;
            } else {*/
        char *pinput_cpy = malloc(sizeof(char) * 514);
        pinput_cpy = strcpy(pinput_cpy, pinput);
        char* token = strtok(pinput_cpy, " \t\r\n");
        if (token == NULL){
            continue;
        }
        myPrint(pinput);
        char** parse_multiple_commands = multiple_commands(pinput);
        int i = 0;
        while (parse_multiple_commands[i] != NULL) {
            char** parse = parse_line(parse_multiple_commands[i]);
            i++;
            if(parse != NULL)
            init_process(parse);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) 
{
    char cmd_buff[514];
    char *pinput;
    if(argv != NULL) {
    batchcmd(argv);
    }
    else
    {
        while (1) {
        myPrint("myshell> ");
        pinput = fgets(cmd_buff, 514, stdin);
        if (!pinput) {
            exit(0);
        }
        int i = 0;
        char** parse_multiple_commands = multiple_commands(pinput);
        while (parse_multiple_commands[i] != NULL) {
            char** parse = parse_line(parse_multiple_commands[i]);
            i++;
            init_process(parse);
        }
    } 
    }
}












