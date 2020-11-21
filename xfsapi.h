/************************************************************************
*                                                                       *
* xfsapi.h      XFS - API functions, types, and definitions             *
*                                                                       *
*               Version 3.10  --  29/11/2007                            *
*                                                                       *
************************************************************************/

#ifndef __inc_xfsapi__h
#define __inc_xfsapi__h
#pragma pack(push,1)

void init(); // Load msxfs.dll and resolve symbols.
/****** Common *********************************************************/
#include <windows.h>

typedef unsigned short USHORT;
typedef char CHAR;
typedef short SHORT;
typedef unsigned long ULONG;
typedef unsigned char UCHAR;
typedef SHORT* LPSHORT;
typedef LPVOID* LPLPVOID;
typedef ULONG* LPULONG;
typedef USHORT* LPUSHORT;

typedef HANDLE HPROVIDER;

typedef ULONG REQUESTID;
typedef REQUESTID* LPREQUESTID;

typedef HANDLE HAPP;
typedef HAPP* LPHAPP;

typedef USHORT HSERVICE;
typedef HSERVICE* LPHSERVICE;

typedef LONG HRESULT;
typedef HRESULT* LPHRESULT;

typedef BOOL(WINAPI* XFSBLOCKINGHOOK)(VOID);
typedef XFSBLOCKINGHOOK* LPXFSBLOCKINGHOOK;

/****** String lengths **************************************************/

#define WFSDDESCRIPTION_LEN                     256
#define WFSDSYSSTATUS_LEN                       256

/****** Values of WFSDEVSTATUS.fwState **********************************/

#define WFS_STAT_DEVONLINE                      (0)
#define WFS_STAT_DEVOFFLINE                     (1)
#define WFS_STAT_DEVPOWEROFF                    (2)
#define WFS_STAT_DEVNODEVICE                    (3)
#define WFS_STAT_DEVHWERROR                     (4)
#define WFS_STAT_DEVUSERERROR                   (5)
#define WFS_STAT_DEVBUSY                        (6)
#define WFS_STAT_DEVFRAUDATTEMPT                (7)

/****** Value of WFS_DEFAULT_HAPP ***************************************/

#define WFS_DEFAULT_HAPP                        (0)

/****** Data Structures *************************************************/

typedef struct _wfs_result
{
    REQUESTID       RequestID;//4
    HSERVICE        hService;//2
    SYSTEMTIME      tsTimestamp;//16
    HRESULT         hResult; //4
    union {
        DWORD       dwCommandCode;
        DWORD       dwEventID;
    } u;//4
    LPVOID          lpBuffer; //offset 30
} WFSRESULT, * LPWFSRESULT;

typedef struct _wfsversion
{
    WORD            wVersion;
    WORD            wLowVersion;
    WORD            wHighVersion;
    CHAR            szDescription[WFSDDESCRIPTION_LEN + 1];
    CHAR            szSystemStatus[WFSDSYSSTATUS_LEN + 1];
} WFSVERSION, * LPWFSVERSION;

/****** Message Structures **********************************************/

typedef struct _wfs_devstatus
{
    LPSTR           lpszPhysicalName;
    LPSTR           lpszWorkstationName;
    DWORD           dwState;
} WFSDEVSTATUS, * LPWFSDEVSTATUS;

typedef struct _wfs_undevmsg
{
    LPSTR           lpszLogicalName;
    LPSTR           lpszWorkstationName;
    LPSTR           lpszAppID;
    DWORD           dwSize;
    LPBYTE          lpbDescription;
    DWORD           dwMsg;
    LPWFSRESULT     lpWFSResult;
} WFSUNDEVMSG, * LPWFSUNDEVMSG;

typedef struct _wfs_appdisc
{
    LPSTR           lpszLogicalName;
    LPSTR           lpszWorkstationName;
    LPSTR           lpszAppID;
} WFSAPPDISC, * LPWFSAPPDISC;

typedef struct _wfs_hwerror
{
    LPSTR           lpszLogicalName;
    LPSTR           lpszPhysicalName;
    LPSTR           lpszWorkstationName;
    LPSTR           lpszAppID;
    DWORD           dwAction;
    DWORD           dwSize;
    LPBYTE          lpbDescription;
} WFSHWERROR, * LPWFSHWERROR;

typedef struct _wfs_vrsnerror
{
    LPSTR           lpszLogicalName;
    LPSTR           lpszWorkstationName;
    LPSTR           lpszAppID;
    DWORD           dwSize;
    LPBYTE          lpbDescription;
    LPWFSVERSION    lpWFSVersion;
} WFSVRSNERROR, * LPWFSVRSNERROR;

/****** Error codes ******************************************************/

