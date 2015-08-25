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

    zval *app_dir_zval = NULL;
    app_dir_zval = zend_read_static_property(Z_OBJCE_P(getThis()), "app_dir", sizeof("app_dir")-1, 0 TSRMLS_DC);

    char* app_dir =  Z_STRVAL_PP(&app_dir_zval);

    php_printf("%s\n", controller_name);
    
    //obtain the controller path
    char controller_path[200] = ""; 
    strcat(controller_path, app_dir);
    strcat(controller_path, "controllers/");
    strcat(controller_path, controller_name);
    strcat(controller_path, ".php");

    php_printf("%s\n", controller_path);
    
    //require the controller
    tem_call_require_syntax(controller_path);
    
    // init the controller instance
    zval *controller_obj;
    MAKE_STD_ZVAL(controller_obj);
    zend_class_entry *controller_ce = tem_load_user_class(controller_name);
    if (controller_ce != NULL)
    {
        object_init_ex(controller_obj, controller_ce);
        walu_call_user_method(NULL, controller_obj, "__construct", "");
        walu_call_user_method(NULL, controller_obj, method_name, "");
        zval_ptr_dtor(&controller_obj);
    }
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
