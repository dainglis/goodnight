/*
* FILE      : goodnight.c
* PROJECT   : goodnight
* AUTHOR    : David Inglis
* DATE      : 2018-12-02
* DESC      :
*   'goodnight' - the blue-light filter using xgamma manipulation
* 
*/

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "preset.h"

#define VERSION "1.0.0"
#define PROGNAME "goodnight"

const char* FLAG_HELP = "help";
const char* FLAG_LIST = "list";
const char* FLAG_ON = "on";
const char* FLAG_OFF = "off";

// prototypes
void print_version(void);
void print_unknown(const char*);
void print_incorrect(const char*);
void print_usage(void);


// program body
int main(int argc, char* argv[]) {
    if (argc > 1) {
        // print usage information
        if (strcmp(argv[1], FLAG_HELP) == 0) {
            print_usage();
        }
        
        // turn on goodnight filter
        else if (strcmp(argv[1], FLAG_ON) == 0) {
            // use last configuration
            if (argc == 2) {
                if (apply_preset(PRESET_DEFAULT)) {
                    printf("goodnight enabled - filter '%s' \n", PRESET_DEFAULT.handle);
                } else {
                    // print some error message about a bad preset
                }
            }
            else if (argc == 3) {
                printf("custom presets are currently disabled\n");
                printf("DEBUG preset name: %s\n", argv[2]);
            }
            else {
                print_incorrect(argv[1]);
            }
            
        }
        
        // turn off goodnight filter
        else if (strcmp(argv[1], FLAG_OFF) == 0) {
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
        else if (strcmp(argv[1], FLAG_LIST) == 0) {
            if (argc == 2) {
                preset preset_default = {"default", 1.0, 1.0, 1.0};
                switch(load_conf(&preset_default)) {
                    case (ERR_NO_CONF):
                        printf("no config file\n");
                        break;
                    case (ERR_BAD_CONF):
                        printf("error in config file\n");
                        break;
                    default:
                        break;
                }
            }
            else {
                print_incorrect(argv[1]);
            }
        }

        // unknown base flag
        else {
            print_unknown(argv[1]);
        }
    }

    // no flags
    else {
        print_usage();
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
    printf("goodnight ver %s\n\n", VERSION);
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
    printf("unknown argument '%s' \nuse argument 'help' for usage information\n", arg);
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
    printf("incorrect usage of the '%s' argument\n", flag);
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

