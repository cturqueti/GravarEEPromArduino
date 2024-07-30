//------------------------------------------------//
//                                                //
//  Programa para gravar a eprom do Motor Drive   //
//                                                //
//------------------------------------------------//
#include <Arduino.h>
#include <EEPROM.h>
#include <Sidetouch200.h>

const int EEPROM_START_ADDRESS = 0;            // Endereço inicial na EEPROM
const int VALUES_COUNT = RAMP_EEPROM_END + 5;  // Número de valores a serem lidos/gravados

void programarEEPROM();
void lerEEPROM();
void writeValuesToEEPROM(uint8_t values[], int count);

void setup() {
    Serial.begin(115200);
    Serial.println("Digite:\n1 Programar EEPROM\n2 Ler EEPROM");
}

void loop() {
    if (Serial.available() > 0) {
        char choice = Serial.read();
        Serial.readStringUntil('\n');
        // Serial.flush();
        // Serial.println(choice);
        switch (choice) {
            case '1':
                //
                programarEEPROM();
                break;
            case '2':
                lerEEPROM();
                break;
            default:
                Serial.println("Escolha inválida. Tente novamente.");
                break;
        }

        // Limpa o buffer da porta serial
        while (Serial.available() > 0) {
            Serial.read();
        }

        // Serial.println("\nDigite:\n1 Programar EEPROM\n2 Ler EEPROM");
    }
}

void programarEEPROM() {
    Serial.println("Digite os valores separados por (;) e pressione Enter:");

    uint8_t buffer[VALUES_COUNT];  // Assumindo que cada valor pode ter até 5 dígitos
    int bufferIndex = 0;
    char c = 0;
    String bufferTmp;

    while (Serial.available() == 0) {
        // Aguarda entrada do usuário
    }
    while (c != '\n') {
        c = Serial.read();

        if (c == ';') {
            if (bufferIndex < VALUES_COUNT) {
                buffer[bufferIndex++] = bufferTmp.toInt();
                bufferTmp = "";
            } else {
                Serial.println("Erro: Estouro de buffer.");
                return;
            }
        } else if (c >= '0' && c <= '9') {
            bufferTmp += c;
        }
    }
    if (bufferIndex < VALUES_COUNT) {
        buffer[bufferIndex] = bufferTmp.toInt();
        bufferIndex++;
    } else {
        Serial.println("Erro: Estouro de buffer.");
        return;
    }

    writeValuesToEEPROM(buffer, bufferIndex);
    Serial.println("Gravação OK");
    return;
}

void lerEEPROM() {
    Serial.println("Valores lidos da EEPROM:");

    for (int i = 0; i < VALUES_COUNT; ++i) {
        int value = EEPROM.read(EEPROM_START_ADDRESS + i);
        Serial.print(value);
        Serial.print(" ");
    }

    Serial.println();
}

void writeValuesToEEPROM(uint8_t values[], int count) {
    for (int i = 0; i < count; ++i) {
        EEPROM.write(EEPROM_START_ADDRESS + i, values[i]);
    }

    // Serial.println("Valores programados na EEPROM.");
}
