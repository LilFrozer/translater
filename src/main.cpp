
/**
 *  Сtrl+B -> translate text
 */

#include <MainHandler.h>
#include <cstdlib>

int main()
{
    std::unique_ptr<MainHandler> app = std::make_unique<MainHandler>();
    app->start();
    return EXIT_SUCCESS;
}
