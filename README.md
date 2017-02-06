# Luaband
整体结构

![image](https://github.com/murisly/luaband/blob/master/img/struct.jpg)

本项目用visual studio 2012 编译，需要用到部分windows头文件。

1.lua解析器为lua源代码，目前是用的是5.2.3，可以更新。lua debug 为解析出错时，捕捉解析器抛出的错误信息

2.lua Engine 对 [lua C API](http://www.lua.org/manual/5.3/)的封装，负责与与lua_State进行交互。

3.Storage对脚本文件抽象，可以来自文件，网络或者内存。可自定义脚本加密解密，以及预编译。

4.Host对脚本执行点的挂载，hook脚本进入，执行，完结各个时间点。

5.Variant实现lua中变量转换到C中变量，lua调用c函数参数传递的原型。

6.wrapper负责组织storage和host。

7.impl为用户自定义C函数部分，其中实现一个Print函数。可添加自定义C函数。

8.Engine Manage 负责管理wrapper、impl，为最终暴露给用户的接口。

# example
CLuaEngineImpl中的print为自定义打印函数