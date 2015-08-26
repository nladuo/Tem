/**
 *  utils/tool.c
 *  @author nladuo
 *  @source url:https://github.com/nladuo/TinyExtMvc
 */
#include "utils/tool.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "Zend/zend_list.h"
#include "Zend/zend_interfaces.h" 

void tem_call_user_function(const char * _function_name, zval* args){
    zval* function_name;
    zval* retval;  
    MAKE_STD_ZVAL(function_name);
    MAKE_STD_ZVAL(retval);
    zval**params=(zval**)malloc(sizeof(zval));  
    ZVAL_STRING(function_name, _function_name, 0);//turn char* to zval*
    params[0]=args;  

    if(call_user_function(CG(function_table), NULL, function_name, 
            retval, 1, params TSRMLS_DC)==FAILURE){  
        zend_error(E_ERROR,"call function failed");  
    }

    zval_ptr_dtor(&retval);  
}


void tem_call_require_syntax(char *filepath){
    zval* retval;
    zend_file_handle zfd;
    zfd.type = ZEND_HANDLE_FILENAME;
    zfd.filename = filepath;
    zfd.free_filename = 0;
    zfd.opened_path = NULL;
    zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC,&retval,1,&zfd); 
    php_write(&retval, 0 TSRMLS_CC);
}

int walu_call_user_method(zval** retval, zval* obj, char* function_name, char* paras, ...){ 
      short int paras_count=0;
      zval*** parameters = NULL;
      long long_tmp; 
      char *string_tmp; 
      zval *zval_tmp; 
      double dou_tmp; 
      int i;         //仅与调用有关的变量
      int fun_re, retval_is_null=0; 
      HashTable *function_table;         //接收参数 
      paras_count = strlen(paras); 
      if(paras_count > 0) {
                parameters = (zval***)emalloc(sizeof(zval**) * paras_count); 
                va_list ap;
                va_start(ap,paras); 
                for(i=0; i<paras_count; i++){
                        parameters[i] = (zval**)emalloc(sizeof(zval*));
                         switch(paras[i]) {
                                case 's': 
                                       MAKE_STD_ZVAL(*parameters[i]); 
                                       string_tmp = va_arg(ap, char*); 
                                       long_tmp = va_arg(ap, long);  
                                      ZVAL_STRINGL(*parameters[i], string_tmp, long_tmp, 1);
                                        break; 
                                case 'l':  
                                       MAKE_STD_ZVAL(*parameters[i]);
                                        long_tmp = va_arg(ap, long); 
                                        ZVAL_LONG(*parameters[i], long_tmp);  
                                       break;                                 case 'd':  
                                       MAKE_STD_ZVAL(*parameters[i]);
                                        dou_tmp = va_arg(ap, double);
                                        ZVAL_DOUBLE(*parameters[i], dou_tmp);
                                        break; 
                                case 'n': 
                                       MAKE_STD_ZVAL(*parameters[i]); 
                                       ZVAL_NULL(*parameters[i]); 
                                       break;
                                 case 'z': 
                                       zval_tmp = va_arg(ap, zval*); 
                                       *parameters[i] = zval_tmp;
                                        break;
                                 case 'b':  
                                      MAKE_STD_ZVAL(*parameters[i]);  
                                      ZVAL_BOOL(*parameters[i], (int)va_arg(ap, int));
                                        break; 
                               default:
                                        zend_error(E_ERROR, "Unsupported type:%c in walu_call_user_function", paras[i]);                                        return 0; 
                        } 
               }
                 va_end(ap);
        }         //构造参数执行call_user_function_ex  
      zval *_function_name;
        MAKE_STD_ZVAL(_function_name);
        ZVAL_STRINGL(_function_name, function_name, strlen(function_name), 1);
          if(retval == NULL){
                retval_is_null = 1; 
               retval = (zval**)emalloc(sizeof(zval*));
        }         //开始函数调用 
       if(obj) { 
               function_table = &Z_OBJCE_P(obj)->function_table;
        }else{
                function_table = (CG(function_table));
        }
        zend_fcall_info fci;
        fci.size = sizeof(fci);
        fci.function_table = function_table;
        fci.object_ptr = obj ? obj : NULL;
        fci.function_name = _function_name; 
        fci.retval_ptr_ptr = retval; 
        fci.param_count = paras_count;
        fci.params = parameters;
        fci.no_separation = 1;
        fci.symbol_table = NULL; 
        fun_re = zend_call_function(&fci, NULL TSRMLS_CC);         //函数调用结束。
        if(retval_is_null == 1){ 
               zval_ptr_dtor(retval);
                efree(retval);
        }
        zval_ptr_dtor(&_function_name);         //free掉parameter及其里面的每个元素zval**，及每个元素zval**对应的zval*        //对于传进来的zval，不进行free，由参数调用者自行free 
        if(paras_count > 0){ 
               for(i=0; i<paras_count; i++){
                        if(paras[i] != 'z') {
                                zval_ptr_dtor(parameters[i]);
                        }
                        efree(parameters[i]);
                }
                efree(parameters);
        } 
       return fun_re;
}