#include "cmd.h"
#include "cdm.h"

const char* cdmtype2str(WORD type)
{
    switch (type)
    {
    case WFS_CDM_TELLERBILL:      return "Bills (Teller)";
    case WFS_CDM_SELFSERVICEBILL: return "Bills (Self-Serve)";
    case WFS_CDM_TELLERCOIN:      return "Coins (Teller)";
    case WFS_CDM_SELFSERVICECOIN: return "Coins (Self-Serve)";
    }
    return "Unknown";
}

const char* pos2str(DWORD pos) {
    switch (pos) {
    case WFS_CDM_POSLEFT: return "LEFT";
    case WFS_CDM_POSRIGHT: return "RIGHT";
    case WFS_CDM_POSFRONT: return "FRONT";
    case WFS_CDM_POSREAR: return "REAR";
    case WFS_CDM_POSTOP: return "TOP";
    case WFS_CDM_POSBOTTOM: return "BOTTOM";
    case WFS_CDM_POSCENTER: return "CENTER";
    default: return "UNKNOWN";
    }
}

// CONTROL CASH DISPENSER (WFS_CDM_DISPENSE)
// ------------------------------------------------------
// The input parameters are amount, currency and mix number. In this case the amount is
// denominatedand, if this succeeds, the items are dispensed.
// Denominations are significantly more complex to use because they require to know
// the bill numbers from each cash.
HRESULT cdm_dispense(int argc, char** argv)
{
    if (argc < 2 || argc > 4) return WFSC_BAD_CMD;
    Service * svc = get_service(atoi(argv[0]));
    if (!svc) return WFSC_BAD_CMD;
    LPWFSRESULT res; HRESULT out;

    DWORD  amt = atoi(argv[1]);
    USHORT mix = (argc == 4) ? atoi(argv[3]) : 1;
    char* cur = (argc >= 3) ? argv[2] : (char*)"USD";
    if (strlen(cur) < 3) return WFSC_BAD_CMD;

    lock(svc);
    WFSCDMDENOMINATION denom;
    denom.cCurrencyID[0] = cur[0];
    denom.cCurrencyID[1] = cur[1];
    denom.cCurrencyID[2] = cur[2];

    denom.lpulValues = NULL; // Let the ATM figure out the actual denomination.
    denom.usCount = 0;

    denom.ulAmount = amt; // Amount to dispense.
    denom.ulCashBox = 0;  // Unused.

    WFSCDMDISPENSE cmdDispense;
    cmdDispense.bPresent = TRUE;              // Present money to customer.
    cmdDispense.fwPosition = WFS_CDM_POSNULL; // Default output position.
    cmdDispense.usMixNumber = mix;
    cmdDispense.lpDenomination = &denom;
    cmdDispense.usTellerID = 0; // UNUSED
    out = WFSExecute(svc->handle, WFS_CMD_CDM_DISPENSE, &cmdDispense, WFS_INDEFINITE_WAIT, &res);
    printf("[+] Dispensing %d %s from %s...\n", amt, cur, svc->name.c_str());
    check(out);
    unlock(svc);
    return WFS_SUCCESS;
}

// Dump cash dispenser capabilities
HRESULT cdm_caps(int argc, char** argv)
{
    if (argc != 1) return WFSC_BAD_CMD;
    Service * svc = get_service(atoi(argv[0]));
    if (!svc) return WFSC_BAD_CMD;
    LPWFSRESULT res; HRESULT out;

    out = WFSGetInfo(svc->handle, WFS_INF_CDM_CAPABILITIES, /*IN*/ NULL, WFS_INDEFINITE_WAIT, /*OUT*/ &res);
    check(out);
    printf("[+] Capabilities for %s\n", svc->name.c_str());
    if (out == WFS_SUCCESS)
    {
        LPWFSCDMCAPS caps = (LPWFSCDMCAPS)res->lpBuffer;
        printf("    bCashBox: %d\n", caps->bCashBox);
        printf("    bCompound: %d\n", caps->bCompound);
        printf("    bIntermediateStacker: %d\n", caps->bIntermediateStacker);
        printf("    bItemsTakenSensor: %d\n", caps->bItemsTakenSensor);
        printf("    bPowerSaveControl: %d\n", caps->bPowerSaveControl);
        printf("    bPrepareDispense: %d\n", caps->bPrepareDispense);
        printf("    bSafedoor: %d\n", caps->bSafeDoor);
        printf("    bShutter: %d\n", caps->bShutter);
        printf("    bShutterControl: %d\n", caps->bShutterControl);
        printf("    fwExchangeType: %d\n", caps->fwExchangeType);
        printf("    fwType: %s\n", cdmtype2str(caps->fwType));
        printf("    Max Dispense: %d\n", caps->wMaxDispenseItems);
        printf("    Extra: %s\n", caps->lpszExtra);
        WORD pos = caps->fwPositions; // Specifies the CDM output positions.
        printf("    Positions:\n");
        printf("    ----------\n");
        if (pos & WFS_CDM_POSLEFT)   printf("      - LEFT\n");
        if (pos & WFS_CDM_POSRIGHT)  printf("      - RIGHT\n");
        if (pos & WFS_CDM_POSTOP)    printf("      - TOP\n");
        if (pos & WFS_CDM_POSBOTTOM) printf("      - BOTTOM\n");
        if (pos & WFS_CDM_POSFRONT)  printf("      - FRONT\n");
        if (pos & WFS_CDM_POSREAR)   printf("      - REAR\n");
        if (pos & WFS_CDM_POSCENTER) printf("      - CENTER\n");
    }

    if (res) WFSFreeResult(res);
    return WFS_SUCCESS;
}

