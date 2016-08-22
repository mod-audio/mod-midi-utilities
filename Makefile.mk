
CC ?= gcc
CXX ?= g++

_FLAGS    = -Wall -Wextra -O3 -fPIC -Wno-unused-parameter
CFLAGS   += $(_FLAGS) -std=c99
CXXFLAGS += $(_FLAGS) -std=c++11
