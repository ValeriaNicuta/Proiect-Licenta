# Pașii de instalare și pornire a proiectului

Pentru a porni funcționalitatea proiectului de licență este nevoie de completat următorii pași:
1. Instalare mediu software
    - Se instalează [Arduino IDE](https://www.arduino.cc/en/software/OldSoftwareReleases) versiunea 1.0.1
    - Mediul software utilzat este [Atmel Studio 7](https://atmel-studio.software.informer.com/7.0/)
    - [AVRDUDE](https://github.com/avrdudes/avrdude) este programul care ne permite să generăm și să încărcăm fișierul .elf necesar pentru punerea în mișcare a proiectului. Fișierul executabil (.exe)
2. Configurare tool extern
   După instalarea Atmel Studio, este necesară realizarea unei configurări pentru a putea încărca codul pe plăcuță
   - Se conectează cablul USB la laptop
   - În ”Device Manager” va apărea secțiunea de PORTs. Verificăm pe care din portul COM este conectată plăcuța Arduino (în cazul proiectului, am configurat pentru COM4)
   - După instalarea Arduino IDE, în ”\arduino-1.0.1-windows\arduino-1.0.1\hardware\tools\avr\bin\” se va muta fișierul avr.exe obținut după descărcarea AVRDUDE
   - În Atmel Studio, navigăm la secțiunea Tools
   - La meniul Tools navigăm până la opțiunea External Tools. Aici se vor realiza toate configurațiile necesare pentru portul USB utilizat
   - Meniul External Tools ne permite să configurăm instrumentele externe pentru generarea și încărcarea codurilor.
     La Title dăm o denumire pentru tool-ul nou (în cazul proiectului prezent este Arduino COM4)
     La Command se va trece calea fișierului avr.exe ”\arduino-1.0.1-windows\arduino-1.0.1\hardware\tools\avr\bin\avrdude.exe”
     La Arguments se va trece: numărul portului de comumnicație stabilit in Device Manager, calea fișierului avrdude.conf, numele microcontrolerului utilzat și a plăcuței din care face parte, și calea fișierului .hex.
     La final se va obține următoarea comandă: -C "D:\arduino-1.0.1-windows\arduino-1.0.1\hardware\tools\avr\etc\avrdude.conf" -p atmega328p -c arduino -P COM6 -b 115200 -U flash:w:"$(ProjectDir)Debug\$(TargetName).hex":i
   - Se aplică configurațiile și se inchide meniul External Tool
3. Creare proiect și importare fișiere
   - La pagina de Start afișată la deschiderea Atmel Studio, se selectează opțiunea New Project
   - Se selectează opțiunea ”GCC C ASF Board Project” și se atribuie o denumire pentru noul proiect
   - La dechiderea proiectului nou, avem opțiunea de a crea fișiere noi, sau de a importa fișiere existente
   - Pentru a crea un fișier nou se accesează meniul Solution Explorer, se dă click dreapta pe folder-ul src și se selectează Add -> New Item.
   - Pentru a importa un fișier deja existent în proiect se accesează Solution Explorer, se dă click dreapta pe src și se selectează Add -> Existing Item
4. Încărcare proiect pe plăcuță
   - Facem build la proiect prin selectarea meniului Build -> Build Solution
   - Dacă nu apar eror, încărcăm fișierul generat pe plăcuță prin Tools -> Arduino COM4
