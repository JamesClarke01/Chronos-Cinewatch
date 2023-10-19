# Overview
This is a custom smartwatch prototype to assist cinema ushers.
Designed for the eZ430-Chronos Development Tool.

The server scrapes the screening times from the cinema website and transmits it to the watch, using the kit provided short range radio USB device.
The user can then view this data on the watch.
Note: Scraping code not included in this repository.

The server code is written by me, and the the client code is a modified version of the operating system provided with the development tool.
# Data Transmission Documentation
## Data file
The data is scraped and saved to a file in the format:
```
ddmm,Shhmm,Shhmm,Shhmm
```
Where 

| Acronym | Description         | 
| ------- | ------------------- |
| dd      | day of data         |
| mm      | month of data       |
| S       | Screen No.          |
| hh      | Hour screen exits   |
| mm      | minute screen exits |
## Server Data Transmission
**cineSyncer.c** will process the Data File and transmit the data from the USB radio device using the following packet structure:
### Packet 1 (Header Packet)

| Packet Index | data index | contents          | value | Example Value |
| ------------ | ---------- | ----------------- | ----- | ------------- |
| 0            |            | packet start      | 0xFF  |               |
| 1            |            | Write instruction | 0x31  |               |
| 2            |            | Packet Length     |       |               |
| 3            | 0          | CMD_CINESYNC      | 0x08  |               |
| 4            | 1          | packetNum         | 0     |               |
| 5            | 2          | day               |       | 25            |
| 6            | 3          | month             |       | 8             |

### Packet 2 (Out Packet)

| Packet Index | data index | contents          | value | Example Value |
| ------------ | ---------- | ----------------- | ----- | ------------- |
| 0            |            | packet start      | 0xFF  |               |
| 1            |            | Write instruction | 0x31  |               |
| 2            |            | Packet Length     |       |               |
| 3            | 0          | CMD_CINESYNC      | 0x08  |               |
| 4            | 1          | packetNum         | 1     |               |
| 5            | 2          | outAmount         |       |               |
| 6            | 3          | Screen 1          |       |               |
| 7            | 4          | Hour 1            |       |               |
| 8            | 5          | Min 1             |       |               |

# Watch Menu
## Menu Item 1 - Main Cine Mode
Shows the times a screen comes out.

| Button | Function             |
| ------ | -------------------- |
| DOWN   | Cycle through times  |
| Hold * | Jump to Current time |       |                      |
## Menu Item 2 - Cine Syncer
Used to manually trigger the cinema time syncing functions

| Button | Function |
| ------ | -------- |
| DOWN | Sync     |





