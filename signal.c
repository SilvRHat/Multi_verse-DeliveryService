// Signal Code Pattern Library
// Gavin Zimmerman

// Allows dynamic function calls

// Signal Source
#include "signal.h"


// SOURCE
// SignalFire
    // @brief Invokes all bound functions connected to signal
    // @param s Signal to fire functions connect on
    // @param ... Arguments to pass in as a va_list
void SignalFire(SignalInstance *s, ...) {
    int i=0;
    va_list args;
    
    while (s->_functions[i] != NULL) {
        va_start(args, s);
        ((void(*)())(s->_functions[i]))(args);
        va_end(args);
        i++;
    }
}

// SignalConnect 
    // @brief Binds a provided function to the signal
    // @param s Signal to bind function with
    // @param func Function to bind onto signal
    // @return If connection was successful
int SignalConnect(SignalInstance *s, void* func) {
    if (s->_connections+1==MAX_SIGNAL_CONNECTIONS)
        return -1;
    
    s->_functions[s->_connections] = func;
    s->_connections += 1;
    return 0;
}

// SignalDisconnect
    // @brief Unbinds a provided function from the signal if connected
    // @param s Signal to disconnect function from
    // @param func Function to disconnect
    // @return If disconnection was successful
int SignalDisconnect(SignalInstance *s, void* func) {
    for (int i=0; i<s->_connections; i++) {
        if (s->_functions[i]!=func) 
            continue;
        
        for (; i<s->_connections-1; i++) 
            s->_functions[i]=s->_functions[i+1];
    
        s->_connections--;
        s->_functions[s->_connections]=NULL;
        return 0;
    }
    return -1;
}

// Signal Destroy
    // @brief Unbinds all functions connected to the signal
    // @param s Signal to clean contents of
void SignalDestroy(SignalInstance *s) {
    for (int i=0; i<MAX_SIGNAL_CONNECTIONS; i++)
        s->_functions[i]=NULL;
    s->_connections=0;
}


// DOCUMENTATION
// Test Program
/*
int foo(va_list args) { 
    int x = va_arg(args, int), y = va_arg(args, int);

    printf("foo called with %d %d\n", x ,y);
    return 0;
}

int bar(va_list args) { 
    int x = va_arg(args, int), y = va_arg(args, int);

    printf("bar called with %d %d\n", x ,y);
    return 0;
}

int main() {
    SignalInstance s;
    int x=11, y=5;

    SignalConnect(&s, foo);
    SignalConnect(&s, bar);
    SignalFire(&s, x, y);
}
*/