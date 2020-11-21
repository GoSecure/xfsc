#include "xfsapi.h"
#include "xfscdm.h"
#include "cmd.h"

#include <stdio.h>

// GLOBALS
WFSVERSION apiver, svcver, spiver;
HSERVICE svc;
HRESULT out;
LPSTR LOGICAL_NAME;

// Process a series of commands delimited by ;
void do_script(char* script) {
    char* context;
    char* tok = strtok_s(script, ";", &context);
    while (tok)
    {
        handle(tok);
        tok = strtok_s(NULL, ";", &context);
    }
}

int main(int argc, char** argv)
{

    printf(">>> XFS CLI - Copyright (c) 2019 GoSecure Inc. <<< \n");

    printf("--\n");
    printf("[+] Loading msxfs.dll... ");
    init();
    printf("OK\n");

    printf("[+] Establishing connection with XFSManager\n");
    out = WFSStartUp(3, &apiver);
    check(out);
    if (out != WFS_SUCCESS) exit(1);

    // Very rudimentary scripting support for now.
    if (argc == 3 && strcmp(argv[1], "-c")==0) do_script(argv[2]);

    printf("[*] API: %s\n", apiver.szDescription);

    char cmd[1024];
    do {
        printf("xfs> ");
        auto len = gets_s(cmd, 1024);
        handle(cmd);
    } while (1);

    return 0;
}