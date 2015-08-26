/**
 *  tem.c
 *  @author nladuo
 *  @source url:https://github.com/nladuo/TinyExtMvc
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_tem.h"
#include "kernel/router.h"
#include "kernel/controller.h"
#include "kernel/model.h"
#include "kernel/loader.h"

static zend_function_entry tem_functions[]={
    {NULL, NULL, NULL}
};
 
PHP_MINIT_FUNCTION(tem)
{

    //Load Router Class
    if(start_up_tem_router() == FAILURE){
        return FAILURE;
    }

    //Load Controller Class
    if(start_up_tem_controller() == FAILURE){
        return FAILURE;
    }

    //Load Loader Class
    if(start_up_tem_loader() == FAILURE){
        return FAILURE;
    }

    //Load Model Class
    if(start_up_tem_model() == FAILURE){
        return FAILURE;
    }
    
    return SUCCESS;
}
 
PHP_MSHUTDOWN_FUNCTION(tem) {
    return SUCCESS;
}
 
PHP_RINIT_FUNCTION(tem) {
    return SUCCESS;
}
PHP_RSHUTDOWN_FUNCTION(tem) {
    return SUCCESS;
}
 
PHP_MINFO_FUNCTION(tem) {
    php_info_print_table_start();
    php_info_print_table_row(2, "Tem Module", "enabled");
    php_info_print_table_row(2, "version", PHP_TEM_EXTVER);
    php_info_print_table_end();
}

//module entry
zend_module_entry tem_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
     STANDARD_MODULE_HEADER,
#endif
    PHP_TEM_EXTNAME,
    tem_functions, /* Functions */
    PHP_MINIT(tem), /* MINIT */
    PHP_MSHUTDOWN(tem),
    PHP_RINIT(tem),
    PHP_RSHUTDOWN(tem),
    PHP_MINFO(tem),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_TEM_EXTVER, 
#endif
    STANDARD_MODULE_PROPERTIES
};

 
#ifdef COMPILE_DL_TEM
ZEND_GET_MODULE(tem)
#endif
