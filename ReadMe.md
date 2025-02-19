# O projekcie
Repozytorium jest poświęcone oprogramowaniu robota modularnego Gizmo realizowanego w ramach pracy inżynierskiej. Pod ścieżką ```source/NUCLEO-F44RE_Gizmo``` znajduje się projekt **STM32CubeIDE** dostosowany pod mikroprocesor STM32F446RE. Peryferia potrzebne do pracy składają się z:
- 2x akcelerometr ADXL345
- 2x serwomechanizm ST3020
- 2x transceiver CAN (MCP2551 lub podobne)
- 1x wyświetlacz LCD 1.8in Waveshare
- 1x karta microSD obsługująca format FATFS (przetestowano na SanDisk 32GB)
***Oprogramowanie jest udostępnione na otwartej licencji GPLv3***

Sama praca jest dostępna do wglądu w pliku ```Bachelors_thesis.pdf```. Możliwe jest również lokalne skompilowanie pracy, której pliki źródłowe znajdują się pod ścieżką ```extras/bachelors```. Rekomendowanym kompilatorem jest *XeLaTeX*.

## Wygląd robota
Bryła robota została opracowana przez Kacpra Olszewskiego (Politechnika Warszawska)

<img src="/extras/res_readme/gizmo_real.jpg" width="40%" height="40%"/>

## Panel sterowniczy
Pod ścieżką ```app/control_panlel.py``` znajduje się aplikacja służąca do obsługi robota. Została napisana z użyciem bibliotek **Pyside6** oraz **Serial** w języku **Python** (3.11.0)


