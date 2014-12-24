Abyon kernel
======================
**Author: rkx1209**  
**Overview:**  
Abyon is a tiny kernel aimed to linux like system.  
It haven't been stable yet and you should be carefull when running.  
(explain more detail below)

Installation
---------------
``` shell
$ git clone https://github.com/RKX1209/Abyon.git
$ cd Abyon  
$ make install 
```
After building, 'Abyon.img' will be deployed in your current directory.  
It is the body of Abyon kernel itself.   
  
[**NOTICE**]  
**When you run 'make install', makefile create directory /tmp/Abyon and start building project there.**  
**You should be carefull of name collision to your own file under /tmp directory.**   

How to use
---------------
(Example using qemu virtual machine)  
``` shell
$ qemu-system-i386 -m 4000 -fda Abyon.img
```
You can run this kernel by using virtual machine, like QEMU, Bochs...etc  
**Please don't run on real machine because it can make trouble.**  
This kernel only supports i386 system.  


License
----------
Copyright &copy; 2014 rkx1209  
Licensed under the [GNU General Public License Version 2.0][GPL2]  

[GPL2]: http://www.gnu.org/licenses/gp2.html
