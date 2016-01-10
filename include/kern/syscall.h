//
// Created by Peijie You on 16/1/1.
//


/*******system call design******
 *
 * todo
 * process related syscall
 * 1.   fork
 * 2.   exec
 * 3.   kill / exit?
 * 4.   waitforpid?
 * 5.   getpid
 * -    getuid
 *
 * file related syscall
 * 0x10.   open
 * 0x11.   close
 * 0x12.   read
 * 0x13.   write
 * 0x14.   seek
 * -    getdir
 * -    chdir
 *
 * time related syscall
 * 0x20.  sleep
 * 0x21.  time
 * 0x22.  clock
 *
 * -    ipc related?
 */

#ifndef AIMV6_SYSCALL_H
#define AIMV6_SYSCALL_H

#include<config.h>

/*---process related---*/
#define ID_FORK     0x1
#define ID_EXEC     0x2
#define ID_KILL     0x3
#define ID_WAIT     0x4
#define ID_GETPID   0x5
/*---file related---*/
#define ID_OPEN     0x10
#define ID_CLOSE    0x11
#define ID_READ     0x12
#define ID_WRITE    0x13
#define ID_SEEK     0x14
/*---time related---*/
#define ID_SLEEP    0x20
#define ID_TIME     0x21
#define ID_CLOCK    0x22

void syscall(int id);

#endif //AIMV6_SYSCALL_H
