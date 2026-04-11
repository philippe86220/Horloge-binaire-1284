# Binary Clock – ATmega1284 + DS3231

---

## ⭐ Quick Overview

Standalone HH:MM:SS binary clock based on an ATmega1284 microcontroller  
and a DS3231 RTC module, designed for both educational  
and electronics learning purposes.

---

## ⏱️ Principle of a Binary Clock

A binary clock does not display time using conventional digits, but instead uses **LEDs representing the binary bits of numbers.**

Each group of LEDs corresponds to one decimal digit of the time:

- tens of hours  
- units of hours  
- tens of minutes  
- units of minutes  
- tens of seconds  
- units of seconds  

Each LED represents a binary weight:

```
1   2   4   8
```

A lit LED means that its weight is included in the value of the digit.

---

## 🔎 Reading Example

If the LEDs corresponding to weights 1 and 4 are ON:

```
4 + 1 = 5
```
The displayed digit is therefore 5.

---

## 🧠 Fundamental Principle

A binary clock does not directly show the time.

👉 It displays the binary decomposition of each decimal digit.

For example:

```
Time: 19:37:42
```

becomes:

```
1  → 01  
9  → 1001  
3  → 011  
7  → 0111  
4  → 100  
2  → 0010  
```

Each LED group lights up according to these bits when they are equal to 1.

---

## 🎯 Role of the Program

The microcontroller continuously performs the following sequence:

1. Read the time from the RTC module  
2. Separate each decimal digit  
3. Convert each digit into binary  
4. Turn ON or OFF the corresponding LEDs  

The program does not compute the time.  
It simply translates the time into LED states.

---

This binary clock is based on an ATmega1284 microcontroller  
and a removable DS3231 RTC module, allowing occasional time adjustments  
while maintaining excellent accuracy.

This project was developed in 2024 with functional, educational, and electronics-oriented goals, emphasizing:

- code understanding  
- hardware mastery  
- simplicity of principles  

---

## 🧭 Project Objectives

- Build a standalone binary clock  
- Understand the relationship **time → binary → LEDs**  
- Work without the String class, using only **C-strings**  
- Use an AVR microcontroller beyond the Arduino Uno  
- Design a complete circuit (power supply, MCU, RTC, display)

---

## 🖼️ Final Result

The clock displays:

- **Hours** (tens + units)  
- **Minutes** (tens + units)  
- **Seconds** (tens + units)  

Each decimal digit is represented in binary using LEDs organized by weights (1, 2, 4, 8).

![connexions](/docs/connexions.jpg)
![rendu](/docs/IMG_2888.jpg)
![renduVerso](/docs/IMG_2891.jpg)

---

## 🧰 Hardware Used (BOM)

### Main Components
- ATmega1284 (DIP40)
- DIP-40 IC socket (recommended to make replacement easier if needed)
- DS3231 RTC module (Adafruit or equivalent)
- Breakaway 2.54 mm female header strip (to plug in the DS3231 RTC module and remove it for time setting)
- Voltage regulator **L7805ABP (TO-220)**
- Red and green LEDs
  - for example http://www.gotronic.fr/art-led-5-mm-rouge-l53lid-2091.htm
  - http://www.gotronic.fr/art-led-5-mm-verte-l53lgd-2093.htm
- 470 Ω resistors (depending on the LED and the desired brightness)
- 1N4007 diode
- Filtering capacitors
- DC jack connector

### Power Supply
- DC input (jack)
- **5 V** regulation
- The project runs entirely on 5 V

---

## 🔌 Power Supply and Clock

- **Power supply**: 9 V DC adapter, capable of delivering up to 0.5 A (or more).
- **Voltage**: regulated 5 V
- **MCU clock**: internal oscillator
- **External crystal**: ❌ not used
- **Frequency**: internal 8 MHz

Choosing a **DS3231** RTC ensures excellent accuracy, independent of the microcontroller's internal clock.  
Preferably avoid AliExpress-type modules; for example you can use:  
http://www.gotronic.fr/art-module-rtc-i2c-ds3231-ada3013-24708.htm

---

## 📐 Wiring Diagram

The complete electronic schematic is provided:

