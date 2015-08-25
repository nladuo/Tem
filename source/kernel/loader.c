#include "kernel/loader.h"
#include "kernel/router.h"

zend_class_entry *tem_loader_ce;

ZEND_METHOD(Loader, __construct)
{

    php_printf("loader init");

    // zval*   instance = NULL;
    // // zend_class_entry *ce;
    // // ce = Z_OBJCE_P(getThis());
    // if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, 
    //         "z", &instance) == FAILURE ){
    //     zend_error(E_ERROR,"argument(s) error!!\n");  
    // }

    // //zend_update_property(ce, getThis(), "", sizeof("app_dir")-1, instance TSRMLS_CC);

}

ZEND_METHOD(Loader, view)
{
    char*   view_path;
    long    view_path_len;
    zval*   datas_arr = NULL;
    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, 
            "s|a", &view_path, &view_path_len, &datas_arr) == FAILURE ){
        zend_error(E_ERROR,"argument(s) error!!\n");  
    }

    zval *app_dir_zval = NULL;
    app_dir_zval = zend_read_static_property(tem_router_ce, "app_dir", sizeof("app_dir")-1, 0 TSRMLS_DC);
    char* app_dir =  Z_STRVAL_PP(&app_dir_zval);

    char view_absolute_path[200] = "";
    strcat(view_absolute_path, app_dir);
    strcat(view_absolute_path, "views/");
    strcat(view_absolute_path, view_path);
    php_printf("%s\n", view_absolute_path);

    php_var_dump(&datas_arr, 0);

    //etract the array()
    if (NULL != datas_arr){
        tem_call_user_function("extract", datas_arr);
    }

    //require the view
    tem_call_require_syntax(view_absolute_path);
}

ZEND_METHOD(Loader, model)
{

    //php_printf("in controller __construct");
}



static zend_function_entry loader_method[]={
    ZEND_ME(Loader, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Loader, view, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
 

int start_up_tem_loader(){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Tem", "Loader", loader_method);
    tem_loader_ce = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}
