/** \file object.hpp
 * Object definition
 *
 * Objects are entities manipulated by the API, they may be of various type but class Object is the parent one. The API can only manipulate classes inheriting from class Object.
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <cstdint>
#include <cstdlib>
#include <vector>

#include "error.hpp"

/** \brief Object types
 *
 * This enumerated type has positive values smaller than 256.
 * Hence, it can be converted to uint8_t type for dumping.
 */
enum type_t{
  /** \brief data element */
  OBJ_DATA,
  /** \brief key element */
  OBJ_KEY
};

/** \brief Class Object: class of all objects in the memory */
class Object
{
public:
  /* \brief Default constructor 
   *
   * By default, an object has a zero length and a #OBJ_DATA type.
   * <HR>
   */
  Object();
  /* \brief Destructor
   *
   * Free the memory corresponding to the object value if not NULL.
   * <HR>
   */
  ~Object();
  /** \brief This function serialize the Object class into an array of bytes.
   *
   * This function is for example used to dump an Object class when saving the API in a file.
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
  myerror_t dump(uint8_t **pointer, size_t *size);
  /** \brief This function parse a buffer to create an Object class.
   *
   * This function is for example used to construct Object classes when loading the API from a file.
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
  static myerror_t parse(uint8_t *buffer, Object **res);
  /** \brief Type of the Object. */
  const type_t type;
  /** \brief Add a boolean attribute to the Object. 
   *
   * <HR>
   * \param value is the value of the new attribute. There is no room for an attribute name: the index in the attribute vector determines to which attribute the value corresponds.
   *
   * <HR>
   **/
  void addAttribute(bool value);
  /** \brief Add an 32-bit unsigned integer attribute to the Object. 
   *
   * <HR>
   * \param value is the value of the new attribute. There is no room for an attribute name: the index in the attribute vector determines to which attribute the value corresponds.
   *
   * <HR>
   */
  void addAttribute(unsigned value);
  /** \brief Value of the Object. */
  uint8_t *value;
  /** \brief Byte length of the Object value */
  uint32_t valueLen;
private:
  /** \brief Object boolean attributes. */
  std::vector<bool> booleanAttributes;
  /** \brief Object unsigned integers attributes (32-bit). */
  std::vector<uint32_t> unsignedAttributes;
};

#endif
