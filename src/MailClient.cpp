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

#include <list>
#include <mailio/mime.hpp>

using mailio::dialog_error;
using mailio::imap_error;
using mailio::imaps;
using mailio::message;
using std::list;
using std::map;
using std::string;
using std::vector;

vector<string> extract_folders(const imaps::mailbox_folder_t &folder) {
  vector<string> flds;
  for (auto &f : folder.folders) {
    flds.push_back(f.first);
    if (!f.second.folders.empty()) {
      vector<string> ff = extract_folders(f.second);
      flds.insert(flds.end(), ff.begin(), ff.end());
    }
  }
  return flds;
}

void MailClient::login(const string &mail, const string &password) {
  try {
    connection_.authenticate(mail, password, imaps::auth_method_t::LOGIN);
  } catch (imap_error &exc) {
    spdlog::error(exc.what());
  } catch (dialog_error &exc) {
    spdlog::error(exc.what());
  }

  spdlog::info("Successfully logged in");
}

MailClient::MailClient(const string &server)
    : connection_(imaps("imap." + server, IMAPS_SERVER)) {
  curr_mailbox_ = "inbox";
  spdlog::info("Connected to server: " + server);
}

map<unsigned long, message> MailClient::fetch_mail() {
  imaps::mailbox_stat_t stats = connection_.select(curr_mailbox_);
  map<unsigned long, message> messs;
  list<imaps::messages_range_t> range = {{1, stats.messages_no}};
  try {
    connection_.fetch(range, messs, false, false,
                      mailio::codec::line_len_policy_t::NONE);
  } catch (const mailio::mime_error &exc) {
    spdlog::error(exc.what());
    throw exc;
  }

  spdlog::info("Fetched messages from: " + curr_mailbox_);
  return messs;
}

vector<string> MailClient::get_folders() {
  imaps::mailbox_folder_t flds = connection_.list_folders("");
  return extract_folders(flds);
}

void MailClient::select_mailbox(const string &mb) { connection_.select(mb); }
