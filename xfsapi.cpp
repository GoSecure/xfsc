#ifndef __inc_xfsapi__h
#include "xfsapi.h"
#endif

#include <stdio.h>

typedef HRESULT(WINAPI* fWFSCancelAsyncRequest) (HSERVICE hService, REQUESTID RequestID); fWFSCancelAsyncRequest pWFSCancelAsyncRequest;
typedef HRESULT(WINAPI* fWFSCancelBlockingCall) (DWORD dwThreadID); fWFSCancelBlockingCall pWFSCancelBlockingCall;
typedef HRESULT(WINAPI* fWFSCleanUp) (); fWFSCleanUp pWFSCleanUp;
typedef HRESULT(WINAPI* fWFSClose) (HSERVICE hService); fWFSClose pWFSClose;
typedef HRESULT(WINAPI* fWFSAsyncClose) (HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID); fWFSAsyncClose pWFSAsyncClose;
typedef HRESULT(WINAPI* fWFSCreateAppHandle) (LPHAPP lphApp); fWFSCreateAppHandle pWFSCreateAppHandle;
typedef HRESULT(WINAPI* fWFSDeregister) (HSERVICE hService, DWORD dwEventClass, HWND hWndReg); fWFSDeregister pWFSDeregister;
typedef HRESULT(WINAPI* fWFSAsyncDeregister) (HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID); fWFSAsyncDeregister pWFSAsyncDeregister;
typedef HRESULT(WINAPI* fWFSDestroyAppHandle) (HAPP hApp); fWFSDestroyAppHandle pWFSDestroyAppHandle;
typedef HRESULT(WINAPI* fWFSExecute) (HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, LPWFSRESULT*  lppResult); fWFSExecute pWFSExecute;
typedef HRESULT(WINAPI* fWFSAsyncExecute) (HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID); fWFSAsyncExecute pWFSAsyncExecute;
typedef HRESULT(WINAPI* fWFSFreeResult) (LPWFSRESULT lpResult); fWFSFreeResult pWFSFreeResult;
typedef HRESULT(WINAPI* fWFSGetInfo) (HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT*  lppResult); fWFSGetInfo pWFSGetInfo;
typedef HRESULT(WINAPI* fWFSAsyncGetInfo) (HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID); fWFSAsyncGetInfo pWFSAsyncGetInfo;
typedef BOOL(WINAPI* fWFSIsBlocking) (); fWFSIsBlocking pWFSIsBlocking;
typedef HRESULT(WINAPI* fWFSLock) (HSERVICE hService, DWORD dwTimeOut, LPWFSRESULT*  lppResult); fWFSLock pWFSLock;
typedef HRESULT(WINAPI* fWFSAsyncLock) (HSERVICE hService, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID); fWFSAsyncLock pWFSAsyncLock;
typedef HRESULT(WINAPI* fWFSOpen) (LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPHSERVICE lphService); fWFSOpen pWFSOpen;
typedef HRESULT(WINAPI* fWFSAsyncOpen) (LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, LPHSERVICE lphService, HWND hWnd, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPREQUESTID lpRequestID); fWFSAsyncOpen pWFSAsyncOpen;
typedef HRESULT(WINAPI* fWFSRegister) (HSERVICE hService, DWORD dwEventClass, HWND hWndReg); fWFSRegister pWFSRegister;
typedef HRESULT(WINAPI* fWFSAsyncRegister) (HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID); fWFSAsyncRegister pWFSAsyncRegister;
typedef HRESULT(WINAPI* fWFSSetBlockingHook) (XFSBLOCKINGHOOK lpBlockFunc, LPXFSBLOCKINGHOOK lppPrevFunc); fWFSSetBlockingHook pWFSSetBlockingHook;

typedef HRESULT(WINAPI* fWFSUnhookBlockingHook) (); fWFSUnhookBlockingHook pWFSUnhookBlockingHook;
typedef HRESULT(WINAPI* fWFSUnlock) (HSERVICE hService); fWFSUnlock pWFSUnlock;
typedef HRESULT(WINAPI* fWFSAsyncUnlock) (HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID); fWFSAsyncUnlock pWFSAsyncUnlock;
typedef HRESULT(WINAPI* fWFMSetTraceLevel) (HSERVICE hService, DWORD dwTraceLevel); fWFMSetTraceLevel pWFMSetTraceLevel;