#define WFS_SUCCESS                             (0)
#define WFS_ERR_ALREADY_STARTED                 (-1)
#define WFS_ERR_API_VER_TOO_HIGH                (-2)
#define WFS_ERR_API_VER_TOO_LOW                 (-3)
#define WFS_ERR_CANCELED                        (-4)
#define WFS_ERR_CFG_INVALID_HKEY                (-5)
#define WFS_ERR_CFG_INVALID_NAME                (-6)
#define WFS_ERR_CFG_INVALID_SUBKEY              (-7)
#define WFS_ERR_CFG_INVALID_VALUE               (-8)
#define WFS_ERR_CFG_KEY_NOT_EMPTY               (-9)
#define WFS_ERR_CFG_NAME_TOO_LONG               (-10)
#define WFS_ERR_CFG_NO_MORE_ITEMS               (-11)
#define WFS_ERR_CFG_VALUE_TOO_LONG              (-12)
#define WFS_ERR_DEV_NOT_READY                   (-13)
#define WFS_ERR_HARDWARE_ERROR                  (-14)
#define WFS_ERR_INTERNAL_ERROR                  (-15)
#define WFS_ERR_INVALID_ADDRESS                 (-16)
#define WFS_ERR_INVALID_APP_HANDLE              (-17)
#define WFS_ERR_INVALID_BUFFER                  (-18)
#define WFS_ERR_INVALID_CATEGORY                (-19)
#define WFS_ERR_INVALID_COMMAND                 (-20)
#define WFS_ERR_INVALID_EVENT_CLASS             (-21)
#define WFS_ERR_INVALID_HSERVICE                (-22)
#define WFS_ERR_INVALID_HPROVIDER               (-23)
#define WFS_ERR_INVALID_HWND                    (-24)
#define WFS_ERR_INVALID_HWNDREG                 (-25)
#define WFS_ERR_INVALID_POINTER                 (-26)
#define WFS_ERR_INVALID_REQ_ID                  (-27)
#define WFS_ERR_INVALID_RESULT                  (-28)
#define WFS_ERR_INVALID_SERVPROV                (-29)
#define WFS_ERR_INVALID_TIMER                   (-30)
#define WFS_ERR_INVALID_TRACELEVEL              (-31)
#define WFS_ERR_LOCKED                          (-32)
#define WFS_ERR_NO_BLOCKING_CALL                (-33)
#define WFS_ERR_NO_SERVPROV                     (-34)
#define WFS_ERR_NO_SUCH_THREAD                  (-35)
#define WFS_ERR_NO_TIMER                        (-36)
#define WFS_ERR_NOT_LOCKED                      (-37)
#define WFS_ERR_NOT_OK_TO_UNLOAD                (-38)
#define WFS_ERR_NOT_STARTED                     (-39)
#define WFS_ERR_NOT_REGISTERED                  (-40)
#define WFS_ERR_OP_IN_PROGRESS                  (-41)
#define WFS_ERR_OUT_OF_MEMORY                   (-42)
#define WFS_ERR_SERVICE_NOT_FOUND               (-43)
#define WFS_ERR_SPI_VER_TOO_HIGH                (-44)
#define WFS_ERR_SPI_VER_TOO_LOW                 (-45)
#define WFS_ERR_SRVC_VER_TOO_HIGH               (-46)
#define WFS_ERR_SRVC_VER_TOO_LOW                (-47)
#define WFS_ERR_TIMEOUT                         (-48)
#define WFS_ERR_UNSUPP_CATEGORY                 (-49)
#define WFS_ERR_UNSUPP_COMMAND                  (-50)
#define WFS_ERR_VERSION_ERROR_IN_SRVC           (-51)
#define WFS_ERR_INVALID_DATA                    (-52)
#define WFS_ERR_SOFTWARE_ERROR                  (-53)
#define WFS_ERR_CONNECTION_LOST                 (-54)
#define WFS_ERR_USER_ERROR                      (-55)
#define WFS_ERR_UNSUPP_DATA                     (-56)
#define WFS_ERR_FRAUD_ATTEMPT                   (-57)
#define WFS_ERR_SEQUENCE_ERROR                  (-58)


#define WFS_INDEFINITE_WAIT                     0

/****** Messages ********************************************************/

/* Message-No = (WM_USER + No) */

#define WFS_OPEN_COMPLETE                       (WM_USER + 1)
#define WFS_CLOSE_COMPLETE                      (WM_USER + 2)
#define WFS_LOCK_COMPLETE                       (WM_USER + 3)
#define WFS_UNLOCK_COMPLETE                     (WM_USER + 4)
#define WFS_REGISTER_COMPLETE                   (WM_USER + 5)
#define WFS_DEREGISTER_COMPLETE                 (WM_USER + 6)
#define WFS_GETINFO_COMPLETE                    (WM_USER + 7)
#define WFS_EXECUTE_COMPLETE                    (WM_USER + 8)

