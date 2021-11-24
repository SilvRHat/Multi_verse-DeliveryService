// Signal - Header
// Programming pattern to provide easy calls on functions
// Gavin Zimmerman

#ifndef SIGNAL_H
#define SIGNAL_H

// DEPENDENCIES
#include <stdarg.h>
#include <stddef.h>

// CLASSES
#define MAX_SIGNAL_CONNECTIONS 64
struct signal_s {
    int _connections;
    void* _functions[MAX_SIGNAL_CONNECTIONS];
};
typedef struct signal_s signal;
#define NewSignal {0, {NULL}}


// MEMBERS
void SignalFire(signal *s, ...);
int SignalConnect(signal *s, void* func);
int SignalDisconnect(signal *s, void* func);
void SignalDestroy(signal *s);

#endif