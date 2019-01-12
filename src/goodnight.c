/*
* FILE      : goodnight.c
* PROJECT   : goodnight
* AUTHOR    : David Inglis
* DATE      : 2018-12-02
* DESC      :
*   'goodnight' - the blue-light filter using xgamma manipulation
*
*   the goodnight.c file contains the command argument reader, command 
*   functions, and custom print dialogue functions
* 
*/

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "preset.h"

#define VERSION "1.0.0"
#define PROGNAME "goodnight"


// console argument commands
const char* CMD_HELP = "help";
const char* CMD_LIST = "list";
const char* CMD_ON = "on";
const char* CMD_OFF = "off";


// prototypes
bool commands(int, char*[]);
void print_presets(preset*, int, int);
void print_version(void);
void print_unknown(const char*);
void print_incorrect(const char*);
void print_usage(void);


// program body
int main(int argc, char* argv[]) {
    if (argc > 1) {
        commands(argc, argv);
    }
    else {
        print_version();
        printf("sleep tight\n");
    }

    return 0;
}


/*
* FUNCTION  : commands
* DESC      :
*   Processes commands passed to the program from the command line.
* PARAMS    :
*   int argc        : number of command-line arguments. this function
*       expects argc to be at least 1, but a sanity check is in place
*   char* argv[]    : the array of argument strings. size is 'argc'
* RETURNS   :
*   bool : command validity. returns true if a valid command has been
*       processed, and false otherwise.
*/
bool commands(int argc, char* argv[]) {
    int preset_count = 0;
    preset preset_list[PRESETS_MAX];

    // exits function if an empty args list has been mistakenly passed
    if (argc < 1) {
        return false;
    }

    // populates preset_list from config file
    preset_list[0] = PRESET_DEFAULT;
    preset_count++;
    int conf_status = load_conf(preset_list, &preset_count);
    
    // print usage information
    if (strcmp(CMD_HELP, argv[1]) == 0) {
        print_usage();
    }
    
    // turn on goodnight filter
    else if (strcmp(CMD_ON, argv[1]) == 0) {
        // use last configuration
        if (argc == 2) {
            if (apply_preset(PRESET_DEFAULT)) {
                printf("goodnight enabled - filter '%s' \n", PRESET_DEFAULT.handle);
            } else {
                // print some error message about a bad preset
            }
        }
        else if (argc == 3) {
            int pre_idx = -1;
            print_presets(preset_list, preset_count, conf_status);
            //printf("custom presets are currently disabled\n");
            //printf("DEBUG preset name: %s\n", argv[2]);

            for (int i = 0; i < preset_count; i++) {
                if (strcmp(preset_list[i].handle, argv[2]) == 0) {
                    pre_idx = i;
                }
            }
            if (pre_idx >= 0) {
                printf("a preset with the handle '%s' exists in the config\n", preset_list[pre_idx].handle);
                apply_preset(preset_list[pre_idx]);
            }
            else {
                printf("no preset found with the handle '%s'\n", argv[2]);
            }
        }
        else {
            print_incorrect(argv[1]);
        }
        
    }
    
    // turn off goodnight filter
    else if (strcmp(CMD_OFF, argv[1]) == 0) {
        if (argc == 2) {
            if (apply_preset(PRESET_CLEAR)) {
                printf("goodnight disabled\n");
            }
        }
        else {
            print_incorrect(argv[1]);
        }
    }

    // list contents of config file
    else if (strcmp(CMD_LIST, argv[1]) == 0) {
        if (argc == 2) {
            print_presets(preset_list, preset_count, conf_status);
        }
        else {
            print_incorrect(argv[1]);
        }
    }

    // unknown base flag
    else {
        print_unknown(argv[1]);
    }

    return true;
}

void print_presets(preset* preset_list, int preset_count, int conf_status) {
    // print preset_list contents
    for (int i = 0; i < preset_count; i++) {
        printf("preset '%s': %6.4f %6.4f %6.4f\n", 
                preset_list[i].handle,
                preset_list[i].rgamma,
                preset_list[i].ggamma,
                preset_list[i].bgamma);
    }
    switch(conf_status) {
        case (ERR_NO_CONF):
            printf("no config file\n");
            break;
        case (ERR_BAD_CONF):
            printf("error in config file\n");
            break;
    }
}


/*
* FUNCTION  : print_version
* DESC      :
*     Prints version information for the 'goodnight' program
* PARAMS    :
*     void
* RETURNS   : 
*     void
*/
void print_version(void) {
    printf("goodnight ver %s\n", VERSION);
}


/*
* FUNCTION  : print_unknown
* DESC      :
*     Prints dialog informing the user they have entered an
*     unknown argument
* PARAMS    :
*     const char* cmd : the unknown argument entered by the user
* RETURNS   : 
*     void
*/
void print_unknown(const char* arg) {
    printf("unknown command '%s' \nuse command 'help' for usage information\n", arg);
}


/*
* FUNCTION  : print_incorrect
* DESC      :
*     Prints dialog informing the user they have entered an 
*     argument incorrectly
* PARAMS    :
*     const char* cmd : the incorrect argument entered by the user
* RETURNS   : 
*     void
*/
void print_incorrect(const char* flag) {
    printf("incorrect usage of the '%s' command\n", flag);
}

/*
* FUNCTION  : print_usage
* DESC      :
*     Prints usage information for the 'goodnight' program
* PARAMS    :
*     void
* RETURNS   : 
*     void
*/
void print_usage(void) {
    //printf("%4c%-12s%s\n", ' ', "", "");

    print_version();

    printf("SYNOPSIS\n");
    printf("%4c%s %s\n\n", ' ', PROGNAME, "[ help | on [handle] | off | list ]");
    printf("USAGE\n");
    printf("%4c%-8s%s\n\n", ' ', "help", "prints this usage information");
    printf("%4c%s\n", ' ', "on [handle]"); 
    printf("%12c%s\n", ' ', "activates goodnight filter with the preset specified by 'handle'");
    printf("%12c%s\n\n", ' ', "(default if no preset specified)");
    printf("%4c%-8s%s\n\n", ' ', "off", "deactivates goodnight filter");
    printf("%4c%-8s%s\n\n", ' ', "list", "shows list of filter presets from config file");
    printf("FILES\n");
    printf("%4c%s\n\n", ' ', "~/.config/goodnight/config");
}

