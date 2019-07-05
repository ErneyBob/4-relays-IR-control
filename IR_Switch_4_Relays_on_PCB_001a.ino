// P02 Infrared remote control 4 relays on PCB board

#include <IRremote.h>
IRrecv irrecv(9);
decode_results results;

uint32_t val, irkey[17] =
{
  16580863, 16613503, 16597183, 16589023, 16621663, 16605343, 16584943, 16617583, 16601263, 
  16593103, 16625743, 16609423, 16615543, 16591063, 16623703, 16607383, 16619623
};


int16_t TimeStored4Relay[6], PresetPot, CountDownTimer;
const uint16_t LengthOnPeriod[5] = {100, 1200, 6000, 24000, 60000};
uint8_t SelectOnPeriod, DeviceSelect, relay[6];

void setup()
{
  for (int8_t f = 10; f < 14; f++)
  {
    pinMode(f, OUTPUT); digitalWrite(f, 0);
  }
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the IR receiver
  PresetPot = analogRead(A0);
  PresetPot = map(PresetPot, 0, 1023, 1, 9);
  for (int8_t f = 0; f < PresetPot; f++)
  {
    flash();
    delay (200);
  }
}

void loop()
{
  if (irrecv.decode(&results)) 
  {
    val = (results.value);

    if (val == irkey[PresetPot - 1] && DeviceSelect == 1)
    {
      Serial.print ("==select==");
      val = 0;
      DeviceSelect = 2;
      CountDownTimer = 1000;
      flash(); flash(); flash();
    }
    if (DeviceSelect == 1) DeviceSelect = 0;
    if (val == irkey[9] && DeviceSelect == 0)
    {
      Serial.print ("==*==");
      DeviceSelect = 1;
      flash();
    }
    if (DeviceSelect == 2 && CountDownTimer > 1)
    {
      for (int8_t g = 0; g < 4; g++)
      {
        if (val == irkey[g])
        {
          relay[g] = 1 - relay[g];
          digitalWrite(g + 10, relay[g]);
          flash();
        }
        if (val == irkey[g + 4])
        {
          TimeStored4Relay[g] = LengthOnPeriod[SelectOnPeriod];
          relay[g] = 1;
          digitalWrite(g + 10, 1);
          flash();
        }
      }
      if (val == irkey[8])
      {
        for (int8_t f = 10; f < 14; f++)
        {
          TimeStored4Relay[f - 10] = 0;
          relay[f - 10] = 1;
          digitalWrite(f, 1);
          delay (100);
        };
        flash();
      }
      if (val == irkey[10])
      {
        for (int8_t f = 10; f < 14; f++)
        {
          TimeStored4Relay[f - 10] = 0;
          relay[f - 10] = 0;
          digitalWrite(f, 0);
          delay (100);
        };
        flash();
      }

      if (val == irkey[11])
      {
        DeviceSelect = 0;
        flash();
      }
      if (val == irkey[14]) DisplayOnPeriod();
      if (val == irkey[12])
      {
        SelectOnPeriod++;
        if (SelectOnPeriod == 5) SelectOnPeriod = 0;
        DisplayOnPeriod();
      }
      if (val == irkey[16])
      {
        SelectOnPeriod--;
        if (SelectOnPeriod == -1) SelectOnPeriod = 4;
        DisplayOnPeriod();
      }
    }
    Serial.print(val); Serial.print(" SelectOnPeriod= "); Serial.println(SelectOnPeriod);
    irrecv.resume(); // Receive the next value
  }
  for (int8_t g = 0; g < 4; g++)
  {
    TimeStored4Relay[g] = TimeStored4Relay[g] - (TimeStored4Relay[g] > 1);
    if (TimeStored4Relay[g] == 2)
    {
      digitalWrite(g + 10, 0);
      relay[g] = 0;
    }
  }
  if (CountDownTimer > 0) CountDownTimer--;
  if (CountDownTimer == 2) DeviceSelect = 0;
  delay(100);
}

void flash()
{
  delay (125);
  digitalWrite(8, 0);
  delay (125);
  digitalWrite(8, 1);
  Serial.print("flash===");
}

void DisplayOnPeriod()
{
  digitalWrite(8, LOW);
  delay (1000);
  for (int8_t f = 1; f < SelectOnPeriod + 2; f++)
  {
    delay(250);
    digitalWrite(8, HIGH);
    delay (125);
    digitalWrite(8, LOW);
  }
  delay(1000);
  digitalWrite(8, HIGH);
}

//unsigned long irkey[17] = {16738455,16750695,16756815,16724175,16718055,16743045,16716015,16726215,16734885,16728765,16730805,16732845,16736925,16720605,16712445,16761405,16754775};
