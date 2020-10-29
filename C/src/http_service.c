#include "fio_cli.h"
#include "main.h"
#include <string.h>

#define FILEBUF_SIZE 4000


/* TODO: edit this function to handle HTTP data and answer Websocket requests.*/
static void on_http_request(http_s *h) {

  //get origin ip
  fio_str_info_s peer_addr = http_peer_addr(h);
  char* peer_ip = peer_addr.data;

  //set html mimetype
  http_set_header(h, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find("html", 4));

  /* set a response and send it (finnish vs. destroy). */
  char* html_text = "<html>\
    <head>\
    </head>\
    <body>\
        <h1>Your ip address is: IP            </h1>\
        <p> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Amet purus gravida quis blandit turpis cursus in hac. Id aliquet lectus proin nibh nisl condimentum id venenatis. Mi in nulla posuere sollicitudin aliquam. Vulputate odio ut enim blandit volutpat maecenas. Donec adipiscing tristique risus nec. Tempus iaculis urna id volutpat lacus laoreet non curabitur gravida. Dui accumsan sit amet nulla facilisi morbi tempus iaculis urna. Nec ultrices dui sapien eget mi proin sed libero. Dui ut ornare lectus sit. Dictumst quisque sagittis purus sit amet. Non odio euismod lacinia at quis. Nullam vehicula ipsum a arcu cursus. Vestibulum lectus mauris ultrices eros.\
            Eu mi bibendum neque egestas. Elit sed vulputate mi sit. Id diam maecenas ultricies mi. Sit amet est placerat in egestas erat imperdiet. Etiam erat velit scelerisque in dictum non. Aliquam faucibus purus in massa. Imperdiet dui accumsan sit amet nulla facilisi morbi tempus. A cras semper auctor neque vitae tempus quam pellentesque. Vel orci porta non pulvinar neque laoreet suspendisse. Varius morbi enim nunc faucibus a. Ultricies mi eget mauris pharetra. Lobortis elementum nibh tellus molestie nunc. Sit amet volutpat consequat mauris. Urna condimentum mattis pellentesque id. In hac habitasse platea dictumst. Sapien eget mi proin sed. Sit amet mauris commodo quis imperdiet massa. Dolor purus non enim praesent elementum facilisis leo vel fringilla. Vitae proin sagittis nisl rhoncus mattis rhoncus urna.\
            Cras adipiscing enim eu turpis. Aliquam ultrices sagittis orci a scelerisque. Enim eu turpis egestas pretium. Gravida quis blandit turpis cursus. Id diam maecenas ultricies mi eget mauris pharetra et ultrices. Habitant morbi tristique senectus et netus et malesuada fames ac. Molestie at elementum eu facilisis sed odio. Dui faucibus in ornare quam viverra orci sagittis eu volutpat. Laoreet sit amet cursus sit. Blandit volutpat maecenas volutpat blandit aliquam etiam. Id faucibus nisl tincidunt eget nullam non nisi est sit. Cursus mattis molestie a iaculis. Tellus at urna condimentum mattis pellentesque id nibh tortor id. Natoque penatibus et magnis dis parturient montes nascetur ridiculus mus. Amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor. In fermentum posuere urna nec. Ullamcorper dignissim cras tincidunt lobortis feugiat vivamus at augue eget. Dolor sed viverra ipsum nunc aliquet bibendum enim facilisis. Dolor sed viverra ipsum nunc aliquet bibendum enim.\
            Nullam vehicula ipsum a arcu cursus vitae congue mauris. Libero nunc consequat interdum varius sit. Aliquet risus feugiat in ante metus dictum. Amet mattis vulputate enim nulla aliquet porttitor lacus luctus. Suspendisse ultrices gravida dictum fusce ut. Aenean et tortor at risus viverra. Eget egestas purus viverra accumsan in nisl nisi scelerisque. Id diam maecenas ultricies mi eget mauris. Orci eu lobortis elementum nibh tellus molestie nunc non. Risus nec feugiat in fermentum posuere urna nec tincidunt. Nunc aliquet bibendum enim facilisis gravida neque convallis a cras. Vel pretium lectus quam id leo in vitae turpis massa. Enim tortor at auctor urna nunc id cursus. Consequat ac felis donec et odio pellentesque diam volutpat commodo.\
            Leo urna molestie at elementum eu. Neque convallis a cras semper auctor neque. Sed blandit libero volutpat sed cras ornare arcu. Euismod elementum nisi quis eleifend quam adipiscing vitae. Vestibulum lorem sed risus ultricies tristique nulla. Neque vitae tempus quam pellentesque nec nam aliquam sem et. Pellentesque sit amet porttitor eget dolor morbi non arcu. Sit amet mauris commodo quis imperdiet. Duis at tellus at urna condimentum mattis. Vitae proin sagittis nisl rhoncus mattis rhoncus urna neque viverra. A iaculis at erat pellentesque adipiscing commodo elit at. Maecenas pharetra convallis posuere morbi leo. </p>\
</body>\
</html>";
  char* html_body = (char*) malloc(strlen(html_text) + 1);
  strcpy(html_body, html_text);

  char* ip_placeholder = strstr(html_body, "IP     ");
  strncpy(ip_placeholder,peer_ip, strlen(peer_ip));

  if (http_send_body(h, html_body, strlen(html_body)) != 0)
 	  puts("Error: couldnt read html");

  free(html_body);
  //fiobj_free(input_body);
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
