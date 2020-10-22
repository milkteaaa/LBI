#pragma once
extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lobject.h"
#include "Lua/lstate.h"
#include "Lua/lfunc.h"
#include "Lua/lopcodes.h"
#include "Lua/lstring.h"
#include "Lua/ldo.h"
#include "Lua/llex.h"
#include "Lua/lvm.h"
}
#include <Windows.h>
#include "retcheck.h"

lua_State* m_L;
uintptr_t m_rL;
DWORD Add(DWORD func);
typedef int (*rbx_CFunction) (DWORD rL);

#define ASLR(x) (x + 0x400000 + (DWORD)GetModuleHandleA(0))
#define IdentityOffset1 0
#define IdentityOffset2 0
#define StateOffset 0
#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 4
#define RLUA_TNUMBER 3
#define RLUA_TBOOLEAN 1
#define RLUA_TSTRING 5
#define RLUA_TTHREAD 8
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 9
#define RLUA_TPROTO 10
#define RLUA_TUPVALUE 11


void setidentity(uintptr_t rstate, int level)
{
	int v3 = *(uintptr_t*)(rstate + IdentityOffset1);
	*(uintptr_t*)(v3 + IdentityOffset2) = level;
}

int returnidentity(uintptr_t rstate)
{
	uintptr_t a1 = rstate;
	int v3 = *(uintptr_t*)(a1 + IdentityOffset1);
	int level = *(uintptr_t*)(v3 + IdentityOffset2);
	return level;
}

using t_d = int(__cdecl*)(DWORD v1, const char* v2, const char* v3, size_t v4);
t_d r_lua_deserialize = (t_d)unprotect(ASLR(0x11eb210));

using s_t = void(__cdecl*)(DWORD v1);
s_t r_lua_spawn = (s_t)unprotect(ASLR(0x726f30));

using rgetfield = int(__stdcall*)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)(unprotect(ASLR(0x11E0080)));

using sf_t = void(__stdcall*)(DWORD v1, int v2, const char* v3);
sf_t r_lua_setfield = (sf_t)unprotect(ASLR(0x11E1520));

using ps_t = void(__cdecl*)(DWORD v1, const char* v2, int v3);
ps_t r_lua_pushlstring = (ps_t)unprotect(ASLR(0x11E0B30));

using rnewthread = DWORD(__cdecl*)(DWORD);
rnewthread r_lua_newthread = (rnewthread)(unprotect(ASLR(0x11E05B0)));

using pushvalue = void(__cdecl*)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(ASLR(0x11E0B30)));

using rpushcclosure = void(__stdcall*)(DWORD rL, int fn, int non, int a1, int xd);
rpushcclosure rluapushcclosure = (rpushcclosure)(unprotect(ASLR(0x11E0940)));


using rsettop = void(__stdcall*)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(unprotect(ASLR(0x11E1840)));

using rLua_pcall = int(__cdecl*)(DWORD lst, int nargs, int nresults, int errfunc);
rLua_pcall r_lua_pcall = (rLua_pcall)(unprotect(ASLR(0x11e0830)));

using rtolstring = char* (__fastcall*)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(ASLR(0x11E1A60)));

using rtonumber = double(__cdecl*)(DWORD, int, int);
rtonumber r_lua_tonumber = (rtonumber)(ASLR(0x11E1B90));

using rtype = DWORD(__cdecl*)(DWORD, int);
rtype r_lua_type = (rtype)(unprotect(ASLR(0x11e1f50)));

using rpushstring = void(__stdcall*)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(unprotect(ASLR(0x11E0C50)));

using pushnumber = double(__cdecl*)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(ASLR(0x11e0bf0)));

using rgetmetatable = void* (__fastcall*)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(ASLR(0x11E0130)));

using rboolean = int(__cdecl*)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)(unprotect(ASLR(0x11e08f0)));

using rxmove = void(__cdecl*)(DWORD, DWORD, DWORD);
rxmove r_lua_xmove = (rxmove)(unprotect(ASLR(0x11e2000)));

using checktype = void(__cdecl*)(int, int, int);
checktype rluaL_checktype = (checktype)(unprotect(ASLR(0x11e2470)));

using checknumber = lua_Number(__cdecl*)(int, int);
checknumber rluaL_checknumber = (checknumber)(unprotect(ASLR(0x11E2400)));

using checklstring = const char* (__cdecl*)(int, int, unsigned int*);
checklstring rluaL_checklstring = (checklstring)(unprotect(ASLR(0x11E23D0)));

typedef void(__cdecl* r_remove)(DWORD, DWORD);
r_remove r_lua_remove = (r_remove)(unprotect((ASLR(0x11E12B0))));

using rtouserdata = void* (__cdecl*)(DWORD, int);
rtouserdata r_lua_touserdata = (rtouserdata)(unprotect(ASLR(0x11E1E60)));

using rsettable = void* (__cdecl*)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(ASLR(0x11E17C0)));

using rcreatetable = void(__cdecl*)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(ASLR(0x11DFE40)));

using rcfunc = bool(__cdecl*)(DWORD, DWORD);
rcfunc r_lua_iscfunction = (rcfunc)(unprotect(ASLR(0x11E03B0)));

using tptr = std::uintptr_t(__cdecl*)(DWORD, int);
tptr r_lua_topointer = (tptr)(unprotect(ASLR(0x11e1c20)));

