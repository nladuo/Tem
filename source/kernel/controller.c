#include "kernel/controller.h"
#include "kernel/loader.h"

zend_class_entry *tem_controller_ce;


ZEND_METHOD(Controller, __construct)
{
    //php_printf("in controller __construct");
    zval *load_obj;
    MAKE_STD_ZVAL(load_obj);
    object_init_ex(load_obj, tem_loader_ce);
    //walu_call_user_method(NULL, load_obj, "__construct", "z", getThis());
    walu_call_user_method(NULL, load_obj, "__construct", "");

    add_property_zval_ex(getThis(), "load", strlen("load") + 1, load_obj);

    //zend_declare_property(tem_controller_ce, "load", strlen("load"), load_obj, ZEND_ACC_PUBLIC TSRMLS_DC);
    
    //zend_update_property(tem_loader_ce, load_obj, "load", sizeof("load")-1, load_obj TSRMLS_CC);
    zval_ptr_dtor(&load_obj);
}



static zend_function_entry controller_method[]={
    ZEND_ME(Controller, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    {NULL, NULL, NULL}
};
 

int start_up_tem_controller(){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Tem", "Controller", controller_method);
    tem_controller_ce = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}
