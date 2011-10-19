/*
##############################################################################
#
# Copyright (c) 20011-2011 
Paul Ochoa "palichis". (http://solid-ec.org) 
Diego Abad "perilla"
All Rights Reserved.
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
#include "get.h"
#include "js0n.h"
#include "post.h"


#include <iostream>
#include <curl/curl.h>
#include <cstring>

std::string val;
struct json_object *jscore;
struct json_object *jarray;
char const* res;
char* cpy;
int main() {
  set_score("4e8f2bff8fcd52780600003d","pali=","60:eb:69:44:72:95","4e8dfba96d57483cad000020","T@qDfDlUCAVMZHyYEPo3pM@]J","18", "10");
  //std::string = get_scores("LID","SCOPE")
  val = get_scores("4e8f2bff8fcd52780600003d","3");
  //json_object = get_json(std::string)
  jscore = get_json(val);
  printf("\nnew_obj.to_string()=1%s\n", json_object_to_json_string(jscore));
  jscore = json_object_object_get(jscore, "scores");
  //jscore = json_object_array_get_idx(jscore, i);
  //jscore = json_object_object_get(jscore, );
  res = json_object_to_json_string(jscore);
  //json_object_put(jscore);
  for (int i = 0;i<3;i++)
    {
      jarray = json_object_array_get_idx(jscore, i);
      printf("\n\n()=1%s\n", json_object_to_json_string(jarray));
    }
  printf("\nne%s\n", cpy);
  
  
  //jscore = json_object_object_get(jscore, "username");
  
  //printf("new_obj.to_string()=2%s\n", json_object_to_json_string(jscore));
  //json_object_put(jscore);
  //tds::string = set_score("lid","username","userkey","key","secret","points", "data")

  //res = set_score(reinterpret_cast<const unsigned char*>("lid"));


  /*
  const std::string s = "íó" ;

  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
  std::string decoded = base64_decode(encoded);


  std::cout << "decoded: " << decoded << std::endl;

  return 0;*/
}

