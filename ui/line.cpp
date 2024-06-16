#include "line.h"

Line::Line(const char *name, const char *cmdline, double n_recv_value,
           double n_sent_value, pid_t pid, uid_t uid, const char *nDevicename) {
    assert(pid >= 0);
    assert(pid <= PID_MAX);
    m_name = name;
    m_cmdline = cmdline;
    sent_value = n_sent_value;
    recv_value = n_recv_value;
    devicename = nDevicename;
    m_pid = pid;
    m_uid = uid;
    assert(m_pid >= 0);
}

int Line ::GreatestFirst(const void *ma, const void *mb) {
    //sortRecv is used to sort values by received
    bool sortRecv = true;
    Line **pa = (Line **) ma;
    Line **pb = (Line **) mb;
    Line *a = *pa;
    Line *b = *pb;
    double aValue;
    if (sortRecv) {
        aValue = a->recv_value;
    } else {
        aValue = a->sent_value;
    }

    double bValue;
    if (sortRecv) {
        bValue = b->recv_value;
    } else {
        bValue = b->sent_value;
    }

    if (aValue > bValue) {
        return -1;
    }
    if (aValue == bValue) {
        return 0;
    }
    return 1;
}
