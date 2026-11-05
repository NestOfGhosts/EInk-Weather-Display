# WeaStat
### Is probably what im going to call it ignore the repo name 

## Premise
A pcb with an eink display that shows outside temperatures with a free weather api, and shows inside temperatures with a bme280. All powered with a 18650 battery, attempting to minimize battery drain.

Made by me, for Hack Club Half Life

## BOM

[BOM.csv](https://github.com/NestOfGhosts/EInk-Weather-Display/blob/main/BOM.csv) is only a table of items I personally needed to purchase, not including things I already have. Below is the proper list of materials

|Thing You Need|Details|
|---|---|
|Soldering Equipment|and the expertise to use it. All soldering done for this project should only be through hole|
|Esp32|The Pcb is assuming a DOIT ESP32 DEVKIT V1, and im using a clone|
|Battery & Battery holder|Only necessary if you plan on using a battery to power it. You can use the usb port to power it instead. *More details abt holder below|
|Eink Display|Pcb is assuming a WeAct 2.9"|
|BME280|Pcb is assuming a 6 pin bme, but you could get away with a 4 pin, utilizing the 4 leftmost holes|
|Any 2 buttons|Pcb is assuming a 5 pin mx cherry compatible keyboard switch|

*If you want to use a battery, either use a normal battery holder and an 1N4007 diode to slightly lower the voltage, or a dedicated module that controls 3v3 output ([example](https://www.aliexpress.com/item/1005006351276180.html?spm=a2g0o.detail.0.0.2164unO6unO65w&mp=1&pdp_npi=6%40dis%21CAD%21CAD+4.67%21CAD+4.67%21%21CAD+4.67%21%21%21%4021030f8317938421479141980e0f47%2112000046129008872%21ct%21CA%216330886401%21%211%210%21))

If you have different parts than the pcb assumes, you can edit the [.kicad_pcb file](https://github.com/NestOfGhosts/EInk-Weather-Display/blob/main/Pcb%20Files/weatstat.kicad_pcb) in KiCad with the footprints of your actual parts.

## TBC
