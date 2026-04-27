#include <Translater.h>

Translater::Translater() : UseClipboard{}
{
    std::cout << "Translater()\n";
    this->start();
}

Translater::~Translater()
{
    std::cout << "~Translater()\n";
}

void Translater::useBuffer(const std::string &text)
{
    std::cout << "text-> " << text << std::endl;
}
