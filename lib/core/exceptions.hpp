/**
 * @file exceptions.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Some exceptions for making debugging easier.
 * @version 1.0
 * @date 2022-01-12
 */

/**
 * @brief Try catch handler
 * 
 * @code{cpp}
 * try {
 *      Debug_Printf(1,20,true,0,"1");
 *      throw();
 *      Debug_Printf(1,21,true,0,"2");
 * } catch {
 *      Debug_Printf(1,22,true,0,"ERROR");
 * }
 * @endcode
 */
#define try bool __HadError=false;
#define catch __ExitJmp:if(__HadError)
#define throw(x) __HadError=true;goto __ExitJmp;
