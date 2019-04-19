
#ifndef TELEGRAMSTER_LOGGER_H
#define TELEGRAMSTER_LOGGER_H

#ifdef __cplusplus
extern "C" void logIt(const char* message);
#else
extern void logIt(const char* message);
#endif

#endif
