#include "Wire.h"

#define LCD_ADDR 0x27  // آدرس I2C نمایشگر
#define LCD_SDA 40     // پین SDA
#define LCD_SCL 41     // پین SCL

#define UPBtn 8    // پین دکمه بالا
#define DOWNBtn 3  // پین دکمه پایین
#define OKBtn 46   // پین دکمه تأیید

// کلاس برای مدیریت نمایشگر LCD
class LiquidCrystal {
public:
  LiquidCrystal() {}
  LiquidCrystal(uint8_t Address, uint8_t SDA, uint8_t SCL) {
    _Address = Address;
    _SDA = SDA;
    _SCL = SCL;

    // آغاز ارتباط I2C با سرعت بالا
    Wire.begin(_SDA, _SCL, 1000000);
    delay(100);
  }

  void begin(uint8_t line = 2, uint8_t column = 16) {
    _line = line;
    _column = column;
    lcdSendCommand(0X0C);  // روشن کردن نمایشگر و خاموش کردن مکان‌نما
    delay(50);
    lcdSendCommand(0X28);  // تنظیم نمایشگر دو سطری با بلوک‌های 5x8
    delay(100);

    clear();  // پاک کردن نمایشگر
  }

  // نوشتن متن روی نمایشگر
  void write(String data) {
    int i = 0;
    while (data[i] != '\0') {
      lcdSendData(data[i]);
      i++;
    }
  }

  // پاک کردن نمایشگر
  void clear() {
    lcdSendCommand(0X01);
    delay(1);
  }

  // تنظیم مکان‌نما
  void setCursor(int y, int x) {
    uint8_t address;
    if (x == 0) {
      address = y;  // آدرس سطر اول
    } else {
      address = 0x40 + y;  // آدرس سطر دوم
    }
    lcdSendCommand(0x80 | address);  // ارسال آدرس به DDRAM
  }

private:
  uint8_t _SDA;
  uint8_t _SCL;
  uint8_t _Address;

  uint8_t _line;
  uint8_t _column;

  // ارسال فرمان به نمایشگر
  void lcdSendCommand(byte command) {
    Wire.beginTransmission(_Address);
    Wire.write(command & 0xF0 | 0x0C);         // ارسال نیم‌بایت اول
    Wire.write(command & 0xF0 | 0x08);         // ارسال نیم‌بایت اول بدون فعال‌سازی
    Wire.write((command << 4) & 0xF0 | 0x0C);  // ارسال نیم‌بایت دوم
    Wire.write((command << 4) & 0xF0 | 0x08);  // ارسال نیم‌بایت دوم بدون فعال‌سازی
    Wire.endTransmission();
    delay(5);
  }

  // ارسال داده به نمایشگر
  void lcdSendData(byte data) {
    Wire.beginTransmission(_Address);
    Wire.write(data & 0xF0 | 0x0D);         // ارسال نیم‌بایت اول برای داده
    Wire.write(data & 0xF0 | 0x09);         // ارسال نیم‌بایت اول بدون فعال‌سازی
    Wire.write((data << 4) & 0xF0 | 0x0D);  // ارسال نیم‌بایت دوم برای داده
    Wire.write((data << 4) & 0xF0 | 0x09);  // ارسال نیم‌بایت دوم بدون فعال‌سازی
    Wire.endTransmission();
    delay(5);
  }
};

// کلاس برای مدیریت دکمه‌ها با دیبانس
class DebouncedButton {
private:
  uint8_t pin;             // شماره پین دکمه
  bool state;              // وضعیت فعلی دکمه
  bool lastState;          // وضعیت قبلی دکمه
  uint64_t lastTime;       // زمان آخرین تغییر وضعیت
  uint16_t debounceDelay;  // تاخیر دیبانس (به میلی‌ثانیه)

public:
  // سازنده: تنظیم مقادیر اولیه
  DebouncedButton(uint8_t btnPin, uint16_t delay = 50)
    : pin(btnPin), state(LOW), lastState(LOW), lastTime(0), debounceDelay(delay) {
    pinMode(pin, INPUT_PULLDOWN);  // تنظیم پین به‌عنوان ورودی
  }

  // متد برای بررسی وضعیت دکمه و اعمال دیبانس
  bool check() {
    bool reading = digitalRead(pin);  // خواندن وضعیت فعلی دکمه

    // بررسی تغییر وضعیت برای ریست تایمر دیبانس
    if (reading != lastState) {
      lastTime = millis();
    }

    // اعمال دیبانس بر اساس زمان
    if ((millis() - lastTime) > debounceDelay) {
      if (reading != state) {
        state = reading;  // به‌روزرسانی وضعیت دکمه

        // اگر دکمه فشار داده شده باشد، true بازگردانده شود
        if (state == HIGH) {
          lastState = reading;  // به‌روزرسانی وضعیت قبلی
          return true;
        }
      }
    }

    lastState = reading;  // به‌روزرسانی وضعیت قبلی
    return false;         // دکمه فشار داده نشده است
  }

  // متد برای دریافت وضعیت فعلی دکمه
  bool getState() const {
    return state;
  }

  // تنظیم تاخیر دیبانس
  void setDebounceDelay(uint16_t delay) {
    debounceDelay = delay;
  }
};

// تنظیم تعداد ستون‌ها و سطرهای LCD
int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal lcd(LCD_ADDR, LCD_SDA, LCD_SCL);
DebouncedButton upBtn(UPBtn);
DebouncedButton downBtn(DOWNBtn);
DebouncedButton okBtn(OKBtn);

// پایان بخش lcd

