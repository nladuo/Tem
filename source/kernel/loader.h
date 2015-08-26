/**
 *  kernel/loader.h
 *  @author nladuo
 *  @source url:https://github.com/nladuo/TinyExtMvc
 */
#ifndef __LOADER_H
#define __LOADER_H
#include "php.h"

#define MODELS_DIR 	"models"
#define VIEWS_DIR 	"views"
#define CONTROLLERS_DIR	"controllers"

extern zend_class_entry *tem_loader_ce;
int start_up_tem_loader();

#endif //__LOADER_H