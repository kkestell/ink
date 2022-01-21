#ifndef _EFI_H
#define _EFI_H

// ============================================================================
// Basic types and constants
// ============================================================================

#include <stddef.h>
#include <stdint.h>

// EFIABI can be defined to __attribute__((ms_abi)) for example
#ifndef EFIABI
#define EFIABI
#endif

#define EFI_FALSE 0
#define EFI_TRUE  1

#define EFI_SUCCESS                 0x0000000000000000
#define EFI_ERR                     0x8000000000000000
#define EFI_LOAD_ERROR              (EFI_ERR | 0x0000000000000001)
#define EFI_INVALID_PARAMETER       (EFI_ERR | 0x0000000000000002)
#define EFI_UNSUPPORTED             (EFI_ERR | 0x0000000000000003)
#define EFI_BAD_BUFFER_SIZE         (EFI_ERR | 0x0000000000000004)
#define EFI_BUFFER_TOO_SMALL        (EFI_ERR | 0x0000000000000005)
#define EFI_NOT_READY               (EFI_ERR | 0x0000000000000006)
#define EFI_DEVICE_ERROR            (EFI_ERR | 0x0000000000000007)
#define EFI_WRITE_PROTECTED         (EFI_ERR | 0x0000000000000008)
#define EFI_OUT_OF_RESOURCES        (EFI_ERR | 0x0000000000000009)
#define EFI_VOLUME_CORRUPTED        (EFI_ERR | 0x000000000000000a)
#define EFI_VOLUME_FULL             (EFI_ERR | 0x000000000000000b)
#define EFI_NO_MEDIA                (EFI_ERR | 0x000000000000000c)
#define EFI_MEDIA_CHANGED           (EFI_ERR | 0x000000000000000d)
#define EFI_NOT_FOUND               (EFI_ERR | 0x000000000000000e)
#define EFI_ACCESS_DENIED           (EFI_ERR | 0x000000000000000f)
#define EFI_NO_RESPONSE             (EFI_ERR | 0x0000000000000010)
#define EFI_NO_MAPPING              (EFI_ERR | 0x0000000000000011)
#define EFI_TIMEOUT                 (EFI_ERR | 0x0000000000000012)
#define EFI_NOT_STARTED             (EFI_ERR | 0x0000000000000013)
#define EFI_ALREADY_STARTED         (EFI_ERR | 0x0000000000000014)
#define EFI_ABORTED                 (EFI_ERR | 0x0000000000000015)
#define EFI_ICMP_ERROR              (EFI_ERR | 0x0000000000000016)
#define EFI_TFTP_ERROR              (EFI_ERR | 0x0000000000000017)
#define EFI_PROTOCOL_ERROR          (EFI_ERR | 0x0000000000000018)
#define EFI_INCOMPATIBLE_VERSION    (EFI_ERR | 0x0000000000000019)
#define EFI_SECURITY_VIOLATION      (EFI_ERR | 0x000000000000001a)
#define EFI_CRC_ERROR               (EFI_ERR | 0x000000000000001b)
#define EFI_END_OF_MEDIA            (EFI_ERR | 0x000000000000001c)
#define EFI_END_OF_FILE             (EFI_ERR | 0x000000000000001f)
#define EFI_INVALID_LANGUAGE        (EFI_ERR | 0x0000000000000020)
#define EFI_COMPROMISED_DATA        (EFI_ERR | 0x0000000000000021)
#define EFI_IP_ADDRESS_CONFLICT     (EFI_ERR | 0x0000000000000022)
#define EFI_HTTP_ERROR              (EFI_ERR | 0x0000000000000023)

#define EFI_MEMORY_UC               0x0000000000000001
#define EFI_MEMORY_WC               0x0000000000000002
#define EFI_MEMORY_WT               0x0000000000000004
#define EFI_MEMORY_WB               0x0000000000000008
#define EFI_MEMORY_UCE              0x0000000000000010
#define EFI_MEMORY_WP               0x0000000000001000
#define EFI_MEMORY_RP               0x0000000000002000
#define EFI_MEMORY_XP               0x0000000000004000
#define EFI_MEMORY_NV               0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE    0x0000000000010000
#define EFI_MEMORY_RO               0x0000000000020000
#define EFI_MEMORY_RUNTIME          0x8000000000000000

typedef uint8_t  BOOLEAN;
typedef uint16_t CHAR16;
typedef void     *EFI_EVENT;
typedef void     *EFI_HANDLE;
typedef uint64_t EFI_LBA;
typedef uint64_t EFI_PHYSICAL_ADDRESS;
typedef uint64_t EFI_STATUS;
typedef uint64_t EFI_TL;
typedef uint64_t EFI_TPL;
typedef uint64_t EFI_VIRTUAL_ADDRESS;
typedef int8_t   INT8;
typedef int16_t  INT16;
typedef int32_t  INT32;
typedef int64_t  INT64;
typedef int64_t  INTN;
typedef uint8_t  UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef uint64_t UINTN;
typedef void     VOID;

