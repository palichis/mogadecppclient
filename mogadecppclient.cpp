#include "post.h"
#include "get.h"
#include "js0n.h"
#include "encode/base64.h"

#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "json.h"
#include <gcrypt.h>



char *writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
  int result = 0;
  if(buffer != NULL) 
    {
      buffer -> append(data, size * nmemb);
      result = size * nmemb;
    }
  return 0;
} 

std::string get_scores(const std::string lid,const std::string scope)
{
  struct json_object *new_obj;
  CURLcode res;
  CURL* curl_handle = curl_easy_init();
  int i;
  const char *curl_get;
  if (curl_handle)
    {  
      std::string buffer;
      std::string url;
      char *buffer1;
      url = "http://api2.mogade.com/api/gamma/scores?lid=" + lid +"&scope=" + scope;
      curl_get = strdup(url.c_str());
      curl_easy_setopt(curl_handle, CURLOPT_URL, curl_get);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writer);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &buffer );
      res = curl_easy_perform(curl_handle);
      curl_easy_cleanup(curl_handle);
      return buffer;
   } 
}

struct json_object *get_json(std::string curl_score)
{
  struct json_object *json_obj;
  MC_SET_DEBUG(1);
  json_obj = json_tokener_parse(curl_score.c_str());
  //json_object_put(new_obj);
  return json_obj;
}

char *convert(std::string text){
  char* encrypt;
  encrypt = new char[text.length() + 1];
  strcpy(encrypt, text.c_str());

  int msg_len = strlen(encrypt);

  int hash_len = gcry_md_get_algo_dlen( GCRY_MD_SHA1 );

  unsigned char hash[ hash_len ];

  char *out = (char *) malloc( sizeof(char) * ((hash_len*2)+1) );
  char *p = out;

  gcry_md_hash_buffer( GCRY_MD_SHA1, hash, encrypt, msg_len );

  int i;
  for ( i = 0; i < hash_len; i++, p += 2 ) {
    snprintf ( p, 3, "%02x", hash[i] );
  }
  
  return out;

}

/*
  lid = id of leaderboards
  username = nick of gamer
  userkey = player identification device
  points = points of gamer
  key = id of game
  secret = secret of game on mogade
  data = data of user, define a level, dificulty etc....
*/

void set_score(std::string lid, std::string username, std::string userkey, std::string key, std::string secret, std::string points, std::string data ) 
{
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if(curl)
    {
      std::string str_post;
      std::string str_convert;
      char *str_sig;
      char* url_post;
      str_post = "lid="+lid+"&username="+username+"&userkey="+userkey+"&points="+points+"&data="+data+"&key="+key+"&sig=";
      url_post = new char[str_post.length() + 1];
      strcpy(url_post, str_post.c_str());
      str_convert = "data|"+data+"|key|"+key+"|lid|"+lid+"|points|"+points+"|userkey|"+userkey+"|username|"+username+"|"+secret;
      str_sig = convert(str_convert);
      char* url_sig = (char*)malloc(strlen(str_sig) + strlen(url_post));
      sprintf(url_sig, "%s%s", url_post, str_sig);
      curl_easy_setopt(curl, CURLOPT_URL, "http://api2.mogade.com/api/gamma/scores");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS,url_sig);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
    }
}
