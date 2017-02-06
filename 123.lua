#123
local t = {}
t[1] = 10;
t[2] = 20;
t[3] = 30;
t[5] = 50;
t[7] = 70;
t["test"] = "test";
local it;

function add(a, b)
	
	for i,v in pairs(t) do
		it = i;
	end
	return 2;
end

add(2, 3)
