/**
 *  kernel/loader.c
 *  @author nladuo
 *  @source url:https://github.com/nladuo/TinyExtMvc
 */
#include "kernel/loader.h"
#include "kernel/router.h"
#include "kernel/model.h"

zend_class_entry *tem_loader_ce;

ZEND_METHOD(Loader, __construct)
{
    zval*   instance = NULL;
    // zend_class_entry *ce;
    // ce = Z_OBJCE_P(getThis());
    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, 
            "o", &instance) == FAILURE ){
        zend_error(E_ERROR,"argument(s) error!!\n");  
    }

    add_property_zval_ex(getThis(), "instance", strlen("instance") + 1, instance TSRMLS_DC);
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
    //php_printf("%s\n", view_absolute_path);

    //php_var_dump(&datas_arr, 0);

    //etract the array()
    if (NULL != datas_arr){
        tem_call_user_function("extract", datas_arr);
    }

    //require the view
    tem_call_require_syntax(view_absolute_path);
}

ZEND_METHOD(Loader, model)
{
    char*   model_name;
    long    model_name_len;
    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, 
            "s", &model_name, &model_name_len) == FAILURE ){
        zend_error(E_ERROR,"argument(s) error!!\n");  
    }

    zval *app_dir_zval = NULL;
    app_dir_zval = zend_read_static_property(tem_router_ce, "app_dir", sizeof("app_dir")-1, 0 TSRMLS_DC);
    char* app_dir =  Z_STRVAL_PP(&app_dir_zval);

    char model_absolute_path[200] = "";
    strcat(model_absolute_path, app_dir);
    strcat(model_absolute_path, "models/");
    strcat(model_absolute_path, model_name);
    strcat(model_absolute_path, ".php");
    //php_printf("%s\n", model_absolute_path);

    //require the model.php
    tem_call_require_syntax(model_absolute_path);

    zend_class_entry **ce   = NULL;
    zval * ins;
  
    //find the required class from zend class table
    if (zend_lookup_class(model_name, model_name_len, &ce TSRMLS_CC) == FAILURE
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

    //add model to the instance(controller) property
    zval *instance = NULL;
    instance = zend_read_property(tem_loader_ce ,getThis(), "instance", sizeof("instance")-1, 0 TSRMLS_DC);
    //php_var_dump(&instance, 0);
    add_property_zval_ex(instance, model_name, strlen(model_name) + 1, ins TSRMLS_DC);

    zval_ptr_dtor(&ins);
}



static zend_function_entry loader_method[]={
    ZEND_ME(Loader, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Loader, view, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(Loader, model, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
 

int start_up_tem_loader(){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Tem", "Loader", loader_method);
    tem_loader_ce = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}
