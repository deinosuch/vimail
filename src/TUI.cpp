// Copyright (c) 2025 Adam Paseka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "TUI.h"

#include <ncurses.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>

using std::string;

WINDOW* create_window(int height, int width, int starty, int startx,
                      const string& title) {
  WINDOW* local_window = newwin(height, width, starty, startx);
  box(local_window, 0, 0);
  mvwprintw(local_window, 0, TITLE_OFFSET, "%s", title.c_str());
  wrefresh(local_window);

  return local_window;
}

void clear_area(WINDOW* win, int start_y, int start_x, int height, int width) {
  for (int y = 0; y < height; ++y) {
    wmove(win, start_y + y, start_x);
    for (int x = 0; x < width; ++x) {
      waddch(win, ' ');
    }
  }
  wrefresh(win);
}

void print_to_window(WINDOW* win, const std::string& content) {
  int x_max, y_max;
  getmaxyx(win, y_max, x_max);
  y_max -= 2;
  x_max -= 2;
  clear_area(win, 1, 1, y_max, x_max);
  wmove(win, 1, 1);
  int y;
  y = 1;
  std::istringstream iss(content);
  string line;
  while (std::getline(iss, line)) {
    while (line.length() > x_max) {
      if (y > y_max) {
        break;
      }
      mvwprintw(win, y++, 1, "%s", line.substr(0, x_max).c_str());
      line = line.substr(x_max);
    }
    if (y > y_max) {
      break;
    }

    mvwprintw(win, y++, 1, "%s", line.c_str());
  }
  wrefresh(win);
}

TUI::TUI(const string& column_title, const string& left_header_title,
         const string& right_header_title, const string& header_title,
         const string& content_title) {
  spdlog::info("Initiliazing terminal app");

  initscr();
  noecho();

  int left_width = COLS / SPLIT_RATIO;
  int right_width = COLS - left_width;
  int small_header_width = right_width / 2;

  column_ = create_window(LINES, left_width, 0, 0, column_title);
  left_header_ = create_window(HEADER_HEIGHT, small_header_width, 0, left_width,
                               left_header_title);
  right_header_ =
      create_window(HEADER_HEIGHT, small_header_width, 0,
                    left_width + small_header_width, right_header_title);

  header_ = create_window(HEADER_HEIGHT, right_width, HEADER_HEIGHT, left_width,
                          header_title);
  content_ = create_window(LINES - 2 * HEADER_HEIGHT, right_width,
                           2 * HEADER_HEIGHT, left_width, content_title);

  current_ = 0;

  spdlog::info("Created app windows");
}

void TUI::quit() {
  getchar();
  endwin();
}

void TUI::add_element(element&& el) {
  els_.push_back(std::move(el));
  spdlog::info("Added new mail: " + els_[els_.size() - 1].header);
}

void TUI::run() {
  spdlog::info("running application");

  int x_column_max, y_column_max;
  getmaxyx(column_, y_column_max, x_column_max);
  size_t shown_mails = std::min((size_t)y_column_max, els_.size());
  while (true) {
    for (size_t i = 0; i < shown_mails; ++i) {
      if (i == current_) {
        wattron(column_, A_REVERSE);
      }
      element& el = els_[i];
      wmove(column_, i + 1, 1);

      wattron(column_, A_BOLD);
      wprintw(column_, "%s", el.header.c_str());
      wattroff(column_, A_BOLD);

      wprintw(column_, " %s", el.left_header.c_str());

      int x, y;
      getyx(column_, y, x);
      string end_line(x_column_max - x - 1, ' ');
      wprintw(column_, "%s", end_line.c_str());

      wattroff(column_, A_REVERSE);
    }
    wrefresh(column_);

    print_mail_(current_);

    char input = getchar();

    switch (input) {
      case UP:
        current_ -= current_ != 0;
        break;
      case DOWN:
        current_ += current_ != els_.size() - 1;
        break;
    }
  }
}

void TUI::print_mail_(size_t mail_no) {
  element& el = els_[mail_no];

  print_to_window(left_header_, el.left_header);
  print_to_window(right_header_, el.right_header);
  print_to_window(header_, el.header);
  print_to_window(content_, el.content);
}
