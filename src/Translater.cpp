#include "ClipboardBuffer.h"
#include <Translater.h>
#include <unistd.h>
#include <memory>

Translater::Translater() : UseClipboard{}
{
    std::cout << "Translater()\n";

}

Translater::~Translater()
{
}

void Translater::useText()
{
    std::cout << "TEXT: " << m_interfacePtr->get_buffer() << std::endl;
}
