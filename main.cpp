#include <cinatra.hpp>
#include <iostream>

int main() {
  uint32_t max_thread_num = std::thread::hardware_concurrency();
  cinatra::http_server server(max_thread_num);
  server.listen("0.0.0.0", "80");
  server.set_http_handler<cinatra::GET, cinatra::POST>(
      "/", [](cinatra::request &req, cinatra::response &res) {
        res.set_status_and_content(cinatra::status_type::ok, "hello world");
      });

  server.run();
  return 0;
}
