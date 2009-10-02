    /*
 * Displays linux /proc/pid/stat in human-readable format
 *
 * Build: gcc -o procstat procstat.c
 * Usage: procstat pid
 *        cat /proc/pid/stat | procstat
 *
 * Homepage: http://www.brokestream.com/procstat.html
 * Version : 2009-03-05
 *
 * Ivan Tikhonov, http://www.brokestream.com, kefeer@netangels.ru
 *
 * 2007-09-19 changed HZ=100 error to warning
 *
 * 2009-03-05 tickspersec are taken from sysconf (Sabuj Pattanayek)
 *
 */


/* Copyright (C) 2009 Ivan Tikhonov

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ivan Tikhonov, kefeer@brokestream.com

*/


#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <linux/limits.h>
#include <sys/times.h>
#include <sstream>
#include "procUtils.h"

namespace evf{
  namespace utils{
    



    typedef long long int num;

    num pid;
    char tcomm[PATH_MAX];
    char state;
    
    num ppid;
    num pgid;
    num sid;
    num tty_nr;
    num tty_pgrp;
    
    num flags;
    num min_flt;
    num cmin_flt;
    num maj_flt;
    num cmaj_flt;
    num utime;
    num stimev;
    
    num cutime;
    num cstime;
    num priority;
    num nicev;
    num num_threads;
    num it_real_value;
    
    unsigned long long start_time;
    
    num vsize;
    num rss;
    num rsslim;
    num start_code;
    num end_code;
    num start_stack;
    num esp;
    num eip;
    
    num pending;
    num blocked;
    num sigign;
    num sigcatch;
    num wchan;
    num zero1;
    num zero2;
    num exit_signal;
    num cpu;
    num rt_priority;
    num policy;
    
    long tickspersec;
    char obuf[4096];
    FILE *input;

    void readone(num *x) { fscanf(input, "%lld ", x); }
    void readunsigned(unsigned long long *x) { fscanf(input, "%llu ", x); }
    void readstr(char *x) {  fscanf(input, "%s ", x);}
    void readchar(char *x) {  fscanf(input, "%c ", x);}
    
    void printone(char *name, num x) {  sprintf(obuf,"%20s: %lld\n", name, x);}
    void printonex(char *name, num x) {  sprintf(obuf,"%20s: %016llx\n", name, x);}
    void printunsigned(char *name, unsigned long long x) {  sprintf(obuf,"%20s: %llu\n", name, x);}
    void printchar(char *name, char x) {  sprintf(obuf,"%20s: %c\n", name, x);}
    void printstr(char *name, char *x) {  sprintf(obuf,"%20s: %s\n", name, x);}
    void printtime(char *name, num x) {  sprintf(obuf,"%20s: %f\n", name, (((double)x) / tickspersec));}
    
    int gettimesinceboot() {
      FILE *procuptime;
      int sec, ssec;
      
      procuptime = fopen("/proc/uptime", "r");
      fscanf(procuptime, "%d.%ds", &sec, &ssec);
      fclose(procuptime);
      return (sec*tickspersec)+ssec;
    }

    void printtimediff(char *name, num x) {
      int sinceboot = gettimesinceboot();
      int running = sinceboot - x;
      time_t rt = time(NULL) - (running / tickspersec);
      char buf[1024];
      
      strftime(buf, sizeof(buf), "%m.%d %H:%M", localtime(&rt));
      sprintf(obuf,"%20s: %s (%u.%us)\n", name, buf, running / tickspersec, running % tickspersec);
    }
    
    void procStat(std::ostringstream *out) {
      tickspersec = sysconf(_SC_CLK_TCK);
      input = NULL;
      
      std::ostringstream ost; 
      ost << "/proc/" << getpid() << "/stat";
      input = fopen(ost.str().c_str(), "r");
      

      readone(&pid);
      readstr(tcomm);
      readchar(&state);
      readone(&ppid);
      readone(&pgid);
      readone(&sid);
      readone(&tty_nr);
      readone(&tty_pgrp);
      readone(&flags);
      readone(&min_flt);
      readone(&cmin_flt);
      readone(&maj_flt);
      readone(&cmaj_flt);
      readone(&utime);
      readone(&stimev);
      readone(&cutime);
      readone(&cstime);
      readone(&priority);
      readone(&nicev);
      readone(&num_threads);
      readone(&it_real_value);
      readunsigned(&start_time);
      readone(&vsize);
      readone(&rss);
      readone(&rsslim);
      readone(&start_code);
      readone(&end_code);
      readone(&start_stack);
      readone(&esp);
      readone(&eip);
      readone(&pending);
      readone(&blocked);
      readone(&sigign);
      readone(&sigcatch);
      readone(&wchan);
      readone(&zero1);
      readone(&zero2);
      readone(&exit_signal);
      readone(&cpu);
      readone(&rt_priority);
      readone(&policy);
      
      {
	printone("pid", pid); *out << obuf;
	printstr("tcomm", tcomm); *out << obuf;
	printchar("state", state); *out << obuf;
	printone("ppid", ppid); *out << obuf;
	printone("pgid", pgid); *out << obuf;
	printone("sid", sid); *out << obuf;
	printone("tty_nr", tty_nr); *out << obuf;
	printone("tty_pgrp", tty_pgrp); *out << obuf;
	printone("flags", flags); *out << obuf;
	printone("min_flt", min_flt); *out << obuf;
	printone("cmin_flt", cmin_flt); *out << obuf;
	printone("maj_flt", maj_flt); *out << obuf;
	printone("cmaj_flt", cmaj_flt); *out << obuf;
	printtime("utime", utime); *out << obuf;
	printtime("stime", stimev); *out << obuf;
	printtime("cutime", cutime); *out << obuf;
	printtime("cstime", cstime); *out << obuf;
	printone("priority", priority); *out << obuf;
	printone("nice", nicev); *out << obuf;
	printone("num_threads", num_threads); *out << obuf;
	printtime("it_real_value", it_real_value); *out << obuf;
	printtimediff("start_time", start_time); *out << obuf;
	printone("vsize", vsize); *out << obuf;
	printone("rss", rss); *out << obuf;
	printone("rsslim", rsslim); *out << obuf;
	printone("start_code", start_code); *out << obuf;
	printone("end_code", end_code); *out << obuf;
	printone("start_stack", start_stack); *out << obuf;
	printone("esp", esp); *out << obuf;
	printone("eip", eip); *out << obuf;
	printonex("pending", pending); *out << obuf;
	printonex("blocked", blocked); *out << obuf;
	printonex("sigign", sigign); *out << obuf;
	printonex("sigcatch", sigcatch); *out << obuf;
	printone("wchan", wchan); *out << obuf;
	printone("zero1", zero1); *out << obuf;
	printone("zero2", zero2); *out << obuf;
	printonex("exit_signal", exit_signal); *out << obuf;
	printone("cpu", cpu); *out << obuf;
	printone("rt_priority", rt_priority); *out << obuf;
	printone("policy", policy); *out << obuf;
      }
      fclose(input);
    }



  }
}
