# O projekcie
Repozytorium jest poświęcone oprogramowaniu robota modularnego Gizmo realizowanego w ramach pracy inżynierskiej. Pod ścieżką 'source/NUCLEO-F44RE_Gizmo' znajduje się projekt **STM32CubeIDE** dostosowany pod mikroprocesor STM32F446RE. Peryferia potrzebne do pracy składają się z:
- 2x akcelerometr ADXL345
- 2x serwomechanizm ST3020
- 2x transceiver CAN (MCP2551 lub podobne)
- 1x wyświetlacz LCD 1.8in Waveshare
- 1x karta microSD obsługująca format FATFS (przetestowano na SanDisk 32GB)
Oprogramowanie jest udostępnione na otwartej licencji GPLv3

## Panel sterowniczy
Pod ścieżką 'app/control_panlel.py' znajduje się aplikacja służąca do obsługi robota. Została napisana z użyciem bibliotek **Pyside6** oraz **Serial**.


