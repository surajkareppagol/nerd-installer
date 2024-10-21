#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <util.h>
#include <fonts.h>

/* Free memory after usage */
char *create_zip_file_name(char *font)
{
  int length = strlen(font);

  /* .zip */
  char *zip = calloc(1, (length + 5) * sizeof(char));

  strncat(zip, font, length);
  strncat(zip, ".zip", 5);

  return zip;
}

FILE *create_zip_file(char *file)
{
  char *zip = create_zip_file_name(file);

  FILE *zip_file = fopen(zip, "wb");

  free(zip);

  return zip_file;
}

char *create_url(char *font)
{
  int length_font     = strlen(font);
  int length_init_url = strlen(INIT_URL);

  char *url = malloc(length_init_url + length_font + 5);

  strncpy(url, INIT_URL, length_init_url);

  strncat(url, font, length_font);
  strncat(url, ".zip", 5);

  return url;
}

char *create_dir(char *font)
{
  char *home = getenv("HOME");

  int length_font = strlen(font);
  int length_home = strlen(home);

  size_t size = length_home + length_font + 21;

  char *directory = calloc(1, size * sizeof(char));

  strncpy(directory, home, length_home);

  strncat(directory, "/.local/share/fonts/", 22);
  strncat(directory, font, length_font);

  struct stat stat_ = {0};

  if (stat(directory, &stat_) == -1) {
    mkdir(directory, 0700);
  }

  return directory;
}
