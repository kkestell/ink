#ifndef __BOOT_UEFI_H
#define __BOOT_UEFI_H

// ============================================================================
// Basic types and constants
// ============================================================================

#include <stdint.h>

// UEFIABI can be defined to __attribute__((ms_abi)) for example
#ifndef UEFIABI
#define UEFIABI
#endif

#define UEFI_FALSE 0
#define UEFI_TRUE  1

#define UEFI_SUCCESS                 0x0000000000000000
#define UEFI_ERR                     0x8000000000000000
#define UEFI_LOAD_ERROR              (UEFI_ERR | 0x0000000000000001)
#define UEFI_INVALID_PARAMETER       (UEFI_ERR | 0x0000000000000002)
#define UEFI_UNSUPPORTED             (UEFI_ERR | 0x0000000000000003)
#define UEFI_BAD_BUFFER_SIZE         (UEFI_ERR | 0x0000000000000004)
#define UEFI_BUFFER_TOO_SMALL        (UEFI_ERR | 0x0000000000000005)
#define UEFI_NOT_READY               (UEFI_ERR | 0x0000000000000006)
#define UEFI_DEVICE_ERROR            (UEFI_ERR | 0x0000000000000007)
#define UEFI_WRITE_PROTECTED         (UEFI_ERR | 0x0000000000000008)
#define UEFI_OUT_OF_RESOURCES        (UEFI_ERR | 0x0000000000000009)
#define UEFI_VOLUME_CORRUPTED        (UEFI_ERR | 0x000000000000000a)
#define UEFI_VOLUME_FULL             (UEFI_ERR | 0x000000000000000b)
#define UEFI_NO_MEDIA                (UEFI_ERR | 0x000000000000000c)
#define UEFI_MEDIA_CHANGED           (UEFI_ERR | 0x000000000000000d)
#define UEFI_NOT_FOUND               (UEFI_ERR | 0x000000000000000e)
#define UEFI_ACCESS_DENIED           (UEFI_ERR | 0x000000000000000f)
#define UEFI_NO_RESPONSE             (UEFI_ERR | 0x0000000000000010)
#define UEFI_NO_MAPPING              (UEFI_ERR | 0x0000000000000011)
#define UEFI_TIMEOUT                 (UEFI_ERR | 0x0000000000000012)
#define UEFI_NOT_STARTED             (UEFI_ERR | 0x0000000000000013)
#define UEFI_ALREADY_STARTED         (UEFI_ERR | 0x0000000000000014)
#define UEFI_ABORTED                 (UEFI_ERR | 0x0000000000000015)
#define UEFI_ICMP_ERROR              (UEFI_ERR | 0x0000000000000016)
#define UEFI_TFTP_ERROR              (UEFI_ERR | 0x0000000000000017)
#define UEFI_PROTOCOL_ERROR          (UEFI_ERR | 0x0000000000000018)
#define UEFI_INCOMPATIBLE_VERSION    (UEFI_ERR | 0x0000000000000019)
#define UEFI_SECURITY_VIOLATION      (UEFI_ERR | 0x000000000000001a)
#define UEFI_CRC_ERROR               (UEFI_ERR | 0x000000000000001b)
#define UEFI_END_OF_MEDIA            (UEFI_ERR | 0x000000000000001c)
#define UEFI_END_OF_FILE             (UEFI_ERR | 0x000000000000001f)
#define UEFI_INVALID_LANGUAGE        (UEFI_ERR | 0x0000000000000020)
#define UEFI_COMPROMISED_DATA        (UEFI_ERR | 0x0000000000000021)
#define UEFI_IP_ADDRESS_CONFLICT     (UEFI_ERR | 0x0000000000000022)
#define UEFI_HTTP_ERROR              (UEFI_ERR | 0x0000000000000023)

#define UEFI_MEMORY_UC               0x0000000000000001
#define UEFI_MEMORY_WC               0x0000000000000002
#define UEFI_MEMORY_WT               0x0000000000000004
#define UEFI_MEMORY_WB               0x0000000000000008
#define UEFI_MEMORY_UCE              0x0000000000000010
#define UEFI_MEMORY_WP               0x0000000000001000
#define UEFI_MEMORY_RP               0x0000000000002000
#define UEFI_MEMORY_XP               0x0000000000004000
#define UEFI_MEMORY_NV               0x0000000000008000
#define UEFI_MEMORY_MORE_RELIABLE    0x0000000000010000
#define UEFI_MEMORY_RO               0x0000000000020000
#define UEFI_MEMORY_RUNTIME          0x8000000000000000

typedef uint8_t     BOOLEAN;
typedef uint16_t    CHAR16;
typedef void        *UEFI_EVENT;
typedef void        *UEFI_HANDLE;
typedef uint64_t    UEFI_LBA;
typedef uint64_t    UEFI_PHYSICAL_ADDRESS;
typedef uint64_t    UEFI_STATUS;
typedef uint64_t    UEFI_TL;
typedef uint64_t    UEFI_TPL;
typedef uint64_t    UEFI_VIRTUAL_ADDRESS;
typedef int8_t      INT8;
typedef int16_t     INT16;
typedef int32_t     INT32;
typedef int64_t     INT64;
typedef int64_t     INTN;
typedef uint8_t     UINT8;
typedef uint16_t    UINT16;
typedef uint32_t    UINT32;
typedef uint64_t    UINT64;
typedef uint64_t    UINTN;
typedef void        VOID;

typedef struct UEFI_GUID {
    UINT32  Data1;
    UINT16  Data2;
    UINT16  Data3;
    UINT8   Data4[8];
} UEFI_GUID;

