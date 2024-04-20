#ifndef INITIALIZE_HPP
#define INITIALIZE_HPP
inline bool ENGINE_INITIALIZED = false;


#include "bit_operations.hpp"
#include "magic_values.hpp"
#include "generate_moves.hpp"

void initialize_engine() noexcept;
#endif // INITIALIZE_HPP