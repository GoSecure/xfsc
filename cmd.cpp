#include <map>
#include <vector>

#include <windows.h>
#include <winreg.h>

#include "xfscdm.h"
#include "xfsapi.h"
#include "cmd.h"
#include "cdm.h"

#define HKEY_LOGICAL_SERVICES ".DEFAULT\\XFS\\LOGICAL_SERVICES"

std::map<HSERVICE, Service*> services;


Service* get_service(HSERVICE id)
{
    if (!services.count(id)) return NULL;
    return services[id];
}

void del_service(Service* svc)
{
    if (!svc) return;
    services.erase(svc->handle);
    WFSClose(svc->handle);
    delete svc;
}

HRESULT open_svc(int argc, char** argv)
{
    if (argc != 1) return WFSC_BAD_CMD;
    WFSVERSION svcver, spiver;
    HSERVICE svc;
    char* service = argv[0];
    printf("[+] Opening Service: %s\n", service);

    HRESULT out = WFSOpen(service, WFS_DEFAULT_HAPP, (char*)APPID, /*traceLevel=*/0, WFS_INDEFINITE_WAIT, 3, &svcver, &spiver, &svc);
    check(out);

    if (out == WFS_SUCCESS)
    {
        services[svc] = new Service(std::string(service), svc);
        printf("[*] Service:  %s\n", svcver.szDescription);
        printf("[*] Provider: %s\n", spiver.szDescription);
        printf("[+] %s assigned to handle %d\n", service, svc);
    }
    return out;
}

HRESULT close_svc(int argc, char** argv)
{
    if (argc != 1) return WFSC_BAD_CMD;
    HSERVICE id = atoi(argv[0]);
    Service * svc = get_service(id);
    if (!svc)
    {
        printf("[-] Service %d is not open.\n", id);
        return WFS_SUCCESS;
    }
    del_service(svc);
    return WFS_SUCCESS;
}

HRESULT do_quit(int argc, char** argv)
{
    printf("[+] Exiting...\n");
    // Close all services.
    for (auto& kv : services)
    {
        printf("    Closing %s\n", kv.second->name.c_str());
        WFSClose(kv.second->handle);
        delete kv.second;
    }
    services.clear();

    WFSCleanUp();
    exit(0);
    return WFS_SUCCESS;
}

HRESULT list_svc(int argc, char** argv)
{
    for (auto& kv : services) printf("%5d - %s\n", kv.first, kv.second->name.c_str());
    return WFS_SUCCESS;
}

HRESULT scan_svc(int argc, char** argv)
{
    // All services are configured in the registry at HKU\.DEFAULT\XFS\LOGICAL_SERVICES
    // or at a given user's SID. Currently scanning only supports DEFAULT. It could be
    // enhanced to scan all SIDs or filter on a specific SID.
    HKEY xfs;
    LRESULT res = RegOpenKeyEx(HKEY_USERS, HKEY_LOGICAL_SERVICES, NULL, KEY_READ, &xfs);

    if (res != ERROR_SUCCESS)
    {
        printf("[-] Code %d - No logical services found. Try with a SID?\n", res);
        return WFS_SUCCESS;
    }

    char name[100], clss[5];
    DWORD len = 100, type, class_len;

    while (RegEnumKeyEx(xfs, 0, (char*)& name, &len, NULL, NULL, NULL, NULL) != ERROR_NO_MORE_ITEMS)
    {
        res = RegGetValue(xfs, name, "class", RRF_RT_REG_SZ, &type, &clss, &class_len);
        if (res != ERROR_SUCCESS) continue;
        printf("%s - %s\n", clss, name);
    }

    return WFS_SUCCESS;
}

static cmd_t commands[] = {
    { "open",     "Establish a connection with a service provider.\nUsage: open <logical_name>", open_svc, NULL },
    { "close",    "Close an existing service connection.\nUsage: close <id>", close_svc, NULL },
    { "list",     "List active services.", list_svc, NULL },
    { "scan",     "Scans the computer for XFS services.", scan_svc, NULL},
    { "help",     "Print help about a function.\nUsage: help [function]", NULL, NULL},
    // ----
    { "cdm",      "Control a cash dispenser module.", cdm_dispatch, NULL},
    { "quit",     "Disconnect from the XFS manager.", do_quit, NULL },
    { "exit",     "See quit.", do_quit, NULL },
};


