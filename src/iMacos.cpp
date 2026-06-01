//
// Created by Алексей Подоплелов on 01.06.2026.
//
#include  <TextScanner.h>
#ifdef __APPLE__

// -> вызываем из кода нажатие нужной нам комбинации клавиш
// keyCode = a, b, c or ...
// modifier = shift, ctrl, alt, cmd
void IMacos::call_key_combination(CGKeyCode keyCode, CGEventFlags modifier) {
    CGEventRef keyDown = CGEventCreateKeyboardEvent(NULL, keyCode, true);
    CGEventSetFlags(keyDown, modifier);
    CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, keyCode, false);

    CGEventPost(kCGSessionEventTap, keyDown);
    usleep(USLEEP_TIMEOUT);
    CGEventPost(kCGSessionEventTap, keyUp);

    CFRelease(keyDown);
    CFRelease(keyUp);
}

CGEventRef IMacos::handler_system_scanning(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    // ptr на себя
    auto *ptr = static_cast<IMacos*>(refcon);
    if ( !ptr )
        return event;

    // игнорируем все, что не связано с нажатиями клавиатуры
    if ( type != kCGEventKeyDown && type != kCGEventKeyUp )
        return event;

    // получаем коды нажатой клавиши и модификатора
    // CGKeyCode keyCode = static_cast<CGKeyCode>(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
    // CGEventFlags flags = CGEventGetFlags(event);

    // если нажали нужные клавиши, то захватываем текст
    // bool isCtrlPressed = (flags & kCGEventFlagMaskControl) != 0;    // is ctrl pressed?
    // bool isBPressed = (keyCode == KEY_B);                           // is B pressed
    // if( isCtrlPressed && isBPressed && type == kCGEventKeyDown ) {
    //     std::cout << "shortcut(macos) 'Ctrl+B' activated!" << std::endl;
    //     ptr->call_key_combination(KEY_C, kCGEventFlagMaskCommand);
    // }

    return event;
}

// -> начинаем сканировать и ждем ввод
void IMacos::waiting_for_using() {
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,     // -> только текущий пользователь
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp),
        &IMacos::handler_system_scanning,
        this
    );

    if ( !eventTap ) {
        throw std::runtime_error("ec: failed to start scanning clipboard");
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    CFRunLoopRun();

    CFRelease(runLoopSource);
    CFRelease(eventTap);
}

// -> выбираем режим
void IMacos::start_scan(MST mst) {
    switch (mst) {
        case MST::Clipboard: {
            std::cout << "mode:clipboard_scanner. use Ctrl+B" << std::endl;
            std::cout.flush();
            this->waiting_for_using();
            break;
        }
        case MST::ScreenArea: {
            break;
        }
        default: {
            break;
        }
    }
}

void IMacos::copy_to_buffer_and_paste(const std::string& text) {
    std::string escapedText = text;
    size_t pos = 0;
    while ((pos = escapedText.find('"', pos)) != std::string::npos) {
        escapedText.replace(pos, 1, "\\\"");
        pos += 2;
    }
    pos = 0;
    while ((pos = escapedText.find('\\', pos)) != std::string::npos) {
        if (pos + 1 < escapedText.size() && escapedText[pos + 1] == '"') {
            pos += 2;
            continue;
        }
        escapedText.replace(pos, 1, "\\\\");
        pos += 2;
    }
    std::string command = "echo \"" + escapedText + "\" | pbcopy";
    system(command.c_str());
    usleep(USLEEP_TIMEOUT);
    this->call_key_combination(KEY_V, kCGEventFlagMaskCommand);
}

#endif