typedef struct EFI_GUID
{
    UINT32  Data1;
    UINT16  Data2;
    UINT16  Data3;
    UINT8   Data4[8];
} EFI_GUID;

typedef struct EFI_MEMORY_DESCRIPTOR
{
    UINT32                 Type;
    EFI_PHYSICAL_ADDRESS  PhysicalStart;
    EFI_VIRTUAL_ADDRESS   VirtualStart;
    UINT64                 NumberOfPages;
    UINT64                 Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef struct EFI_TABLE_HEADER
{
    UINT64  Signature;
    UINT32  Revision;
    UINT32  HeaderSize;
    UINT32  CRC32;
    UINT32  Reserved;
} EFI_TABLE_HEADER;

struct EFI_SYSTEM_TABLE;

// =============================================================================
// Error handling
// =============================================================================

#define EFI_ERROR(status) (((INTN)(UINTN)(status)) < 0)

const wchar_t* efi_error_message(EFI_STATUS const status);

// =============================================================================
// Guids
// =============================================================================

// Protocols

#define EFI_ACPI_TABLE_PROTOCOL_GUID               {0xffe06bdd,  0x6107, 0x46a6, {0x7b, 0xb2, 0x5a, 0x9c, 0x7e, 0xc5, 0x28, 0x5c}}
#define EFI_DEVICE_PATH_PROTOCOL_GUID              {0x09576e91,  0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID          {0x9042a9de,  0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}}
#define EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID {0xbc62157e,  0x3e33, 0x4fec, {0x99, 0x20, 0x2d, 0x3b, 0x36, 0xd7, 0x50, 0xdf}}
#define EFI_LOADED_IMAGE_PROTOCOL_GUID             {0x5b1b31a1,  0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID       {0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID        {0x387477c1,  0x69c7, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID       {0x387477c2,  0x69c7, 0x11d2, {0x82, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_VGA_MINI_PORT_PROTOCOL_GUID            {0xc7735a2f,  0x88f5, 0x4882, {0xae, 0x63, 0xfa, 0xac, 0x8c, 0x8b, 0x86, 0xb3}}

#define MPS_TABLE_GUID                              {0xeb9d2d2f, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define SAL_SYSTEM_TABLE_GUID                       {0xeb9d2d32, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define SMBIOS_TABLE_GUID                           {0xeb9d2d31, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define SMBIOS2_TABLE_GUID                          {0xf2fd1544, 0x9794, 0x4a2c, {0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94}}

#define ACPI_TABLE_GUID                             {0xeb9d2d30, 0x2d88, 0x11d3, {0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}
#define EFO_ACPI_10_TABLE_GUID                      ACPI_TABLE_GUID

#define EFI_ACPI_TABLE_GUID                        {0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}}
#define EFI_ACPI_20_TABLE_GUID                     EFI_ACPI_TABLE_GUID

// =============================================================================
// Macros
// =============================================================================

#define EFI_PAGE_MASK  0xFFF 
#define EFI_PAGE_SHIFT 12

#define EFI_SIZE_TO_PAGES(size) (((size) >> EFI_PAGE_SHIFT) + (((size) & EFI_PAGE_MASK) ? 1 : 0))

#define EFI_BS_CALL(call) {                                                   \
    EFI_STATUS status = efi_system_table->BootServices->call;                 \
    if (EFI_ERROR(status))                                                    \
    {                                                                         \
        kprintf(L"%s\r\n", efi_error_message(status));                        \
        return status;                                                        \
    }                                                                         \
}

// =============================================================================
// Simple Text Input Protocol
// =============================================================================

typedef struct EFI_INPUT_KEY
{
    UINT16  ScanCode;
    UINT16  UnicodeChar;
} EFI_INPUT_KEY;

struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef EFI_STATUS (*EFI_INPUT_RESET)(struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, BOOLEAN ExtendedVerification);
typedef EFI_STATUS (*EFI_INPUT_READ_KEY)(struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, EFI_INPUT_KEY *Key);

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL
{
    EFI_INPUT_RESET     Reset;
    EFI_INPUT_READ_KEY  ReadKeyStroke;
    EFI_EVENT           WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// =============================================================================
// Simple Text Output Protocol
// =============================================================================

typedef struct SIMPLE_TEXT_OUTPUT_MODE
{
    INT32   MaxMode;
    INT32   Mode;
    INT32   Attribute;
    INT32   CursorColumn;
    INT32   CursorRow;
    BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (*EFI_TEXT_RESET)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN ExtendedVerification);
typedef EFI_STATUS (*EFI_TEXT_STRING)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
typedef EFI_STATUS (*EFI_TEXT_TEST_STRING)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
typedef EFI_STATUS (*EFI_TEXT_QUERY_MODE)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber, UINTN *Columns, UINTN *Rows);
typedef EFI_STATUS (*EFI_TEXT_SET_MODE)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber);
typedef EFI_STATUS (*EFI_TEXT_SET_ATTRIBUTE)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Attribute);
typedef EFI_STATUS (*EFI_TEXT_CLEAR_SCREEN)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
typedef EFI_STATUS (*EFI_TEXT_SET_CURSOR_POSITION)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Column, UINTN Row);
typedef EFI_STATUS (*EFI_TEXT_ENABLE_CURSOR)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, BOOLEAN Visible);

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
{
    EFI_TEXT_RESET                  Reset;
    EFI_TEXT_STRING                 OutputString;
    EFI_TEXT_TEST_STRING            TestString;
    EFI_TEXT_QUERY_MODE             QueryMode;
    EFI_TEXT_SET_MODE               SetMode;
    EFI_TEXT_SET_ATTRIBUTE          SetAttribute;
    EFI_TEXT_CLEAR_SCREEN           ClearScreen;
    EFI_TEXT_SET_CURSOR_POSITION    SetCursorPosition;
    EFI_TEXT_ENABLE_CURSOR          EnableCursor;
    SIMPLE_TEXT_OUTPUT_MODE         *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// =============================================================================
// Time
// =============================================================================

#define EFI_TIME_ADJUST_DAYLIGHT    0x01
#define EFI_TIME_IN_DAYLIGHT        0x02

#define EFI_UNSPECIFIED_TIMEZONE    0x07ff

typedef struct EFI_TIME
{
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
} EFI_TIME;

typedef struct EFI_TIME_CAPABILITIES
{
    UINT32  Resolution;
    UINT32  Accuracy;
    BOOLEAN SetsToZero;
} EFI_TIME_CAPABILITIES;

// =============================================================================
// Runtime Services
// =============================================================================

#define EFI_OPTIONAL_POINTER               0x00000001

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET  0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET        0x00040000

typedef enum EFI_RESET_TYPE
{
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef struct EFI_CAPSULE_BLOCK_DESCRIPTOR
{
    UINT64 Length;
    union
    {
        EFI_PHYSICAL_ADDRESS    DataBlock;
        EFI_PHYSICAL_ADDRESS    ContinuationPointer;
    } Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct EFI_CAPSULE_HEADER
{
    EFI_GUID CapsuleGuid;
    UINT32    HeaderSize;
    UINT32    Flags;
    UINT32    CapsuleImageSize;
} EFI_CAPSULE_HEADER;

typedef EFI_STATUS (EFIABI *EFI_GET_TIME)(EFI_TIME *Time, EFI_TIME_CAPABILITIES *Capabilities);
typedef EFI_STATUS (EFIABI *EFI_SET_TIME)(EFI_TIME *Time);
typedef EFI_STATUS (EFIABI *EFI_GET_WAKEUP_TIME)(BOOLEAN *Enabled, BOOLEAN *Pending, EFI_TIME *Time);
typedef EFI_STATUS (EFIABI *EFI_SET_WAKEUP_TIME)(BOOLEAN Enable, EFI_TIME *Time);
typedef EFI_STATUS (EFIABI *EFI_SET_VIRTUAL_ADDRESS_MAP)(UINTN MemoryMapSize, UINTN DescriptorSize, UINT32 DescriptorVersion, EFI_MEMORY_DESCRIPTOR *VirtualMap);
typedef EFI_STATUS (EFIABI *EFI_CONVERT_POINTER)(UINTN DebugDisposition, VOID **Address);
typedef EFI_STATUS (EFIABI *EFI_GET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 *Attributes, UINTN *DataSize, VOID *Data);
typedef EFI_STATUS (EFIABI *EFI_GET_NEXT_VARIABLE_NAME)(UINTN *VariableNameSize, CHAR16 *VariableName, EFI_GUID *VendorGuid);
typedef EFI_STATUS (EFIABI *EFI_SET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 Attributes, UINTN DataSize, VOID *Data);
typedef EFI_STATUS (EFIABI *EFI_GET_NEXT_HIGH_MONO_COUNT)(UINT32 *HighCount);
typedef EFI_STATUS (EFIABI *EFI_RESET_SYSTEM)(EFI_RESET_TYPE ResetType, EFI_STATUS ResetStatus, UINTN DataSize, VOID *ResetData);
typedef EFI_STATUS (EFIABI *EFI_UPDATE_CAPSULE)(EFI_CAPSULE_HEADER **CapsuleHeaderArray, UINTN CapsuleCount, EFI_PHYSICAL_ADDRESS ScatterGatherList);
typedef EFI_STATUS (EFIABI *EFI_QUERY_CAPSULE_CAPABILITIES)(EFI_CAPSULE_HEADER **CapsuleHeaderArray, UINTN CapsuleCount, UINT64 *MaximumCapsuleSize, EFI_RESET_TYPE *ResetType);
typedef EFI_STATUS (EFIABI *EFI_QUERY_VARIABLE_INFO)(UINT32 Attributes, UINT64 *MaximumVariableStorageSize, UINT64 *RemainingVariableStorageSize, UINT64 *MaximumVariableSize);

typedef struct EFI_RUNTIME_SERVICES
{
    EFI_TABLE_HEADER               Hdr;
    EFI_GET_TIME                   GetTime;
    EFI_SET_TIME                   SetTime;
    EFI_GET_WAKEUP_TIME            GetWakeupTime;
    EFI_SET_WAKEUP_TIME            SetWakeupTime;
    EFI_SET_VIRTUAL_ADDRESS_MAP    SetVirtualAddressMap;
    EFI_CONVERT_POINTER            ConvertPointer;
    EFI_GET_VARIABLE               GetVariable;
    EFI_GET_NEXT_VARIABLE_NAME     GetNextVariableName;
    EFI_SET_VARIABLE               SetVariable;
    EFI_GET_NEXT_HIGH_MONO_COUNT   GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM               ResetSystem;
    EFI_UPDATE_CAPSULE             UpdateCapsule;
    EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
    EFI_QUERY_VARIABLE_INFO        QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

// =============================================================================
// Device Path Protocol
// =============================================================================

#define EFI_HARDWARE_DEVICE_PATH                0x01
#define EFI_ACPI_DEVICE_PATH                    0x02
#define EFI_MESSAGING_DEVICE_PATH               0x03
#define EFI_MEDIA_DEVICE_PATH                   0x04
#define EFI_BIOS_BOOT_SPECIFICATION_DEVICE_PATH 0x05
#define EFI_END_OF_HARDWARE_DEVICE_PATH         0x7f

typedef struct EFI_DEVICE_PATH_PROTOCOL
{
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

// =============================================================================
// Boot Services
// =============================================================================

#define EVT_TIMER                                0x80000000
#define EVT_RUNTIME                              0x40000000
#define EVT_NOTIFY_WAIT                          0x00000100
#define EVT_NOTIFY_SIGNAL                        0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES            0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE        0x00000202

#define TPL_APPLICATION                          4
#define TPL_CALLBACK                             8
#define TPL_NOTIFY                               16
#define TPL_HIGH_LEVEL                           31

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL    0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL          0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL         0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER   0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER             0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE             0x00000020

#define UefiReservedMemoryType                   0x00000000

#define UefiLoaderCode                           0x00000001
#define UefiLoaderData                           0x00000002

#define UefiBootServicesCode                     0x00000003
#define UefiBootServicesData                     0x00000004
#define UefiRuntimeServicesCode                  0x00000005
#define UefiRuntimeServicesData                  0x00000006
#define UefiConventionalMemory                   0x00000007
#define UefiUnusableMemory                       0x00000008
#define UefiACPIReclaimMemory                    0x00000009
#define UefiACPIMemoryNVS                        0x0000000a
#define UefiMemoryMappedIO                       0x0000000b
#define UefiMemoryMappedIOPortSpace              0x0000000c
#define UefiPalCode                              0x0000000d
#define UefiPersistentMemory                     0x0000000e
#define UefiMaxMemoryType                        0x0000000e

typedef VOID (*EFI_EVENT_NOTIFY)(EFI_EVENT Event, VOID *Context);

typedef enum EFI_TIMER_DELAY
{
    TimerCancel,
    TimerPeriodic,
    TimerRelative
} EFI_TIMER_DELAY;

typedef enum EFI_ALLOCATE_TYPE
{
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum EFI_INTERFACE_TYPE
{
    EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef enum EFI_LOCATE_SEARCH_TYPE
{
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef struct EFI_OPEN_PROTOCOL_INFORMATION_ENTRY
{
    EFI_HANDLE  AgentHandle;
    EFI_HANDLE  ControllerHandle;
    UINT32      Attributes;
    UINT32      OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef EFI_STATUS (*EFI_RAISE_TPL)(EFI_TPL NewTpl);
typedef EFI_STATUS (*EFI_RESTORE_TPL)(EFI_TPL OldTpl);
typedef EFI_STATUS (*EFI_ALLOCATE_PAGES)(EFI_ALLOCATE_TYPE Type, UINTN MemoryType, UINTN Pages, EFI_PHYSICAL_ADDRESS *Memory);
typedef EFI_STATUS (*EFI_FREE_PAGES)(EFI_PHYSICAL_ADDRESS Memory, UINTN Pages);
typedef EFI_STATUS (*EFI_GET_MEMORY_MAP)(UINTN *MemoryMapSize, EFI_MEMORY_DESCRIPTOR *MemoryMap, UINTN *MapKey, UINTN *DescriptorSize, UINT32 *DescriptorVersion);
typedef EFI_STATUS (*EFI_ALLOCATE_POOL)(UINTN PoolType, UINTN Size, VOID **Buffer);
typedef EFI_STATUS (*EFI_FREE_POOL)(VOID *Buffer);
typedef EFI_STATUS (*EFI_CREATE_EVENT)(UINT32 Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY NotifyFunction, VOID *NotifyContext, EFI_GUID *EventGroup, EFI_EVENT *Event);
typedef EFI_STATUS (*EFI_SET_TIMER)(EFI_EVENT Event, EFI_TIMER_DELAY Type, UINT64 TriggerTime);
typedef EFI_STATUS (*EFI_WAIT_FOR_EVENT)(UINTN NumberOfEvents, EFI_EVENT *Event, UINTN *Index);
typedef EFI_STATUS (*EFI_SIGNAL_EVENT)(EFI_EVENT Event);
typedef EFI_STATUS (*EFI_CLOSE_EVENT)(EFI_EVENT Event);
typedef EFI_STATUS (*EFI_CHECK_EVENT)(EFI_EVENT Event);
typedef EFI_STATUS (*EFI_INSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE *Handle, EFI_GUID *Protocol, EFI_INTERFACE_TYPE InterfaceType, VOID *Interface);
typedef EFI_STATUS (*EFI_REINSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID *OldInterface, VOID *NewInterface);
typedef EFI_STATUS (*EFI_UNINSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID *Interface);
typedef EFI_STATUS (*EFI_HANDLE_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID **Interface);
typedef EFI_STATUS (*EFI_REGISTER_PROTOCOL_NOTIFY)(EFI_GUID *Protocol, EFI_EVENT Event, VOID **Registration);
typedef EFI_STATUS (*EFI_LOCATE_HANDLE)(EFI_LOCATE_SEARCH_TYPE SearchType, EFI_GUID *Protocol, VOID *SearchKey, UINTN *BufferSize, EFI_HANDLE *Buffer);
typedef EFI_STATUS (*EFI_LOCATE_DEVICE_PATH)(EFI_GUID *Protocol, EFI_DEVICE_PATH_PROTOCOL **DevicePath, EFI_HANDLE *Device);
typedef EFI_STATUS (*EFI_INSTALL_CONFIGURATION_TABLE)(EFI_GUID *Guid, VOID *Table);
typedef EFI_STATUS (*EFI_IMAGE_LOAD)(BOOLEAN BootPolicy, EFI_HANDLE ParentImageHandle, EFI_DEVICE_PATH_PROTOCOL *DevicePath, VOID *SourceBuffer, UINTN SourceSize, EFI_HANDLE *ImageHandle);
typedef EFI_STATUS (*EFI_IMAGE_START)(EFI_HANDLE ImageHandle, UINTN *ExitDataSize, CHAR16 **ExitData);
typedef EFI_STATUS (*EFI_EXIT)(EFI_HANDLE ImageHandle, EFI_STATUS ExitStatus, UINTN ExitDataSize, CHAR16 *ExitData);
typedef EFI_STATUS (*EFI_IMAGE_UNLOAD)(EFI_HANDLE ImageHandle);
typedef EFI_STATUS (*EFI_EXIT_BOOT_SERVICES)(EFI_HANDLE ImageHandle, UINTN MapKey);
typedef EFI_STATUS (*EFI_GET_NEXT_MONOTONIC_COUNT)(UINT64 *Count);
typedef EFI_STATUS (*EFI_STALL)(UINTN Microseconds);
typedef EFI_STATUS (*EFI_SET_WATCHDOG_TIMER)(UINTN Timeout, UINT64 WatchdogCode, UINTN DataSize, CHAR16 *WatchdogData);
typedef EFI_STATUS (*EFI_CONNECT_CONTROLLER)(EFI_HANDLE ControllerHandle, EFI_HANDLE *DriverImageHandle, EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath, BOOLEAN Recursive);
typedef EFI_STATUS (*EFI_DISCONNECT_CONTROLLER)(EFI_HANDLE ControllerHandle, EFI_HANDLE DriverImageHandle, EFI_HANDLE ChildHandle);
typedef EFI_STATUS (*EFI_OPEN_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, void **Interface, EFI_HANDLE AgentHandle, EFI_HANDLE ControllerHandle, UINT32 Attributes);
typedef EFI_STATUS (*EFI_CLOSE_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, EFI_HANDLE AgentHandle, EFI_HANDLE ControllerHandle);
typedef EFI_STATUS (*EFI_OPEN_PROTOCOL_INFORMATION)(EFI_HANDLE Handle, EFI_GUID *Protocol, EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer, UINTN *EntryCount);
typedef EFI_STATUS (*EFI_PROTOCOLS_PER_HANDLE)(EFI_HANDLE Handle, EFI_GUID ***ProtocolBuffer, UINTN *ProtocolBufferCount);
typedef EFI_STATUS (*EFI_LOCATE_HANDLE_BUFFER)(EFI_LOCATE_SEARCH_TYPE SearchType, EFI_GUID *Protocol, VOID *SearchKey, UINTN *NoHandles, EFI_HANDLE **Buffer);
typedef EFI_STATUS (*EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol, VOID *Registration, VOID **Interface);
typedef EFI_STATUS (*EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(EFI_HANDLE *Handle, ...);
typedef EFI_STATUS (*EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(EFI_HANDLE *Handle, ...);
typedef EFI_STATUS (*EFI_CALCULATE_CRC32)(VOID *Data, UINTN DataSize, UINT32 *Crc32);
typedef EFI_STATUS (*EFI_COPY_MEM)(VOID *Destination, VOID *Source, UINTN Length);
typedef EFI_STATUS (*EFI_SET_MEM)(VOID *Buffer, UINTN Size, UINT8 Value);
typedef EFI_STATUS (*EFI_CREATE_EVENT_EX)(UINT32 Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY NotifyFunction, VOID *NotifyContext, EFI_GUID *EventGroup, EFI_EVENT *Event);

typedef struct EFI_BOOT_SERVICES
{
    EFI_TABLE_HEADER                            Hdr;
    EFI_RAISE_TPL                               RaiseTPL;
    EFI_RESTORE_TPL                             RestoreTPL;
    EFI_ALLOCATE_PAGES                          AllocatePages;
    EFI_FREE_PAGES                              FreePages;
    EFI_GET_MEMORY_MAP                          GetMemoryMap;
    EFI_ALLOCATE_POOL                           AllocatePool;
    EFI_FREE_POOL                               FreePool;
    EFI_CREATE_EVENT                            CreateEvent;
    EFI_SET_TIMER                               SetTimer;
    EFI_WAIT_FOR_EVENT                          WaitForEvent;
    EFI_SIGNAL_EVENT                            SignalEvent;
    EFI_CLOSE_EVENT                             CloseEvent;
    EFI_CHECK_EVENT                             CheckEvent;
    EFI_INSTALL_PROTOCOL_INTERFACE              InstallProtocolInterface;
    EFI_REINSTALL_PROTOCOL_INTERFACE            ReinstallProtocolInterface;
    EFI_UNINSTALL_PROTOCOL_INTERFACE            UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL                         HandleProtocol;
    VOID                                        *Reserved;
    EFI_REGISTER_PROTOCOL_NOTIFY                RegisterProtocolNotify;
    EFI_LOCATE_HANDLE                           LocateHandle;
    EFI_LOCATE_DEVICE_PATH                      LocateDevicePath;
    EFI_INSTALL_CONFIGURATION_TABLE             InstallConfigurationTable;
    EFI_IMAGE_LOAD                              LoadImage;
    EFI_IMAGE_START                             StartImage;
    EFI_EXIT                                    Exit;
    EFI_IMAGE_UNLOAD                            UnloadImage;
    EFI_EXIT_BOOT_SERVICES                      ExitBootServices;
    EFI_GET_NEXT_MONOTONIC_COUNT                GetNextMonotonicCount;
    EFI_STALL                                   Stall;
    EFI_SET_WATCHDOG_TIMER                      SetWatchdogTimer;
    EFI_CONNECT_CONTROLLER                      ConnectController;
    EFI_DISCONNECT_CONTROLLER                   DisconnectController;
    EFI_OPEN_PROTOCOL                           OpenProtocol;
    EFI_CLOSE_PROTOCOL                          CloseProtocol;
    EFI_OPEN_PROTOCOL_INFORMATION               OpenProtocolInformation;
    EFI_PROTOCOLS_PER_HANDLE                    ProtocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER                    LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL                         LocateProtocol;
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES    InstallMultipleProtocolInterfaces;
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES  UninstallMultipleProtocolInterfaces;
    EFI_CALCULATE_CRC32                         CalculateCrc32;
    EFI_COPY_MEM                                CopyMem;
    EFI_SET_MEM                                 SetMem;
    EFI_CREATE_EVENT_EX                         CreateEventEx;
} EFI_BOOT_SERVICES;

// =============================================================================
// Configuration Table
// =============================================================================

typedef struct EFI_CONFIGURATION_TABLE
{
    EFI_GUID    VendorGuid;
    VOID        *VendorTable;
} EFI_CONFIGURATION_TABLE;

// =============================================================================
// ACPI Table Protocol
// =============================================================================

struct EFI_ACPI_TABLE_PROTOCOL;

typedef EFI_STATUS (*EFI_ACPI_TABLE_INSTALL_ACPI_TABLE)(struct EFI_ACPI_TABLE_PROTOCOL *This, VOID *AcpiTableBuffer, UINTN AcpiTableBufferSize, UINTN *TableKey);
typedef EFI_STATUS (*EFI_ACPI_TABLE_UNINSTALL_ACPI_TABLE)(struct EFI_ACPI_TABLE_PROTOCOL *This, UINTN  TableKey);

typedef struct EFI_ACPI_TABLE_PROTOCOL
{
    EFI_ACPI_TABLE_INSTALL_ACPI_TABLE   InstallAcpiTable;
    EFI_ACPI_TABLE_UNINSTALL_ACPI_TABLE UninstallAcpiTable;
} EFI_ACPI_TABLE_PROTOCOL;

// =============================================================================
// File Protocol
// =============================================================================

#define EFI_FILE_INFO_ID                {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_FILE_SYSTEM_INFO_ID         {0x09576e93, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}} 
#define EFI_FILE_SYSTEM_VOLUME_LABEL_ID {0xdb47d7d3, 0xfe81, 0x11d3, {0x9a, 0x35, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}

#define EFI_FILE_PROTOCOL_REVISION          0x00010000
#define EFI_FILE_PROTOCOL_REVISION2         0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION   EFI_FILE_PROTOCOL_REVISION2

#define EFI_FILE_MODE_READ      0x0000000000000001
#define EFI_FILE_MODE_WRITE     0x0000000000000002
#define EFI_FILE_MODE_CREATE    0x8000000000000000

#define EFI_FILE_READ_ONLY  0x0000000000000001
#define EFI_FILE_HIDDEN     0x0000000000000002
#define EFI_FILE_SYSTEM     0x0000000000000004
#define EFI_FILE_RESERVED   0x0000000000000008
#define EFI_FILE_DIRECTORY  0x0000000000000010
#define EFI_FILE_ARCHIVE    0x0000000000000020
#define EFI_FILE_VALID_ATTR 0x0000000000000037

typedef struct EFI_FILE_INFO
{
    UINT64      Size;
    UINT64      FileSize;
    UINT64      PhysicalSize;
    EFI_TIME    CreateTime;
    EFI_TIME    LastAccessTime;
    EFI_TIME    ModificationTime;
    UINT64      Attribute;
    CHAR16      *FileName;
} EFI_FILE_INFO;

typedef struct EFI_FILE_SYSTEM_INFO
{
    UINT64  Size;
    BOOLEAN ReadOnly;
    UINT64  VolumeSize;
    UINT64  FreeSpace;
    UINT32  BlockSize;
    CHAR16  VolumeLabel;
} EFI_FILE_SYSTEM_INFO;

typedef struct EFI_FILE_SYSTEM_VOLUME_LABEL
{
    CHAR16 *VolumeLabel;
} EFI_FILE_SYSTEM_VOLUME_LABEL;

typedef struct EFI_FILE_IO_TOKEN
{
    EFI_EVENT   Event;
    EFI_STATUS  Status;
    UINTN       BufferSize;
    VOID        *Buffer;
} EFI_FILE_IO_TOKEN;

struct EFI_FILE_PROTOCOL;

typedef EFI_STATUS (*EFI_FILE_OPEN)(struct EFI_FILE_PROTOCOL *This, struct EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes);
typedef EFI_STATUS (*EFI_FILE_CLOSE)(struct EFI_FILE_PROTOCOL *This);
typedef EFI_STATUS (*EFI_FILE_DELETE)(struct EFI_FILE_PROTOCOL *This);
typedef EFI_STATUS (*EFI_FILE_READ)(struct EFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);
typedef EFI_STATUS (*EFI_FILE_WRITE)(struct EFI_FILE_PROTOCOL *This, UINTN *BufferSize, VOID *Buffer);
typedef EFI_STATUS (*EFI_FILE_GET_POSITION)(struct EFI_FILE_PROTOCOL *This, UINT64 *Position);
typedef EFI_STATUS (*EFI_FILE_SET_POSITION)(struct EFI_FILE_PROTOCOL *This, UINT64 Position);
typedef EFI_STATUS (*EFI_FILE_GET_INFO)(struct EFI_FILE_PROTOCOL *This, EFI_GUID *InformationType, UINTN *BufferSize, VOID *Buffer);
typedef EFI_STATUS (*EFI_FILE_SET_INFO)(struct EFI_FILE_PROTOCOL *This, EFI_GUID *InformationType, UINTN BufferSize, VOID *Buffer);
typedef EFI_STATUS (*EFI_FILE_FLUSH)(struct EFI_FILE_PROTOCOL *This);
typedef EFI_STATUS (*EFI_FILE_OPEN_EX)(struct EFI_FILE_PROTOCOL *This, struct EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes, EFI_FILE_IO_TOKEN *Token);
typedef EFI_STATUS (*EFI_FILE_READ_EX)(struct EFI_FILE_PROTOCOL *This, EFI_FILE_IO_TOKEN *Token);
typedef EFI_STATUS (*EFI_FILE_WRITE_EX)(struct EFI_FILE_PROTOCOL *This, EFI_FILE_IO_TOKEN *Token);
typedef EFI_STATUS (*EFI_FILE_FLUSH_EX)(struct EFI_FILE_PROTOCOL *This, EFI_FILE_IO_TOKEN *Token);

typedef struct EFI_FILE_PROTOCOL
{
    UINT64                  Revision;
    EFI_FILE_OPEN          Open;
    EFI_FILE_CLOSE         Close;
    EFI_FILE_DELETE        Delete;
    EFI_FILE_READ          Read;
    EFI_FILE_WRITE         Write;
    EFI_FILE_GET_POSITION  GetPosition;
    EFI_FILE_SET_POSITION  SetPosition;
    EFI_FILE_GET_INFO      GetInfo;
    EFI_FILE_SET_INFO      SetInfo;
    EFI_FILE_FLUSH         Flush;
    EFI_FILE_OPEN_EX       OpenEx;
    EFI_FILE_READ_EX       ReadEx;
    EFI_FILE_WRITE_EX      WriteEx;
    EFI_FILE_FLUSH_EX      FlushEx;
} EFI_FILE_PROTOCOL;

// =============================================================================
// Graphics Output Protocol
// =============================================================================

typedef struct EFI_PIXEL_BITMASK
{
    UINT32  RedMask;
    UINT32  GreenMask;
    UINT32  BlueMask;
    UINT32  ReservedMask;
} EFI_PIXEL_BITMASK;

typedef enum EFI_GRAPHICS_PIXEL_FORMAT
{
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
{
    UINT32                      Version;
    UINT32                      HorizontalResolution;
    UINT32                      VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT  PixelFormat;
    EFI_PIXEL_BITMASK          PixelInformation;
    UINT32                      PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE
{
    UINT32                                  MaxMode;
    UINT32                                  Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *Info;
    UINTN                                   SizeOfInfo;
    EFI_PHYSICAL_ADDRESS                   FrameBufferBase;
    UINTN                                   FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL
{
    UINT8   BLUE;
    UINT8   GREEN;
    UINT8   RED;
    UINT8   Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef enum EFI_GRAPHICS_OUTPUT_BLT_OPERATION
{
    EfiBltVideoFill,
    EfiBltVideoToBltBuffer,
    EfiBltBufferToVideo,
    EfiBltVideoToVideo,
    EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;

struct EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef EFI_STATUS (*EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE)(struct EFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber, UINTN *SizeOfInfo, EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);
typedef EFI_STATUS (*EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE)(struct EFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber);
typedef EFI_STATUS (*EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT)(struct EFI_GRAPHICS_OUTPUT_PROTOCOL *This, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation, UINTN SourceX, UINTN SourceY, UINTN DestinationX, UINTN DestinationY, UINTN Width, UINTN Height, UINTN Delta);

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE   SetMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT        Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE       *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

// =============================================================================
// Loaded Image Protocol
// =============================================================================

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION  0x1000

typedef EFI_STATUS (*EFI_IMAGE_UNLOAD)(EFI_HANDLE ImageHandle);

typedef struct EFI_LOADED_IMAGE_PROTOCOL
{
    UINT32                    Revision;
    EFI_HANDLE               ParentHandle;
    struct EFI_SYSTEM_TABLE  *SystemTable;
    EFI_HANDLE               DeviceHandle;
    EFI_DEVICE_PATH_PROTOCOL *FilePath;
    VOID                      *Reserved;
    UINT32                    LoadOptionsSize;
    VOID                      *LoadOptions;
    VOID                      *ImageBase;
    UINT64                    ImageSize;
    UINTN                     ImageCodeType;
    EFI_IMAGE_UNLOAD         UnLoad;
} EFI_LOADED_IMAGE_PROTOCOL;

// =============================================================================
// Simple File System Protocol
// =============================================================================

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION    0x00010000

struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef EFI_STATUS (*EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,  EFI_FILE_PROTOCOL **Root);

typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
{
    UINT64                                       Revision;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME OpenVolume;
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

// =============================================================================
// System Table
// =============================================================================

typedef struct EFI_SYSTEM_TABLE
{
    EFI_TABLE_HEADER                   Hdr;
    CHAR16                              *FirmwareVendor;
    UINT32                              FirmwareRevision;
    EFI_HANDLE                         ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL     *ConIn;
    EFI_HANDLE                         ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *ConOut;
    EFI_HANDLE                         StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *StdErr;
    EFI_RUNTIME_SERVICES               *RuntimeServices;
    EFI_BOOT_SERVICES                  *BootServices;
    UINTN                               NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE            *ConfigurationTable;
} EFI_SYSTEM_TABLE;

void *efi_image_handle;
EFI_SYSTEM_TABLE *efi_system_table;

void efi_init(void *image_handle, EFI_SYSTEM_TABLE *system_table);

#endif // _EFI_H
