# wmidump

## Build

    gcc wmidump.c -std=gnu99 -o wmidump

## Usage

Find the _WDG method of your WMI device and copy the content of the
buffer in a file.

    Name (_WDG, Buffer (0x50)
    {
      --- cut here ---
      /* 0000 */    0xD0, 0x5E, 0x84, 0x97, 0x6D, 0x4E, 0xDE, 0x11, 
      /* 0008 */    0x8A, 0x39, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66, 
      /* 0010 */    0x42, 0x43, 0x01, 0x02, 0xA0, 0x47, 0x67, 0x46, 
      /* 0018 */    0xEC, 0x70, 0xDE, 0x11, 0x8A, 0x39, 0x08, 0x00, 
      /* 0020 */    0x20, 0x0C, 0x9A, 0x66, 0x42, 0x44, 0x01, 0x02, 
      /* 0028 */    0x72, 0x0F, 0xBC, 0xAB, 0xA1, 0x8E, 0xD1, 0x11, 
      /* 0030 */    0x00, 0xA0, 0xC9, 0x06, 0x29, 0x10, 0x00, 0x00, 
      /* 0038 */    0xD2, 0x00, 0x01, 0x08, 0x21, 0x12, 0x90, 0x05, 
      /* 0040 */    0x66, 0xD5, 0xD1, 0x11, 0xB2, 0xF0, 0x00, 0xA0, 
      /* 0048 */    0xC9, 0x06, 0x29, 0x10, 0x4D, 0x4F, 0x01, 0x00
      --- end cut ---
    })

Then run ./wmidump < file and it should output something like that:

    $ ./wmidump < ../wdg
    97845ED0-4E6D-11DE-8A39-0800200C9A66:
            object_id: BC
            notify_id: 42
            reserved: 43
            instance_count: 1
            flags: 0x2 ACPI_WMI_METHOD 
    466747A0-70EC-11DE-8A39-0800200C9A66:
            object_id: BD
            notify_id: 42
            reserved: 44
            instance_count: 1
            flags: 0x2 ACPI_WMI_METHOD 
    ABBC0F72-8EA1-11D1-00A0-C90629100000:
            object_id: �
            notify_id: D2
            reserved: 00
            instance_count: 1
            flags: 0x8 ACPI_WMI_EVENT 
    05901221-D566-11D1-B2F0-00A0C9062910:
            object_id: MO
            notify_id: 4D
            reserved: 4F
            instance_count: 1
            flags: 0

## wmixtract.py

wmixtract.py is a small python script to extract _WDG and WQXX buffers.
Resulting _WDG files can be parsed with wmidump directly.
WQXX buffers contain compiled MOF (Managed Object Format) and can be
decompiled using the wmimofck.exe program available in Windows Driver Kit (WDK).

references:
* "Using Wmimofck.exe": http://msdn.microsoft.com/en-us/library/windows/hardware/ff565588(v=vs.85).aspx
* "Windows Instrumentation: WMI and ACPI": http://msdn.microsoft.com/en-us/windows/hardware/gg463463