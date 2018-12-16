#include "enigma.h"

/* encipher - implementation of the engima cipher function */

int
encipher(int c)
{
  int j;                        /* index for counting */
  int idx;                      /* rotor index */

  if (isalpha(c))
    {

      pos[0] = (pos[0] + 1) % 26;  /* first, advances the rotors */
      if (pos[0] == step[0])
        pos[1] = (pos[1] + 1) % 26;
      if (double_step)
        {
          pos[1] = (pos[1] + 1) % 26;
          pos[2] = (pos[2] + 1) % 26;
          double_step = FALSE;
        }
      if (pos[1] == step[1])
        double_step = TRUE;

      c -= 'A';                 /* start to encipher */
      if (n_plugs != 0)
        c = data[0][c];
      for (j=0;j<3;j++)         /* do rotors forward */
        {
          idx = (c + pos[j]) % 26;
          c   = (c + data[j+1][idx]) % 26;
        }
      c = (data[4][c]) % 26;    /* reflecting rotor */
      for (j=0;j<3;j++)         /* do rotors reverse */
        {
          idx = (c + pos[2-j]) % 26;
          c   = (c + data[j+5][idx]) % 26;
        }
      if (n_plugs != 0)
        c = data[0][c];
      c += 'A';
    }
  return(c);
}

/* read_keyfile - a simple function to read in the key file */

void
read_keyfile( char *str )
{
  FILE *kf;                         /* input key FILE pointer */
  int num;                          /* dummy returned from fscanf() */
  int idx;                          /* index/counter */
  char a[3];                        /* dummy for input */

  kf = fopen(str, "r");
  num = fscanf(kf, "%d %d %d\n", &order[0], &order[1], &order[2]);
  num = fscanf(kf, "%c %c %c\n", &ring[1], &ring[2], &ring[3]);
  num = fscanf(kf, "%d\n", &n_plugs);

  if (n_plugs != 0)
    num = fscanf(kf, "%[^\n]\n", plugs);
  num = fscanf(kf, "%c %c %c\n", &a[0], &a[1], &a[2]); /* rotor positions */

  for (idx = 0; idx < 3; idx++)
    {
      (order[idx])--;
      ring[idx+1] = toupper(ring[idx+1]);
      pos[idx] = toupper(a[idx]) - 'A';
    }
  fclose(kf);
}

/* encipher_file - open and encipher a file */

void encipher_file(char *file_name)
{
  FILE *fp;                     /* plaintext/ciphertext FILE pointer */
  char line[LINE_LEN + 1];      /* input data line, inc. '\n' */
  char *ret_val;                /* value from fgets(), used for EOF check */
  char c;                       /* character from data line */
  int len;                      /* length of data line */
  int idx;                      /* index/counter */

  fp = fopen(file_name, "r");
  ret_val = fgets(line, LINE_LEN, fp);
  while(ret_val != NULL)
    {
      len = strlen(line);
      for (idx=0;idx<len;idx++)
        {
          c = line[idx];
          if (isalpha('c'))
            {
              c = encipher((int)(toupper(c)));
              line[idx] = c;
            }
        }
      printf("%s", line);
      ret_val = fgets(line, LINE_LEN, fp);
    }
  fclose(fp);
}

/* encipher_file_to_file - open and encipher a file, then save to another file */

void encipher_file_to_file(char *file_name, char *enciphered)
{
  FILE *fp;                     /* plaintext/ciphertext FILE pointer */
  FILE *encip;		    		/* result file */
  char line[LINE_LEN + 1];      /* input data line, inc. '\n' */
  char *ret_val;                /* value from fgets(), used for EOF check */
  char c;                       /* character from data line */
  int len;                      /* length of data line */
  int idx;                      /* index/counter */

  fp = fopen(file_name, "r");
  encip = fopen(enciphered, "w");
  ret_val = fgets(line, LINE_LEN, fp);
  while(ret_val != NULL)
    {
      len = strlen(line);
      for (idx=0;idx<len;idx++)
        {
          c = line[idx];
          if (isalpha('c'))
            {
              c = encipher((int)(toupper(c)));
              line[idx] = c;
            }
        }
      fputs(line, encip);
      ret_val = fgets(line, LINE_LEN, fp);
    }
  fclose(encip);
  fclose(fp);
}

/* init_mach - set up data according to the input data */

void init_mach( void )
{
  int i, j;                      /* indexes */
  int ds;                        /* used during ring settings */
  int u, v;                      /* temps for plugboard input */

  /* setup rotor data */
  for (j=0;j<26;j++)
    data[4][j] = ((int)ref_rotor[j]-'A'+26)%26;

  for (i=1;i<4;i++)
    {
      step[i-1] = step_data[order[i-1]];
      for (j=0;j<26;j++)
        {
          data[i][j] = ((int)(rotor[order[i-1]][j])-'A'+26)%26;
          data[8-i][data[i][j]] = j;
        }
    }

  /* setup ring settings */
  ring[7] = ring[1];
  ring[6] = ring[2];
  ring[5] = ring[3];
  for (i=1;i<8;i++)
    if (i != 4)
      {
        ds = (int)(ring[i]) - 'A';
        if (ds != 0)
          {
            for (j=0;j<26;j++)
              data[0][j] = data[i][j];
            for (j=0;j<26;j++)
              data[i][j] = data[0][(26-ds+j)%26];
          }
      }

  /* setup plug data */
  if (n_plugs != 0)
    {
      j = 0;
      for (i=0;i<26;i++)
        data[0][i] = i;
      for (i=0;i<n_plugs;i++)
        {
          while (!isalpha((int)plugs[j]))
          {
            j++;
            if (plugs[j] == '\0')
              break;
          }
          u = toupper(plugs[j++]) - 'A';
          v = toupper(plugs[j++]) - 'A';
          data[0][u] = v;
          data[0][u] = u;
        }
    }

  /* convert all moving rotor data to displacements */
  for (i=1;i<8;i++)
    {
      if (i!=4)
        for (j=0;j<26;j++)
          data[i][j] = (data[i][j] - j + 26) % 26;
    }

  /* setup rotor starting positions */
  double_step = FALSE;              /* no previous rotor position */
  /* input function has already done the rotor positions */
}

/* usage - function to print out the correct usage of the program */

void
usage( char *str )
{
  fprintf(stderr, "\n\tusage: %s [<keyfile>] <infile> <outputfilename>\n", str);
  fprintf(stderr, "\t<outputfilename> is optional, omitted results output is stdout\n\n");
  fprintf(stderr, "\tkeyfile has the form:\n");
  fprintf(stderr, "\t\tn n n\t\t- for rotor order, 1 <= n <= 5\n");
  fprintf(stderr, "\t\tx x x\t\t- for ring settings, x alpha\n");
  fprintf(stderr, "\t\tn\t\t- number of plugs, 0 <= n <= 13\n");
  fprintf(stderr, "\t\txx xx xx ...\t- plug letter pairs, one pair"
          " for each n\n");
  fprintf(stderr, "\t\tx x x\t\t- initial rotor position, x alpha\n\n");
  fprintf(stderr, "\tboth keyfile and infile should end with an empty line\n\n");
  exit(0);
}