- Standalone ATmega1284
- DS3231 RTC via I2C
- LED groups for hours / minutes / seconds
- Power regulation and filtering

👉 The schematic is valid for both a 1284 and a 1284P:

![Schematic](/docs/Schematic_Horloge-Binaire-ATmega1284.png)

---

## 💾 Flashing and Configuration of the ATmega1284

### Arduino Core Used

This project uses **MightyCore 3.0.3**, developed by MCUdude, and **Arduino IDE 1.8.19**:

https://github.com/MCUdude/MightyCore

MightyCore allows you to use the ATmega1284 directly from the Arduino IDE.

---

### Installing MightyCore

1. Open **File → Preferences**
2. Add the following URL:

https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json

3. Open **Tools → Board → Boards Manager**
4. Search for **MightyCore**
5. Install

---

⚙️ **Configuration in Arduino IDE**

![wiring](/docs/IMG_2920.jpg)

Select:

- Board: ATmega1284
- Clock: Internal 8 MHz
- BOD: 2.7 V
- Variant: 1284
- Pinout: Standard
- Bootloader: No bootloader
- Programmer: AVRISP mkII

👉 These settings automatically configure the fuses.  
A bootloader is not required for this project,  
because the microcontroller is programmed directly via an ISP interface such as **AVRISP mkII**.

---

🔌 **ISP Wiring**
Connection of the AVRISP mkII programmer:  
Programmer → ATmega1284
```
+---------------+---------------------+
| Programmer    | ATmega1284 (DIP40) |
+---------------+---------------------+
| MOSI          | pin 6               |
| MISO          | pin 7               |
| SCK           | pin 8               |
| RESET         | pin 9               |
| VCC           | pin 10 & AVCC pin 30|
| GND           | pin 11 & 31         |
+---------------+---------------------+
```
⚠️ SPI pins correspond to PORT B of the microcontroller:
```
PB5 = MOSI
PB6 = MISO
PB7 = SCK
```
![standard](/docs/standard.png)
![wiring](/docs/IMG_2921.jpg)

---

🔥 **Burning the Initialization Sequence**

Click:
```
Tools → Burn Bootloader
```
This step:
- configures the fuses
- selects the internal 8 MHz clock
- disables the bootloader
  
👉 No other action is required.

---

⚙️ **Uploading the `.ino` File**

![upload](/docs/IMG_2926.jpg)

## 🧠 Code Operating Principle

Every second, the program performs the following sequence:

1. Read the time from the RTC
2. Separate tens and units
3. Convert each digit into binary
4. Build a global binary string
5. Drive the LEDs directly from this string

The binary string is the **single source of truth**.

---

## 🧩 Important Technical Choices

- ❌ No `String` class
- ✅ Exclusive use of `char[]`
- ✅ No `delay()`
- ✅ Code intentionally readable and structured
- ✅ Strong educational approach

---

## 📚 Further Improvements

- Add manual time setting  
  (the RTC module is very accurate, so this is not essential,  
  and it must remain removable to be adjustable via the Arduino IDE)
- Add date display
- Port the project to 3.3 V

---

## 🔍 Program Flow

### 1️⃣ Time Acquisition

```cpp
RTC.actualiser();
RTC.heure();
RTC.minute();
RTC.seconde();
```
- The DS3231 is the time reference.

---

### 2️⃣ Tens / Units Separation

```cpp
uint8_t dizaine(uint8_t x) {
  return  ((x) / 10);
}
uint8_t unite(uint8_t x) {
  return ((x) % 10);
}
```
---

### 3️⃣ Decimal → Binary Conversion (very pedagogical explanation)

🎯 **Purpose of the Function**

The `binaire()` function transforms a **decimal digit** (for example `2`, `7`, `9`)
into a sequence of ON/OFF LEDs, represented here by a string of characters '0' and '1'.

Examples:

- `2` → `"10"`
- `5` → `"101"`
- `9` → `"1001"`

---

🧠 **Fundamental Principle**

Even though the ATmega1284 is an 8‑bit microcontroller
(meaning its registers and ALU process data in 8‑bit blocks),
this does not mean all information must be displayed using 8 bits.

