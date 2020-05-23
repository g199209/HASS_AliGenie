#include <cinatra.hpp>
#include <iostream>

using std::cout;
using std::endl;

struct PrintContent {
  bool before(cinatra::request& req, cinatra::response& res) {
    cout << "=====================================================" << endl;
    cout << "Req: " << endl;
    cout << req.head() << endl << endl;
    cout << req.body() << endl << endl;
    return true;
  }

  bool after(cinatra::request& req, cinatra::response& res) {
    cout << "Rsp: " << endl;
    cout << res.response_str() << endl;
    cout << "=====================================================" << endl
         << endl;
    return true;
  }
};

int main() {
  uint32_t max_thread_num = std::thread::hardware_concurrency();
  cinatra::http_ssl_server server(max_thread_num);
  server.set_ssl_conf({"hass.gaomf.store.crt", "hass.gaomf.store.key"});
  auto r = server.listen("10.9.56.154", "443");
  if (!r) {
    cout << "Listen FAILED!" << endl;
    return -1;
  }
  server.set_http_handler<cinatra::GET>(
      "/auth/authorize",
      [](cinatra::request& req, cinatra::response& res) {
        std::string url(req.get_query_value("redirect_uri"));
        url += "&code=testcode";

        //        res.set_status_and_content(cinatra::status_type::ok,
        //        "1111111111");

        res.redirect(url);
        //        res.set_status_and_content(cinatra::status_type::temporary_redirect,
        //                                   std::move(url));
      },
      PrintContent{});
  server.set_http_handler<cinatra::POST>(
      "/auth/token",
      [](cinatra::request& req, cinatra::response& res) {
        std::string url(req.get_query_value("redirect_uri"));
        res.add_header("Cache-Control", "no-store");
        res.add_header("Pragma", "no-cache");
        res.set_status_and_content(
            cinatra::status_type::ok,
            "{"
            "\"access_token\": \"53919549-2f37-4622-88d0-08ea9d8cb291\","
            "\"token_type\": \"bearer\","
            "\"expires_in\":7200,"
            "\"refresh_token\": \"a65cd42a-93d5-4f3d-8966-9324da28ccb0\""
            "}",
            cinatra::res_content_type::json);
      },
      PrintContent{});

  cout << "Start Listen!" << endl;
  server.run();
  return 0;
}
