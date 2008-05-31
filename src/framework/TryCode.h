#pragma once

class TryCode
{
    public:
    static int Execute(int (*CodeToExecute)(void), void (*CodeToShutdown)(void), char *Name);
    static void Execute(void (*CodeToExecute)(void), void (*CodeToShutdown)(void), char *Name);
};