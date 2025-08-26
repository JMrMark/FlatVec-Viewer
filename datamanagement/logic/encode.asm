PUBLIC encode_bytes

.code
encode_bytes PROC
    ; RCX = input, RDX = length, R8 = output
    ; Приклад: просто повертаємо
    ret
encode_bytes ENDP
END