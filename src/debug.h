#pragma once

#ifndef RELEASE
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif
