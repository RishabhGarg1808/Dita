#ifndef DITA_NETUTIL_TAB_H
#define DITA_NETUTIL_TAB_H

#include "../ui_mainwindow.h"
#include "../mainwindow.h"

#include "../core/nethogs/src/process.h"
#include "../core/nethogs/src/devices.h"
#include "../core/nethogs/src/decpcap.h"
#include "line.h"
#include <QVector>
#include <QErrorMessage>
#include <QStandardItemModel>
#include <QTableView>
#include <thread>
#include <stdexcept>
#include <fcntl.h>
#include <set>
#include <vector>
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

extern std::set<pid_t> pidsToWatch;
extern ProcList *processes;
static std::pair<int, int> self_pipe = std::make_pair(-1, -1);

class Nethogs : public QObject {
    Q_OBJECT
    signals:
        void updateNetUtilSignal(QVector<Line*>, int );
private:
    time_t last_refresh_time = 0;
    fd_set pc_loop_fd_set{};
    std::vector<int> pc_loop_fd_list;
    bool pc_loop_use_select = true;
    int promisc = 0;
    bool all = true;
    char *filter = NULL;
    int garbage_collection_period = 50;
    char errbuf[PCAP_ERRBUF_SIZE]{};
    device *devices;
    int nb_devices = 0;
    int nb_failed_devices = 0;

    device *current_dev;
    struct dpargs *userdata;
    int refresh_count = 0;

    static void error_msg(const std::string &msg);
    static void quit_cb(int /* i */);
    static void forceExit(bool success,const char *msg,...);
    static std::pair<int, int> create_self_pipe();
    bool wait_for_next_trigger();
    void clean_up();
    void update();
    void process_loop();
public:
    Nethogs();
    void start();
};
#endif //DITA_NETUTIL_TAB_H
