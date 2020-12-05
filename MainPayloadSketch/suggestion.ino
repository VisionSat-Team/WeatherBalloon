unsigned long delay;
boolean wireHasBeenCut;
boolean currentDataSaved;

void setup() {
    delay = 60000000L;  // can always change this variable
    unsigned long prev = micros();
    currentDataSaved = true;
}

void loop() {
    if (commandRecieved) {
        doCommand();
        return;
    }
    // stops capturing and saving data after wire has been cut
    if (!wireHasBeenCut) {
        unsigned long now = micros();

        if (now - prev >= delay) {
            prev = now;
            captureData();  // 1 minute has passed
            currentDataSaved = false;
        } else if (!currentDataSaved){
            saveData();
            currentDataSaved = true;
        }
    } else {
        //  whatever is to be done after the wire is cut
    }
}
