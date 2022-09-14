void relaySetup()
{
    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, OUTPUT);

    // relayHi(R1);
    // relayHi(R2);
    // relayHi(R3);
    // relayHi(R4);
    // delay(1000);
    // relayLo(R1);
    // relayLo(R2);
    // relayLo(R3);
    // relayLo(R4);

}

void relayHi(int relay){

    digitalWrite(relay, HIGH);
}

void relayLo(int relay){
    digitalWrite(relay, LOW);
}