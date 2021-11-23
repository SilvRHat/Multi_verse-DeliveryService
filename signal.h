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
struct signal {
    int _connections;
    void* _functions[MAX_SIGNAL_CONNECTIONS];
};
#define NewSignal {0, {NULL}}


// MEMBERS
void SignalFire(struct signal *s, ...);
int SignalConnect(struct signal *s, void* func);
int SignalDisconnect(struct signal *s, void* func);
void SignalDestroy(struct signal *s);

#endif