using toboolean = bool(__cdecl*)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(unprotect(ASLR(0x11e1980)));

using rcall = void(__cdecl*)(DWORD, DWORD, DWORD);
rcall r_lua_call = (rcall)(unprotect(ASLR(0x11dfc90)));

using rsetmt = void(__cdecl*)(DWORD, int);
rsetmt r_lua_setmetatable = (rsetmt)(unprotect(ASLR(0x11e15d0)));

using rnewuserdata = void* (__cdecl*)(DWORD, size_t, int);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(unprotect(ASLR(0x11E0640)));

using insertt = void(__cdecl*)(DWORD, int);
insertt r_lua_insert = (insertt)(unprotect(ASLR(0x11e0320)));

typedef void(__cdecl* lua_rawsetiok)(DWORD, DWORD, DWORD);
lua_rawsetiok r_lua_rawseti = (lua_rawsetiok)unprotect((ASLR(0x11E11E0)));

typedef TValue* (__cdecl* rrindex2)(int lst, int idx);
rrindex2 r_lua_index2 = (rrindex2)(ASLR(0x11dfb70));

typedef void(__cdecl* lua_pushnilok)(DWORD ok);
lua_pushnilok   r_lua_pushnil = (lua_pushnilok)unprotect((ASLR(0x11E0BA0)));

typedef DWORD(__cdecl* lua_nextok)(DWORD rL, int idx);
lua_nextok   r_lua_next = (lua_nextok)unprotect((ASLR(0x11e06c0)));
/*
typedef void(__cdecl* luaL_register)(int ls, int fn, const char* name);
luaL_register r_luaL_register = (luaL_register)(unprotect(ASLR(0x11DA3A0)));
*/
auto addrError = unprotect(ASLR(0x11e24d0));
#define ccError __cdecl

auto addrNewUserData = unprotect(ASLR(0x11E0640));
#define ccNewUserData __cdecl

auto addrCheckType = unprotect(ASLR(0x11e2470));
#define ccCheckType __cdecl


auto addrToLString = unprotect(ASLR(0x11E1A60));
#define ccToLString __fastcall   

static DWORD FnFire = ASLR(0x00BB8C40);
void r_lua_pushcclosure(int ls, rbx_CFunction fn, int n)
{
	DWORD addr = Add((DWORD)fn);
	rluapushcclosure(ls, (DWORD)(rbx_CFunction)addr, NULL, n, NULL);
}

void r_lua_setreadonly(DWORD State, int Index, bool Boolean) {
	*(BYTE*)(r_lua_topointer(State, Index) + 8) = Boolean;
}


void r_lua_pushcfunction(int ls, rbx_CFunction fn) {
	r_lua_pushcclosure(ls, fn, 0);
}

uintptr_t  RLuaNilObject;

#define luaO_nilobject		(&luaO_nilobject_)
typedef TValue* StkId;

union r_Value
{
	void* gc;
	void* p;
	double n;
	bool b;
};

typedef struct r_TValue
{
	r_Value value;
	byte tt;
	byte shit;
};

const char* GetStr(TString* o) { return (const char*)((uintptr_t)(o)+0x18); };
#define r_setnvalue(obj, x) { TValue *i_o = (obj); i_o->value.n = (x); i_o->tt = RLUA_TNUMBER; }

uintptr_t DXorKey;

double XorDouble(double num)
{
	uint64_t U64_Xor = *reinterpret_cast<uint64_t*>(&num) ^ *reinterpret_cast<uint64_t*>(DXorKey);
	return *reinterpret_cast<double*>(&U64_Xor);
}


DWORD r_topoff = 24;
DWORD r_baseoff = 20;
int r_lua_gettop(int rL)
{
	return *(DWORD*)(rL + r_topoff) - *(DWORD*)(rL + r_baseoff) >> 4;
}
TValue*& r_top(uintptr_t rL)
{
	return *reinterpret_cast<TValue**>(rL + r_topoff);
}
TValue*& r_base(uintptr_t rL)
{
	return *reinterpret_cast<TValue**>(rL + r_baseoff);
}
TValue* r_index2adr(uintptr_t rL, int idx)
{
	if (idx <= 0)
		return r_lua_index2(rL, idx);
	else
		return r_base(rL) + (idx - 1);
}

using getDataModel_t = void(__thiscall*)(DWORD, DWORD);
using getDataModel_tt = DWORD(__cdecl*)();

typedef DWORD(__cdecl* getdatamodel2ok)();
getdatamodel2ok   getdatamodel2 = (getdatamodel2ok)(ASLR(0xE4D090));

typedef DWORD(__thiscall* getdatamodelok)(DWORD, DWORD);
getdatamodelok   r_getdatamodel = (getdatamodelok)(ASLR(0xE4D1E0));

uintptr_t DataModel;

uintptr_t ReturnDataModel()
{
	static DWORD DMPad[16]{};
	r_getdatamodel(getdatamodel2(), (DWORD)DMPad);
	DWORD DM = DMPad[0];
	return DM + 12;
}

uintptr_t ReturnState(uintptr_t v2) {
	return *(DWORD*)(v2 + 0xA4) - (DWORD)(v2 + 0xA4);
}

const char* r_lua_getclass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindChildOfInstance(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(r_lua_getclass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}