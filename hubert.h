/**
 * @mainpage
 * This project simulates a client-server infrastructure for ordering food
 * via a central server (hubert). It realies heavily on POSIX IPC.
 * Therefore you get hubert, client and restaurant executables.
 *
 * @section Make
 * To compile hubert:
 * @code
 * make hubert
 * @endcode
 * To compile client:
 * @code
 * make client
 * @endcode
 * To compile restaurant:
 * @code
 * make restaurant
 * @endcode
 * To generate documentation:
 * @code
 * make doc
 * @endcode
 * To clean:
 * @code
 * make clean
 * @endcode
 * To do all of them (except for cleaning):
 * @code
 * make all
 * @endcode
 * or just
 * @code
 * make
 * @endcode
 *
 * @section hubert Hubert's program logic
 * @snippet hubert.c hubert_main
 * @section client Client's program logic
 * @snippet client.c client_main
 * @section restaurant Restaurant's program logic
 * @snippet restaurant.c restaurant_main
 *
 * @see carrier.h
 * @see entity.h
 * @see hubert.h
 * @see kitchen.h
 * @see message_queue.h
 * @see misc.h
 * @see protocol.h
 * @see restaurant_list.h
 */

#ifndef HUBERT_H
#define HUBERT_H

/**
 * Hubert's name to be used everywhere
 */
#define HUBERT_NAME "hubert"

#endif
