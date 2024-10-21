#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

#include <extract.h>
#include <ui.h>
#include <util.h>

int extract_zip(char *font, char *directory) {
  char *zip = create_zip_file_name(font);

  int error = 0;

  zip_t *archive = NULL;
  archive = zip_open(zip, 0, &error);

  if (archive == NULL) {
    return 1;
  }

  struct zip_stat *z_info = calloc(256, sizeof(int));
  zip_stat_init(z_info);

  int count = 0;

  while ((zip_stat_index(archive, count, 0, z_info)) == 0) {
    char *content = calloc(z_info->size + 1, sizeof(char));
    zip_file_t *fd = zip_fopen_index(archive, count, 0);

    zip_fread(fd, content, z_info->size);

    int size = strlen(directory) + strlen(z_info->name) + 2;
    char *path = calloc(size, sizeof(char));

    snprintf(path, size, "%s/%s", directory, z_info->name);

    FILE *file = fopen(path, "w");
    fwrite(content, z_info->size, 1, file);

    fclose(file);

    free(content);
    free(fd);
    free(path);

    count++;
  }

  free(directory);

  return 0;
}