#include "object.hpp"

#include <cstdio>
#include <cstring>

Object::Object() : type(OBJ_DATA) , valueLen(0) , value(NULL)
{
}

Object::~Object()
{
  if (value != NULL)
    free(value);
}
myerror_t Object::dump(uint8_t **pointer, size_t *size)
{
  unsigned i;
  size_t length = 0;
  size_t offset = 0;
  if ( (pointer == NULL) || (*pointer != NULL) )
    return ERROR_PARAM;

  // First compute final length
  // value field
  length += 4 + valueLen;
  // booleans attributes
  length += 4 + booleanAttributes.size();
  // unsigned attributes
  length += 4 + 4*unsignedAttributes.size();
  // allocate 4 bytes more to put the global size
  (*pointer) = (uint8_t*) malloc(length+4);
  (*size) = length + 4;
  
  // now fill the buffer
  offset = 0;
  ((uint32_t *)((*pointer)+offset))[0] = (uint32_t) length;
  offset += 4;
  memcpy((*pointer)+offset,(char*)&valueLen,4);
  offset += 4;
  memcpy((*pointer)+offset,(char*)value,valueLen);
  offset += valueLen;
  ((uint32_t *)((*pointer)+offset))[0] = (uint32_t) booleanAttributes.size();
  offset += 4;
  for ( i = 0 ; i < booleanAttributes.size() ; i++ )
    {
      if ( booleanAttributes[i] )
	(*pointer)[offset] = 0xFF;
      else
	(*pointer)[offset] = 0x00;
      offset ++;
    }
  ((uint32_t *)((*pointer)+offset))[0] = (uint32_t) unsignedAttributes.size();
  offset += 4;
  for ( i = 0 ; i < unsignedAttributes.size() ; i++ )
    {
      ((uint32_t *)((*pointer)+offset))[0] = (uint32_t) unsignedAttributes[i];
      offset += 4;
    }
  
  return NO_ERROR;
}

myerror_t Object::parse(uint8_t *buffer, Object **res)
{
  myerror_t ret = NO_ERROR;
  Object *temp;
  uint32_t count = 0, offset = 0;
  unsigned i;
  if ( (res == NULL) || ((*res) != NULL) )
    {
      ret = ERROR_PARAM;
    }
  else
    {
      temp = new Object();
      if ( temp == NULL )
	{
	  ret = ERROR_MEM;
	}
    }
  if ( ret == NO_ERROR )
    {
      count = ((uint32_t*)(&buffer[offset]))[0];
      offset += 4;
      temp->valueLen = count;
      temp->value = (uint8_t*) malloc(count);
      if ( temp->value == NULL )
	{
	  ret = ERROR_MEM;
	}
    }
  if ( ret == NO_ERROR )
    {
      memcpy(temp->value,buffer+offset,count);
      offset += count;
      count = ((uint32_t*)(&buffer[offset]))[0];
      offset += 4;
      for ( i = 0 ; i < count ; i ++ )
	{
	  temp->addAttribute(buffer[offset]?true:false);
	  offset ++;
	}
      count = ((uint32_t*)(&buffer[offset]))[0];
      offset += 4;
      for ( i = 0 ; i < count ; i ++ )
	{
	  temp->addAttribute(((uint32_t*)(&buffer[offset]))[0]);
	  offset += 4;
	}
      (*res) = temp;
    }
  return ret;
}

void Object::addAttribute(bool value)
{
  booleanAttributes.push_back(value);
}

void Object::addAttribute(unsigned value)
{
  unsignedAttributes.push_back(value);
}