// Generic help printing.
// This is not fully implemented yet.
void help(cmd_t * cmds, int count, char* cmd)
{
    if (cmd == NULL) printf("Supported Commands\n------------------\nTODO\n");
    /*for (int i = 0; i < sizeof(cmds) / sizeof(cmd_t); ++i)
    {
        if (cmd == NULL) printf("%30s%s\n", cmds[i].name, )
    }*/
}

// Command parser and dispatch
char** parse(char* line, int* argc) {
    char* context;
    std::vector<char*> argv;
    char* tok = strtok_s(line, " ", &context);
    while (tok) {
        int len = strlen(tok) + 1;
        char* arg = (char*)malloc(sizeof(char) * len);
        strcpy_s(arg, len, tok);
        argv.push_back(arg);
        tok = strtok_s(NULL, " ", &context);
    }
    *argc = argv.size();
    char** args = (char**)malloc(sizeof(char*) * argv.size());
    memcpy(args, argv.data(), argv.size() * sizeof(char*));
    return args;
}

void free_args(int argc, char** args) {
    for (int i = 0; i < argc; ++i) free(args[i]);
    free(args);
}

// Check for error and print to terminal if necessary
void check(HRESULT res)
{
    if (res == WFS_SUCCESS) return;
    printf("[!] Error %d: ", res);

    switch (res)
    {
    case WFS_ERR_ALREADY_STARTED: /* -1) */
        printf("Already Started");
        break;
    case WFS_ERR_API_VER_TOO_HIGH: /* -2) */
        printf("Version too high");
        break;
    case WFS_ERR_API_VER_TOO_LOW: /* -3) */
        printf("Version too low");
        break;
    case WFS_ERR_CANCELED: /* -4) */
        printf("Canceled");
        break;
    case WFS_ERR_CFG_INVALID_HKEY: /* -5) */ break;
    case WFS_ERR_CFG_INVALID_NAME: /* -6) */ break;
    case WFS_ERR_CFG_INVALID_SUBKEY: /* -7) */ break;
    case WFS_ERR_CFG_INVALID_VALUE: /* -8) */ break;
    case WFS_ERR_CFG_KEY_NOT_EMPTY: /* -9) */ break;
    case WFS_ERR_CFG_NAME_TOO_LONG: /* -10) */ break;
    case WFS_ERR_CFG_NO_MORE_ITEMS: /* -11) */ break;
    case WFS_ERR_CFG_VALUE_TOO_LONG: /* -12) */ break;
    case WFS_ERR_DEV_NOT_READY: /* -13) */
        printf("Device Not Ready");
        break;
    case WFS_ERR_HARDWARE_ERROR: /* -14) */ break;
    case WFS_ERR_INTERNAL_ERROR: /* -15) */
        printf("Internal Error");
        break;
    case WFS_ERR_INVALID_ADDRESS: /* -16) */ break;
    case WFS_ERR_INVALID_APP_HANDLE: /* -17) */ break;
    case WFS_ERR_INVALID_BUFFER: /* -18) */ break;
    case WFS_ERR_INVALID_CATEGORY: /* -19) */ break;
    case WFS_ERR_INVALID_COMMAND: /* -20) */ break;
    case WFS_ERR_INVALID_EVENT_CLASS: /* -21) */ break;
    case WFS_ERR_INVALID_HSERVICE: /* -22) */
        printf("Invalid service handle.");
        break;
    case WFS_ERR_INVALID_HPROVIDER: /* -23) */ break;
    case WFS_ERR_INVALID_HWND: /* -24) */ break;
    case WFS_ERR_INVALID_HWNDREG: /* -25) */ break;
    case WFS_ERR_INVALID_POINTER: /* -26) */ break;
    case WFS_ERR_INVALID_REQ_ID: /* -27) */ break;
    case WFS_ERR_INVALID_RESULT: /* -28) */ break;
    case WFS_ERR_INVALID_SERVPROV: /* -29) */ break;
    case WFS_ERR_INVALID_TIMER: /* -30) */ break;
    case WFS_ERR_INVALID_TRACELEVEL: /* -31) */ break;
    case WFS_ERR_LOCKED: /* -32) */ break;
    case WFS_ERR_NO_BLOCKING_CALL: /* -33) */ break;
    case WFS_ERR_NO_SERVPROV: /* -34) */ break;
    case WFS_ERR_NO_SUCH_THREAD: /* -35) */ break;
    case WFS_ERR_NO_TIMER: /* -36) */ break;
    case WFS_ERR_NOT_LOCKED: /* -37) */ break;
    case WFS_ERR_NOT_OK_TO_UNLOAD: /* -38) */ break;
    case WFS_ERR_NOT_STARTED: /* -39) */ break;
    case WFS_ERR_NOT_REGISTERED: /* -40) */ break;
    case WFS_ERR_OP_IN_PROGRESS: /* -41) */ break;
    case WFS_ERR_OUT_OF_MEMORY: /* -42) */ break;
    case WFS_ERR_SERVICE_NOT_FOUND: /* -43) */ break;
    case WFS_ERR_SPI_VER_TOO_HIGH: /* -44) */ break;
    case WFS_ERR_SPI_VER_TOO_LOW: /* -45) */ break;
    case WFS_ERR_SRVC_VER_TOO_HIGH: /* -46) */ break;
    case WFS_ERR_SRVC_VER_TOO_LOW: /* -47) */ break;
    case WFS_ERR_TIMEOUT: /* -48) */ break;
    case WFS_ERR_UNSUPP_CATEGORY: /* -49) */ break;
    case WFS_ERR_UNSUPP_COMMAND: /* -50) */ break;
    case WFS_ERR_VERSION_ERROR_IN_SRVC: /* -51) */ break;
    case WFS_ERR_INVALID_DATA: /* -52) */ break;
    case WFS_ERR_SOFTWARE_ERROR: /* -53) */ break;
    case WFS_ERR_CONNECTION_LOST: /* -54) */ break;
    case WFS_ERR_USER_ERROR: /* -55) */ break;
    case WFS_ERR_UNSUPP_DATA: /* -56) */ break;
    case WFS_ERR_FRAUD_ATTEMPT: /* -57) */ break;
    case WFS_ERR_SEQUENCE_ERROR: /* -58) */ break;
    case WFS_ERR_CDM_INVALIDCURRENCY:
        printf("Invalid Currency");
        break;
    case WFS_ERR_CDM_INVALIDTELLERID:
        printf("Invalid Teller ID");
        break;
    case WFS_ERR_CDM_CASHUNITERROR:
        printf("Cash Unit Error");
        break;
    case WFS_EXEE_CDM_CASHUNITERROR:
    case WFS_ERR_CDM_INVALIDDENOMINATION:
        printf("Invalid Denomination");
        break;
    case WFS_ERR_CDM_INVALIDMIXNUMBER:
        printf("Invalid Mix Number");
        break;
    case WFS_ERR_CDM_NOCURRENCYMIX:
        printf("No Currency Mix");
        break;
    case WFS_ERR_CDM_NOTDISPENSABLE:
        printf("Not Dispensable");
        break;
    case WFS_ERR_CDM_TOOMANYITEMS:
        printf("Too Many Items to Dispense");
        break;
    default:
        printf("Unspecified Error");
    }

    printf(".\n");
}

