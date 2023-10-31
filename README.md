# Search-Images 1.0.0
> Compiler : MinGW
> 
> Ide      : CLion

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://#)

Applicazione  in <strong>C</strong> per le ricerche dell'immagine leggendo il binario ed il descriptor in hexadecimal.

[![monitor-ui](https://github.com/peruvianit/search-images/blob/develop/wiki/hexEd.png)](https://#)

### Info

```c
Programma per la ricerca di immagini da un path di origine e vengono copiate su una cartella destinazione, il nome delle
 immagini sono generate con un nome univoco.
-o       path di origine dove se vuole iniziare a cercare l'immagine
-d       path destinazione dove saranno copiate l'immagine se non 'e attiva l'opzione -n
-e       Esclude l'immgini con estensione de un formato immagine valido e cerca su file con estensione diversa o senza e
stensione
-n       Non copia i file trovati, solo mostra il Summary
-v       Verbose da quello che sta processando il programma
-h       Help

Utilizzo: ./search_image.exe -o <PATH ORIGINE> -d <PATH DESTINA
ZIONE> [-e] [-n] [-v] [-h]
```
### Nuove feature

- [v1.1.0] 

### Bugs

- [v1.0.1] 

### Todos

- [ ] Gestione errori