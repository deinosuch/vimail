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

#ifndef INCLUDE_MAILCLIENT_H_
#define INCLUDE_MAILCLIENT_H_

#include <mailio/imap.hpp>
#include <map>
#include <string>

constexpr int IMAPS_SERVER = 993;

class MailClient {
 private:
  mailio::imaps connection_;
  std::string curr_mailbox_;

 public:
  /**
   * Creates a MailClient object and connects to a mail server
   *
   * @param server Server to connect to
   */
  explicit MailClient(const std::string &server);

  /**
   * Logs in to a mail server
   *
   * \param mail Mail address
   * \param password Password for the address
   */
  void login(const std::string &mail, const std::string &password);

  /**
   * Gets mail from a selected mail folder
   *
   * @param messs Map to be filled with mails
   */
  void fetch_mail(std::map<unsigned long, mailio::message> &messs);
};

#endif  // INCLUDE_MAILCLIENT_H_
