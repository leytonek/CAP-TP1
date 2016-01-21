#include "user.hpp"

#include <cstring>

User::User(const char login[MAX_LOGIN_LEN], uint32_t id, role_t role) : id(id), role(role)
{
  memset(this->login,0,MAX_LOGIN_LEN+1);
  strncpy(this->login,login,MAX_LOGIN_LEN);
}

User::~User()
{
}

myerror_t User::dump(uint8_t **pointer, size_t *size) const
{
  size_t offset = 0;
  if ( (pointer == NULL) || (*pointer != NULL) )
    return ERROR_PARAM;
  (*pointer) = (uint8_t*) malloc(dumpSize);
  
  // filling the buffer
  memcpy(*pointer + offset,&id,4);
  offset += 4;
  memcpy(*pointer + offset,&role,1);
  offset += 1;
  memcpy(*pointer + offset,login,MAX_LOGIN_LEN);
  offset += MAX_LOGIN_LEN;

  (*size) = offset;
  return NO_ERROR;
}

myerror_t User::parse(uint8_t *buffer, User **res)
{
  myerror_t ret = NO_ERROR;
  char *login;
  uint32_t id;
  role_t role;
  if ( (res == NULL) ||  ((*res) != NULL) )
    {
      ret = ERROR_PARAM;
    }
  else
    {
      id = ((uint32_t*) buffer)[0];
      role = (role_t) buffer[4];
      login = (char*) buffer + 5;
      (*res) = new User(login,id,role);
       if ( (*res) == NULL )
	{
	  ret = ERROR_MEM;
	}
    }
  return ret;
}

const char* User::getLogin() const
{
  return login;
}
