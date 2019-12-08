# CoAsync
An async library for Arduino

## Usage
To use this library for arduino copy the "CoAsync" folder to your Arduino libraries directory. From there on you should be able to include it with ``<CoAsync.h>`` in your arduino projects.



## Example
```C
#include <CoAsync.h>

//Make an async instance with 20 functions that can be assigned to
CoAsync async(20);

//Variables for the times
unsigned long int currentTime;
unsigned long int startTime;

//Show a little message and restart the async func
void execFunc() {
    //Show message
    Serial.println("5 SECONDS HAVE PASSED");
    //Set the start time
    startTime = millis();
    //Add it to the async functions
    async.add(execFunc, checkFunc);
}

//Checks if the currentTime is 5 seconds futher from last call
bool checkFunc() {
    if(currentTime >= startTime + 5000) {
        return true;
    } else {
        return false;
    }
}

//Setup
void setup() {
    //Start the serial on 9600 baud rate
    Serial.begin(9600);
    //Start the time
    startTime = millis();
    //Add function to the async handler
    async.add(execFunc, checkFunc);
}

//Loop function
void loop() {
    //Update the current time and show it
    currentTime = millis();
    Serial.println(currentTime);
    //Wait 500ms
    delay(500);
    //Handle all async functions
    async.handle();
}
```

## License
This project is licensed under the [MIT license](https://opensource.org/licenses/MIT)

## Dependencies
This project uses one library, also made by myself:
 - [CoMemoryPool](https://github.com/frankbekema/CoMemoryPool)

## Questions?
If you have any questions feel free to contact Frank Bekema on frankbekema10@gmail.com or open up an issue.
