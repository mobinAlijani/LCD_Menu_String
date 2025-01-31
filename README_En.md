# LCD Menu with Buttons

This project is designed to manage an LCD menu using buttons. You can navigate through the menu and select different items using the up, down, and confirm buttons.

## Installation and Setup

### Required Hardware:
- Arduino
- LCD with I2C interface
- Push Buttons

### Connections:
- LCD to I2C pins (SDA: pin 40, SCL: pin 41)
- Up button to pin 8
- Down button to pin 3
- Confirm button to pin 46

### Libraries:
- [Wire](https://www.arduino.cc/en/Reference/Wire)

## How to Use

1. Upload the code to your Arduino.
2. Make the hardware connections as described above.
3. The menu will be displayed on the LCD, and you can navigate through it using the buttons.

## Main Functions

### LiquidCrystal Class

This class is used to manage the LCD display.

- **LiquidCrystal(uint8_t Address, uint8_t SDA, uint8_t SCL):** Constructor to set the I2C address and SDA, SCL pins.
- **void begin(uint8_t line = 2, uint8_t column = 16):** Initialize the display with the specified number of lines and columns.
- **void write(String data):** Write text to the display.
- **void clear():** Clear the display.
- **void setCursor(int y, int x):** Set the cursor position on the display.

### DebouncedButton Class

This class is used to manage buttons with debounce functionality.

- **DebouncedButton(uint8_t btnPin, uint16_t delay = 50):** Constructor to set the button pin and debounce delay.
- **bool check():** Check the button state with debounce.
- **bool getState() const:** Get the current state of the button.
- **void setDebounceDelay(uint16_t delay):** Set the debounce delay.

### Menu Functions

- **void calculateMenuItems(const String& menu):** Calculate the number of menu items.
- **void splitMenuItems(const String& menu):** Split the menu items.
- **void showMenu(const String& menu):** Display the menu on the LCD.
- **void handelButton():** Handle button presses.
- **void moveArrow(int direction):** Move the cursor in the menu.
- **void handleMenuSelection():** Handle menu item selection.
- **void chooseMenu(const String& selectedMenu):** Select a submenu.
- **void exit():** Exit the menu.
- **void actionMenu(int selected, String subMenu):** Perform actions for the selected menu item.
- **void cleanup():** Free the memory allocated for the array pointer.

## Author

This project was created by [mobinAlijani](https://github.com/mobinAlijani).

## License

This project is licensed under the MIT License. For more information, see the [LICENSE](LICENSE) file.