typedef struct UEFI_MEMORY_DESCRIPTOR {
    UINT32                  Type;
    UEFI_PHYSICAL_ADDRESS    PhysicalStart;
    UEFI_VIRTUAL_ADDRESS     VirtualStart;
    UINT64                  NumberOfPages;
    UINT64                  Attribute;
} UEFI_MEMORY_DESCRIPTOR;

typedef struct UEFI_TABLE_HEADER {
    UINT64  Signature;
    UINT32  Revision;
    UINT32  HeaderSize;
    UINT32  CRC32;
    UINT32  Reserved;
} UEFI_TABLE_HEADER;

struct UEFI_SYSTEM_TABLE;

// ============================================================================
// Guids
// ============================================================================

// Protocols

#define UEFI_ACPI_TABLE_PROTOCOL_GUID               {0xffe06bdd,  0x6107, 0x46a6, {0x7b, 0xb2, 0x5a, 0x9c, 0x7e, 0xc5, 0x28, 0x5c}}
#define UEFI_DEVICE_PATH_PROTOCOL_GUID              {0x09576e91,  0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define UEFI_GRAPHICS_OUTPUT_PROTOCOL_GUID          {0x9042a9de,  0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}}
#define UEFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID {0xbc62157e,  0x3e33, 0x4fec, {0x99, 0x20, 0x2d, 0x3b, 0x36, 0xd7, 0x50, 0xdf}}
#define UEFI_LOADED_IMAGE_PROTOCOL_GUID             {0x5b1b31a1,  0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID       {0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define UEFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID        {0x387477c1,  0x69c7, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID       {0x387477c2,  0x69c7, 0x11d2, {0x82, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define UEFI_VGA_MINI_PORT_PROTOCOL_GUID            {0xc7735a2f,  0x88f5, 0x4882, {0xae, 0x63, 0xfa, 0xac, 0x8c, 0x8b, 0x86, 0xb3}}

#define MPS_TABLE_GUID        {0xeb9d2d2f, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define SAL_SYSTEM_TABLE_GUID {0xeb9d2d32, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define SMBIOS_TABLE_GUID     {0xeb9d2d31, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define SMBIOS2_TABLE_GUID    {0xf2fd1544, 0x9794, 0x4a2c, {0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94}}

#define ACPI_TABLE_GUID         {0xeb9d2d30, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define EFO_ACPI_10_TABLE_GUID  ACPI_TABLE_GUID

#define UEFI_ACPI_TABLE_GUID    {0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}}
#define UEFI_ACPI_20_TABLE_GUID UEFI_ACPI_TABLE_GUID

/*
 * efi-stip.h
 *
 * The UEFI Simple Text Input Protocol.
 */

typedef struct UEFI_INPUT_KEY {
    UINT16  ScanCode;
    UINT16  UnicodeChar;
} UEFI_INPUT_KEY;

struct UEFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef UEFI_STATUS (*UEFI_INPUT_RESET)(struct UEFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, BOOLEAN ExtendedVerification);
typedef UEFI_STATUS (*UEFI_INPUT_READ_KEY)(struct UEFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, UEFI_INPUT_KEY *Key);

typedef struct UEFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    UEFI_INPUT_RESET     Reset;
    UEFI_INPUT_READ_KEY  ReadKeyStroke;
    UEFI_EVENT           WaitForKey;
} UEFI_SIMPLE_TEXT_INPUT_PROTOCOL;

/*
 * efi-stop.h
 *
 * UEFI Simple Text Output Protocol.
 */

typedef struct SIMPLE_TEXT_OUTPUT_MODE {
    INT32   MaxMode;
    INT32   Mode;
    INT32   Attribute;
    INT32   CursorColumn;
    INT32   CursorRow;
    BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef UEFI_STATUS (*UEFI_TEXT_RESET)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN ExtendedVerification);
typedef UEFI_STATUS (*UEFI_TEXT_STRING)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
typedef UEFI_STATUS (*UEFI_TEXT_TEST_STRING)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
typedef UEFI_STATUS (*UEFI_TEXT_QUERY_MODE)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber, UINTN *Columns, UINTN *Rows);
typedef UEFI_STATUS (*UEFI_TEXT_SET_MODE)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber);
typedef UEFI_STATUS (*UEFI_TEXT_SET_ATTRIBUTE)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Attribute);
typedef UEFI_STATUS (*UEFI_TEXT_CLEAR_SCREEN)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
typedef UEFI_STATUS (*UEFI_TEXT_SET_CURSOR_POSITION)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Column, UINTN Row);
typedef UEFI_STATUS (*UEFI_TEXT_ENABLE_CURSOR)(struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN Visible);

typedef struct UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    UEFI_TEXT_RESET                  Reset;
    UEFI_TEXT_STRING                 OutputString;
    UEFI_TEXT_TEST_STRING            TestString;
    UEFI_TEXT_QUERY_MODE             QueryMode;
    UEFI_TEXT_SET_MODE               SetMode;
    UEFI_TEXT_SET_ATTRIBUTE          SetAttribute;
    UEFI_TEXT_CLEAR_SCREEN           ClearScreen;
    UEFI_TEXT_SET_CURSOR_POSITION    SetCursorPosition;
    UEFI_TEXT_ENABLE_CURSOR          EnableCursor;
    SIMPLE_TEXT_OUTPUT_MODE         *Mode;
} UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

/*
 * efi-time.h
 *
 * UEFI Time representation.
 */

#define UEFI_TIME_ADJUST_DAYLIGHT    0x01
#define UEFI_TIME_IN_DAYLIGHT        0x02

#define UEFI_UNSPECIFIED_TIMEZONE    0x07ff

