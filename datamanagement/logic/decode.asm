PUBLIC decode_bytes

.code
decode_bytes PROC
    ; RCX = input, RDX = length, R8 = output
    ; Приклад: просто повертаємо
    ret
decode_bytes ENDP
END