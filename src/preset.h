/*
* FILE      : preset.h
* PROJECT   : bluelight
* AUTHOR    : David Inglis
* DATE      : 2018-12-18
* DESC      :
*   contains struct definition and function prototypes for the 
*   'preset' structure
*/

#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VALID_CONF 0
#define ERR_NO_CONF 1
#define ERR_BAD_CONF 2

#define BUFFER_SIZE 1000

#define PRESETS_MAX 20

// min/max values for presets
#define XGAMMA_UPPER 2.000
#define XGAMMA_LOWER 0.100


// preset structure definition
typedef struct preset {
    char handle[BUFFER_SIZE];
    float rgamma;
    float ggamma;
    float bgamma;
    struct preset* next;
} preset;
    
// preset constants
extern const preset PRESET_CLEAR;
extern const preset PRESET_DEFAULT;


// function prototypes for preset.c
int load_defaults(preset*, int*);
int load_conf(preset*, int*);

bool validate_preset(preset);
bool apply_preset(preset);

bool frange(float, float, float);
