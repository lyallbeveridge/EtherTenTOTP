#include "rtc.h"

ThreeWire myWire(4, 5, 6); // IO, SCLK, CE (reset?)
RtcDS1302<ThreeWire> Rtc(myWire);

void initRTC() {
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // Disable write protection
    Rtc.SetIsWriteProtected(false);

    // Start the RTC if not running
    if (!Rtc.IsDateTimeValid())
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println(F("RTC lost confidence in the DateTime!"));
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println(F("RTC was write protected, enabling writing now"));
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println(F("RTC was not actively running, starting now"));
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled)
    {
        Serial.println(F("RTC is older than compile time!  (Updating DateTime)"));
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled)
    {
        Serial.println(F("RTC is newer than compile time. (this is expected)"));
    }
    else if (now == compiled)
    {
        Serial.println(F("RTC is the same as compile time! (not expected but all is fine)"));
    }
}

uint64_t getUnixTime() {
    RtcDateTime now = Rtc.GetDateTime();
    return now.Unix64Time();
}
