#include <windows.h>
#include <iostream>
#include <sstream>
#include <functional>
#include "winutility.h"

QString getDeviceHWID() {
    std::stringstream ss;
    
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\BIOS",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        char buffer[256];
        DWORD size = sizeof(buffer);
        DWORD type = REG_SZ;
        
        if (RegQueryValueExA(hKey, "BaseBoardSerialNumber", NULL,
            &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) {
            ss << buffer << "|";
        }
        
        size = sizeof(buffer);
        if (RegQueryValueExA(hKey, "SystemProductName", NULL,
            &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) {
            ss << buffer << "|";
        }
        
        RegCloseKey(hKey);
    }
    
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        char buffer[256];
        DWORD size = sizeof(buffer);
        DWORD type = REG_SZ;
        
        if (RegQueryValueExA(hKey, "SystemBiosVersion", NULL,
            &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) {
            ss << buffer;
        }
        
        RegCloseKey(hKey);
    }
    
    std::string combined = ss.str();
    std::hash<std::string> hasher;
    std::stringstream result;
    result << std::hex << std::uppercase << hasher(combined);
    
    return QString::fromStdString(result.str());
}