// Dump cash dispenser mix algorithms
HRESULT cdm_mixes(int argc, char** argv)
{
    if (argc != 1) return WFSC_BAD_CMD;
    Service * svc = get_service(atoi(argv[0]));
    if (!svc) return WFSC_BAD_CMD;
    LPWFSRESULT res; HRESULT out;

    out = WFSGetInfo(svc->handle, WFS_INF_CDM_MIX_TYPES, NULL, WFS_INDEFINITE_WAIT, &res);

    printf("[+] Mixing Algorithms for %s\n", svc->name.c_str());
    check(out);
    if (res) {
        LPWFSCDMMIXTYPE* mixes = (LPWFSCDMMIXTYPE*)res->lpBuffer;
        while (*mixes) {
            LPWFSCDMMIXTYPE mix = *mixes;
            printf("    %02d - %02d - %s\n", mix->usMixNumber, mix->usMixType, mix->lpszName);
            mixes++;
        }
        WFSFreeResult(res);
    }
    return WFS_SUCCESS;
}

// Dump cash dispenser info
HRESULT cdm_info(int argc, char** argv)
{
    if (argc != 1) return WFSC_BAD_CMD;
    Service * svc = get_service(atoi(argv[0]));
    if (!svc) return WFSC_BAD_CMD;
    LPWFSRESULT res; HRESULT out;

    out = WFSGetInfo(svc->handle, WFS_INF_CDM_STATUS, NULL, WFS_INDEFINITE_WAIT, &res);

    printf("[+] Dispenser Satus for %s:\n", svc->name.c_str());
    check(out);
    if (res) {
        LPWFSCDMSTATUS s = (LPWFSCDMSTATUS)res->lpBuffer;
        printf("    Device:    %d\n", s->fwDevice);
        printf("    Dispenser: %d\n", s->fwDispenser);
        printf("    Stacker:   %d\n", s->fwIntermediateStacker);
        printf("    Safedoor:  %d\n", s->fwSafeDoor);
        printf("    Position:  %d\n", s->wDevicePosition);
        printf("    Output Positions:\n");
        printf("    -----------------\n");
        LPWFSCDMOUTPOS* pos = s->lppPositions;
        auto i = 0;
        while (*pos) {
            LPWFSCDMOUTPOS p = *pos;
            printf("    %02d - %s (status=0x%x) Shutter=0x%x  Transport=0x%x (status=%d) \n", i, pos2str(p->fwPosition), p->fwPositionStatus, p->fwShutter, p->fwTransport, p->fwTransportStatus);
            pos++; ++i;
        }
    }

    out = WFSGetInfo(svc->handle, WFS_INF_CDM_CASH_UNIT_INFO, NULL, WFS_INDEFINITE_WAIT, &res);
    printf("[+] Cash Units for %s:\n", svc->name.c_str());
    check(out);
    if (res) {
        LPWFSCDMCUINFO info = (LPWFSCDMCUINFO)res->lpBuffer;
        LPWFSCDMCASHUNIT* units = (LPWFSCDMCASHUNIT*)info->lppList;
        printf("[*] Found %02d cash units (may not reflect physical units)\n", info->usCount);
        for (WORD i = 0; i < info->usCount; ++i) {
            LPWFSCDMCASHUNIT unit = units[i];
            printf("    %02d: %5s - (%c%c%c) - %s\n", i, (char*)unit->cUnitID, unit->cCurrencyID[0], unit->cCurrencyID[1], unit->cCurrencyID[2], unit->lpszCashUnitName);
            printf("    Type=%d Value=%d\n", unit->usType, unit->ulValues);
            printf("    Min=%08d Max=%08d Cur=%08d\n", unit->ulMinimum, unit->ulMaximum, unit->ulCount);
            printf("    %s\n", unit->bAppLock ? "LOCKED" : "USABLE");
        }

        WFSFreeResult(res);
    }
    return out;
}

// Register subcommands for parser
static cmd_t commands[] = {
    { "info",     "Queries information about the cash dispenser.\nUsage: cdm info <id>", cdm_info, NULL },
    { "caps",     "Queries capabilities of the cash dispenser.\nUsage: cdm caps <id>", cdm_caps, NULL },
    { "mix",      "Displays supported mixing algorithms\nUsage: cdm mix <id>", cdm_mixes, NULL },
    { "dispense", "Dispense cash\nUsage: cdm dispense <id> <amount> [currency=USD] [mix=1]", cdm_dispense, NULL },
};

// cdm subcommand implementation
HRESULT cdm_dispatch(int argc, char** argv) {
    if (!argc)
    {
        printf("Cash Dispenser Module Commands: info | caps | dispense\n");
        return WFS_SUCCESS;
    }
    dispatch(commands, CMD_COUNT(commands), argc, argv);

    char* cmd = argv[0];

    return WFS_SUCCESS;
}