In this project, we display only isolated decimal digits (0 to 9).

A decimal digit does not always require 8 bits in binary:

- tens of hours range only from **0 to 2**
- therefore they need only **2 bits**
- displaying all 8 bits would be useless
  
👉 The role of the `binaire()` function is therefore:

- to examine all 8 bits of the number
- but to keep only the bits actually useful for display

---

🔍 **Function Signature**

```cpp
binaire(value, w, buffer);
```
- `value`: the number to convert (e.g., 2, 7, 9)
- `w`: indicates **how many bits are ignored at the beginning**
- `buffer`: the resulting `'0'/'1'` string

⚠️ **Key point for beginners**  
👉 w is NOT the number of bits to write.  
👉 It is the index from which writing begins, therefore the number of leading positions ignored.

---

🧩 **How the Function Works (step by step)**

The function:

1. Always looks at the **most significant bit** (bit 7),
2. Shifts the number to the left,
3. Repeats the operation **8 times**,
4. Writes bits only when `x > w`.

In other words:

> “I examine the bits one by one, but I start writing only when I reach the area that interests me.”

---

🧠 **Simple Visualization (without code)**

Imagine a ruler with 8 positions (the 8 bits):

```
[ b7 ][ b6 ][ b5 ][ b4 ][ b3 ][ b2 ][ b1 ][ b0 ]
```

- The function always scans all 8 positions
- `w` means:
👉 **“Ignore the left positions up to this one.”**

---

🧮 **Why `w = 5` for tens of hours**

**Possible values**

Tens of hours can be:
- 0
- 1
- 2

In binary:

- 0 → `00`
- 1 → `01`
- 2 → `10`

👉 **Only 2 bits are required**

---

**What does `w = 5` do?**

If `w = 5`, the condition `x > w` becomes true only for:

```
x = 6 and x = 7
```

➡️ therefore **2 bits are written**

This is exactly what we want.

```cpp
binaire(hourTens, 5, tabHourTens); // writes 2 bits
```

---

🧮 **The Formula (introduced afterwards)**

Once the concept is understood, we can state the mathematical rule:

> Number of bits written = `7 - w`

Therefore:
- if we want 2 bits → `7 - w = 2` → `w = 5`
- if we want 4 bits → `7 - w = 4` → `w = 3`

---

📊 **Summary Table**

| Group            | Max Value | Required Bits | `w` Value |
| ---------------- | --------- | ------------- | --------- |
| Hour tens        | 2         | 2 bits        | `5`       |
| Hour units       | 9         | 4 bits        | `3`       |
| Minute tens      | 5         | 3 bits        | `4`       |
| Minute units     | 9         | 4 bits        | `3`       |
| Second tens      | 5         | 3 bits        | `4`       |
| Second units     | 9         | 4 bits        | `3`       |

👉 **The table shows that nothing is arbitrary**

> The `binaire()` function always examines 8 bits but keeps only the strictly necessary ones for displaying the digit.  
> The parameter `w` simply indicates **from which point useful bits start being written**.

---

🧮 **Helper function to compute w: `uint8_t w_from_bits(uint8_t bits)`**
```cpp
uint8_t w_from_bits(uint8_t bits) {
  return (uint8_t)(7 - bits);
}
```

For better readability, we do not manipulate w directly.

We simply indicate the number of required bits:

```cpp
binaire(value, w_from_bits(3), buffer);
```

Internal formula:
```cpp
w = 7 - number_of_bits
```
---

### 4️⃣ concatener() Function — Pedagogical Explanation

```cpp
char tabConcatenation[20 + 1];
```
-> concatenates in order:

HHt | HHu | MMt | MMu | SSt | SSu

via the function:

```cpp
void concatener(char *dst, uint8_t dstSize,
                const char *a, const char *b, const char *c,
                const char *d, const char *e, const char *f)
{
```

🎯 **Objective**

The `concatener()` function builds **a single global binary string** from multiple smaller strings containing hour, minute, and second bits.

It deliberately replaces standard functions such as `strcat()` in order to:

- keep full memory control,
- prevent buffer overflow,
- remain pedagogical.

---

🧩 **Parameters Passed**

The function call is:

