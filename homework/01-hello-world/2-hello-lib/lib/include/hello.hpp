#ifndef HELLO_HPP
#define HELLO_HPP

#ifdef WIN32
#define LIB_API __declspec(dllexport)
#else
#define LIB_API
#endif
namespace hw /* hello world */
{

/**
 * @brief print greeting
 *
 * @return int
 */
LIB_API int
hello();

} // namespace hw

#endif // HELLO_HPP