// Top-level user input handler.
void handle(char* cmd) {
    int argc;
    char** args = parse(cmd, &argc);
    if (!argc) return;

    dispatch(commands, CMD_COUNT(commands), argc, args);
    free_args(argc, args);
}

// Dispatch command to sub-handler
HRESULT dispatch(cmd_t * fdt, int count, int argc, char** argv) {
    HRESULT res = WFSC_BAD_CMD;
    if (!argc) return res;
    // TODO: Help handler.

    for (; count > 0; --count) {
        cmd_t* c = fdt++;
        if (strcmp(c->name, argv[0]) == 0) {
            if (!c->func) {
                printf("[!] Unimplemented!\n");
                break;
            }
            res = c->func(argc - 1, argv + 1);
            if (res == WFSC_BAD_CMD) printf("[!] %s\n", c->help);
            break;
        }
    }
    return res;
}

// Lock WFS manager mutex for the given service
void lock(Service * svc) {
    LPWFSRESULT res;
    HRESULT out = WFSLock(svc->handle, WFS_INDEFINITE_WAIT, &res);
    check(out);

    // TODO: Check the result and handle when service is already locked.
    if (res) WFSFreeResult(res);
    res = NULL;
}

// Unlock WFS manager mutex for the given service
void unlock(Service * svc)
{
    HRESULT out = WFSUnlock(svc->handle);
    check(out);
}
