/*
##############################################################################
#
# Copyright (c) 20011-2011 
#Paul Ochoa "palichis". (http://solid-ec.org) 
#Diego Abad "perilla"
#All Rights Reserved.
#                    General contacts <palichis@solid-ec.org>
#
# WARNING: This program as such is intended to be used by professional
# programmers who take the whole responsability of assessing all potential
# consequences resulting from its eventual inadequacies and bugs
# End users who are looking for a ready-to-use solution with commercial
# garantees and support are strongly adviced to contract a Free Software
# Service Company
#
# This program is Free Software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
##############################################################################
*/
#include "post.h"
#include "get.h"
#include "js0n.h"
#include "encode/urlencode.h"

#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "json/json.h"
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
      if(buffer.length() != 0)
	return buffer;
      else
	return "1";
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

std::string set_score(std::string lid, std::string username, std::string userkey, std::string key, std::string secret, std::string points, std::string data ) 
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
      char* enusername;
      char* enuserkey;
      char* endata;
      std::string buffer;
      endata = new char[data.length() + 1];
      strcpy(endata, data.c_str());
      endata = url_encode(endata);


      enuserkey = new char[userkey.length() + 1];
      strcpy(enuserkey, userkey.c_str());
      enuserkey = url_encode(enuserkey);

      enusername = new char[username.length() + 1];
      strcpy(enusername, username.c_str());
      enusername = url_encode(enusername);

      printf("\n\n\n%s\n\n\n%s\n\n\n",enusername,endata);
      str_post = "lid="+lid+"&username="+enusername+"&userkey="+enuserkey+"&points="+points+"&data="+endata+"&key="+key+"&sig=";
      url_post = new char[str_post.length() + 1];
      strcpy(url_post, str_post.c_str());
      str_convert = "data|"+data+"|key|"+key+"|lid|"+lid+"|points|"+points+"|userkey|"+userkey+"|username|"+username+"|"+secret;
      str_sig = convert(str_convert);
      char* url_sig = (char*)malloc(strlen(str_sig) + strlen(url_post));
      sprintf(url_sig, "%s%s", url_post, str_sig);
      curl_easy_setopt(curl, CURLOPT_URL, "http://api2.mogade.com/api/gamma/scores");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS,url_sig);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer );
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      if(buffer.length() != 0)
	return buffer;
      else
	return "1";
      return buffer;
    }
}
