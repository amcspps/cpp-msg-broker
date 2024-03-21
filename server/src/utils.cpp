// Copyright (mostly) 2007 - 2021, Alan Antonuk and the rabbitmq-c contributors.
// SPDX-License-Identifier: mit
// + Pavel Popov additions 2024

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/framing.h>
#include <stdint.h>
#include <stdexcept>
#include "utils.hpp"
#include <iomanip>
#include <glog/logging.h>

void die_on_error(int x, const std::string& context) {
  if (x < 0) {
    throw std::runtime_error(context + ": " + std::to_string(x));
  }
}

void die_on_amqp_error(amqp_rpc_reply_t x, const std::string& context) {
  switch (x.reply_type) {
    case AMQP_RESPONSE_NORMAL:
      return;

    case AMQP_RESPONSE_NONE:
      LOG(ERROR) << context + ": " + "missing RPC reply type";
      throw std::runtime_error(context + ": " + "missing RPC reply type");

    case AMQP_RESPONSE_LIBRARY_EXCEPTION:
      LOG(ERROR) << context + ": " + std::to_string(x.library_error);
      throw std::runtime_error(context + ": " + std::to_string(x.library_error));

    case AMQP_RESPONSE_SERVER_EXCEPTION:
      switch (x.reply.id) {
        case AMQP_CONNECTION_CLOSE_METHOD: {
          amqp_connection_close_t *m =
              (amqp_connection_close_t *)x.reply.decoded;
          LOG(ERROR) << context + ": server connection error " + std::to_string(m->reply_code)
                                   + ", message: " + std::string(reinterpret_cast<char *>(m->reply_text.bytes), m->reply_text.len);
          throw std::runtime_error(context + ": server connection error " + std::to_string(m->reply_code)
                                   + ", message: " + std::string(reinterpret_cast<char *>(m->reply_text.bytes), m->reply_text.len));
        }
        case AMQP_CHANNEL_CLOSE_METHOD: {
          amqp_channel_close_t *m = (amqp_channel_close_t *)x.reply.decoded;
          LOG(ERROR) << context + ": server channel error " + std::to_string(m->reply_code)
                                   + ", message: " + std::string(reinterpret_cast<char *>(m->reply_text.bytes), m->reply_text.len);
          throw std::runtime_error(context + ": server channel error " + std::to_string(m->reply_code)
                                   + ", message: " + std::string(reinterpret_cast<char *>(m->reply_text.bytes), m->reply_text.len));
        }
        default:
          std::stringstream ss;
          ss << context << ": unknown server error, method id 0x" << std::hex << x.reply.id;
          LOG(ERROR) << ss.str();
          throw std::runtime_error(ss.str());
      }
      break;
  }
}