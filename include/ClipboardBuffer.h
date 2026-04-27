#ifndef CLIPBOARDBUFFER_H
#define CLIPBOARDBUFFER_H

#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <unistd.h>

// 1.0
class BaseInterfaceClipboard
{
protected:
    std::string m_buffer{""};
public:
    BaseInterfaceClipboard() = default;
    virtual ~BaseInterfaceClipboard() = default;
    virtual void startScanUser() = 0;
    const std::string &get_buffer() const
    {
        return m_buffer;
    }
};

#ifdef __APPLE__
    #include <CoreGraphics/CoreGraphics.h>
    #include <CoreFoundation/CoreFoundation.h>
    #define KEY_C 8
    #define KEY_B 11

/**
*   for macOS lol
*   1.1
*/
class MacOSInterfaceClipboard : public BaseInterfaceClipboard
{
private:
    static CGEventRef handlerSystemScanning(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);
    void callKeyCombination(CGKeyCode keyCode, CGEventFlags modifier);
public:
    void startScanUser() override final;
    MacOSInterfaceClipboard() = default;
    ~MacOSInterfaceClipboard() = default;
};
#endif

#include <thread>

// 2.0
class UseClipboard
{
private:
    // -> separation on macOS/Win/Linux
    std::unique_ptr<BaseInterfaceClipboard> m_interfacePtr;

    // -> thread, cause on mac it working with issue, soooo...
    // i found solution)))
    std::thread m_thread;
    void checkBufThread();
    std::atomic<bool> m_running{true};
public:
    UseClipboard();
    virtual ~UseClipboard();
    void start();
    virtual void useBuffer(const std::string &text) = 0;
};

#endif