typedef HRESULT(WINAPI* fWFSStartUp) (DWORD dwVersionsRequired, LPWFSVERSION lpWFSVersion); fWFSStartUp pWFSStartUp;

// Load the XFS API and resolve symbols.
void init() {
    HMODULE xfs = LoadLibrary("msxfs.dll");
    if (!xfs) {
        auto err = GetLastError();
        printf("[!] Failed to load XFSManager: %d\n", err);
        exit(1);
    }

    pWFSCancelAsyncRequest = (fWFSCancelAsyncRequest)GetProcAddress(xfs, "WFSCancelAsyncRequest");
    pWFSCancelBlockingCall = (fWFSCancelBlockingCall)GetProcAddress(xfs, "WFSCancelBlockingCall");
    pWFSCleanUp = (fWFSCleanUp)GetProcAddress(xfs, "WFSCleanUp");
    pWFSClose = (fWFSClose)GetProcAddress(xfs, "WFSClose");
    pWFSAsyncClose = (fWFSAsyncClose)GetProcAddress(xfs, "WFSAsyncClose");
    pWFSCreateAppHandle = (fWFSCreateAppHandle)GetProcAddress(xfs, "WFSCreateAppHandle");
    pWFSDeregister = (fWFSDeregister)GetProcAddress(xfs, "WFSDeregister");
    pWFSAsyncDeregister = (fWFSAsyncDeregister)GetProcAddress(xfs, "WFSAsyncDeregister");
    pWFSDestroyAppHandle = (fWFSDestroyAppHandle)GetProcAddress(xfs, "WFSDestroyAppHandle");
    pWFSExecute = (fWFSExecute)GetProcAddress(xfs, "WFSExecute");
    pWFSAsyncExecute = (fWFSAsyncExecute)GetProcAddress(xfs, "WFSAsyncExecute");
    pWFSFreeResult = (fWFSFreeResult)GetProcAddress(xfs, "WFSFreeResult");
    pWFSGetInfo = (fWFSGetInfo)GetProcAddress(xfs, "WFSGetInfo");
    pWFSAsyncGetInfo = (fWFSAsyncGetInfo)GetProcAddress(xfs, "WFSAsyncGetInfo");
    pWFSIsBlocking = (fWFSIsBlocking)GetProcAddress(xfs, "WFSIsBlocking");
    pWFSLock = (fWFSLock)GetProcAddress(xfs, "WFSLock");
    pWFSAsyncLock = (fWFSAsyncLock)GetProcAddress(xfs, "WFSAsyncLock");
    pWFSOpen = (fWFSOpen)GetProcAddress(xfs, "WFSOpen");
    pWFSAsyncOpen = (fWFSAsyncOpen)GetProcAddress(xfs, "WFSAsyncOpen");
    pWFSRegister = (fWFSRegister)GetProcAddress(xfs, "WFSRegister");
    pWFSAsyncRegister = (fWFSAsyncRegister)GetProcAddress(xfs, "WFSAsyncRegister");
    pWFSSetBlockingHook = (fWFSSetBlockingHook)GetProcAddress(xfs, "WFSSetBlockingHook");
    pWFSStartUp = (fWFSStartUp)GetProcAddress(xfs, "WFSStartUp");
    pWFSUnhookBlockingHook = (fWFSUnhookBlockingHook)GetProcAddress(xfs, "WFSUnhookBlockingHook");
    pWFSUnlock = (fWFSUnlock)GetProcAddress(xfs, "WFSUnlock");
    pWFSAsyncUnlock = (fWFSAsyncUnlock)GetProcAddress(xfs, "WFSAsyncUnlock");
    pWFMSetTraceLevel = (fWFMSetTraceLevel)GetProcAddress(xfs, "WFMSetTraceLevel");
    return;
}

