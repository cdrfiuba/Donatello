
#pragma once

#include <stdint.h>

#define STATE(state_name) void state_##state_name##_function()

#define EXEC_STATE(state) dispatch_table[static_cast<uint8_t>(state)]()

using StateFunctionType = void(*)();

void state_init_function();
void state_follow_line_function();

enum class State : uint8_t {
  init = 0,
  follow_line = 1
};

StateFunctionType dispatch_table[2] = {
  state_init_function,
  state_follow_line_function
};
