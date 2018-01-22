/**
 * @file
 * A carrier based on semaphores
 */

#ifndef CARRIER_H
#define CARRIER_H

#define CARRIER_SEM 0xABBA //!< IPC key for this feature
#define N_CARRIERS  15     //!< Number of carriers available

/**
 * A carrier is actually only a sem_num
 */
typedef int Carrier;

/**
 * A carrier fleet is actually only a sem_id
 */
typedef int CarrierFleet;

/**
 * Create semaphore set
 */
CarrierFleet createCarrierFleet();

/**
 * Retrieve semaphore set
 */
CarrierFleet openCarrierFleet();

/**
 * Start new thread that is going to liberate the corresponding semaphore in
 * a few seconds
 */
Carrier dispatchCarrier(CarrierFleet fleet);

/**
 * Try to access semaphore and wait
 */
void waitForCarrier(Carrier carrier, CarrierFleet fleet);

/**
 * Delete semaphore set
 */
void removeCarrierFleet(CarrierFleet fleet);

#endif
