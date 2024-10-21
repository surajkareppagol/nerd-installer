#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <extract.h>
#include <fetcher.h>
#include <fonts.h>
#include <ui.h>
#include <util.h>

#include <zip.h>

int main() {
  /**************************************/
  /*            CURL INIT               */
  /**************************************/

  CURL *curl = fetcher_init();

  if (curl == NULL) {
    return 1;
  }

  /**************************************/
  /*            UI INIT                 */
  /**************************************/

  int rows, columns;

  ui_setup(&rows, &columns);

  ui_create_header("Nerd Installer - Install Nerd Fonts", rows, columns / 2);

  ui_create_footer("  Quit (q), Help (h)  ", rows - 2, 4);

  /**************************************/
  /*            KEY EVENTS              */
  /**************************************/

  int selected = 0;
  bool loop = true;
  int key_value = 0;

  ui_create_font_menu(selected, rows);

  while (loop) {
    int key = getch();

    switch (key) {
    case KEY_UP:
      key_value = 0;
      (selected == 0) ? (selected = 0) : (selected -= 1);
      break;
    case KEY_DOWN:
      key_value = 0;
      (selected == FONTS - 1) ? (selected = FONTS - 1) : (selected += 1);
      break;
    case 'q':
      endwin();
      return 0;
    case '\n':
      loop = false;
      break;
    default:
      /* Manage multi-key presses */
      if (isdigit(key)) {
        key_value = key_value * 10 + (key - '0');

        if (key_value >= 0 && key_value <= FONTS - 1) {
          selected = key_value - 1;
        } else {
          key_value = selected + 1;
        }
      }
    }

    ui_create_font_menu(selected, rows);
  }

  /**************************************/
  /*         CURL SET OPTIONS           */
  /**************************************/

  char *selected_font = fonts[selected];

  FILE *zip = create_zip_file(selected_font);
  char *url = create_url(selected_font);

  fetcher_options(curl, url, zip);

  ui_clear_screen(stdscr);
  ui_refresh(stdscr);

  /**************************************/
  /*        CURL FILE DOWNLOAD          */
  /**************************************/

  /* New window available as window */
  ui_create_download_window(rows, columns);

  int rows_ = 0;
  int columns_ = 0;

  getmaxyx(window, rows_, columns_);
  ui_center_print(window, rows_, columns_, "Downloading ZIP file...");

  fetcher_perform(curl);
  fetcher_cleanup(curl);

  /* IMPROTANT */
  fclose(zip);

  /**************************************/
  /*            EXTRACT ZIP             */
  /**************************************/

  char *directory = create_dir(selected_font);

  ui_center_print(window, rows_, columns_, "Extracting ZIP file...");

  if (extract_zip(selected_font, directory)) {
    ui_center_print(window, rows_, columns_, "ZIP Extraction failed...");
  } else {
    ui_center_print(window, rows_, columns_, "Restart | fc-cache -fv");
  }

  endwin();

  return 0;
}
