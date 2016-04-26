# TEM
a tiny php MVC framework implemented by c extension.
## Installation(Linux)
###Note:
The testing enviornment is Ubuntu 14.04, and the PHP Version is 5.4.6. <br>
###Process:
download the php-src(version >= 5.3)<br>
rename the TEM's source folder to tem, and put it under the ./php-src/ext/<br>
use the following commands to generate the dynamic library:tem.so(./php-src/ext/tem/modules/tem.so)
```
$phpize & ./configure & make
```
finally, add the extension in php.ini<br>
for example:
```
extension=/usr/local/lib/tem.so
```

##Usage
The default controller is:Home, and the default method is:index.<br>
The complete url formation is , for example, http://127.0.0.1/Tem/public/index.php?c=Home&&m=index

##License
MIT
