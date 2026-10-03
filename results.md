# Результаты
## На x86:  
Bool errors: 8  
Atomic relaxed bool errors: 0  
## На ARM(эмуляция с помощью QEMU):
Bool errors: 8
Atomic relaxed bool errors: 0  

# Вывод
увидеть различия между системами не удалось, все запуски показывали один результат, возможно, на реальном arm смогли бы увидеть memory reording. ошибки с обычным bool скорее происходят из-за data race, а не из-за memory reording