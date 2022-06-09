#pragma once

#include "../lib/functions/convert.hpp"
#include "internal.hpp"

// commands
#include "commands/clear.cpp"
#include "commands/credits.cpp"
#include "commands/exit.cpp"
#include "commands/help.cpp"
#include "commands/ls.cpp"
#include "commands/test.cpp"

static int been_there_done_that = 0;

void display_host() {
    // will be /fls0/path/to/current/directory/
    terminal->ClearBuffer();
    terminal->SetColor(color(0,255,0));
    char host[16] = "root@cp:";
    terminal->WriteChars(host, true);

    terminal->SetColor(color(0,0,255));
    terminal->WriteChars(g_path, true);

    terminal->SetColor(0xFFFF); //white
    // add "$" character
    terminal->WriteBuffer('$', false);
    // add space
    terminal->WriteBuffer(' ', false);
    
    // reset bufferPosition
    terminal->bufferInPos = 0;
}

int psuedo_main(int argc, char **argv)
{
    char *s = argv[0];
    char *name = argv[0];
    const struct Applet *a = applets;

    while (*s != '\0') {
	if (*s++ == '/')
	    name = s;
    }

    while (a->name[0] != 0) {
        if (strcmp(name, a->name) == 0) {
            terminal->ClearBuffer();	
            int status;
            status = ((*(a->main)) (argc, argv));
            if (status < 0) {
                terminal->ClearBuffer();
                char err[ARGV_SIZE];
                strcat(err, a->name);
                strcat(err, ": An error occurred\n");
                terminal->WriteChars(err);
            }
            return(status);
        }
        a++;
    }
    return (cpshell_main(argc, argv));
}

int cpshell_main(int argc, char **argv)
{
    argc--;
    argv++;

    if (been_there_done_that == 1 || argc < 1) {
        Applet *a = applets;
        terminal->ClearBuffer();
        char msg[] = "Currently defined functions:\n";
        terminal->WriteChars(msg);

        char cmds[APPLET_SIZE];
        while (a->name[0] != 0) {
            strcpy(cmds, (a++)->name);
            // check if terminal->bufferCX is at the end of the line + 2 for ', '
            if ((terminal->bufferCX + strlen(a->name) + 2) >= terminal->xmax) {
                terminal->WriteBuffer('\n', false);
                terminal->ClearBuffer();
            } else {
                strcat(cmds, ", ");
            }
            terminal->WriteChars(cmds, true);
        }
        // remove last comma
        terminal->RemoveLast();
        terminal->RemoveLast();
        terminal->WriteBuffer('\n', false);
        return -1;
    } else {
        /* If we've already been here once, exit now */
        been_there_done_that = 1;
        return (psuedo_main(argc, argv));
    }
}

void cpshell_init() {
    // init applets
    strcpy(applets[0].name, "help");
    applets[0].main = help_main;
    strcpy(applets[1].name, "test");
    applets[1].main = test_main;
    strcpy(applets[2].name, "clear");
    applets[2].main = clear_main;
    strcpy(applets[3].name, "exit");
    applets[3].main = exit_main;
    strcpy(applets[4].name, "credits");
    applets[4].main = credits_main;
    strcpy(applets[5].name, "ls");
    applets[5].main = ls_main;

    memset(&applets[6], 0, sizeof(Applet));

    // init file system
    // Reference: SnailMath/filemgr

    //initialize g_path
	memset(g_path,0,sizeof(g_path));
	strcpy (g_path, "\\fls0\\");

    //convert from char to wchar
    for(int i=0; g_path[i]!=0; i++){
        wchar_t ch = g_path[i];
        g_wpath[i] = ch;
    }
    
    //add the * to the file path 
    {
        int i=0;
        while(g_wpath[i]!=0)i++; //seek to the end of the string
        g_wpath[i++]='*'; //add an *
        g_wpath[i  ]= 0 ; //add the 0
    }

    // welcome
	char welcomeMessage[] = "Welcome to CPShell!\nRunning on Classpad OS v2.1.2\nWritten by: CPShell Team\nType 'help' for a list of commands.\n\n\n";
	terminal->WriteChars(welcomeMessage);
}
