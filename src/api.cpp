#include "api.hpp"
#include "object.hpp"

#include <cstdio>
#include <cstring>

API::API(const char* filename)
{
  FILE *in;
  myerror_t ret = NO_ERROR;
  /** <ol> */
  /** <li> Check that the file exists */
  in = fopen(filename,"rb");
  if ( in == NULL )
    {
      fprintf(stderr,"[error] File %s cannot be opened !\n",filename);
    }
  else
    {

      /** <li> If yes copy filename to API#filename and close the file */
      fclose(in);
      strncpy(this->filename,filename,100);
      /** <li> Call API#loadAPI() to load data from the file */
      ret = loadAPI();
      /** <li> Test return code and set API::isInit in case of success */
      if ( ret == NO_ERROR )
	{
	  this->isInit = true;
	}
    }
  /** </ol> */
}

API::~API()
{
  /** <ol> */
  /** <li> Dump API to the file API#filename */
  saveAPI();
  /** <li> Free all classes using reset() function. */
  reset();
  /** </ol> */
}

void API::reset()
{
  /* Objects are deleted by the call to clear() */
  users.clear();
  memory.clear();
}

myerror_t API::saveAPI()
{
  /** <ol> */
  myerror_t ret = NO_ERROR;
  uint8_t *buffer = NULL;
  size_t buffer_size;
  uint32_t count;
  FILE *out;
  /** <li> Opening the file */
  out = fopen(filename,"wb");
  if ( out == NULL )
    ret = ERROR_IO;
  /** <li> Dumping the header */
  if ( ret == NO_ERROR )
    {
      count = users.size();
      fwrite(&count,sizeof(uint32_t),1,out);
      count = memory.size();
      fwrite(&count,sizeof(uint32_t),1,out);
    }
  /** <li> Dumping users */
  for ( auto it = users.begin() ; (it != users.end()) && (ret == NO_ERROR) ; it ++ )
    {
      buffer = NULL;
      ret = (*it)->dump(&buffer,&buffer_size);
      // dump it to the file
      if ( ret == NO_ERROR )
	{
	  if ( fwrite(buffer,buffer_size,1,out) != 1 )
	    ret = ERROR_IO;
 	}
      if ( buffer != NULL )
	free(buffer);
    }
  /** <li> Dumping memory */
  for ( auto it = memory.begin() ; (it != memory.end()) && (ret == NO_ERROR) ; it ++ )
    {
      buffer = NULL;
      ret = (*it)->dump(&buffer,&buffer_size);      
      // dump it to the file
      if ( ret == NO_ERROR )
	{
	  if ( fwrite(buffer,buffer_size,1,out) != 1 )
	    ret = ERROR_IO;
	}
      if ( buffer != NULL )
	free(buffer);
    }
  /** <li> Close the file */
  if ( out != NULL )
    fclose(out);
  /** </ol> */
  return NO_ERROR;
}

myerror_t API::loadAPI()
{
  /** <ol> */
  myerror_t ret = NO_ERROR;
  uint8_t *buffer = NULL;
  unsigned i;
  uint32_t user_count = 0, obj_count = 0, obj_size = 0;
  FILE *in;
  User *newUser;
  Object *newObject;
  /** <li> Opening the file */
  in = fopen(filename,"rb");
  if ( in == NULL )
    ret = ERROR_IO;
  /** <li> Reading header */
  if ( ret == NO_ERROR )
    {

      fread(&user_count,sizeof(uint32_t),1,in);
      fread(&obj_count,sizeof(uint32_t),1,in);
    }
  /** <li> Reading users */
  users.clear();
  for ( unsigned i = 0 ;( i < user_count) && (ret == NO_ERROR) ; i++ )
    {
      buffer = (uint8_t*) malloc(User::dumpSize);
      if ( buffer == NULL )
	{
	  ret = ERROR_MEM;
	}
      else
	{
	  if ( fread(buffer,User::dumpSize,1,in) != 1 )
	    ret = ERROR_IO;
	  if ( ret == NO_ERROR )
	    {
	      newUser = NULL;
	      ret = User::parse(buffer,&newUser);
	      if ( ret == NO_ERROR )
		{
		  users.push_back(newUser);
		}
	    }
	  free(buffer);
	}
    }
  /** <li> Reading objects */
  memory.clear();
  for ( unsigned i = 0 ;( i < obj_count) && (ret == NO_ERROR) ; i++ )
    {
      fread(&obj_size,4,1,in);
      buffer = (uint8_t*) malloc(obj_size);
      if ( buffer == NULL )
	{
	  ret = ERROR_MEM;
	}
      else
	{
	  if ( fread(buffer,obj_size,1,in) != 1 )
	    ret = ERROR_IO;
	  if ( ret == NO_ERROR )
	    {
	      newObject = NULL;
	      ret = Object::parse(buffer,&newObject);
	      if ( ret == NO_ERROR )
		{
		  memory.push_back(newObject);
		}
	    }
	  free(buffer);
	}
    }
  /** <li> Closing the file */
  if ( in != NULL )
    fclose(in);
  /** </ol> */
  return NO_ERROR;
}

myerror_t API::addObject(Object *obj)
{
  memory.push_back(obj);
  return NO_ERROR;
}

myerror_t API::addUser(const User *user)
{
  myerror_t ret = NO_ERROR;
  unsigned i;
  for ( i = 0 ; (i < users.size()) && (ret == NO_ERROR) ; i++ )
    {
      if ( strncmp(users[i]->getLogin(),user->getLogin(),MAX_LOGIN_LEN) == 0 )
	{
	  ret = ERROR_PARAM;
	}
    }
  if ( ret == NO_ERROR )
    {
      users.push_back(user);
    }
  return ret;
}

uint32_t API::getFreeId()
{
  bool found = false;
  uint32_t id=0;
  while(!found){
    found=true;
    for (auto it = users.begin(); it != users.end(); it++)
    {
      if (id == (*it)->id)
      {
        found = false;
      }
    }
    id++;
  }
  return id;
}

myerror_t API::createAdmin(const char login[MAX_LOGIN_LEN])
{
  myerror_t ret = NO_ERROR;
  User* admin = new User(login, getFreeId(), USER_ADMIN);
  users.push_back(admin);
  return ret;
}


myerror_t API::init()
{
  myerror_t ret = NO_ERROR;
  printf("---- Initialisation ----\n");
  printf("---- createAdmin(\"admin\") ----\n");
  createAdmin("admin");
  return ret;
}

bool API::isLogged(uint32_t id)
{
  return (id == logged->id);
}

myerror_t API::login(const char login[MAX_LOGIN_LEN])
{
  myerror_t ret = NO_ERROR;

  if(logged != NULL){ret=ERROR_SEC; return ret;}

  const User* user = findUserByLogin(login);
  if(user == NULL){ret=ERROR_PARAM; return ret;}

  char mdp[MAX_MDP_LEN];
  printf("Veuillez entrer le mot de passe :\n");
  scanf("%29s",mdp);

  if(!verifMDP(*user,mdp)){ret=ERROR_SEC; return ret;}
  logged = user;
  printf("%s a bien été connecté !\n",logged->getLogin());

  return ret;
}

const User* API::findUserByLogin(const char login[MAX_LOGIN_LEN])
{
  for (auto it = users.begin(); it != users.end(); it++)
  {
    if (strncmp(login,(*it)->getLogin(),MAX_LOGIN_LEN) == 0)
    {
      return (*it);
    }
  }
  return NULL;
}

bool API::verifMDP(User user, char mdp[MAX_MDP_LEN]){
  return true;
}