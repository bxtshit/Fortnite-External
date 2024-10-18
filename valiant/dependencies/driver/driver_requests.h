#pragma once
#include <basetsd.h>
#include <winnt.h>

#define IOCTL_READWRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1000, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_BASE_ADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1001, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_GUARDED_REGION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1002, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_SECURITY 0x12A3B45

typedef struct _ReadWrite {
    INT32 Security;
    INT32 ProcessId;
    ULONGLONG Address;
    ULONGLONG Buffer;
    ULONGLONG Size;
    BOOLEAN Write;
} ReadWrite, * pReadWrite;

typedef struct _module_base
{
	INT32 Security;
	INT32 ProcessId;
	ULONGLONG* Address;
} BaseAddress, * pBaseAddress;