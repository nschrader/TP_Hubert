/**
 * @file
 * Useful functions and definitions
 */

#ifndef MISC_H
#define MISC_H

#define HUBERT_NAME "hubert"//!< Hubert's name to be used everywhere

#define IPC_NOFLAGS   0     //!< Convenience macro
#define IPC_ALLWRITE  0666  //!< Convenience macro

#define ERROR         -1    //!< Convenience macro to detect erros form syscalls
#define NO_ERRNO      0     //!< Convenience macro for errno not indicating any error
#define CHILD         0     //!< Convenience macro to detect child after fork()

/**
 * Handle a fatal error and exit program
 * @param x
 * The errorMessage to write to stderr
 */
#define fatal(x) _fatal("[Fatal] " x);
//! Backend function for @ref fatal
void _fatal(char* errorMessage);

/**
 * Write a warning to stderr
 * @param x
 * The error message
 */
#define warning(x) _warning("[Warning] " x);
//! Backend function for @ref warning
void _warning(char* errorMessage);

/**
 * Write an info to stdout
 * @param x
 * The error message
 */
#define info(x) _info("[Info] " x)
//! Backend function for @ref info
void _info(char* infoMessage);

/**
 * Attempt to access a semaphore
 * @param semid
 * The IPC semid
 * @param sem_num
 * The IPC sem_num
 */
void V(int semid, int sem_num);

/**
 * Release a semaphore after @ref V
 * @param semid
 * The IPC semid
 * @param sem_num
 * The IPC sem_num
 */
void P(int semid, int sem_num);

/**
 * Attempt to access a semaphore (blocking)
 * @param semid
 * The IPC semid
 * @param sem_num
 * The IPC sem_num
 */
void V2(int semid, int sem_num);

/**
 * Release a semaphore after @ref V2
 * @param semid
 * The IPC semid
 * @param sem_num
 * The IPC sem_num
 */
void P2(int semid, int sem_num);

#endif