typedef struct UEFI_TIME {
    UINT16  Year;
    UINT8   Month;
    UINT8   Day;
    UINT8   Hour;
    UINT8   Minute;
    UINT8   Second;
    UINT8   Pad1;
    UINT32  Nanosecond;
    INT16   TimeZone;
    UINT8   Daylight;
    UINT8   PAD2;
} UEFI_TIME;

typedef struct UEFI_TIME_CAPABILITIES {
    UINT32  Resolution;
    UINT32  Accuracy;
    BOOLEAN SetsToZero;
} UEFI_TIME_CAPABILITIES;

/*
 * efi-rs.h
 *
 * UEFI Runtime Services.
 */

#define UEFI_OPTIONAL_POINTER                0x00000001

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET  0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET        0x00040000

typedef enum UEFI_RESET_TYPE {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} UEFI_RESET_TYPE;

typedef struct UEFI_CAPSULE_BLOCK_DESCRIPTOR {
    UINT64  Length;
    union {
        UEFI_PHYSICAL_ADDRESS    DataBlock;
        UEFI_PHYSICAL_ADDRESS    ContinuationPointer;
    } Union;
} UEFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct UEFI_CAPSULE_HEADER {
    UEFI_GUID    CapsuleGuid;
    UINT32      HeaderSize;
    UINT32      Flags;
    UINT32      CapsuleImageSize;
} UEFI_CAPSULE_HEADER;

typedef UEFI_STATUS (UEFIABI *UEFI_GET_TIME)(UEFI_TIME *Time, UEFI_TIME_CAPABILITIES *Capabilities);
typedef UEFI_STATUS (UEFIABI *UEFI_SET_TIME)(UEFI_TIME *Time);
typedef UEFI_STATUS (UEFIABI *UEFI_GET_WAKEUP_TIME)(BOOLEAN *Enabled, BOOLEAN *Pending, UEFI_TIME *Time);
typedef UEFI_STATUS (UEFIABI *UEFI_SET_WAKEUP_TIME)(BOOLEAN Enable, UEFI_TIME *Time);
typedef UEFI_STATUS (UEFIABI *UEFI_SET_VIRTUAL_ADDRESS_MAP)(UINTN MemoryMapSize, UINTN DescriptorSize, UINT32 DescriptorVersion, UEFI_MEMORY_DESCRIPTOR *VirtualMap);
typedef UEFI_STATUS (UEFIABI *UEFI_CONVERT_POINTER)(UINTN DebugDisposition, VOID **Address);
typedef UEFI_STATUS (UEFIABI *UEFI_GET_VARIABLE)(CHAR16 *VariableName, UEFI_GUID *VendorGuid, UINT32 *Attributes, UINTN *DataSize, VOID *Data);
typedef UEFI_STATUS (UEFIABI *UEFI_GET_NEXT_VARIABLE_NAME)(UINTN *VariableNameSize, CHAR16 *VariableName, UEFI_GUID *VendorGuid);
typedef UEFI_STATUS (UEFIABI *UEFI_SET_VARIABLE)(CHAR16 *VariableName, UEFI_GUID *VendorGuid, UINT32 Attributes, UINTN DataSize, VOID *Data);
typedef UEFI_STATUS (UEFIABI *UEFI_GET_NEXT_HIGH_MONO_COUNT)(UINT32 *HighCount);
typedef UEFI_STATUS (UEFIABI *UEFI_RESET_SYSTEM)(UEFI_RESET_TYPE ResetType, UEFI_STATUS ResetStatus, UINTN DataSize, VOID *ResetData);
typedef UEFI_STATUS (UEFIABI *UEFI_UPDATE_CAPSULE)(UEFI_CAPSULE_HEADER **CapsuleHeaderArray, UINTN CapsuleCount, UEFI_PHYSICAL_ADDRESS ScatterGatherList);
typedef UEFI_STATUS (UEFIABI *UEFI_QUERY_CAPSULE_CAPABILITIES)(UEFI_CAPSULE_HEADER **CapsuleHeaderArray, UINTN CapsuleCount, UINT64 *MaximumCapsuleSize, UEFI_RESET_TYPE *ResetType);
typedef UEFI_STATUS (UEFIABI *UEFI_QUERY_VARIABLE_INFO)(UINT32 Attributes, UINT64 *MaximumVariableStorageSize, UINT64 *RemainingVariableStorageSize, UINT64 *MaximumVariableSize);

typedef struct UEFI_RUNTIME_SERVICES {
    UEFI_TABLE_HEADER                Hdr;
    UEFI_GET_TIME                    GetTime;
    UEFI_SET_TIME                    SetTime;
    UEFI_GET_WAKEUP_TIME             GetWakeupTime;
    UEFI_SET_WAKEUP_TIME             SetWakeupTime;
    UEFI_SET_VIRTUAL_ADDRESS_MAP     SetVirtualAddressMap;
    UEFI_CONVERT_POINTER             ConvertPointer;
    UEFI_GET_VARIABLE                GetVariable;
    UEFI_GET_NEXT_VARIABLE_NAME      GetNextVariableName;
    UEFI_SET_VARIABLE                SetVariable;
    UEFI_GET_NEXT_HIGH_MONO_COUNT    GetNextHighMonotonicCount;
    UEFI_RESET_SYSTEM                ResetSystem;
    UEFI_UPDATE_CAPSULE              UpdateCapsule;
    UEFI_QUERY_CAPSULE_CAPABILITIES  QueryCapsuleCapabilities;
    UEFI_QUERY_VARIABLE_INFO         QueryVariableInfo;
} UEFI_RUNTIME_SERVICES;

