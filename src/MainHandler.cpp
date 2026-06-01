//
// Created by Алексей Подоплелов on 01.06.2026.
//

#include "../include/MainHandler.h"

MainHandler::MainHandler()
{
#ifdef __APPLE__
    this->ITextScannerPtr_ = std::make_unique<IMacos>();
    this->IApiPtr_ = std::make_unique<DeepSeekApiHandler>();
#endif
}

MainHandler::~MainHandler()
{
    m_running = false;
    if ( m_thread.joinable() )
        m_thread.join();
}

void MainHandler::start()
{
    m_thread = std::thread(&MainHandler::checkBufThread, this);
    m_thread.detach();
    ITextScannerPtr_->start_scan(ModeScannerText::Clipboard);
}

void MainHandler::checkBufThread()
{
    std::cout << "checkBuf()\n";
    while( m_running )
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // std::cout << "checking!\n";

        std::string strBuffer = "";
        static std::string prevBuffer = "";
        char buffer[128];

        // !!! for getting text from clipboard on mac we`ll use utility "pbpaste" !!!
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("pbpaste", "r"), pclose);
        if( !pipe ) {
            throw std::runtime_error("error pbpaste");
        }

        while( fgets(buffer, sizeof(buffer), pipe.get()) != nullptr ) {
            strBuffer += buffer;
        }

        if( prevBuffer != strBuffer ) {
            // static int i = 0;
            // std::cout << "found! = " << strBuffer << std::endl;
            prevBuffer = strBuffer;
            str res = this->IApiPtr_->call_api(strBuffer, "");;
            this->ITextScannerPtr_->copy_to_buffer_and_paste(res);
            // ++i;
            // if ( i == 2 )
            //     break;
        }
    }
}