// متغیرهای مربوط به منوی LCD
const String EXIT_KEYWORD = "exit";
int size = 0;
byte offset = 0;
byte arrow = 0;
String menu = "1)menu1{1_1)menu1_1{11)menu1_2~12)menu1_3~13)exit}~1_2)menu1_2~1_3)menu1_3~1_4)exit}~2)menu2~3)menu3{3_1)menu3_1~3_2)menu3_2~3_3)menu3_3~3_4)exit}~4)menu4~5)menu5";
String curentMenu = menu;
String* arr = nullptr;  // تعریف پوینتر برای آرایه دینامیک

// پایان منوی lcd

void setup() {
  lcd.begin();
  Serial.begin(115200);
  showMenu(curentMenu);  // نمایش منوی فعلی
}

void loop() {
  handelButton();  // پردازش دکمه‌ها
}

// محاسبه تعداد آیتم‌های منو
void calculateMenuItems(const String& menu) {
  size = 1;  // حداقل یک آیتم وجود دارد
  int nestedLevel = 0;

  for (char c : menu) {
    if (c == '{') nestedLevel++;
    else if (c == '}') nestedLevel--;
    else if (c == '~' && nestedLevel == 0) size++;
  }

  // تخصیص حافظه برای آرایه دینامیک
  if (arr != nullptr) delete[] arr;  // آزادسازی حافظه قبلی (در صورت نیاز)
  arr = new String[size];            // تخصیص حافظه متناسب با تعداد آیتم‌ها
}

// جدا کردن آیتم‌های منو
void splitMenuItems(const String& menu) {
  int start = 0;
  int nestedLevel = 0;
  int index = 0;

  // پیمایش متن منو برای جدا کردن آیتم‌ها
  for (int i = 0; i < menu.length(); i++) {
    char c = menu[i];
    if (c == '{') nestedLevel++;
    else if (c == '}') nestedLevel--;
    else if ((c == '~' && nestedLevel == 0) || i == menu.length() - 1) {
      // مشخص کردن پایان آیتم
      int end = (i == menu.length() - 1) ? i + 1 : i;
      String item = menu.substring(start, end);  // استخراج
      item.trim();                               // حذف فاصله‌ها
      arr[index++] = item;
      start = end + 1;  // تنظیم شروع برای آیتم بعدی
    }
  }
}

// نمایش منو روی LCD
void showMenu(const String& menu) {
  calculateMenuItems(menu);  // شمارش تعداد آیتم‌های منو
  splitMenuItems(menu);      // جدا کردن آیتم‌های منو

  lcd.clear();  // پاک کردن نمایشگر
  for (int i = offset; i < offset + lcdRows && i < size; i++) {
    if (i == offset + arrow) {
      lcd.setCursor(0, i - offset);
      lcd.write(">");
    }
    lcd.setCursor(2, i - offset);
    String item = arr[i];
    lcd.write(item.substring(0, item.indexOf('{')));  // نمایش نام آیتم
  }
}

// پردازش دکمه‌ها
void handelButton() {
  // بررسی دکمه‌های بالا، پایین و تأیید
  if (downBtn.check()) {
    moveArrow(1);
    showMenu(curentMenu);
  }

  if (upBtn.check()) {
    moveArrow(-1);
    showMenu(curentMenu);
  }

  if (okBtn.check()) {
    handleMenuSelection();
    showMenu(curentMenu);
  }
}

// حرکت مکان‌نما در منو
void moveArrow(int direction) {
  int end = min(lcdRows, size);  // حداکثر تعداد ردیف قابل نمایش

  if (direction < 0) {  // حرکت به سمت بالا
    if (arrow > 0)
      arrow += direction;
    else if (offset > 0)
      offset += direction;
  } else {  // حرکت به سمت پایین
    if (arrow < end - 1)
      arrow += direction;
    else if (offset < size - lcdRows)
      offset += direction;
  }
}

// پردازش انتخاب آیتم منو
void handleMenuSelection() {
  int selected = offset + arrow;  // آیتم انتخاب‌شده

  String selectedMenu = arr[selected];
  selectedMenu.trim();  // حذف فاصله‌ها از ابتدا و انتهای متن

  Serial.println(selectedMenu);

  // بررسی نوع آیتم انتخاب‌شده
  if (selectedMenu.indexOf('{') != -1)
    chooseMenu(selectedMenu);
  else if (selectedMenu.indexOf(EXIT_KEYWORD) != -1)
    exit();
  else
    actionMenu(selected, selectedMenu);
}

// انتخاب منوی فرعی
void chooseMenu(const String& selectedMenu) {
  arrow = 0;  // بازنشانی نشانگر
  offset = 0;

  // استخراج زیرمنو
  int start = selectedMenu.indexOf('{') + 1;
  int end = selectedMenu.lastIndexOf('}');

  curentMenu = selectedMenu.substring(start, end);
  curentMenu.trim();  // حذف فاصله‌های اضافی

  Serial.println(curentMenu);
  cleanup();
}

// خروج از منو
void exit() {
  // تنظیم مقادیر به حالت پیش‌فرض برای بازگشت به منوی اصلی
  arrow = 0;          // موقعیت نشانگر به حالت پیش‌فرض
  offset = 0;         // جبران موقعیت منو
  curentMenu = menu;  // بازگشت به منوی اصلی
}

// انجام عملیات مربوط به آیتم انتخاب‌شده
void actionMenu(int selected, String subMenu) {
  // چاپ اطلاعات منو برای اشکال‌زدایی
  String menuName = subMenu.substring(0, subMenu.indexOf('{'));
  Serial.printf("Current Menu: %s, Selected Index: %d\n", menuName.c_str(), selected);
}

// تابع برای آزادسازی حافظه پوینتر آرایه
void cleanup() {
  if (arr != nullptr) {
    delete[] arr;
    arr = nullptr;
  }
}
