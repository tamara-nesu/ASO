#include <windows.h>
#include <stdio.h>

void showSubkeys(HKEY hRoot, const char* keyPath) {

    HKEY hKey;
    LONG result;

    result = RegOpenKeyExA(hRoot, keyPath, 0, KEY_READ, &hKey);

    if (result != ERROR_SUCCESS) {
        fprintf(stderr, "Error: Could not open the key '%s'.\n", keyPath);
        return;
    }

    char subKeyName[256];
    DWORD index = 0;

    printf("Subkeys from %s:\n", keyPath);

    while (1) {
        result = RegEnumKeyA(hKey, index, subKeyName, sizeof(subKeyName));

        if (result == ERROR_NO_MORE_ITEMS) {
            break;
        }

        if (result == ERROR_SUCCESS) {
            printf("Subkey %lu: %s\n", index, subKeyName);
        }
        else {
            fprintf(stderr, "Error at reading the subkey with index: %lu.\n", index);
            break;
        }

        index++;
    }

    RegCloseKey(hKey);
}

int main() {
    showSubkeys(HKEY_CURRENT_USER, "Software");

    return 0;
}