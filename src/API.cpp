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

#include "API.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <mailio/message.hpp>
#include <string>

#include "MailClient.h"
#include "TUI.h"

using mailio::message;
using std::map;
using std::string;

TUI::element export_message(const message& mess) {
  std::string content;
  if (mess.content_type().type == mailio::mime::media_type_t::MULTIPART) {
    content = mess.parts()[0].content();
  } else if (mess.content_type().type == mailio::mime::media_type_t::TEXT) {
    content = mess.content();
  } else {
    spdlog::error("Unknown mail media type");
  }

  return {mess.from_to_string(), mess.recipients_to_string(), mess.subject(),
          content};
}

void API::init() {
  auto logger = spdlog::basic_logger_mt("file_logger", "logs.txt", true);
  spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v");
  logger->flush_on(spdlog::level::info);
  spdlog::set_default_logger(logger);

  string server = "gmail.com";

  spdlog::info("Initializing...");

  MailClient mc(server);
  mc.login("testervimail@gmail.com", "bcgxcgsmtejfdyhu");
  map<unsigned long, message> msgs;
  mc.fetch_mail(msgs);

  TUI ui("Inbox", "From", "To", "Subject", "Content");
  for (unsigned long i = msgs.size(); i >= 1; --i) {
    ui.add_element(export_message(msgs[i]));
  }

  ui.run();

  ui.quit();
}
