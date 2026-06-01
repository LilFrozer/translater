#ifndef TEXT_SCANNER_H
#define TEXT_SCANNER_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <ApiHandler.h>

enum class ModeScannerText : u16 {
    Clipboard = 0x0,
    ScreenArea = 0x1
};
using MST = ModeScannerText;

// -> interface scanner -> needed to check if u are on MAC or WIN
class ITextScaner {
public:
    ITextScaner() = default;

    virtual ~ITextScaner() = default;
    virtual void start_scan(MST mst) = 0;
    virtual void copy_to_buffer_and_paste(const std::string& text) = 0;
};

/**
 *  apple
 */
#ifdef __APPLE__

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#define KEY_C 0x08
#define KEY_B 0x11
#define KEY_V 0x09
#define USLEEP_TIMEOUT 10000
class IMacos : public ITextScaner
{
private:
    static CGEventRef handler_system_scanning(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);
    void call_key_combination(CGKeyCode keyCode, CGEventFlags modifier);
    void waiting_for_using();
public:
    void start_scan(MST mst) override;
    void copy_to_buffer_and_paste(const std::string& text) override;
    IMacos() = default;
    ~IMacos() override = default;
};
#endif

#endif
