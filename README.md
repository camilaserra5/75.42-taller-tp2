# [75.42] Taller de programación I
## Trabajo práctico 2: Verificador de programas eBPF simplificado

**Camila Serra**  
**97422**  
**camilaserra5@gmail.com**  
**https://github.com/camilaserra5/75.42-taller-tp2**

_________________
## Introducción
Para este trabajo se implementará un verificador de programas eBPF. El objetivo es el diseño y construcción de un sistema orientado a objetos y con programación concurrente. Se desarrollará en el lenguaje de programación C++11.

En el presente informe se detalla la solución propuesta y las diferentes pruebas realizadas para garantizar el correcto funcionamiento.

* [Objetivos](#objetivos)
* [Diseño](#diseño)
* [Pruebas](#pruebas)
* [Referencias](#referencias)

## Objetivos
El objetivo del presente trabajo es realizar un verificador de programas eBPF. El verificador debe detectar bucles de código y/o instrucciones sin ejecutar.

El programa será ejecutado de la siguiente manera (uno o más archivos):
```bash
./tp [num hilos] [archivo...]
```

Los archivos deben ser procesados en paralelo, en la cantidad de hilos definidos por línea de comando. Los archivos serán verificados según la siguiente lógica:

Formato aceptado de archivos:
```
[etiqueta]: [instrucción] [param], [param]
```
Siendo la etiqueta opcional, y los parámetros también (0 o más).

La instrucción **ret** puede tener 0 o más argumentos y marca el fin del programa.

Las instrucciones de salto serían análogos a los if o whiles en C++. Hay 3 modos distintos de salto:
* **[jmp] [etiqueta]** siempre salta a la etiqueta.
* **[jmp] [param], [etiqueta]** salta a la etiqueta si el parámetro evalúa a true, de lo contrario sigue a la próxima instrucción.
* **[jmp] [param], [etiqueta1], [etiqueta2]** salta a la etiqueta1 si el parámetro evalúa a true, de lo contrario a la etiqueta2

Las instrucciones válidas para salto son: **jmp, ja, jeq, jneq, jne, jlt, jle, jgt, jge, jset**

Una vez procesado todo el archivo, se deberá construir un grafo con el orden de ejecución del programa. Luego, verificar que no haya ciclos o instrucciones sin usar.

El programa deberá imprimir por cada archivo dependiendo el caso:

**[nombre archivo] GOOD**

**[nombre archivo] FAIL: cycle detected**

**[nombre archivo] FAIL: unused instructions detected**

## Diseño
El sistema principal funciona de la siguiente manera. Primero, se leen todos los archivos y se agregan a un objeto que luego será pasado a todos los hilos. A su vez, se pasará otro objeto a los hilos que es el encargado de almacenar los resultados.
Luego, se crean los n hilos y a cada uno se le pasa por referencia un objeto Locker que contiene los dos objetos comunes mencionados anteriormente.

![](http://www.plantuml.com/plantuml/png/ZP9DIyGm48Rl-HLphA0LzUP1zkJ9WtWLaRIPqhXf9kHZrNzlqX1giYoxbrBcVVCywT2Tyzpwi2WsIu4m4Vc1VXZZeoSBmQ5b9XRaA0tN7XOkTJLueN7VT9wbmbTB8pf7TVA6BYZljf0b0TmyhLnuXFxM6xY7AHBkrcT-VqZNaGVkCLz04HcOo08UqFwk5GPhb5YLFqAv46dKfPhc2rx5Rk7LvHW8F16X5cmJwx0CQ84cyBD5Bjpcpoj9OetVIRf1hi4s6wZkaWyoA8HjBwea0fjsyRPTrvlWvi-1Z_ikJofLwbQuCgbXvUl-mVAaC2-1djaSdBI-ou_-YwJkBhOKJlkZHQQnKliyZxnozgZ919xFYdCNfM5HVm00)

Luego, cada hilo ejecutará la siguiente secuencia accediendo a los objetos en común que se encargan de utilizar el mutex para que no haya problemas de concurrencia.
Cada hilo pide un archivo, y mientras haya disponibles, los procesa y almacena el resultado.

![](http://www.plantuml.com/plantuml/png/jLBDJWCX4BxFKynnc-WBcDGq9lRaeV4DS9bQ8hjiO33x-CwwjikW73qHFkRx-O13OXCv3nukpXAUG-2tC0DCWhhUJMPaV0dz1qK57PsdKmmzfHIYddwbb3sdxRIMKi3pp3HQia-d8u3fsNqQfglZkk3ze_R31xoOj42fssqadKapH68H4wQNfHioqxpJVc19x_zKeDAzf6Xt7y9q6yFz7Zbc0iHsetVY1P92YFzM2R5TwYmdencMbArQga3nSiAUrfdkgbBMK8-CYbW6YDyVfx3NJ6LUVpAX6sjNz9RXxaTIPQcPW1fgNorYVPcMVYDj8oa25OCY70JEW_y2)


El siguiente diagrama muestra como se realiza el procesamiento de cada archivo. Basicamente se realiza 1 pasada por el archivo buscando las etiquetas y se almacena un mapa con <etiqueta, nro de línea>. Luego, en la segunda pasada, se agregan las aristas al grafo de acuerdo a lo pedido.
Cuando se encuentra un salto, se busca el índice de la etiqueta en el mapa, y se agrega la arista correspondiente.
Una vez creado el grafo, se busca por ciclos o instrucciones no usadas con DFS.

![](http://www.plantuml.com/plantuml/png/hPB1JiCm38RlUOeSMp4gRTmue0aqqD0aEC03P8stHgH9b3XGtXvdHLjK61lJBbPhV_xzs-qqeF0OMyqQ9O6lhCMbxHWJDLhFA-QegchbX44-wn2C17dxV9_gwbqW4C9g3-afBrVyyYPFy6jkuADOAGr6j53k-lEktzgSjpM4K9ICvGNYr2eWJMu9KYOfP_BQMiVNW0jbOD0tq0zEAomqSUNVe0eBiGHTA4DZ8zctva27JdnuiNFPLKBAOlC-ueg8hcG4Ra_tu8Lh1c9zyljmoLyuTIsXaTm_njRDTzxpFNga4O6FkV3hs8B1VMveaPbSmqPwrAyuJmjbdxJVHi8WGRU0pP39TaWu-vGZ1IbUZFzX-Ueao-EpUawAaoCLIQutdyAPRs8aJs6VQjr0_NRtMMjuygyc1f13oPxyySjw257ZmUjKEpHdCofFLsnAwTZgBm00)

## Pruebas
Los siguientes casos de pruebas fueron los provistos por la cátedra:

Test  | Result
------------- | -------------
all_addr_modes  | GOOD
all_jmps  | GOOD
arp  | GOOD
icmp  | GOOD
not_exec_inst  | FAIL: unused instructions detected
not_exec_inst2  | FAIL: unused instructions detected
ret  | GOOD
seccomp | GOOD
tcp | GOOD
vlan | GOOD
wasted_resources | GOOD

Además, estos dos con más de un archivo:

Test  | File | Result
------------- | ------------- | -------------
parallel_bad | arp.bpf | GOOD
 | bad_tcp.bpf | FAIL: cycle detected
 | not_exec_inst.bpf | FAIL: unused instructions detected
 | seccomp.bpf | GOOD
 | vlan.bpf | GOOD
parallel_good | arp.bpf | GOOD
 | icmp.bpf | GOOD
 | seccomp.bpf | GOOD
 | tcp.bpf | GOOD
 | vlan.bpf | GOOD


Por otro lado, se hicieron pruebas para validar más casos de uso. En específico, se utilizaron más archivos para probar el procesamiento en paralelo y con distinta cantidad de hilos.

Cant archivos | Hilos | Resultados
------------- | ------------- | -------------
50 y 100  | 2 | todos GOOD
50 y 100  | 10 | todos GOOD
50 y 100  | 20 | todos GOOD
50 y 100  | 2 | 50% GOOD 50% ciclo
50 y 100  | 10 | 50% GOOD 50% ciclo
50 y 100  | 20 | 50% GOOD 50% ciclo
50 y 100  | 2 | 50% GOOD 50% unused
50 y 100  | 10 | 50% GOOD 50% unused
50 y 100  | 20 | 50% GOOD 50% unused
50 y 100  | 2 | 50% ciclo 50% unused
50 y 100  | 10 | 50% ciclo 50% unused
50 y 100  | 20 | 50% ciclo 50% unused

Todas estas pruebas fueron exitosas y sin perdidas de memoria.

## Referencias
* [cpp reference](https://en.cppreference.com/w/)  
* [cplusplus](http://www.cplusplus.com/doc/)  
* [DFS](https://www.geeksforgeeks.org/detect-cycle-in-a-graph/)
* [Concurrency in c++](https://www.classes.cs.uchicago.edu/archive/2013/spring/12300-1/labs/lab6/)
