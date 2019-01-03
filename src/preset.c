/*
* FILE      : preset.c
* PROJECT   : goodnight
* AUTHOR    : David Inglis
* DATE      : 2018-12-18
* DESC      :
*   contains function definitions for the 'preset' structure
*/

#include "preset.h"
const preset PRESET_CLEAR = { "clear", 1.0, 1.0, 1.0 };
const preset PRESET_DEFAULT = { "default", 1.0, 1.0, 0.9 };

int load_conf(preset* preset_def) {
    
    // processes the goodnight.conf file
    // interprets lines beginning with # as comments
    // ignores whitespace between options
    // 
    // conf file contains goodnight filter presets in the following form:
    //     "filter-name" r g b
    // where r, g, b are floating points between 0.100 and 10.000, but recommended
    // between 0.100 and 1.000

    const int STATE_DEFAULT = 0;
    const int STATE_COMMENT = 1;
    const int STATE_SPACING = 2;
    const int STATE_CSTRING = 3;

    int readstate = STATE_DEFAULT;

    char buffer[BUFFER_SIZE] = "";
    int buffer_count = 0;
    char memory[50][100];
    int memory_count = 0;
    
    preset working_preset = {"", 0, 0, 0};

    char conf[BUFFER_SIZE] = ""; 
    strcat(conf, getenv("HOME"));
    strcat(conf, "/.config/goodnight/config");
    FILE* ifp = fopen(conf, "r");

    if (ifp == NULL) {
        return ERR_NO_CONF;
    }
    
    while(!feof(ifp)) {
        char x = 0;
        x = fgetc(ifp);

        // read #
        // skip until next \n then read next character
        

        // read ' ' or \t or \n
        // skip until next non spacing character then read that character

        if (readstate == STATE_DEFAULT) {
            if (x == '#') {
                readstate = STATE_COMMENT;
            }
            else if (x == '"') {
                readstate = STATE_CSTRING;
            }
            else if (x == ' ' || x == '\t' || x == '\n') {
                // dump buffer
                buffer[buffer_count] = 0;
                
                if (buffer_count > 0) {
                    //printf("%s\n", buffer);
                    sprintf(memory[memory_count], buffer);
                    memory_count++;
                }

                buffer_count = 0; // sets buffer stream indicator to initial position
            }
            else {
                // add character to buffer
                buffer[buffer_count] = x;
                buffer_count++;
            }
        }
        else if (readstate == STATE_COMMENT) {
            if (x == '\n') {
                readstate = STATE_DEFAULT;
            }
        }
        else if (readstate == STATE_CSTRING) {
            if (x == '"') {
                readstate = STATE_DEFAULT;

                // dump buffer
                buffer[buffer_count] = 0;
                
                if (buffer_count > 0) {
                    //printf("%s\n", buffer);
                    sprintf(memory[memory_count], buffer);
                    memory_count++;
                }

                buffer_count = 0; // sets buffer stream indicator to initial position
            }
            else {
                // add character within quote string to buffer
                buffer[buffer_count] = x;
                buffer_count++;
            }
        }
    }

    if (ifp != NULL) {
        fclose(ifp);
    }

    //
    printf("config presets:\n");
    for (int i = 0; i < memory_count; i++) {
        printf("memory %d: %s\n", i, memory[i]);
    }

    return VALID_CONF;
}


/*
* FUNCTION  : validate_preset
* DESC      :
*     Checks whether or not the gamma values of the given preset 'p'
*     are within the bounds specified by GAMMA_UPPER and GAMMA_LOWER,
*     inclusively.
*
*     GAMMA_UPPER and GAMMA_LOWER are chosen such that the gamma values
*     passed to xgamma are not excessively high or low.
* PARAMS    :
*     (struct) preset p : the preset to validate
* RETURNS   : 
*     bool : true if preset gamma values are within the bounds, false
*       otherwise
*/
bool validate_preset(preset p) {
    const float XGAMMA_LOWER = 0.100;
    const float XGAMMA_UPPER = 1.000;

    if (frange(p.rgamma, XGAMMA_LOWER, XGAMMA_UPPER) &&
            frange(p.ggamma, XGAMMA_LOWER, XGAMMA_UPPER) &&
            frange(p.bgamma, XGAMMA_LOWER, XGAMMA_UPPER)) {
        return true;
    } 
    else {
        return false;
    }
}


/*
* FUNCTION  : apply_preset
* DESC      :
*     Applies a valid preset through xgamma. If the preset is invalid 
*     (indicated by validate_preset() ) then the function will return
*     false and xgamma is unchanged.
* PARAMS    :
*     (struct) preset p : the preset to be applied
* RETURNS   : 
*     bool : true if preset is applied using xgamma, false otherwise
*/
bool apply_preset(preset p) {
    if (!validate_preset(p)) {
        return false;
    }

    char command[BUFFER_SIZE] = "";
    sprintf(command, "xgamma -quiet -rgamma %f -ggamma %f -bgamma %f", p.rgamma, p.ggamma, p.bgamma);
    system(command);

    return true;
}



/*
* FUNCTION  : frange
* DESC      : 
*     Checks the given floating-point number 'x' against given 'low' and
*     'high'. Returns true if low <= x <= high, and false otherwise
* PARAMS    :
*     float x    : The number to check
*     float low  : The lower bound
*     float high : The upper bound
* RETURNS   :
*     bool : true if low <= x <= high, false otherwise
*/  
bool frange(float x, float low, float high) {
    if ((low <= x) && (x <= high)) {
        return true;
    }
    else {
        return false;
    }
}


