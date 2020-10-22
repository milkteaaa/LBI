#include <Windows.h>
#include "internet.h"
#include "rstuff.h"
#include "console.h"

const auto LBI = Internet::DownloadURL("https://github.com/yuukixdev/LBIRoblox/blob/master/lbi.luau?raw=true");

void ExecuteScript(const char* script, lua_State* L, uintptr_t RL)
{
    std::string Script = script;
    if (Script.find("https://") != std::string::npos || Script.find("http://") != std::string::npos)
        Script = Internet::DownloadURL(Script);
    luaL_loadbuffer(L, Script.c_str(), Script.length(), "LBI");
    lua_setglobal(L, "__C__");
    luaL_dostring(L, R"(
        local bytecode = string.dump(__C__);
        __SIZE__ = #bytecode
        __BYTECODE__ = bytecode;
        )");
    lua_getglobal(L, "__BYTECODE__");
    const auto lua_bytecode = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "__SIZE__");
    const auto lua_bytecode_size = lua_tonumber(L, -1);
    lua_pop(L, 1);

    r_lua_pushlstring(RL, lua_bytecode, lua_bytecode_size);
    r_lua_setfield(RL, LUA_GLOBALSINDEX, "SCRIPT_VALUE");

    const auto newthread = r_lua_newthread(RL);
    r_lua_deserialize(newthread, "LBI", LBI.c_str(), LBI.length());
    r_lua_spawn(newthread);
}

DWORD WINAPI luapipe(PVOID lvpParameter)
{
	std::string Script = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\LBI"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						Script = Script + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			ExecuteScript(Script.c_str(), m_L, m_rL);
			Script = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

int main()
{
	const char* console_name = "Lua Bytecode Interpreter | Build date: %s / %s\n" __DATE__ __TIME__;
	ConsoleBypass(console_name);
	std::cout << "retrieving datamodel..\n";
	DataModel = ReturnDataModel();
	DWORD ScriptContext = FindChildOfInstance(DataModel, "ScriptContext");
	std::cout << "initializing lua state..\n";
	m_L = luaL_newstate();
	m_rL = *(DWORD*)(ScriptContext + StateOffset) - (DWORD)(ScriptContext + StateOffset);
	std::cout << "setting identity..\n";
	setidentity(m_rL, 7);
	std::cout << "loading libraries..\n";
	luaL_openlibs(m_L);
	std::cout << "creating thread..\n";
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)luapipe, NULL, NULL, NULL);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