#define WFS_EXECUTE_EVENT                       (WM_USER + 20)
#define WFS_SERVICE_EVENT                       (WM_USER + 21)
#define WFS_USER_EVENT                          (WM_USER + 22)
#define WFS_SYSTEM_EVENT                        (WM_USER + 23)

#define WFS_TIMER_EVENT                         (WM_USER + 100)

/****** Event Classes ***************************************************/

#define SERVICE_EVENTS                          (1)
#define USER_EVENTS                             (2)
#define SYSTEM_EVENTS                           (4)
#define EXECUTE_EVENTS                          (8)

/****** System Event IDs ************************************************/

#define WFS_SYSE_UNDELIVERABLE_MSG              (1)
#define WFS_SYSE_HARDWARE_ERROR                 (2)
#define WFS_SYSE_VERSION_ERROR                  (3)
#define WFS_SYSE_DEVICE_STATUS                  (4)
#define WFS_SYSE_APP_DISCONNECT                 (5)
#define WFS_SYSE_SOFTWARE_ERROR                 (6)
#define WFS_SYSE_USER_ERROR                     (7)
#define WFS_SYSE_LOCK_REQUESTED                 (8)
#define WFS_SYSE_FRAUD_ATTEMPT                  (9)


/****** XFS Trace Level ********************************************/

#define WFS_TRACE_API                           (0x00000001)
#define WFS_TRACE_ALL_API                       (0x00000002)
#define WFS_TRACE_SPI                           (0x00000004)
#define WFS_TRACE_ALL_SPI                       (0x00000008)
#define WFS_TRACE_MGR                           (0x00000010)

/****** XFS Error Actions ********************************************/

#define WFS_ERR_ACT_NOACTION                    (0x0000)
#define WFS_ERR_ACT_RESET                       (0x0001)
#define WFS_ERR_ACT_SWERROR                     (0x0002)
#define WFS_ERR_ACT_CONFIG                      (0x0004)
#define WFS_ERR_ACT_HWCLEAR                     (0x0008)
#define WFS_ERR_ACT_HWMAINT                     (0x0010)
#define WFS_ERR_ACT_SUSPEND                     (0x0020)

/****** XFS SNMP MIB Category Codes **********************************/
/* NOTE: To support the XFS SNMP MIB, the WFSGet[Async]Info category codes between 60000 and 60999 are reserved.*/


/****** API functions ***************************************************/
extern "C" {
    HRESULT  WINAPI WFSCancelAsyncRequest(HSERVICE hService, REQUESTID RequestID);
    HRESULT  WINAPI WFSCancelBlockingCall(DWORD dwThreadID);
    HRESULT  WINAPI WFSCleanUp();
    HRESULT  WINAPI WFSClose(HSERVICE hService);
    HRESULT  WINAPI WFSAsyncClose(HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID);
    HRESULT  WINAPI WFSCreateAppHandle(LPHAPP lphApp);
    HRESULT  WINAPI WFSDeregister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg);
    HRESULT  WINAPI WFSAsyncDeregister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID);
    HRESULT  WINAPI WFSDestroyAppHandle(HAPP hApp);
    HRESULT  WINAPI WFSExecute(HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, LPWFSRESULT* lppResult);
    HRESULT  WINAPI WFSAsyncExecute(HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);
    HRESULT  WINAPI WFSFreeResult(LPWFSRESULT lpResult);
    HRESULT  WINAPI WFSGetInfo(HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT* lppResult);
    HRESULT  WINAPI WFSAsyncGetInfo(HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);
    BOOL     WINAPI WFSIsBlocking();
    HRESULT  WINAPI WFSLock(HSERVICE hService, DWORD dwTimeOut, LPWFSRESULT* lppResult);
    HRESULT  WINAPI WFSAsyncLock(HSERVICE hService, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);
    HRESULT  WINAPI WFSOpen(LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPHSERVICE lphService);
    HRESULT  WINAPI WFSAsyncOpen(LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, LPHSERVICE lphService, HWND hWnd, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPREQUESTID lpRequestID);
    HRESULT  WINAPI WFSRegister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg);
    HRESULT  WINAPI WFSAsyncRegister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID);
    HRESULT  WINAPI WFSSetBlockingHook(XFSBLOCKINGHOOK lpBlockFunc, LPXFSBLOCKINGHOOK lppPrevFunc);
    HRESULT  WINAPI WFSStartUp(DWORD dwVersionsRequired, LPWFSVERSION lpWFSVersion);
    HRESULT  WINAPI WFSUnhookBlockingHook();
    HRESULT  WINAPI WFSUnlock(HSERVICE hService);
    HRESULT  WINAPI WFSAsyncUnlock(HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID);
    HRESULT  WINAPI WFMSetTraceLevel(HSERVICE hService, DWORD dwTraceLevel);

}
/*   restore alignment   */
#pragma pack(pop)


#endif  /* __inc_xfsapi__h */
