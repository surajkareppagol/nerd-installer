#ifndef UI_H
#define UI_H

#include <ncurses.h>

#define ui_clear_screen(window) (wclear(window))
#define ui_refresh(window)      (wrefresh(window))

extern WINDOW *window;
extern int w_rows;
extern int w_columns;

int ui_setup(int *rows, int *columns);
void ui_create_font_menu(int selected, int limit);
void ui_create_header(char *title, int row, int column);
void ui_create_footer(char *title, int row, int column);
WINDOW *ui_create_download_window(int rows, int columns);
void ui_center_print(WINDOW *window, int rows, int columns, char *message);
void ui_sleep(int seconds);

#endif