/*
 * efi-dpp.h
 *
 * UEFI device path protocol.
 */


#define UEFI_HARDWARE_DEVICE_PATH                0x01
#define UEFI_ACPI_DEVICE_PATH                    0x02
#define UEFI_MESSAGING_DEVICE_PATH               0x03
#define UEFI_MEDIA_DEVICE_PATH                   0x04
#define UEFI_BIOS_BOOT_SPECIFICATION_DEVICE_PATH 0x05
#define UEFI_END_OF_HARDWARE_DEVICE_PATH         0x7f

typedef struct UEFI_DEVICE_PATH_PROTOCOL {
    UINT8   Type;
    UINT8   SubType;
    UINT8   Length[2];
} UEFI_DEVICE_PATH_PROTOCOL;

/*
 * efi-bs.h
 *
 * UEFI Boot Services.
 */

#define EVT_TIMER                               0x80000000
#define EVT_RUNTIME                             0x40000000
#define EVT_NOTIFY_WAIT                         0x00000100
#define EVT_NOTIFY_SIGNAL                       0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES           0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE       0x00000202

#define TPL_APPLICATION                         4
#define TPL_CALLBACK                            8
#define TPL_NOTIFY                              16
#define TPL_HIGH_LEVEL                          31

#define UEFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL    0x00000001
#define UEFI_OPEN_PROTOCOL_GET_PROTOCOL          0x00000002
#define UEFI_OPEN_PROTOCOL_TEST_PROTOCOL         0x00000004
#define UEFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER   0x00000008
#define UEFI_OPEN_PROTOCOL_BY_DRIVER             0x00000010
#define UEFI_OPEN_PROTOCOL_EXCLUSIVE             0x00000020

#define UefiReservedMemoryType       0x00000000

#define UefiLoaderCode               0x00000001
#define UefiLoaderData               0x00000002

#define UefiBootServicesCode         0x00000003
#define UefiBootServicesData         0x00000004
#define UefiRuntimeServicesCode      0x00000005
#define UefiRuntimeServicesData      0x00000006
#define UefiConventionalMemory       0x00000007
#define UefiUnusableMemory           0x00000008
#define UefiACPIReclaimMemory        0x00000009
#define UefiACPIMemoryNVS            0x0000000a
#define UefiMemoryMappedIO           0x0000000b
#define UefiMemoryMappedIOPortSpace  0x0000000c
#define UefiPalCode                  0x0000000d
#define UefiPersistentMemory         0x0000000e
#define UefiMaxMemoryType            0x0000000e

typedef VOID (*UEFI_EVENT_NOTIFY)(UEFI_EVENT Event, VOID *Context);

typedef enum UEFI_TIMER_DELAY {
    TimerCancel,
    TimerPeriodic,
    TimerRelative
} UEFI_TIMER_DELAY;

typedef enum UEFI_ALLOCATE_TYPE {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} UEFI_ALLOCATE_TYPE;

typedef enum UEFI_INTERFACE_TYPE {
    UEFI_NATIVE_INTERFACE
} UEFI_INTERFACE_TYPE;

typedef enum UEFI_LOCATE_SEARCH_TYPE {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} UEFI_LOCATE_SEARCH_TYPE;

