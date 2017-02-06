
-- local t = package.loadlib("E:\\LuaTest\\Debug\\DllTestd.dll","luaopen_MyLuaDLL");
-- if t then
	-- print(type(t));
	-- MyLuaDLL.HelloWorld();
-- else
	-- print("error");
-- end

function LoadDll()
	print(0);
	local t = package.loadlib("E:\\LuaTest\\Debug\\DllTestd.dll","luaopen_MyLuaDLL");
	if t then
		print(type(t));
		t()
		MyLuaDLL.HelloWorld();
		local a = MyLuaDLL.GetClientPath();
		print(a,b)
	else
		print(2);
	end
	return "123";
end

function add(x, y)
	return x + y, 3;
end

add(1,2)