/*
	HERP DERP
*/

#include <stdio.h>
#include <stdlib.h>
#include "wavsteg.h"

void print_usage(char *filename);

void print_header(wav_header *header)
{
  printf("Channels: %d\n"\
      "Sample Rate: %d\n"\
      "Bitrate %d\n",
      header->num_channels, header->sample_rate, header->bits_per_sample);
}

int main(int argc, char *argv[])
{
  wav_header header;
  FILE *infp, *datafp, *newfp;
  int wav_data, data, i = 16;
  int masks[] = {0x00000000, 0x00010000, 0x00000001, 0x00010001};

  if (argc != 4)
  {
    print_usage(argv[0]);
    exit(1);
  }

  if ((infp = fopen(argv[1], "r")) == NULL)
  {
    printf("Unable to open file: %s.\n", argv[1]);
    exit(1);
  }

  if ((datafp = fopen(argv[2], "r")) == NULL)
  {
    printf("Unable to open file: %s.\n", argv[2]);
    exit(1);
  }

  if ((newfp = fopen(argv[3], "w")) == NULL)
  {
    printf("Unable to open file: %s.\n", argv[3]);
    exit(1);
  }

  fread(&header, sizeof(wav_header), 1, infp);
  print_header(&header);

  fwrite(&header, sizeof(wav_header), 1, newfp);

  while(!feof(infp))
  {
    if (i > 15)
    {
      fread(&data, sizeof(int), 1, datafp);
      i = 0;
    }

    fread(&wav_data, sizeof(int), 1, infp);

    if (!feof(datafp))
      wav_data = wav_data | masks[0x00000003 & data];

    fwrite(&wav_data, sizeof(int), 1, newfp);
    data = data >> 2;
    i++;
  }

  fclose(infp);
  fclose(datafp);
  fclose(newfp);
  return 0;
}

void print_usage(char *filename)
{
  printf("Usage: %s <in file> <data> <new file>\n", filename);
}
