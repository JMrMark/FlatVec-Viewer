PUBLIC decompress_bytes

.code
decompress_bytes PROC
    ; RCX = input, RDX = length, R8 = output
    ; Приклад: просто повертаємо
    ret
decompress_bytes ENDP
END