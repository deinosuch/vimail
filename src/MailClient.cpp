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

#include "MailClient.h"

#include <spdlog/spdlog.h>

#include <string>

using mailio::dialog_error;
using mailio::imap_error;
using mailio::imaps;
using mailio::message;
using std::string;

void MailClient::login(string mail, string password) {
  try {
    connection_.authenticate(mail, password, imaps::auth_method_t::LOGIN);
  } catch (imap_error &exc) {
    spdlog::error(exc.what());
  } catch (dialog_error &exc) {
    spdlog::error(exc.what());
  }
}

MailClient::MailClient(string &server)
    : connection_(imaps("imap." + server, IMAPS_SERVER)) {}

message MailClient::fetch_mail() {
  message msg;
  connection_.fetch("inbox", 2, msg);
  return msg;
}
