
/**
 *  Сtrl+B -> translate text
 */

#include <Translater.h>
#include <cstdlib>

int main()
{
    std::unique_ptr<UseClipboard> app = std::make_unique<Translater>();
    return EXIT_SUCCESS;
}