```cpp
concatener(tabConcatenation, sizeof(tabConcatenation),
           tabHourTens, tabHourUnits,
           tabMinuteTens, tabMinuteUnits,
           tabSecondTens, tabSecondUnits);
```

We therefore pass:

- `dst`: the final array receiving the complete string → `tabConcatenation`
- `dstSize`: its maximum size → `sizeof(tabConcatenation)`
- `a..f`: pointers to 6 source C-strings

⚠️ In C language:
> When an array is passed to a function, its name automatically becomes a pointer to its first element.

Thus:
```cpp
tabHourTens
```
is equivalent to:

```cpp
&tabHourTens[0]
```
---

🧠 **Key Principle: an Array of Pointers**

Instead of copying each array separately, the function creates an array of pointers:

```cpp
const char *srcs[6] = { a, b, c, d, e, f };
```

This array simply contains:

👉 the addresses of the six strings to concatenate.

This allows us to:

- iterate through all sources with a single loop,
- avoid code repetition,
- make the function generic.

---

🔁 **Step 1 — Iterating Through Source Strings**

A for loop iterates over the six pointers:

```cpp
for (uint8_t s = 0; s < 6; s++)
```

At each iteration:

```cpp
const char *p = srcs[s];
```

Pointer `p` receives the address of the first character of the current string.

---

🔁 **Step 2 — Character‑by‑Character Copy**

An inner loop copies characters until the null terminator is reached:

```cpp
while (*p)
```

At each step:

```cpp
dst[i++] = *p++;
```

This compact instruction performs several actions at once:

- `*p` reads the character from the source string
- it is immediately copied into `dst[i]`
- `p++` moves the pointer to the next character
- `i++` advances the destination index

👉 In other words:

> A character is copied, then both source and destination positions advance simultaneously.

---

🧠 **Important: Null Terminator Handling**

When `*p == '\0'`:

👉 the loop stops automatically  
👉 the function moves to the next string.

---

🔒 **Buffer Overflow Protection**

Before each write, the function checks:

```cpp
if (i + 1 >= dstSize)
```

This guarantees:

- there is always space for the terminating '\0'
- no out‑of‑bounds write is possible.

If the maximum size is reached:

👉 the function immediately terminates the string and exits.

---

🏁 **Final Step — End of the Global String**

After all copies are completed:

```cpp
dst[i] = '\0';
```

The string terminator is added.

The `dst` array now becomes a valid C‑string containing all bits.

---

🎯 **One‑Sentence Summary**

> The `concatener()` function successively copies multiple binary strings into a single array, character by character, using an array of pointers to iterate automatically through all sources while guaranteeing no memory overflow.

---

5️⃣ **LED Display = Direct Reading of the String**

```cpp
tabConcatenation[x] == '1'
  ? digitalWrite(pin, HIGH)
  : digitalWrite(pin, LOW);
```

With **three ranges:**

- Hours
- Minutes
- Seconds (plus one final separate bit)

👉 The binary string is the single source of state  
👉 The hardware simply follows.

---

6️⃣ **Time Management**

```cpp
if (millis() - tempsPrecedent >= 1000UL) {
    tempsPrecedent = millis();
  ...
}
```

👉  No `delay()` used

---

## 📦 Software Dependency: RTC Library

This project uses the **simpleRTC** library, developed for DS1307 and DS3231 RTC modules.

- Author: Arduino forum member (@bricoleau)
- Original discussion:  
https://forum.arduino.cc/t/partage-librairie-simplertc-ds1307-ds3231-avec-heures-ete-hiver/376814

This library provides:

- simplified time reading
- automatic summer/winter time handling
- a very lightweight interface suited for AVR microcontrollers

⚠️ **License**

No explicit license is mentioned by the author in the original discussion.  
Therefore, the library is **not redistributed in this repository**.  
Users must download it directly from the original source.

---

## 👤 Author

Project designed and built by **Philippe86220**  
2024 — personal and educational use

---

## 🤝 Acknowledgments

This project benefited from educational discussions with ChatGPT (OpenAI), used as a technical assistant for algorithmic reasoning, code optimization, and documentation writing.

---

## 📄 License

MIT

