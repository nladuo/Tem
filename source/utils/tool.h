/**
 *  utils/tool.h
 *  @author nladuo
 *  @source url:https://github.com/nladuo/TinyExtMvc
 */
#ifndef __HELPER_H
#define __HELPER_H
#include "php.h"

extern void tem_call_user_function(const char * _function_name, zval* args);
extern void tem_call_require_syntax(char *filepath);
extern int walu_call_user_method(zval** retval, zval* obj, char* function_name, char* paras, ...);

#endif