// Signal - Header
// Programming pattern to provide easy calls on functions
// Gavin Zimmerman

#ifndef SIGNAL_H
#define SIGNAL_H

// DEPENDENCIES
#include <stdarg.h>
#include <stddef.h>

// CLASSES
#define NewSignal {0, {NULL}}
#define MAX_SIGNAL_CONNECTIONS 64
struct SignalInstance_s {
    int _connections;
    void* _functions[MAX_SIGNAL_CONNECTIONS];
};
typedef struct SignalInstance_s SignalInstance;


// MEMBERS
void SignalFire(SignalInstance *s, ...);
int SignalConnect(SignalInstance *s, void* func);
int SignalDisconnect(SignalInstance *s, void* func);
void SignalDestroy(SignalInstance *s);

#endif