typedef struct UEFI_OPEN_PROTOCOL_INFORMATION_ENTRY {
    UEFI_HANDLE  AgentHandle;
    UEFI_HANDLE  ControllerHandle;
    UINT32      Attributes;
    UINT32      OpenCount;
} UEFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef UEFI_STATUS (*UEFI_RAISE_TPL)(UEFI_TPL NewTpl);
typedef UEFI_STATUS (*UEFI_RESTORE_TPL)(UEFI_TPL OldTpl);
typedef UEFI_STATUS (*UEFI_ALLOCATE_PAGES)(UEFI_ALLOCATE_TYPE Type, UINTN MemoryType, UINTN Pages, UEFI_PHYSICAL_ADDRESS *Memory);
typedef UEFI_STATUS (*UEFI_FREE_PAGES)(UEFI_PHYSICAL_ADDRESS Memory, UINTN Pages);
typedef UEFI_STATUS (*UEFI_GET_MEMORY_MAP)(UINTN *MemoryMapSize, UEFI_MEMORY_DESCRIPTOR *MemoryMap, UINTN *MapKey, UINTN *DescriptorSize, UINT32 *DescriptorVersion);
typedef UEFI_STATUS (*UEFI_ALLOCATE_POOL)(UINTN PoolType, UINTN Size, VOID **Buffer);
typedef UEFI_STATUS (*UEFI_FREE_POOL)(VOID *Buffer);
typedef UEFI_STATUS (*UEFI_CREATE_EVENT)(UINT32 Type, UEFI_TPL NotifyTpl, UEFI_EVENT_NOTIFY NotifyFunction, VOID *NotifyContext, UEFI_GUID *EventGroup, UEFI_EVENT *Event);
typedef UEFI_STATUS (*UEFI_SET_TIMER)(UEFI_EVENT Event, UEFI_TIMER_DELAY Type, UINT64 TriggerTime);
typedef UEFI_STATUS (*UEFI_WAIT_FOR_EVENT)(UINTN NumberOfEvents, UEFI_EVENT *Event, UINTN *Index);
typedef UEFI_STATUS (*UEFI_SIGNAL_EVENT)(UEFI_EVENT Event);
typedef UEFI_STATUS (*UEFI_CLOSE_EVENT)(UEFI_EVENT Event);
typedef UEFI_STATUS (*UEFI_CHECK_EVENT)(UEFI_EVENT Event);
typedef UEFI_STATUS (*UEFI_INSTALL_PROTOCOL_INTERFACE)(UEFI_HANDLE *Handle, UEFI_GUID *Protocol, UEFI_INTERFACE_TYPE InterfaceType, VOID *Interface);
typedef UEFI_STATUS (*UEFI_REINSTALL_PROTOCOL_INTERFACE)(UEFI_HANDLE Handle, UEFI_GUID *Protocol, VOID *OldInterface, VOID *NewInterface);
typedef UEFI_STATUS (*UEFI_UNINSTALL_PROTOCOL_INTERFACE)(UEFI_HANDLE Handle, UEFI_GUID *Protocol, VOID *Interface);
typedef UEFI_STATUS (*UEFI_HANDLE_PROTOCOL)(UEFI_HANDLE Handle, UEFI_GUID *Protocol, VOID **Interface);
typedef UEFI_STATUS (*UEFI_REGISTER_PROTOCOL_NOTIFY)(UEFI_GUID *Protocol, UEFI_EVENT Event, VOID **Registration);
typedef UEFI_STATUS (*UEFI_LOCATE_HANDLE)(UEFI_LOCATE_SEARCH_TYPE SearchType, UEFI_GUID *Protocol, VOID *SearchKey, UINTN *BufferSize, UEFI_HANDLE *Buffer);
typedef UEFI_STATUS (*UEFI_LOCATE_DEVICE_PATH)(UEFI_GUID *Protocol, UEFI_DEVICE_PATH_PROTOCOL **DevicePath, UEFI_HANDLE *Device);
typedef UEFI_STATUS (*UEFI_INSTALL_CONFIGURATION_TABLE)(UEFI_GUID *Guid, VOID *Table);
typedef UEFI_STATUS (*UEFI_IMAGE_LOAD)(BOOLEAN BootPolicy, UEFI_HANDLE ParentImageHandle, UEFI_DEVICE_PATH_PROTOCOL *DevicePath, VOID *SourceBuffer, UINTN SourceSize, UEFI_HANDLE *ImageHandle);
typedef UEFI_STATUS (*UEFI_IMAGE_START)(UEFI_HANDLE ImageHandle, UINTN *ExitDataSize, CHAR16 **ExitData);
typedef UEFI_STATUS (*UEFI_EXIT)(UEFI_HANDLE ImageHandle, UEFI_STATUS ExitStatus, UINTN ExitDataSize, CHAR16 *ExitData);
typedef UEFI_STATUS (*UEFI_IMAGE_UNLOAD)(UEFI_HANDLE ImageHandle);
typedef UEFI_STATUS (*UEFI_EXIT_BOOT_SERVICES)(UEFI_HANDLE ImageHandle, UINTN MapKey);
typedef UEFI_STATUS (*UEFI_GET_NEXT_MONOTONIC_COUNT)(UINT64 *Count);
typedef UEFI_STATUS (*UEFI_STALL)(UINTN Microseconds);
typedef UEFI_STATUS (*UEFI_SET_WATCHDOG_TIMER)(UINTN Timeout, UINT64 WatchdogCode, UINTN DataSize, CHAR16 *WatchdogData);
typedef UEFI_STATUS (*UEFI_CONNECT_CONTROLLER)(UEFI_HANDLE ControllerHandle, UEFI_HANDLE *DriverImageHandle, UEFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath, BOOLEAN Recursive);
typedef UEFI_STATUS (*UEFI_DISCONNECT_CONTROLLER)(UEFI_HANDLE ControllerHandle, UEFI_HANDLE DriverImageHandle, UEFI_HANDLE ChildHandle);
typedef UEFI_STATUS (*UEFI_OPEN_PROTOCOL)(UEFI_HANDLE Handle, UEFI_GUID *Protocol, void **Interface, UEFI_HANDLE AgentHandle, UEFI_HANDLE ControllerHandle, UINT32 Attributes);
typedef UEFI_STATUS (*UEFI_CLOSE_PROTOCOL)(UEFI_HANDLE Handle, UEFI_GUID *Protocol, UEFI_HANDLE AgentHandle, UEFI_HANDLE ControllerHandle);
typedef UEFI_STATUS (*UEFI_OPEN_PROTOCOL_INFORMATION)(UEFI_HANDLE Handle, UEFI_GUID *Protocol, UEFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer, UINTN *EntryCount);
typedef UEFI_STATUS (*UEFI_PROTOCOLS_PER_HANDLE)(UEFI_HANDLE Handle, UEFI_GUID ***ProtocolBuffer, UINTN *ProtocolBufferCount);
typedef UEFI_STATUS (*UEFI_LOCATE_HANDLE_BUFFER)(UEFI_LOCATE_SEARCH_TYPE SearchType, UEFI_GUID *Protocol, VOID *SearchKey, UINTN *NoHandles, UEFI_HANDLE **Buffer);
typedef UEFI_STATUS (*UEFI_LOCATE_PROTOCOL)(UEFI_GUID *Protocol, VOID *Registration, VOID **Interface);
typedef UEFI_STATUS (*UEFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(UEFI_HANDLE *Handle, ...);
typedef UEFI_STATUS (*UEFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(UEFI_HANDLE *Handle, ...);
typedef UEFI_STATUS (*UEFI_CALCULATE_CRC32)(VOID *Data, UINTN DataSize, UINT32 *Crc32);
typedef UEFI_STATUS (*UEFI_COPY_MEM)(VOID *Destination, VOID *Source, UINTN Length);
typedef UEFI_STATUS (*UEFI_SET_MEM)(VOID *Buffer, UINTN Size, UINT8 Value);
typedef UEFI_STATUS (*UEFI_CREATE_EVENT_EX)(UINT32 Type, UEFI_TPL NotifyTpl, UEFI_EVENT_NOTIFY NotifyFunction, VOID *NotifyContext, UEFI_GUID *EventGroup, UEFI_EVENT *Event);

typedef struct UEFI_BOOT_SERVICES {
    UEFI_TABLE_HEADER                            Hdr;
    UEFI_RAISE_TPL                               RaiseTPL;
    UEFI_RESTORE_TPL                             RestoreTPL;
    UEFI_ALLOCATE_PAGES                          AllocatePages;
    UEFI_FREE_PAGES                              FreePages;
    UEFI_GET_MEMORY_MAP                          GetMemoryMap;
    UEFI_ALLOCATE_POOL                           AllocatePool;
    UEFI_FREE_POOL                               FreePool;
    UEFI_CREATE_EVENT                            CreateEvent;
    UEFI_SET_TIMER                               SetTimer;
    UEFI_WAIT_FOR_EVENT                          WaitForEvent;
    UEFI_SIGNAL_EVENT                            SignalEvent;
    UEFI_CLOSE_EVENT                             CloseEvent;
    UEFI_CHECK_EVENT                             CheckEvent;
    UEFI_INSTALL_PROTOCOL_INTERFACE              InstallProtocolInterface;
    UEFI_REINSTALL_PROTOCOL_INTERFACE            ReinstallProtocolInterface;
    UEFI_UNINSTALL_PROTOCOL_INTERFACE            UninstallProtocolInterface;
    UEFI_HANDLE_PROTOCOL                         HandleProtocol;
    VOID                                        *Reserved;
    UEFI_REGISTER_PROTOCOL_NOTIFY                RegisterProtocolNotify;
    UEFI_LOCATE_HANDLE                           LocateHandle;
    UEFI_LOCATE_DEVICE_PATH                      LocateDevicePath;
    UEFI_INSTALL_CONFIGURATION_TABLE             InstallConfigurationTable;
    UEFI_IMAGE_LOAD                              LoadImage;
    UEFI_IMAGE_START                             StartImage;
    UEFI_EXIT                                    Exit;
    UEFI_IMAGE_UNLOAD                            UnloadImage;
    UEFI_EXIT_BOOT_SERVICES                      ExitBootServices;
    UEFI_GET_NEXT_MONOTONIC_COUNT                GetNextMonotonicCount;
    UEFI_STALL                                   Stall;
    UEFI_SET_WATCHDOG_TIMER                      SetWatchdogTimer;
    UEFI_CONNECT_CONTROLLER                      ConnectController;
    UEFI_DISCONNECT_CONTROLLER                   DisconnectController;
    UEFI_OPEN_PROTOCOL                           OpenProtocol;
    UEFI_CLOSE_PROTOCOL                          CloseProtocol;
    UEFI_OPEN_PROTOCOL_INFORMATION               OpenProtocolInformation;
    UEFI_PROTOCOLS_PER_HANDLE                    ProtocolsPerHandle;
    UEFI_LOCATE_HANDLE_BUFFER                    LocateHandleBuffer;
    UEFI_LOCATE_PROTOCOL                         LocateProtocol;
    UEFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES    InstallMultipleProtocolInterfaces;
    UEFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES  UninstallMultipleProtocolInterfaces;
    UEFI_CALCULATE_CRC32                         CalculateCrc32;
    UEFI_COPY_MEM                                CopyMem;
    UEFI_SET_MEM                                 SetMem;
    UEFI_CREATE_EVENT_EX                         CreateEventEx;
} UEFI_BOOT_SERVICES;

/*
 * efi-ct.h
 *
 * UEFI Configuration Table.
 */


typedef struct UEFI_CONFIGURATION_TABLE {
    UEFI_GUID    VendorGuid;
    VOID        *VendorTable;
} UEFI_CONFIGURATION_TABLE;

/*
 * efi-acpitp.h
 *
 * UEFI ACPI table protocol.
 */


struct UEFI_ACPI_TABLE_PROTOCOL;

typedef UEFI_STATUS (*UEFI_ACPI_TABLE_INSTALL_ACPI_TABLE)(struct UEFI_ACPI_TABLE_PROTOCOL *This, VOID *AcpiTableBuffer, UINTN AcpiTableBufferSize, UINTN *TableKey);
typedef UEFI_STATUS (*UEFI_ACPI_TABLE_UNINSTALL_ACPI_TABLE)(struct UEFI_ACPI_TABLE_PROTOCOL *This, UINTN  TableKey);

typedef struct UEFI_ACPI_TABLE_PROTOCOL {
    UEFI_ACPI_TABLE_INSTALL_ACPI_TABLE   InstallAcpiTable;
    UEFI_ACPI_TABLE_UNINSTALL_ACPI_TABLE UninstallAcpiTable;
} UEFI_ACPI_TABLE_PROTOCOL;

/*
 * efi-fp.h
 *
 * UEFI File Protocol.
 */

#define UEFI_FILE_INFO_ID                {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define UEFI_FILE_SYSTEM_INFO_ID         {0x09576e93, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}} 
#define UEFI_FILE_SYSTEM_VOLUME_LABEL_ID {0xdb47d7d3, 0xfe81, 0x11d3, {0x9a, 0x35, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}

#define UEFI_FILE_PROTOCOL_REVISION          0x00010000
#define UEFI_FILE_PROTOCOL_REVISION2         0x00020000
#define UEFI_FILE_PROTOCOL_LATEST_REVISION   UEFI_FILE_PROTOCOL_REVISION2

#define UEFI_FILE_MODE_READ      0x0000000000000001
#define UEFI_FILE_MODE_WRITE     0x0000000000000002
#define UEFI_FILE_MODE_CREATE    0x8000000000000000

#define UEFI_FILE_READ_ONLY  0x0000000000000001
#define UEFI_FILE_HIDDEN     0x0000000000000002
#define UEFI_FILE_SYSTEM     0x0000000000000004
#define UEFI_FILE_RESERVED   0x0000000000000008
#define UEFI_FILE_DIRECTORY  0x0000000000000010
#define UEFI_FILE_ARCHIVE    0x0000000000000020
#define UEFI_FILE_VALID_ATTR 0x0000000000000037

typedef struct UEFI_FILE_INFO {
    UINT64      Size;
    UINT64      FileSize;
    UINT64      PhysicalSize;
    UEFI_TIME    CreateTime;
    UEFI_TIME    LastAccessTime;
    UEFI_TIME    ModificationTime;
    UINT64      Attribute;
    //CHAR16      FileName[];
    CHAR16      *FileName;
} UEFI_FILE_INFO;

typedef struct UEFI_FILE_SYSTEM_INFO {
    UINT64  Size;
    BOOLEAN ReadOnly;
    UINT64  VolumeSize;
    UINT64  FreeSpace;
    UINT32  BlockSize;
    CHAR16  VolumeLabel;
} UEFI_FILE_SYSTEM_INFO;

typedef struct UEFI_FILE_SYSTEM_VOLUME_LABEL {
    CHAR16 *VolumeLabel;
} UEFI_FILE_SYSTEM_VOLUME_LABEL;

typedef struct UEFI_FILE_IO_TOKEN {
    UEFI_EVENT   Event;
    UEFI_STATUS  Status;
    UINTN       BufferSize;
    VOID        *Buffer;
} UEFI_FILE_IO_TOKEN;

struct UEFI_FILE_PROTOCOL;

typedef UEFI_STATUS (*UEFI_FILE_OPEN)(struct UEFI_FILE_PROTOCOL *This, struct UEFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes);
typedef UEFI_STATUS (*UEFI_FILE_CLOSE)(struct UEFI_FILE_PROTOCOL *This);
typedef UEFI_STATUS (*UEFI_FILE_DELETE)(struct UEFI_FILE_PROTOCOL *This);
typedef UEFI_STATUS (*UEFI_FILE_READ)(struct UEFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);
typedef UEFI_STATUS (*UEFI_FILE_WRITE)(struct UEFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);
typedef UEFI_STATUS (*UEFI_FILE_GET_POSITION)(struct UEFI_FILE_PROTOCOL *This, UINT64 *Position);
typedef UEFI_STATUS (*UEFI_FILE_SET_POSITION)(struct UEFI_FILE_PROTOCOL *This, UINT64 Position);
typedef UEFI_STATUS (*UEFI_FILE_GET_INFO)(struct UEFI_FILE_PROTOCOL *This, UEFI_GUID *InformationType, UINTN *BufferSize, VOID *Buffer);
typedef UEFI_STATUS (*UEFI_FILE_SET_INFO)(struct UEFI_FILE_PROTOCOL *This, UEFI_GUID *InformationType, UINTN BufferSize, VOID *Buffer);
typedef UEFI_STATUS (*UEFI_FILE_FLUSH)(struct UEFI_FILE_PROTOCOL *This);
typedef UEFI_STATUS (*UEFI_FILE_OPEN_EX)(struct UEFI_FILE_PROTOCOL *This, struct UEFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes, UEFI_FILE_IO_TOKEN *Token);
typedef UEFI_STATUS (*UEFI_FILE_READ_EX)(struct UEFI_FILE_PROTOCOL *This, UEFI_FILE_IO_TOKEN *Token);
typedef UEFI_STATUS (*UEFI_FILE_WRITE_EX)(struct UEFI_FILE_PROTOCOL *This, UEFI_FILE_IO_TOKEN *Token);
typedef UEFI_STATUS (*UEFI_FILE_FLUSH_EX)(struct UEFI_FILE_PROTOCOL *This, UEFI_FILE_IO_TOKEN *Token);

typedef struct UEFI_FILE_PROTOCOL {
    UINT64                  Revision;
    UEFI_FILE_OPEN           Open;
    UEFI_FILE_CLOSE          Close;
    UEFI_FILE_DELETE         Delete;
    UEFI_FILE_READ           Read;
    UEFI_FILE_WRITE          Write;
    UEFI_FILE_GET_POSITION   GetPosition;
    UEFI_FILE_SET_POSITION   SetPosition;
    UEFI_FILE_GET_INFO       GetInfo;
    UEFI_FILE_SET_INFO       SetInfo;
    UEFI_FILE_FLUSH          Flush;
    UEFI_FILE_OPEN_EX        OpenEx;
    UEFI_FILE_READ_EX        ReadEx;
    UEFI_FILE_WRITE_EX       WriteEx;
    UEFI_FILE_FLUSH_EX       FlushEx;
} UEFI_FILE_PROTOCOL;

/*
 * efi-gop.h
 *
 * UEFI graphics output protocol.
 */


typedef struct UEFI_PIXEL_BITMASK {
    UINT32  RedMask;
    UINT32  GreenMask;
    UINT32  BlueMask;
    UINT32  ReservedMask;
} UEFI_PIXEL_BITMASK;

typedef enum UEFI_GRAPHICS_PIXEL_FORMAT {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} UEFI_GRAPHICS_PIXEL_FORMAT;

typedef struct UEFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
    UINT32                      Version;
    UINT32                      HorizontalResolution;
    UINT32                      VerticalResolution;
    UEFI_GRAPHICS_PIXEL_FORMAT   PixelFormat;
    UEFI_PIXEL_BITMASK           PixelInformation;
    UINT32                      PixelsPerScanLine;
} UEFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct UEFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
    UINT32                                  MaxMode;
    UINT32                                  Mode;
    UEFI_GRAPHICS_OUTPUT_MODE_INFORMATION    *Info;
    UINTN                                   SizeOfInfo;
    UEFI_PHYSICAL_ADDRESS                    FrameBufferBase;
    UINTN                                   FrameBufferSize;
} UEFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct UEFI_GRAPHICS_OUTPUT_BLT_PIXEL {
    UINT8   BLUE;
    UINT8   GREEN;
    UINT8   RED;
    UINT8   Reserved;
} UEFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef enum UEFI_GRAPHICS_OUTPUT_BLT_OPERATION {
    EfiBltVideoFill,
    EfiBltVideoToBltBuffer,
    EfiBltBufferToVideo,
    EfiBltVideoToVideo,
    EfiGraphicsOutputBltOperationMax
} UEFI_GRAPHICS_OUTPUT_BLT_OPERATION;

