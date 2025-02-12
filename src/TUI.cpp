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
  getchar();
  endwin();
}

void TUI::add_element(element&& el) {
  els.push_back(std::move(el));
  spdlog::info("Added new mail: " + els[els.size() - 1].header);
}

void TUI::populate() {
  for (size_t i = 0; i < els.size(); ++i) {
    wmove(column_, i + 1, 1);

    wattron(column_, A_BOLD);
    wprintw(column_, "%s", els[i].header.c_str());
    wattroff(column_, A_BOLD);

    wprintw(column_, " %s", els[i].left_header.c_str());
  }
  wrefresh(column_);
}
