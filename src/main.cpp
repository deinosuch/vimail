/*

imaps_folders.cpp
-----------------

Connects to IMAP server and lists recursively all folders.


Copyright (C) 2016, Tomislav Karastojkovic (http://www.alepho.com).

Distributed under the FreeBSD license, see the accompanying file LICENSE or
copy at http://www.freebsd.org/copyright/freebsd-license.html.

*/
#include <spdlog/spdlog.h>

#include <iostream>
#include <mailio/imap.hpp>
#include <mailio/message.hpp>
#include <string>

#include "MailClient.h"
#include "TUI.h"

using mailio::message;
using std::cout;
using std::endl;
using std::string;

int main() {
  string server = "gmail.com";

  MailClient client(server);
  client.login("testervimail@gmail.com", "bcgxcgsmtejfdyhu");
  message msg = client.fetch_mail();

  cout << "From: " << msg.from_to_string() << endl;
  cout << "To: " << msg.recipients_to_string() << endl;
  cout << "Subject: " << msg.subject() << endl;

  TUI ui("Inbox", "From", "To", "Subject", "Content");

  ui.quit();

  return 0;
}
