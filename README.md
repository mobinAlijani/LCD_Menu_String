# LCD Menu with Buttons

این پروژه برای مدیریت منوی LCD با استفاده از دکمه‌ها طراحی شده است. شما می‌توانید با استفاده از دکمه‌های بالا، پایین و تأیید، در منو حرکت کرده و آیتم‌های مختلف را انتخاب کنید.

## نصب و راه‌اندازی

### سخت‌افزار مورد نیاز:
- Arduino
- LCD با رابط I2C
- دکمه‌های فشاری (Push Buttons)

### اتصالات:
- LCD به پین‌های I2C (SDA: پین 40، SCL: پین 41)
- دکمه بالا به پین 8
- دکمه پایین به پین 3
- دکمه تأیید به پین 46

### کتابخانه‌ها:
- [Wire](https://www.arduino.cc/en/Reference/Wire)

## نحوه استفاده

1. کد را روی Arduino خود آپلود کنید.
2. اتصالات سخت‌افزاری را طبق توضیحات بالا انجام دهید.
3. منو روی LCD نمایش داده می‌شود و می‌توانید با استفاده از دکمه‌ها در منو حرکت کنید.

## توابع اصلی

### کلاس LiquidCrystal

این کلاس برای مدیریت نمایشگر LCD استفاده می‌شود.

- **LiquidCrystal(uint8_t Address, uint8_t SDA, uint8_t SCL):** سازنده کلاس، تنظیم آدرس I2C و پین‌های SDA و SCL.
- **void begin(uint8_t line = 2, uint8_t column = 16):** آغاز به کار نمایشگر با تعداد خطوط و ستون‌های مشخص.
- **void write(String data):** نوشتن متن روی نمایشگر.
- **void clear():** پاک کردن نمایشگر.
- **void setCursor(int y, int x):** تنظیم مکان‌نما در نمایشگر.

### کلاس DebouncedButton

این کلاس برای مدیریت دکمه‌ها با دیبانس استفاده می‌شود.

- **DebouncedButton(uint8_t btnPin, uint16_t delay = 50):** سازنده کلاس، تنظیم پین دکمه و زمان دیبانس.
- **bool check():** بررسی وضعیت دکمه و اعمال دیبانس.
- **bool getState() const:** دریافت وضعیت فعلی دکمه.
- **void setDebounceDelay(uint16_t delay):** تنظیم تاخیر دیبانس.

### توابع منو

- **void calculateMenuItems(const String& menu):** محاسبه تعداد آیتم‌های منو.
- **void splitMenuItems(const String& menu):** جدا کردن آیتم‌های منو.
- **void showMenu(const String& menu):** نمایش منو روی LCD.
- **void handelButton():** پردازش دکمه‌ها.
- **void moveArrow(int direction):** حرکت مکان‌نما در منو.
- **void handleMenuSelection():** پردازش انتخاب آیتم منو.
- **void chooseMenu(const String& selectedMenu):** انتخاب منوی فرعی.
- **void exit():** خروج از منو.
- **void actionMenu(int selected, String subMenu):** انجام عملیات مربوط به آیتم انتخاب‌شده.
- **void cleanup():** آزادسازی حافظه پوینتر آرایه.

## نویسنده

این پروژه توسط [mobinAlijani](https://github.com/mobinAlijani) ایجاد شده است.

## لایسنس

این پروژه تحت لایسنس MIT منتشر شده است. برای اطلاعات بیشتر به فایل [LICENSE](LICENSE) مراجعه کنید.