struct UEFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef UEFI_STATUS (*UEFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE)(struct UEFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber, UINTN *SizeOfInfo, UEFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);
typedef UEFI_STATUS (*UEFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE)(struct UEFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber);
typedef UEFI_STATUS (*UEFI_GRAPHICS_OUTPUT_PROTOCOL_BLT)(struct UEFI_GRAPHICS_OUTPUT_PROTOCOL *This, UEFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, UEFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation, UINTN SourceX, UINTN SourceY, UINTN DestinationX, UINTN DestinationY, UINTN Width, UINTN Height, UINTN Delta);

typedef struct UEFI_GRAPHICS_OUTPUT_PROTOCOL {
    UEFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
    UEFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE   SetMode;
    UEFI_GRAPHICS_OUTPUT_PROTOCOL_BLT        Blt;
    UEFI_GRAPHICS_OUTPUT_PROTOCOL_MODE       *Mode;
} UEFI_GRAPHICS_OUTPUT_PROTOCOL;

/*
 * efi-lidpp.h
 * 
 * Loaded Image Device Path Protocol.
 */


/*
 * efi-lip.h
 * 
 * UEFI Loaded Image Protocol.
 */


#define UEFI_LOADED_IMAGE_PROTOCOL_REVISION  0x1000

