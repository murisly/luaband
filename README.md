# Luaband
整体结构![image](https://github.com/murisly/luaband/blob/master/img/struct.jpg)

本项目用visual studio 2012 编译，需要用到部分windows头文件。

1.lua源码封装在LuaEngine中，用于与lua_State进行交互，并添加第三方库。

2.LuaManage，wrapper，impl用于预处理脚本(支持自定义加密算法，来源)，预处理挂载点（hook函数），自定义C函数。

3.CLuaEngineImpl中用于自定义C函数，实现不用基本功能。

4.TVariant实现lua中变量转换到C中变量。

# example
CLuaEngineImpl中的print为自定义打印函数