#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE_COMMAND "mail -H"

#define SORT_POSTFIX "| sort -k 3"

#define COMMAND_SIZE 1024

void print_list_mails(int sort_by_sender)
{
    char command[COMMAND_SIZE];
    if (!sort_by_sender)
        sprintf(command, "%s", BASE_COMMAND);
    else
        sprintf(command, "%s %s", BASE_COMMAND, SORT_POSTFIX);
    FILE *pipe = popen(command, "w");

    pclose(pipe);
}

void write_mail(const char *email, const char *title, const char *message)
{
    char command[1024];
    sprintf(command, "mail -s \"%s\" %s", title, email);
    FILE *pipe = popen(command, "w");

    fputs(message, pipe);
    pclose(pipe);
}

int is_sort_by_sender(const char *arg)
{
    if (!strcmp(arg, "nadawca"))
    {
        return 1;
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc != 2 && argc != 4)
    {
        printf("Invalid number of arguments!\n");
        return -1;
    }

    if (argc == 2)
    {
        int sort_by_sender = is_sort_by_sender(argv[1]);
        print_list_mails(sort_by_sender);
    }
    else
    {
        write_mail(argv[1], argv[2], argv[3]);
    }
}