#ifndef CARRIER_H
#define CARRIER_H

#define CARRIER_SEM 0xABBA
#define N_CARRIERS  15

typedef int Carrier;
typedef int CarrierFleet;

CarrierFleet createCarrierFleet();
CarrierFleet openCarrierFleet();
Carrier dispatchCarrier(CarrierFleet fleet);
void waitForCarrier(Carrier carrier, CarrierFleet fleet);
void removeCarrierFleet(CarrierFleet fleet);

#endif
