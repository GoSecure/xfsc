#pragma once

#include "xfsapi.h"
#include <string>


#define WFSC_BAD_CMD 0x42
#define APPID "xfsc"
#define CMD_COUNT(x) (sizeof(x)/sizeof(cmd_t))

typedef HRESULT(*handler_t)(int, char**);
typedef void(*help_fn)();

typedef struct cmd_ {
    const char* name;
    const char* help;
    handler_t   func;
    help_fn     more;
    
} cmd_t;


class Service {
public:
    std::string name;
    // TODO: Other metadata...
    HSERVICE    handle;

    Service(std::string name, HSERVICE h) : name(name), handle(h) {}
    ~Service() {
        if (handle) WFSClose(handle);
    }
};

void handle(char* args);
HRESULT dispatch(cmd_t* fdt, int count, int argc, char** argv);
void check(HRESULT res);

Service* get_service(HSERVICE id);
void     del_service(Service* svc);

void lock(Service* svc);
void unlock(Service* svc);