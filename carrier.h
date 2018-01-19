#ifndef CARRIER_H
#define CARRIER_H

#define CARRIER_SEM 0xABBA

typedef int Carrier;

int initCarrier();
void carrierRide(float time);
void requestCarrier(Carrier carrier);
int isCarrierFree();

#endif
