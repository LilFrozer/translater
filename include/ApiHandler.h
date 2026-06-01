//
// Created by Алексей Подоплелов on 01.06.2026.
//

#ifndef TRANSLATER_PRJ_APIHANDLER_H
#define TRANSLATER_PRJ_APIHANDLER_H

#include <cstdint>
#include <string>

using u16 = uint16_t;
using str = std::string;
enum class ApiForTranslate : u16 {
    GoogleApi = 0x0,        // -> using google translater -> std::cin == only api key
    YandexApi = 0x1,        // -> using yandex translater -> std::cin == only api key
    DeepseekApi = 0x2,      // -> using deepseek api -> std::cin == only api key
    ProjectService = 0x3,   // -> start microservice in this project with custom llm -> std::cin == null
    CustomApi = 0x4         // -> u will ur put url & api key -> std::cin == url, api key
};
using AFT = ApiForTranslate;

class IApiCaller {
public:
    IApiCaller() = default;
    virtual ~IApiCaller() = default;
    virtual str call_api(const str &buf, const str &api_key) = 0;
};

class DeepSeekApiHandler : public IApiCaller {
private:
    const str host_{"openrouter.ai"};
    const str port_{"443"};
    const str target_{"/api/v1/chat/completions"};
public:
    DeepSeekApiHandler() = default;
    ~DeepSeekApiHandler() override = default;
    str call_api(const str &buf, const str &api_key) override;
};


#endif //TRANSLATER_PRJ_APIHANDLER_H