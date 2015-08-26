/**
 *  kernel/router.c
 *  @author nladuo
 *  @source url:https://github.com/nladuo/TinyExtMvc
 */

#include "php_tem.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "kernel/router.h"
#include "kernel/loader.h"
#include "utils/tool.h"

zend_class_entry *tem_router_ce;

ZEND_METHOD(Router, __construct)
{
    zval *app_dir;
    zend_class_entry *ce;
    ce = Z_OBJCE_P(getThis());

    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &app_dir) == FAILURE )
    {
        php_printf("argument(s) error\n");
        RETURN_NULL();
    }
    zend_update_static_property(ce, "app_dir", sizeof("app_dir")-1, app_dir TSRMLS_CC);
}
 
ZEND_METHOD(Router, run)
{
    char* controller_name = DEF_CONTROLLER;
    char* method_name = DEF_METHOD;

    zval * get_arr;
    zval ** get_temp;  

    get_arr = PG(http_globals)[TRACK_VARS_GET];  
    if (zend_hash_find(HASH_OF(get_arr), "c", strlen("c") + 1, (void **)&get_temp) != FAILURE){
        controller_name = Z_STRVAL_PP(get_temp);
    }

    if (zend_hash_find(HASH_OF(get_arr), "m", strlen("m") + 1, (void **)&get_temp) != FAILURE){
        method_name = Z_STRVAL_PP(get_temp);
    }

    // php_var_dump(&get_arr, 0); 
    // php_printf("%s\n", controller_name);
    // php_printf("%s\n", method_name);


    zval *app_dir_zval = NULL;
    app_dir_zval = zend_read_static_property(Z_OBJCE_P(getThis()), "app_dir", sizeof("app_dir")-1, 0 TSRMLS_DC);

    char* app_dir =  Z_STRVAL_PP(&app_dir_zval);

    //php_printf("%s\n", controller_name);
    
    //assign the controller path
    char controller_path[200] = ""; 
    strcat(controller_path, app_dir);
    strcat(controller_path, "controllers/");
    strcat(controller_path, controller_name);
    strcat(controller_path, ".php");

    //php_printf("%s\n", controller_path);
    
    //require the controller
    tem_call_require_syntax(controller_path);
    
    //call 
    int controller_name_len   = strlen(controller_name);
    zend_class_entry **ce   = NULL;
    zval * ins;
  
    //find the required class from zend class table
    if (zend_lookup_class(controller_name, controller_name_len, &ce TSRMLS_CC) == FAILURE
        || (*ce)->ce_flags & ZEND_ACC_INTERFACE
        || (*ce)->ce_flags & ZEND_ACC_ABSTRACT){
        zend_error(E_ERROR,"cannot find class\n");  
    }

    MAKE_STD_ZVAL(ins);
    if(object_init_ex(ins, *ce) == FAILURE) {
        zval_ptr_dtor(&ins);
        zend_error(E_ERROR,"cannot instantiate class\n");  
    }
    //call the constructor
    walu_call_user_method(NULL, ins, "__construct", "");
    walu_call_user_method(NULL, ins, method_name, "");

    zval_ptr_dtor(&ins);
}
    

static zend_function_entry router_method[]={
    ZEND_ME(Router, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Router, run, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
 

int start_up_tem_router(){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Tem", "Router", router_method);
    tem_router_ce = zend_register_internal_class(&ce TSRMLS_CC);

    zend_declare_property_string(tem_router_ce, "app_dir", strlen("app_dir"), ""
        , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_DC);
    return SUCCESS;
}
