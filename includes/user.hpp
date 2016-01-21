/** \file user.hpp
 * User definition
 *
 * User class definition.
 */

#ifndef USER_HPP_
#define USER_HPP_

#include <cstdint>
#include <cstdlib>
#include "error.hpp"

/** \brief Maximum length of a user login. */
constexpr unsigned MAX_LOGIN_LEN = 30;
constexpr unsigned MAX_MDP_LEN = 30;

/** \brief User roles
 *
 * This enumerated type has positive values smaller than 256.
 * Hence, it can be converted to uint8_t type for dumping.
 */
enum role_t{
  /** \brief simple user */
  USER_BASIC = 0,
  /** \brief admin */
  USER_ADMIN = 1
};

/** \brief User class */
class User
{
public:
  /** \brief This constructor create a user from a login, id and role 
   *
   * <HR>
   */
  User(const char login[MAX_LOGIN_LEN], uint32_t id, role_t role);
  /** \brief User destructor 
   *
   * <HR>
   */
  ~User();
  /** \brief This function serialize the User class into an array of bytes.
   *
   * This function is for example used to dump a User class when saving the API in a file.
   * <HR>
   * \param pointer is a pointer to the buffer that will contain the result. *pointer should be initialised to NULL since the memory allocation will be done by the function.
   * \param size is a pointer to an integer that which value will be set to the actual size of the allocated buffer.
   * \return 
   * <ul>
   * <li> #NO_ERROR if no error occured 
   * <li> #ERROR_PARAM if pointer is NULL or *pointer is not NULL
   * </ul>
   *
   * <HR>
   */
  myerror_t dump(uint8_t **pointer, size_t *size) const;
  /** \brief This function parse a buffer to create a User class.
   *
   * This function is for example used to construct User classes when loading the API from a file.
   * <HR>
   * \param buffer is the byte array from which data will be read.
   * \param res is a pointer to the User pointer that will contain the result. *pointer should be initialised to NULL since the memory allocation will be done by the function.
   * \return 
   * <ul>
   * <li> #NO_ERROR if no error occured 
   * <li> #ERROR_PARAM if res is NULL or *res is not NULL
   * </ul>
   *
   * <HR>
   */
  static myerror_t parse(uint8_t *buffer, User **res);
  /** \brief Function returning the login of the user 
   *
   * <HR>
   */
  const char* getLogin() const;
  /** \brief Id of the user, must be unique. */
  const uint32_t id;
  /** \brief Role of the user. */
  const role_t role;
  /** \brief Size of the class once dumped into a byte array. */
  static constexpr size_t dumpSize = MAX_LOGIN_LEN + 5;
private:
  /** \brief Login of the user. 
   *
   * The size of a login is at most #MAX_LOGIN_LEN.
   */
  char login[MAX_LOGIN_LEN+1];
};

#endif
