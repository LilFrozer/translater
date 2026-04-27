#include <ClipboardBuffer.h>
#include <unistd.h>

#ifdef __APPLE__
void MacOSInterfaceClipboard::getTextUser()
{
    m_buffer.clear();

    char buffer[128];

    // !!! for getting text from clipboard on mac we`ll use utility "pbpaste" !!!
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("pbpaste", "r"), pclose);
    if( !pipe ) {
        throw std::runtime_error("error pbpaste");
    }

    while( fgets(buffer, sizeof(buffer), pipe.get()) != nullptr ) {
        this->m_buffer += buffer;
    }

    std::cout << "TEXT: " << m_buffer << std::endl;
}

void MacOSInterfaceClipboard::callKeyCombination(CGKeyCode keyCode, CGEventFlags modifier)
{
    CGEventRef keyDown = CGEventCreateKeyboardEvent(NULL, keyCode, true);
    CGEventSetFlags(keyDown, modifier);
    CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, keyCode, false);

    CGEventPost(kCGSessionEventTap, keyDown);
    usleep(10000);
    CGEventPost(kCGSessionEventTap, keyUp);

    CFRelease(keyDown);
    CFRelease(keyUp);

    this->getTextUser();
}

CGEventRef MacOSInterfaceClipboard::handlerSystemScanning(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    MacOSInterfaceClipboard* ptr = static_cast<MacOSInterfaceClipboard*>(refcon);
    if ( !ptr )
        return event;

    // -> ignoring events, not linked with keyboard
    if( type != kCGEventKeyDown && type != kCGEventKeyUp )
        return event;

    // -> get code of tapped key
    CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    // -> getting modificator (shift, ctrl, alt, cmd)
    CGEventFlags flags = CGEventGetFlags(event);

    bool isCtrlPressed = (flags & kCGEventFlagMaskControl) != 0;    // is ctrl pressed?
    bool isBPressed = (keyCode == KEY_B);                           // is B pressed

    if( isCtrlPressed && isBPressed && type == kCGEventKeyDown ) {
        std::cout << "shortcut 'Ctrl+B' activated!" << std::endl;
        ptr->callKeyCombination(KEY_C, kCGEventFlagMaskCommand);
    }

    return event;
}

void MacOSInterfaceClipboard::startScanUser()
{
    std::cout << "ur welcome to clipboard controller" << std::endl;
    std::cout << "user`s scaner is working! Ctrl+C -> to exit\n";
    std::cout << "Waiting for events..." << std::endl;
    std::cout.flush();
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,     // -> только текущий пользователь
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp),
        &MacOSInterfaceClipboard::handlerSystemScanning,
        this
    );

    if ( !eventTap ) {
        throw std::runtime_error("failed to start scanning");
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    CFRunLoopRun();

    CFRelease(runLoopSource);
    CFRelease(eventTap);
}
#endif