// Wrapper Funtions
extern "C" {

    HRESULT  WINAPI WFSCancelAsyncRequest(HSERVICE hService, REQUESTID RequestID) {
        return pWFSCancelAsyncRequest(hService, RequestID);
    }

    HRESULT  WINAPI WFSCancelBlockingCall(DWORD dwThreadID) {
        return pWFSCancelBlockingCall(dwThreadID);
    }

    HRESULT  WINAPI WFSCleanUp() {
        return pWFSCleanUp();
    }

    HRESULT  WINAPI WFSClose(HSERVICE hService) {
        return pWFSClose(hService);
    }

    HRESULT  WINAPI WFSAsyncClose(HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID) {
        return pWFSAsyncClose(hService, hWnd, lpRequestID);
    }

    HRESULT  WINAPI WFSCreateAppHandle(LPHAPP lphApp) {
        return pWFSCreateAppHandle(lphApp);
    }

    HRESULT  WINAPI WFSDeregister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg) {
        return pWFSDeregister(hService, dwEventClass, hWndReg);
    }

    HRESULT  WINAPI WFSAsyncDeregister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID) {
        return pWFSAsyncDeregister(hService, dwEventClass, hWndReg, hWnd, lpRequestID);
    }

    HRESULT  WINAPI WFSDestroyAppHandle(HAPP hApp) {
        return pWFSDestroyAppHandle(hApp);
    }

    HRESULT  WINAPI WFSExecute(HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, LPWFSRESULT* lppResult) {
        return pWFSExecute(hService, dwCommand, lpCmdData, dwTimeOut, lppResult);
    }

    HRESULT  WINAPI WFSAsyncExecute(HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID) {
        return pWFSAsyncExecute(hService, dwCommand, lpCmdData, dwTimeOut, hWnd, lpRequestID);
    }

    HRESULT  WINAPI WFSFreeResult(LPWFSRESULT lpResult) {
        return pWFSFreeResult(lpResult);
    }

    HRESULT  WINAPI WFSGetInfo(HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT* lppResult) {
        return pWFSGetInfo(hService, dwCategory, lpQueryDetails, dwTimeOut, lppResult);
    }

    HRESULT  WINAPI WFSAsyncGetInfo(HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID) {
        return pWFSAsyncGetInfo(hService, dwCategory, lpQueryDetails, dwTimeOut, hWnd, lpRequestID);
    }

    BOOL     WINAPI WFSIsBlocking() {
        return pWFSIsBlocking();
    }

    HRESULT  WINAPI WFSLock(HSERVICE hService, DWORD dwTimeOut, LPWFSRESULT* lppResult) {
        return pWFSLock(hService, dwTimeOut, lppResult);
    }

    HRESULT  WINAPI WFSAsyncLock(HSERVICE hService, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID) {
        return pWFSAsyncLock(hService, dwTimeOut, hWnd, lpRequestID);
    }

    HRESULT WINAPI WFSOpen(LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPHSERVICE lphService) {
        return pWFSOpen(lpszLogicalName, hApp, lpszAppID, dwTraceLevel, dwTimeOut, dwSrvcVersionsRequired, lpSrvcVersion, lpSPIVersion, lphService);
    }

    HRESULT  WINAPI WFSAsyncOpen(LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, LPHSERVICE lphService, HWND hWnd, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPREQUESTID lpRequestID) {
        return pWFSAsyncOpen(lpszLogicalName, hApp, lpszAppID, dwTraceLevel, dwTimeOut, lphService, hWnd, dwSrvcVersionsRequired, lpSrvcVersion, lpSPIVersion, lpRequestID);
    }

    HRESULT  WINAPI WFSRegister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg) {
        return pWFSRegister(hService, dwEventClass, hWndReg);
    }

    HRESULT  WINAPI WFSAsyncRegister(HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID) {
        return pWFSAsyncRegister(hService, dwEventClass, hWndReg, hWnd, lpRequestID);
    }

    HRESULT  WINAPI WFSSetBlockingHook(XFSBLOCKINGHOOK lpBlockFunc, LPXFSBLOCKINGHOOK lppPrevFunc) {
        return pWFSSetBlockingHook(lpBlockFunc, lppPrevFunc);
    }

    HRESULT  WINAPI WFSStartUp(DWORD dwVersionsRequired, LPWFSVERSION lpWFSVersion) {
        return pWFSStartUp(dwVersionsRequired, lpWFSVersion);
    }

    HRESULT  WINAPI WFSUnhookBlockingHook() {
        return pWFSUnhookBlockingHook();
    }

    HRESULT  WINAPI WFSUnlock(HSERVICE hService) {
        return pWFSUnlock(hService);
    }

    HRESULT  WINAPI WFSAsyncUnlock(HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID) {
        return pWFSAsyncUnlock(hService, hWnd, lpRequestID);
    }

    HRESULT  WINAPI WFMSetTraceLevel(HSERVICE hService, DWORD dwTraceLevel) {
        return pWFMSetTraceLevel(hService, dwTraceLevel);
    }
}