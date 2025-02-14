/*
 * Copyright (c) 2025 Adam Paseka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INCLUDE_TUI_H_
#define INCLUDE_TUI_H_

#include <ncurses.h>

#include <string>
#include <vector>

constexpr int SPLIT_RATIO = 4;
constexpr int TITLE_OFFSET = 3;
constexpr int HEADER_HEIGHT = 3;

constexpr char UP = 'k';
constexpr char DOWN = 'j';

class TUI {
 public:
  struct element {
    std::string left_header;
    std::string right_header;
    std::string header;
    std::string content;
  };
  TUI(const std::string& column_title, const std::string& left_header_title,
      const std::string& right_header_title, const std::string& header_title,
      const std::string& content_title);
  void quit();
  void add_element(element&& el);
  void run();

 private:
  WINDOW* column_;
  WINDOW* content_;
  WINDOW* left_header_;
  WINDOW* right_header_;
  WINDOW* header_;
  std::vector<element> els_;
  size_t current_;

  void print_mail_(size_t mail_no);
};

#endif  // INCLUDE_TUI_H_
