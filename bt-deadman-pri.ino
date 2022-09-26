#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif
//user settings
static int max_missed_matches = 2; //maximum number of loops
char target[] = {"28:df:eb:f9:8b:42"}; //BT Mac to scan for
int cycle_delay = 5; //delay for for starting each loop in seconds


//BTAddress myAddress;
int relay_pin = 22;
BluetoothSerial SerialBT;
static bool btScanAsync = true;
bool thispass = false;
bool wentlow = false;
int counted_missed_matches = 0; 
int missed_matches = 0; 
char mystr[] ="";

void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
	//Serial.printf("Found a device asynchronously: %s\n", pDevice->toString().c_str());  
  mystr == pDevice->toString().c_str();
    if (strstr(target, pDevice->getAddress().toString().c_str()))    
    {
      //Serial.println("HIT");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(relay_pin, HIGH); 
      counted_missed_matches = 0;
      missed_matches = 0;
      thispass = true; //Positive Match 
      wentlow = false; //Reset flag about being off
    }
    else {          
      Serial.println("MISS");
      //++missed_matches;    
    }
  
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("BTdeadman"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode (relay_pin, OUTPUT);
}

void loop() {
  //delay(10000);
    thispass = false;

      //Serial.print("Starting discoverAsync...");
    if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
      delay(1000*cycle_delay);
      //Serial.print("Stopping discoverAsync... ");
      SerialBT.discoverAsyncStop();
      Serial.println("This loop finished");
    } else {
      Serial.println("Error on discoverAsync f.e. not workin after a \"connect\"");
    } //endif
    
    if(thispass){
      Serial.println("last THISPASS true");
    }
    else{
      Serial.println("last THISPASS FALSE");
      ++missed_matches;
      Serial.println(missed_matches);
    }
    if(missed_matches >= max_missed_matches){
      //send pin low
      Serial.println("CUTTING OFF");
      wentlow = true;
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(relay_pin, LOW); 
    }
    Serial.print("Current missed_matches: ");
    Serial.print(missed_matches);
    Serial.print(" of ");
    Serial.println(max_missed_matches);

}
