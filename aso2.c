#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "advapi32.lib")

void afiseazaServicii() {
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCM) {
        printf("Eroare. Cod: %lu\n", GetLastError());
        return;
    }

    DWORD bytesNeeded = 0;
    DWORD servicesReturned = 0;
    DWORD resumeHandle = 0;

    EnumServicesStatusEx(
        hSCM,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        NULL,
        0,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL);

    if (GetLastError() != ERROR_MORE_DATA) {
        printf("Eroare. Cod: %lu\n", GetLastError());
        CloseServiceHandle(hSCM);
        return;
    }

    LPBYTE buffer = (LPBYTE)malloc(bytesNeeded);
    if (buffer == NULL) {
        printf("Eroare.\n");
        CloseServiceHandle(hSCM);
        return;
    }

    LPENUM_SERVICE_STATUS_PROCESS pServices = (LPENUM_SERVICE_STATUS_PROCESS)buffer;

    if (!EnumServicesStatusEx(
        hSCM,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        buffer,
        bytesNeeded,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL)) {
        printf("Eroare la preluarea serviciilor. Cod: %lu\n", GetLastError());
        free(buffer);
        CloseServiceHandle(hSCM);
        return;
    }

    printf("Serviciile care ruleaza pe masina:\n");

    for (DWORD i = 0; i < servicesReturned; ++i) {
        if (pServices[i].ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING) {
            printf("%ls : %ls\n", pServices[i].lpServiceName, pServices[i].lpDisplayName);
        }
    }

    free(buffer);
    CloseServiceHandle(hSCM);
}

int main() {
    AfiseazaServicii();
    return 0;
}