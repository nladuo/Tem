//php_tem.h
#ifndef PHP_TEM_H
#define PHP_TEM_H

#include "php.h"

#define PHP_TEM_EXTNAME 	"tem"
#define PHP_TEM_EXTVER 	"0.10"

#define phpext_tem_ptr &tem_module_entry
extern zend_module_entry tem_module_entry;

#endif