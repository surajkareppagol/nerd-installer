#include <ncurses.h>
#include <string.h>

#include <ui.h>
#include <fonts.h>

WINDOW *window;
int w_rows;
int w_columns;

int ui_setup(int *rows, int *columns)
{
  initscr();
  ui_clear_screen(stdscr);

  getmaxyx(stdscr, *rows, *columns);

  /* Check for colors */
  if (!has_colors()) {
    return 1;
  }

  if (start_color() != OK) {
    return 1;
  }

  /* Enable Keypad */
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  noecho();
  curs_set(0);

  border('|', '|', '=', '=', '+', '+', '+', '+');

  return 0;
}

void ui_create_font_menu(int selected, int limit)
{
  int column_ = 4;
  int row_    = 2;

  for (int i = 0; i < FONTS; i++) {
    if (row_ == (limit - 4)) {
      row_ = 3;
      column_ += 30;
    } else {
      row_ += 1;
    }

    if (i == selected) {
      attron(A_BOLD | A_REVERSE);
      mvprintw(row_, column_, "%2d. %-10s", i + 1, fonts[i]);
      attroff(A_BOLD | A_REVERSE);
      continue;
    }

    mvprintw(row_, column_, "%2d. %-10s", i + 1, fonts[i]);
  }

  ui_refresh(stdscr);
}

void ui_create_header(char *message, int row, int column)
{
  mvaddstr(1, (column) - (strlen(message) / 2), message);
}

void ui_create_footer(char *message, int row, int column)
{
  attron(A_BOLD | A_REVERSE);
  mvaddstr(row, column, message);
  attroff(A_BOLD | A_REVERSE);
}

WINDOW *ui_create_download_window(int rows, int columns)
{
  int row_           = rows / 2;
  int column_        = columns / 2;
  int row_origin_    = row_ - (row_ / 2);
  int column_origin_ = column_ - (column_ / 2);

  w_rows    = row_;
  w_columns = column_;

  window = newwin(row_, column_, row_origin_, column_origin_);

  wborder(window, '|', '|', '=', '=', '+', '+', '+', '+');

  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  wbkgd(window, COLOR_PAIR(1));

  return window;
}

void ui_center_print(WINDOW *window, int rows, int columns, char *message)
{
  int row_    = rows / 2;
  int column_ = columns / 2;

  int length = strlen(message);

  ui_clear_screen(window);
  wborder(window, '|', '|', '=', '=', '+', '+', '+', '+');

  mvwaddstr(window, row_ - 1, column_ - (length / 2), message);
  ui_refresh(window);

  ui_sleep(2);
}

void ui_sleep(int seconds)
{
  int m_seconds = seconds * 1000;

  napms(m_seconds);
}