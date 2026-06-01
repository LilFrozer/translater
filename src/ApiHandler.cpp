//
// Created by Алексей Подоплелов on 01.06.2026.
//

#include "../include/ApiHandler.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <json.hpp>

str DeepSeekApiHandler::call_api(const str &buf, const str &api_key) {
    str user_content{"Переведи на русский(В ответ только перевод):" + buf};
    nlohmann::json payload_json;
    payload_json["model"] = "deepseek/deepseek-v4-flash";
    payload_json["messages"] = nlohmann::json::array();
    payload_json["messages"].push_back({{"role", "user"}, {"content", user_content}});
    payload_json["max_tokens"] = 100;
    payload_json["stream"] = false;
    str payload = payload_json.dump();

    // инициализируем io_context
    boost::asio::io_context ioc;
    boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);

    // Загружаем корневые сертификаты из системы
    ctx.set_default_verify_paths();
    // Включаем проверку сертификата сервера
    ctx.set_verify_mode(boost::asio::ssl::verify_peer);

    boost::beast::ssl_stream<boost::beast::tcp_stream> stream(ioc, ctx);

    // установка tcp соединения
    boost::asio::ip::tcp::resolver resolver(ioc);
    auto const results = resolver.resolve(host_, port_);
    boost::beast::get_lowest_layer(stream).connect(results);

    // tls рукопожатие
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host_.c_str())) {
        boost::system::error_code ec(static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category());
        throw boost::system::system_error(ec);
    }
    stream.handshake(boost::asio::ssl::stream_base::client);

    // http-запрос
    boost::beast::http::request<boost::beast::http::string_body> req;
    req.method(boost::beast::http::verb::post);
    req.target(target_);
    req.version(11);  // HTTP/1.1
    req.set(boost::beast::http::field::host, host_);
    req.set(boost::beast::http::field::authorization, "Bearer " + api_key);
    req.set(boost::beast::http::field::content_type, "application/json");
    req.body() = payload;
    req.content_length(payload.size());
    req.set(boost::beast::http::field::connection, "keep-alive");

    // отправили запрос
    boost::beast::http::write(stream, req);

    // читаем ответ
    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::string_body> res;
    boost::beast::http::read(stream, buffer, res);

    str reply{};
    try {
        std::cout << "HTTP " << res.result_int() << " " << res.reason() << std::endl;
        nlohmann::json data = nlohmann::json::parse(res.body());
        // -> согласно документации DeepSeek, ответ приходит в поле choices[0].message.content
        if (!data["choices"][0]["message"]["content"].is_null())
            reply = data["choices"][0]["message"]["content"];
        std::cout << "reply =" << reply << std::endl;
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error(e.what());
    } catch (const std::exception& e) {
        throw std::runtime_error(e.what());
    }

    // закрываем соединение
    boost::beast::error_code ec;
    stream.shutdown(ec);
    (void)ec;

    return reply;
}
