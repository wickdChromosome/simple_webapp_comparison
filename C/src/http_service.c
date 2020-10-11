#include "fio_cli.h"
#include "main.h"
#include <string.h>

#define FILEBUF_SIZE 4000


/* TODO: edit this function to handle HTTP data and answer Websocket requests.*/
static void on_http_request(http_s *h) {

  const char* filename = "index.html";
  intptr_t start_at = 0;
  intptr_t limit = 0;
  size_t filesize;

  //get origin ip
  fio_str_info_s peer_addr = http_peer_addr(h);
  char* peer_ip = peer_addr.data;

  FIOBJ input_body = fiobj_str_buf(FILEBUF_SIZE);

  //set html mimetype
  http_set_header(h, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find("html", 4));

  filesize = fiobj_str_readfile(input_body, filename, start_at, limit);
  if (filesize == FIOBJ_INVALID) puts("Error: file is fucked");

  /* set a response and send it (finnish vs. destroy). */
  char* html_body = fiobj_obj2cstr(input_body).data;
  char* ip_placeholder = strstr(html_body, "IP     ");

  strncpy(ip_placeholder,peer_ip, strlen(peer_ip));

  if (http_send_body(h, html_body, strlen(html_body)) != 0)
 	  puts("Error: couldnt read html");

  fiobj_free(input_body);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  /* listen for inncoming connections */
  if (http_listen(fio_cli_get("-p"), fio_cli_get("-b"),
                  .on_request = on_http_request,
                  .max_body_size = fio_cli_get_i("-maxbd") * 1024 * 1024,
                  .ws_max_msg_size = fio_cli_get_i("-max-msg") * 1024,
                  .public_folder = fio_cli_get("-public"),
                  .log = fio_cli_get_bool("-log"),
                  .timeout = fio_cli_get_i("-keep-alive"),
                  .ws_timeout = fio_cli_get_i("-ping")) == -1) {
    /* listen failed ?*/
    perror("ERROR: facil couldn't initialize HTTP service (already running?)");
    exit(1);
  }
}
