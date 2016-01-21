/** \file api.hpp
 * API definition
 *
 * API definition 
 */

#ifndef API_HPP_
#define API_HPP_

#include <vector>
#include <string>

#include "error.hpp"
#include "object.hpp"
#include "user.hpp"

/** \brief API class */
class API
{
public:
  /** \brief The constructor initialises API from the file given in parameter. 
   *
   * <HR>
   * \param filename string containing the relative path and filename of the file containing the protected memory content to load.
   *
   * <HR>
   */
  API(const char *filename);

/** \brief The API destructor
   *
   * Automatically dump current API memory to the file used for constructing the object.
   * <HR>
   */
  ~API();
  /** \brief Resets the API
   *
   * Resets the API by deleting users and memory.
   * <HR>
   */
  void reset();

  myerror_t init();

  myerror_t login(const char login[MAX_LOGIN_LEN]);
  myerror_t newKey(uint8_t myKey[16]);
  myerror_t encryptData(const uint8_t plain[], uint8_t cipher[], const uint8_t key[16]);

private:
  /** \brief Boolean variable set to true only once memory have been initialised by loading a file. 
   *
   */
  bool isInit;
  /** \brief Variable containing the relative path and filename of the file from which the memory got initialised and where it should be saved on exit. 
   *
   */
  char filename[100];
  /** \brief List of registered users */
  std::vector<const User*> users;
  /** \brief Memory containing all objects that may be manipulated by the API */
  std::vector<Object*> memory; 
  const User* logged; 

  /** \brief Add an object to the memory
   *
   * <HR>
   * \param obj is a pointer to the Object class that should be added to the memory.
   * \return #NO_ERROR
   *
   * <HR>
   */
myerror_t addObject(Object *obj);
  /** \brief Register a new user
   *
   * Add the user to the list of API users.
   * <HR>
   * \param user is a pointer to the User class that should be registered.
   * \return 
   * <ul>
   * <li> #NO_ERROR if no error occured 
   * <li> #ERROR_PARAM if user is already in the base
   * </ul>
   *
   * <HR>
   */
  myerror_t addUser(const User *user);
  /** \brief This function stores the data into the file API#filename.
   *
   * <HR>
   * \return 
   * <ul>
   * <li> #NO_ERROR if no error occured 
   * <li> #ERROR_IO if an error occured when writing in the file
   * </ul>
   *
   * <HR>
   */
  myerror_t saveAPI();
  /** \brief This function loads the protected data contained in the file API#filename. 
   *
   * <HR>
   * \return 
   * <ul>
   * <li> #NO_ERROR if no error occured 
   * <li> #ERROR_IO if an error occured when reading the file
   * <li> #ERROR_FORMAT if an error occured when parsing the data
   * </ul>
   *
   * <HR>
   */
  myerror_t loadAPI();

  uint32_t getFreeId();

  myerror_t createAdmin(const char login[MAX_LOGIN_LEN]);

  myerror_t addUser(const char login[MAX_LOGIN_LEN]);

  bool isLogged(uint32_t id);

  const User* findUserByLogin(const char login[MAX_LOGIN_LEN]);

  bool verifMDP(User user, std::string mdp);
};

#endif
