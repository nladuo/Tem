/**
 *  kernel/controller.c
 *  @author nladuo
 *  @source url:https://github.com/nladuo/TinyExtMvc
 */

#include "kernel/controller.h"
#include "kernel/loader.h"

zend_class_entry *tem_controller_ce;


ZEND_METHOD(Controller, __construct)
{
    zval *load_obj;
    MAKE_STD_ZVAL(load_obj);
    object_init_ex(load_obj, tem_loader_ce);
    walu_call_user_method(NULL, load_obj, "__construct", "z", getThis());
    add_property_zval_ex(getThis(), "load", strlen("load") + 1, load_obj);

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
