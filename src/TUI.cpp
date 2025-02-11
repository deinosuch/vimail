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

using std::string;

void TUI::init() {
  initscr();
  noecho();

  int split = COLS / SPLIT_RATIO;

  WINDOW* list = create_window(LINES, split, 0, 0);
  mvwprintw(list, 0, 3, "INBOX");
  wattron(list, A_REVERSE);
  mvwprintw(list, 1, 1, "Subject - sender");

  wattroff(list, A_REVERSE);
  wrefresh(list);

  WINDOW* content = create_window(LINES, COLS - split, 0, split);

  /*spdlog::info("TUI initialized");*/

  getchar();
}

void TUI::quit() { endwin(); }

WINDOW* TUI::create_window(int height, int width, int starty, int startx) {
  WINDOW* local_window = newwin(height, width, starty, startx);
  box(local_window, 0, 0);
  wrefresh(local_window);

  return local_window;
}

void TUI::add_element(const string& name, const string& content) {}