typedef UEFI_STATUS (*UEFI_IMAGE_UNLOAD)(UEFI_HANDLE ImageHandle);

typedef struct UEFI_LOADED_IMAGE_PROTOCOL {
    UINT32                    Revision;
    UEFI_HANDLE               ParentHandle;
    struct UEFI_SYSTEM_TABLE  *SystemTable;
    UEFI_HANDLE               DeviceHandle;
    UEFI_DEVICE_PATH_PROTOCOL *FilePath;
    VOID                      *Reserved;
    UINT32                    LoadOptionsSize;
    VOID                      *LoadOptions;
    VOID                      *ImageBase;
    UINT64                    ImageSize;
    UINTN                     ImageCodeType;
    UEFI_IMAGE_UNLOAD         UnLoad;
} UEFI_LOADED_IMAGE_PROTOCOL;

/*
 * efi-sfsp.h
 *
 * UEFI Simple File System Protocol.
 */

#define UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION    0x00010000


struct UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef UEFI_STATUS (*UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(struct UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,  UEFI_FILE_PROTOCOL **Root);

typedef struct UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    UINT64                                       Revision;
    UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME OpenVolume;
} UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

/*******************************************************************************
 * efi-vmpp.h
 *
 * UEFI VGA Mini Port Protocol.
 */

