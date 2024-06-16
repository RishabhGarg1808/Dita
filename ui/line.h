#ifndef DITA_LINE_H
#define DITA_LINE_H

#include <csignal>
#include <assert.h>
#define PID_MAX 4194303
class Line{
public:

    double sent_value;
    double recv_value;
    const char *devicename;
    const char *m_name;
    const char *m_cmdline;
    pid_t m_pid;
    uid_t m_uid;

    Line(const char*, const char *, double ,
         double , pid_t , uid_t , const char *);

    static int GreatestFirst(const void *, const void *);
};

#endif //DITA_LINE_H
