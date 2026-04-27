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
    virtual void getTextUser() = 0;
public:
    virtual void startScanUser() = 0;
    const std::string &get_buffer() const {
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
    void getTextUser() override final;
    static CGEventRef handlerSystemScanning(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);
    void callKeyCombination(CGKeyCode keyCode, CGEventFlags modifier);
public:
    void startScanUser() override final;
};
#endif

// 2.0
class UseClipboard
{
protected:
    std::unique_ptr<BaseInterfaceClipboard> m_interfacePtr;
public:
    UseClipboard()
    {
        std::cout << "UseClipboard()\n";
#ifdef __APPLE__
        m_interfacePtr = std::make_unique<MacOSInterfaceClipboard>();
#endif
        m_interfacePtr->startScanUser();
    }
};

#endif
