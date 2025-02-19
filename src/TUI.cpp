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
  int length_max, height_max;
  getmaxyx(win, height_max, length_max);
  height_max -= 2;
  length_max -= 2;
  clear_area(win, 1, 1, height_max, length_max);
  wmove(win, 1, 1);
  int y = 1;
  std::istringstream iss(content);
  string line;
  while (std::getline(iss, line)) {
    while (line.length() > length_max) {
      if (y > height_max) {
        break;
      }
      mvwprintw(win, y++, 1, "%s", line.substr(0, length_max).c_str());
      line = line.substr(length_max);
    }
    if (y > height_max) {
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

  spdlog::info("Created app windows");
}

void TUI::quit() {
  spdlog::info("Quitting app");
  endwin();
}

void TUI::add_element(element&& el) {
  els_.push_back(std::move(el));
  spdlog::info("Added new mail: " + els_[els_.size() - 1].header);
}

void TUI::add_folder(string&& fld) {
  folders_.push_back(std::move(fld));
  spdlog::info("Added new folder: " + folders_[folders_.size() - 1]);
}

void TUI::run() {
  spdlog::info("Running application");

  int length_max, height_max;
  getmaxyx(column_, height_max, length_max);
  length_max -= 2;
  height_max -= 2;
  size_t shown_mails = std::min((size_t)height_max, els_.size());
  bool exit = false;
  size_t current = 0;

  while (!exit) {
    for (size_t i = 0; i < shown_mails; ++i) {
      if (i == current) {
        wattron(column_, A_REVERSE);
      }
      element& el = els_[i];
      wmove(column_, i + 1, 1);

      size_t header_size = el.header.size();
      size_t total_size = header_size + el.left_header.size() + 1;

      wattron(column_, A_BOLD);
      if (header_size < length_max) {
        wprintw(column_, "%s", el.header.c_str());
        wattroff(column_, A_BOLD);
        if (total_size < length_max) {
          wprintw(column_, " %s", el.left_header.c_str());
        } else {
          wprintw(
              column_, " %s",
              el.left_header.substr(0, length_max - header_size - 1).c_str());
        }
      } else {
        wprintw(column_, "%s", el.header.substr(0, length_max).c_str());
      }
      wattroff(column_, A_BOLD);

      int x, y;
      getyx(column_, y, x);
      if (length_max > x) {
        string end_line(length_max - x + 1, ' ');
        wprintw(column_, "%s", end_line.c_str());
      }

      wattroff(column_, A_REVERSE);
    }
    wrefresh(column_);

    print_mail_(current);

    char input = getchar();

    switch (input) {
      case UP:
        current -= current != 0;
        break;
      case DOWN:
        current += current != shown_mails - 1;
        break;
      case EXIT:
        exit = true;
        break;
      case SELECT_MB:
        mailboxs_(length_max, height_max, exit);
        break;
    }
  }

  quit();
}

void TUI::print_mail_(size_t mail_no) {
  element& el = els_[mail_no];

  print_to_window(left_header_, el.left_header);
  print_to_window(right_header_, el.right_header);
  print_to_window(header_, el.header);
  print_to_window(content_, el.content);
}

void TUI::mailboxs_(int length_max, int height_max, bool& exit) {
  size_t shown_folders = std::min((size_t)height_max, folders_.size());
  size_t current = 0;
  bool selection = false;

  while (!exit && !selection) {
    for (size_t i = 0; i < shown_folders; ++i) {
      if (i == current) {
        wattron(column_, A_REVERSE);
      }
      wmove(column_, i + 1, 1);
      string fld = folders_[i];

      if (fld.size() < length_max) {
        wprintw(column_, "%s", fld.c_str());
      } else {
        wprintw(column_, "%s", fld.substr(0, length_max).c_str());
      }

      int x, y;
      getyx(column_, y, x);
      if (length_max > x) {
        string end_line(length_max - x + 1, ' ');
        wprintw(column_, "%s", end_line.c_str());
      }

      wattroff(column_, A_REVERSE);
    }
    wrefresh(column_);

    char input = getchar();

    switch (input) {
      case UP:
        current -= current != 0;
        break;
      case DOWN:
        current += current != shown_folders - 1;
        break;
      case EXIT:
        exit = true;
        break;
      case SELECT:
        selection = true;
        break;
    }
  }
}
