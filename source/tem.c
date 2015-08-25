//tem.c
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
    //REGISTER_INI_ENTRIES();

    //Load Router Class
    if(start_up_tem_router() == FAILURE){
        return FAILURE;
    }

    //Loader Controller Class
    if(start_up_tem_controller() == FAILURE){
        return FAILURE;
    }

    if(start_up_tem_loader() == FAILURE){
        return FAILURE;
    }


    
    return SUCCESS;
}

// PHP_MSHUTDOWN_FUNCTION(tem)
// {
//     //UNREGISTER_INI_ENTRIES();
//     return SUCCESS;
// }


//module entry
zend_module_entry tem_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
     STANDARD_MODULE_HEADER,
#endif
    PHP_TEM_EXTNAME,
    tem_functions, /* Functions */
    PHP_MINIT(tem), /* MINIT */
    NULL, /* MSHUTDOWN */
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_TEM_EXTVER, 
#endif
    STANDARD_MODULE_PROPERTIES
};

 
#ifdef COMPILE_DL_TEM
ZEND_GET_MODULE(tem)
#endif
