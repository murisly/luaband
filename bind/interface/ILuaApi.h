#pragma once

#include <map>
#include <vector>

namespace binding{namespace lua{

class ILuaElementCollection;
class ILuaApi;
class CLuaVariant;

typedef LONG DISP_ID;
typedef std::vector<CLuaVariant> LuaVarArray;
typedef LuaVarArray DISP_PARAMS;

class ILuaElementCollection
{
public:
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual long get_count(size_t& pcnt) = 0;
	virtual long get_key(size_t index, CLuaVariant& rvarKey) = 0;
	virtual long item(const CLuaVariant& varKey, size_t index, CLuaVariant& rvarRes) = 0;
};

class ILuaApi
{
public:
	typedef std::map<DISP_ID,std::string> DispTable;
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual long GetDispTable(DispTable& table) = 0;
	virtual long Invoke(DISP_ID id,const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult) = 0;
};
}}