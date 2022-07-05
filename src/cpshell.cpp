#pragma once

#include "../lib/functions/convert.hpp"
#include "internal.hpp"
#include "utilities.hpp" // history
#include "loader.hpp" // user profile


// commands
#include "commands/cat.cpp"
#include "commands/cd.cpp"
#include "commands/clear.cpp"
#include "commands/credits.cpp"
#include "commands/date.cpp"
#include "commands/echo.cpp"
#include "commands/exit.cpp"
#include "commands/help.cpp"
#include "commands/history.cpp"
#include "commands/ls.cpp"
#include "commands/osname.cpp"
#include "commands/rand.cpp"
#include "commands/username.cpp"

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

    // set color to TERM_COLOR
    terminal->SetColor(TERM_COLOR);
    
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

    if (HISTORY_ENABLED) {
        int hist_status;
        hist_status = add_history(argc, argv);
        if (hist_status != 0) {
            char err_msg[BUF_SIZE];
            strcpy(err_msg, "Error adding to history.\n");
            terminal->WriteChars(err_msg);
        }
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
            }
            strcat(cmds, ", ");
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
    strcpy(applets[0].name, "cat");
    applets[0].main = cat_main;
    strcpy(applets[1].name, "cd");
    applets[1].main = cd_main;
    strcpy(applets[2].name, "clear");
    applets[2].main = clear_main;
    strcpy(applets[3].name, "credits");
    applets[3].main = credits_main;
    strcpy(applets[4].name, "date");
    applets[4].main = date_main;
    strcpy(applets[5].name, "echo");
    applets[5].main = echo_main;
    strcpy(applets[6].name, "exit");
    applets[6].main = exit_main;
    strcpy(applets[7].name, "help");
    applets[7].main = help_main;
    strcpy(applets[8].name, "history");
    applets[8].main = history_main;
    strcpy(applets[9].name, "ls");
    applets[9].main = ls_main;
    strcpy(applets[10].name, "osname");
    applets[10].main = osname_main;
    strcpy(applets[11].name, "rand");
    applets[11].main = rand_main;
    strcpy(applets[12].name, "username");
    applets[12].main = username_main;

    memset(&applets[13], 0, sizeof(Applet));

    // init file system
    // Reference: SnailMath/filemgr

    //initialize g_path to home ("\\fls0\\")
	memset(g_path,0,sizeof(g_path));
	strcpy (g_path, g_home);

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

    // call load_userprofile
    int profileLoad = load_userprofile();
    if (profileLoad != 0) {
        char err_msg[BUF_SIZE];
        strcpy(err_msg, "Error loading user profile.\n");
        terminal->WriteChars(err_msg);
    }
    LCD_Refresh();
    terminal->ClearBuffer();

    // welcome
	char welcomeMessage[] = "Welcome to CPShell (Running OS v2.1.2)\nWritten by: Sean McGinty (s3ansh33p)\nType 'help' for a list of commands.\n\n";
	terminal->WriteChars(welcomeMessage);
}
