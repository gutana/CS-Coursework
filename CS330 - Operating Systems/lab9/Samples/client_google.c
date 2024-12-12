/*Code modified from
 * https://www.binarytides.com/socket-programming-c-linux-tutorial/
 */
#include <arpa/inet.h> //inet_addr
#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int socket_desc;
  struct sockaddr_in server;
  char *message, server_reply[2000];

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    printf("Could not create socket");
  }

  server.sin_addr.s_addr = inet_addr("172.217.4.196");
  server.sin_family = AF_INET;
  server.sin_port = htons(80);

  // Connect to remote server
  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    printf("connect error");
    return 1;
  }

  printf("Connected\n");

  // Send some data
  message = strdup("GET / HTTP/1.1\r\n\r\n");
  if (write(socket_desc, message, strlen(message)) < 0)
  {
    printf("Send failed\n");
    return 1;
  }
  printf("Data Send\n");

  // Receive a reply from the server - up to 2000 characters
  if (read(socket_desc, server_reply, 2000) < 0)
  {
    printf("recv failed\n");
  }
  printf("Reply received:\n %s\n", server_reply);

  return 0;
}