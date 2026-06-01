//
// Created by Алексей Подоплелов on 01.06.2026.
//

#ifndef TRANSLATER_PRJ_MAINHANDLER_H
#define TRANSLATER_PRJ_MAINHANDLER_H

#include <TextScanner.h>
#include <thread>

class MainHandler
{
private:
    // -> macos/win32
    std::unique_ptr<ITextScaner> ITextScannerPtr_;
    std::unique_ptr<IApiCaller> IApiPtr_;

    std::thread m_thread;
    void checkBufThread();
    std::atomic<bool> m_running{true};
public:
    MainHandler();
    ~MainHandler();
    void start();
};


#endif //TRANSLATER_PRJ_MAINHANDLER_H