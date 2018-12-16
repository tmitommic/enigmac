#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#define LINE_LEN 80
#define NUM_ROTORS 5

char ref_rotor[27] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";  /* reflecting rotor */

char rotor[NUM_ROTORS][27] = { /* pre defined rotors */
  "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
  "AJDKSIRUXBLHWTMCQGZNPYFVOE",
  "BDFHJLCPRTXVZNYEIWGAKMUSQO",
  "ESOVPZJAYQUIRHXLNFTGKDCMWB",
  "VZBRGITYUPSDNHLXAWMJQOFECK",
};

int step_data[NUM_ROTORS] = {  
  16,  4, 21,  9, 25           /* steps at: q, e, v, j, z */
};

/* engima key default settings */

int order[3] = { 0, 1, 2};     /* rotor order, user input is +1 */
char ring[8] = {               /* ring settings/alpha ring to the core rotation */
  '\0', 'A',  'A',  'A',       /* default: AAA */
  '\0', '\0', '\0', '\0' };

int n_plugs = 0;               /* number of plugs */
char plugs[80] = "";           /* plug string */
int pos[3] = { 0, 0, 0 };      /* rotor positions */

/* simulation data and machine state data */

int data[8][26];               /* working array for machine */
int step[3];                   /* steps corresponding to rotors */
int double_step;               /* rotor 2 step twice */

int encipher(int c);
void encipher_file(char *file_name);
void encipher_file_to_file(char *file_name, char *enciphered);
void read_keyfile(char *str);
void init_mach(void);
void usage(char *str);
