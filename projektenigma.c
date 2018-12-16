#include "enigma.h"

int main( int argc, char ** argv )
{
  char *infile;            /* plaintext/ciphertext file name ptr */
  char *enciphered;	       /* result file name ptr */
  init_mach();
  switch(argc)
  {
    case 1: 
		usage(argv[0]);
		break;
    case 2: 
		infile = argv[1];
		encipher_file(infile);
		break;
    case 3:
		read_keyfile(argv[1]);
		infile = argv[2];
		encipher_file(infile);
		break;
    case 4:
		read_keyfile(argv[1]);
		infile = argv[2];
		enciphered = argv[3];
		encipher_file_to_file(infile, enciphered);
		break;
    default:
		usage(argv[0]);
		break;
  }
  return 0;
}
