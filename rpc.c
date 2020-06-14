#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdio.h>

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

void getWalletaddress(char* wallet, char* string_info){
    for(int i =11; i < 46; i ++){ // 46
        wallet[i-11] = string_info[i];
    }
}

void rpc_getnewaddress(char* address, char* wallet) {
  CURL *curl = curl_easy_init();
  struct curl_slist *headers = NULL;

  if (curl) {
      struct string s;
      init_string(&s);
      const char *getnewaddress =
        "{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"getnewaddress\", \"params\": [] }";

      headers = curl_slist_append(headers, "content-type: text/plain;");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      curl_easy_setopt(curl, CURLOPT_URL, address);


      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(getnewaddress));
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getnewaddress);

      curl_easy_setopt(curl, CURLOPT_USERPWD,
          "a:1234");

      curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);

      curl_easy_perform(curl);
      getWalletaddress(wallet, s.ptr);
      free(s.ptr);
    }
}

void rpc_generatetoaddress(char* address, char* wallet) {
  char first[100] = "{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"generatetoaddress\", ";
  char input[250];
  char second[30] = "\"params\": [1, ";
  char last[50];
  strcpy(input, first);
  strcat(input, second);
  sprintf(last, "\"%s\"", wallet);
  strcat(input, last);
  strcat(input, "]}");


  printf("\n----------------------------------------------------------------------------------------\n");
  printf("{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"generatetoaddress 1 2MudsEizxa251KbyYiyfQEZampEriWRNAUd\", \"params\": [] }");
  printf("\n----------------------------------------------------------------------------------------\n");
  printf("%s \n", input);
  // char *input = "{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"generatetoaddress\", \"params\": [] }";

  CURL *curl = curl_easy_init();
  struct curl_slist *headers = NULL;


  if (curl) {
      
      headers = curl_slist_append(headers, "content-type: text/plain;");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      curl_easy_setopt(curl, CURLOPT_URL, address);


      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(input));
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, input);

      curl_easy_setopt(curl, CURLOPT_USERPWD,
          "a:1234");

      curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);

      curl_easy_perform(curl);
  } 
}


int main(int args, char* argv[])
{
    int node_count = 0;
    int port = 0;
    char node_ip[30];
    char string_port[10];
    char ip_format[30] = "http://127.0.0.1:";
    node_count = atoi(argv[1]);
    char wallet_array[node_count][36]; // Wallet address
    char node_address[node_count][30]; // IP address
    for(int i=0; i < node_count; i++){
        port = 11111 + i;
        strcat(node_address[i],ip_format);
        sprintf(string_port, "%d", port);
        strcat(node_address[i], string_port);
    }

    for(int i=0; i < node_count; i++) {
        rpc_getnewaddress(node_address[i], wallet_array[i]);
        printf("\n\n\n\n\n");
        rpc_generatetoaddress(node_address[i], wallet_array[i]);
    }

    

    return 0;
}