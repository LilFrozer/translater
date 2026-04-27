#ifndef TRANSLATER_H
#define TRANSLATER_H

#include <ClipboardBuffer.h>

// 2.1
class Translater : public UseClipboard
{
public:
    Translater();
    ~Translater();
    void useText();
};

#endif
