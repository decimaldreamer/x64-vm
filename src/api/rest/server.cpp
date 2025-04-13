#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <memory>
#include <string>
#include "server.h"

namespace x64vm {
namespace api {
namespace rest {

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class RESTServer {
public:
    RESTServer(const std::string& address) {
        listener_ = std::make_unique<http_listener>(address);
        listener_->support(methods::GET, std::bind(&RESTServer::handle_get, this, std::placeholders::_1));
        listener_->support(methods::POST, std::bind(&RESTServer::handle_post, this, std::placeholders::_1));
        listener_->support(methods::PUT, std::bind(&RESTServer::handle_put, this, std::placeholders::_1));
        listener_->support(methods::DEL, std::bind(&RESTServer::handle_delete, this, std::placeholders::_1));
    }

    void start() {
        listener_->open().wait();
    }

    void stop() {
        listener_->close().wait();
    }

private:
    void handle_get(http_request request) {
        auto path = request.relative_uri().path();
        
        if (path == "/api/v1/status") {
            handle_status(request);
        } else if (path == "/api/v1/metrics") {
            handle_metrics(request);
        } else {
            request.reply(status_codes::NotFound);
        }
    }

    void handle_post(http_request request) {
        auto path = request.relative_uri().path();
        
        if (path == "/api/v1/execute") {
            handle_execute(request);
        } else {
            request.reply(status_codes::NotFound);
        }
    }

    void handle_put(http_request request) {
        auto path = request.relative_uri().path();
        
        if (path == "/api/v1/config") {
            handle_config(request);
        } else {
            request.reply(status_codes::NotFound);
        }
    }

    void handle_delete(http_request request) {
        auto path = request.relative_uri().path();
        
        if (path == "/api/v1/cache") {
            handle_clear_cache(request);
        } else {
            request.reply(status_codes::NotFound);
        }
    }

    void handle_status(http_request request) {
        json::value response;
        response["status"] = json::value::string("running");
        response["version"] = json::value::string("1.0.0");
        response["uptime"] = json::value::number(get_uptime());
        
        request.reply(status_codes::OK, response);
    }

    void handle_metrics(http_request request) {
        json::value response;
        response["instructions_executed"] = json::value::number(get_instructions_executed());
        response["memory_usage"] = json::value::number(get_memory_usage());
        response["cpu_usage"] = json::value::number(get_cpu_usage());
        
        request.reply(status_codes::OK, response);
    }

    void handle_execute(http_request request) {
        request.extract_json().then([=](json::value body) {
            try {
                auto code = body["code"].as_string();
                auto result = execute_code(code);
                
                json::value response;
                response["result"] = json::value::string(result);
                request.reply(status_codes::OK, response);
            } catch (const std::exception& e) {
                request.reply(status_codes::BadRequest, e.what());
            }
        });
    }

    void handle_config(http_request request) {
        request.extract_json().then([=](json::value body) {
            try {
                update_config(body);
                request.reply(status_codes::OK);
            } catch (const std::exception& e) {
                request.reply(status_codes::BadRequest, e.what());
            }
        });
    }

    void handle_clear_cache(http_request request) {
        clear_cache();
        request.reply(status_codes::OK);
    }

    std::unique_ptr<http_listener> listener_;
};

} // namespace rest
} // namespace api
} // namespace x64vm 