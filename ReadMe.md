# O projekcie
Repozytorium jest poświęcone oprogramowaniu robota modularnego Gizmo realizowanego w ramach pracy inżynierskiej. Pod ścieżką ```source/NUCLEO-F44RE_Gizmo``` znajduje się projekt **STM32CubeIDE** dostosowany pod mikroprocesor STM32F446RE. Peryferia potrzebne do pracy składają się z:
- 2x akcelerometr ADXL345
- 2x serwomechanizm ST3020
- 2x transceiver CAN (MCP2551 lub podobne)
- 1x wyświetlacz LCD 1.8in Waveshare
- 1x karta microSD obsługująca format FATFS (przetestowano na SanDisk 32GB)
***Oprogramowanie jest udostępnione na otwartej licencji GPLv3***

Sama praca jest dostępna do wglądu w pliku ```Bachelors_thesis.pdf```. Możliwe jest również lokalne skompilowanie pracy, której pliki źródłowe znajdują się pod ścieżką ```extras/bachelors```. Rekomendowanym kompilatorem jest *XeLaTeX*.

## Robot modularny
Roboty modularne różnią się od standardowych robotów przedewszystkim faktem, że zdolne są one do zmiany swojej 'budowy' w trakcie ich pracy - inaczej mówiąc ich opis kinematyki nie jest stały. Roboty modularne składają się z podstawowych jednostek nazywanych modułami, charakteryzują się one:
- autonomią - są zdolne do samodzielnego ruchu oraz pozyskiwania informacji ze środowiska
- zdolnością do rekonfiguracji - moduły potrafią łączyć się ze sobą w różne topologie
- jednorodnością - są jednakowe pod względem budowy mechanicznej i oprogramowania 

### Wygląd robota Gizmo
Bryła robota została opracowana przez Kacpra Olszewskiego (Politechnika Warszawska)

<img src="/extras/res_readme/gizmo_real.jpg" width="50%" height="50%"/>

Każdy z modułów składa się z dwóch typów członów: sterującego oraz ruchowego. Człon sterujący, widoczny na środku konstrukcji, jest pusty w środku a w jego wnętrzu umieszczona jest większość elektroniki. Człony ruchowe składają się z serwomechanizmów oraz konektorów (białe podstawy). 

## Oprogramowanie

### Wyświetlacz LCD
W trakcie pracy robota wyświetlane są dwa możliwe panele. Pierwszym z nich jest panel inicjalizacji, który mówi o tym w jakim stadium jest rozpoczęcie pracy urządzenia.

<img src="/extras/res_readme/gizmo_real.jpg" width="50%" height="50%"/>

Drugim wariantem wyświetlanej treści jest panel informacyjny. Zawiera on informacje o statusie peryferiów oraz ostatnie odczyty i zadawane parametry ruchu.

<img src="/extras/res_readme/LCD_info.png" width="50%" height="50%"/>

### Panel sterowniczy
Pod ścieżką ```app/control_panlel.py``` znajduje się aplikacja służąca do obsługi robota. Została napisana z użyciem bibliotek **Pyside6** oraz **Serial** w języku **Python** (3.11.0). Pozwala ona na zadawanie ruchu robota, odczyt stanu jego peryferiów, odczyt parametrów ruchu. 

<img src="/extras/res_readme/control_panel.png" width="50%" height="50%"/>

Komunikacja odbywa się za pomocą portu szeregowego (COM port).