/*
struct UEFI_VGA_MINI_PORT_PROTOCOL;

typedef UEFI_STATUS (*UEFI_VGA_MINI_PORT_SET_MODE)(UEFI_VGA_MINI_PORT_PROTOCOL *This, UINTN ModeNumber);

typedef struct UEFI_VGA_MINI_PORT_PROTOCOL {
    UEFI_VGA_MINI_PORT_SET_MODE SetMode;
    UINT64                      VgaMemoryOffset;
    UINT64                      CrtcAddressRegisterOffset;
    UINT64                      CrtcDataRegisterOffset;
    UINT8                       VgaMemoryBar;
    UINT8                       CrtcAddressRegisterBar;
    UINT8                       CrtcDataRegisterBar;
    UINT8                       MaxMode;
} UEFI_VGA_MINI_PORT_PROTOCOL;
*/

/*
 * efi-st.h
 *
 * The UEFI System Table.
 */

typedef struct UEFI_SYSTEM_TABLE {
    UEFI_TABLE_HEADER                   Hdr;
    CHAR16                              *FirmwareVendor;
    UINT32                              FirmwareRevision;
    UEFI_HANDLE                         ConsoleInHandle;
    UEFI_SIMPLE_TEXT_INPUT_PROTOCOL     *ConIn;
    UEFI_HANDLE                         ConsoleOutHandle;
    UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *ConOut;
    UEFI_HANDLE                         StandardErrorHandle;
    UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *StdErr;
    UEFI_RUNTIME_SERVICES               *RuntimeServices;
    UEFI_BOOT_SERVICES                  *BootServices;
    UINTN                               NumberOfTableEntries;
    UEFI_CONFIGURATION_TABLE            *ConfigurationTable;
} UEFI_SYSTEM_TABLE;

UEFI_SYSTEM_TABLE* uefi_system_table;

void uefi_init(void *ih, UEFI_SYSTEM_TABLE *st);

#endif // __BOOT_UEFI_H
