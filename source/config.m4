PHP_ARG_ENABLE(tem,
    [Whether to enable the "tem" extension],
    [  enable-tem        Enable "tem" extension support])

if test $PHP_TEM != "no"; then
    PHP_SUBST(WALU_SHARED_LIBADD)
    PHP_NEW_EXTENSION(tem, 
    	tem.c 		\
    	kernel/controller.c \
    	kernel/loader.c 	\
    	kernel/model.c	\
    	kernel/router.c	\
    	utils/tool.c,
    	$ext_shared)
fi