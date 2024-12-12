/*Modified from https://www.binarytides.com/socket-programming-c-linux-tutorial/
 */
#include <arpa/inet.h>
#include <netdb.h>  //hostent
#include <stdio.h>  //printf
#include <string.h> //strcpy
#include <sys/socket.h>

int main(int argc, char *argv[])
{
  // char *hostname = "www.google.com";
  char ip[100];
  struct hostent *he;
  struct in_addr **addr_list;
  int i;

  char hostname[50];
  printf("\n hostname:");
  scanf("%s", hostname);

  if ((he = gethostbyname(hostname)) == NULL)
  {
    // gethostbyname failed
    herror("gethostbyname");
    return 1;
  }

  // Cast the h_addr_list to in_addr , since h_addr_list also has the ip address
  // in long format only
  addr_list = (struct in_addr **)he->h_addr_list;

  // Return the first one;
  strcpy(ip, inet_ntoa(*addr_list[0]));

  printf("%s resolved to : %s\n", hostname, ip);
  return 0;
}