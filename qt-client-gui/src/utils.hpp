// Copyright 2007 - 2021, Alan Antonuk and the rabbitmq-c contributors.
// SPDX-License-Identifier: mit
#ifndef librabbitmq_examples_utils_h
#define librabbitmq_examples_utils_h

#include <string>

extern void die_on_error(int x, const std::string& context);
extern void die_on_amqp_error(amqp_rpc_reply_t x,  const std::string& context);

#endif
