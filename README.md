## Quelenium for Selenium Server Standalone
Qt library for Selenium Server Standalone (http://www.seleniumhq.org/)

## Version
0.1b

## Using

```
#include <WebDriver>
#include <WebDriverException>
...
WebDriver* driver = new WebDriver("127.0.0.1", "4444", BrowserType::FIREFOX);
```
For more use case see tests directory