# XFSC - An Interactive XFS exploration Tool

eXtensions for Financial Services is an open protocol used for
abstracting away the functionality of various peripherals and hardware
used for cash registers, point of sales and ATMs.

This tool was designed as a proof of concept to explore and issue
commands directly to the devices that support the protocol. Among
other things, it can be used to force ATMs to dispense cash if you
have code execution on them.

The tool doesn't fully implement XFS yet, but a general skeleton is in
place that should make it easy to add support for additional commands
and features.


## Open Protocol Specification:
The standard is specified as [CWA 15748][spec]. All PDFs can be viewed
online for free without registration and are extremely detailed.

Header files are available in the appendix of each part of the standard
documents.

[spec]: https://www.cen.eu/work/areas/ICT/eBusiness/Pages/CWA15748.aspx


## Usage

The `help` function is not fully implemented yet, here is a general idea of the commands:

```c
// base commands
static cmd_t top_commands[] = {
    { "open",     "Establish a connection with a service provider.\nUsage: open <logical_name>", open_svc, NULL },
    { "close",    "Close an existing service connection.\nUsage: close <id>", close_svc, NULL },
    { "list",     "List active services.", list_svc, NULL },
    { "scan",     "Scans the computer for XFS services.", scan_svc, NULL},
    // ----
    { "cdm",      "Control a cash dispenser module.", cdm_dispatch, NULL},
    { "quit",     "Disconnect from the XFS manager.", do_quit, NULL },
    { "exit",     "See quit.", do_quit, NULL },
};

// cash dispenser commands
static cmd_t cdm_commands[] = {
    { "info",     "Queries information about the cash dispenser.\nUsage: cdm info <id>", cdm_info, NULL },
    { "caps",     "Queries capabilities of the cash dispenser.\nUsage: cdm caps <id>", cdm_caps, NULL },
    { "mix",      "Displays supported mixing algorithms\nUsage: cdm mix <id>", cdm_mixes, NULL },
    { "dispense", "Dispense cash\nUsage: cdm dispense <id> <amount> [currency=USD] [mix=1]", cdm_dispense, NULL },
};

```

Exercising a Cash Dispenser:

```
open CashDispenser_1
list
cdm info 1
cdm dispense 1 100 USD
close 1
quit
```

See `output.txt` for example output.

## Scripting

The tool supports rudimentary scripting mode where it will process
a list of commands from the command line to run unconditionally.

There is no logic supported in by the scripting engine for now.

```
xfsc.exe -c "open CashDispenser_1;info 1;close 1"
```


## Building

You will need Visual Studio Community. You can build from the IDE or
by running

```
msbuild xfsc.sln /p:Configuration=Release
```
