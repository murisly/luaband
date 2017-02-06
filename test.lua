
function Main(argc, argv)
	if argc ~= 0 and #argv ~= 0 then
		if argv[1] == "Scan" then
			oaApi.Print("test");
		end
	end
end