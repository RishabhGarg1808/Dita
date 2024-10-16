#include "netutil_tab.h"
#include "core/nethogs/src/cui.h"
#include "core/nethogs/src/nethogs.cpp"
#include "../ui_mainwindow.h"

/* DO NOT REMOVE quit_cb and forceExit functions
 * The following functions are used to handle the self-pipe trick.
 * These functions are declared in the main.cpp in nethogs which are currently
 * unavailable due to include errors so the functions are declared here
 * */

void quit_cb(int /* i */) {
    if (self_pipe.second != -1) {
        write(self_pipe.second, "x", 1);
    } else {
        exit(0);
    }
}

void forceExit(bool success,const char* msg,...) {
    if ((!tracemode) && (!DEBUG)) {
        exit_ui();
    }

    va_list argp;
    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);
    std::cerr << std::endl;

    if (success)
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

std::list<handle> handles;

//Utils function to dislapy error messages in ui
void Nethogs::error_msg(const std::string &msg) {
    auto messagebox = new QErrorMessage();
    messagebox->setWindowTitle("Network Utilization Error");
    messagebox->showMessage(QString::fromStdString(msg));
    messagebox->exec();
}

void Nethogs::quit_cb(int /* i */) {
    if (self_pipe.second != -1) {
        write(self_pipe.second, "x", 1);
    } else {
        exit(0);
    }
}

void Nethogs::forceExit(bool success,const char *msg,...) {
    if ((!tracemode) && (!DEBUG)) {
        //exit_ui();
    }

    va_list argp;
    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);
    std::cerr << std::endl;

    if (success)
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

std::pair<int, int> Nethogs::create_self_pipe() {
    int pfd[2];
    if (pipe(pfd) == -1)
        return std::make_pair(-1, -1);

    if (fcntl(pfd[0], F_SETFL, fcntl(pfd[0], F_GETFL) | O_NONBLOCK) == -1)
        return std::make_pair(-1, -1);

    if (fcntl(pfd[1], F_SETFL, fcntl(pfd[1], F_GETFL) | O_NONBLOCK) == -1)
        return std::make_pair(-1, -1);

    return std::make_pair(pfd[0], pfd[1]);
}

bool Nethogs::wait_for_next_trigger() {
    if (pc_loop_use_select) {
        FD_ZERO(&pc_loop_fd_set);
        int nfds = 0;
        for (int &it: pc_loop_fd_list) {
            int const fd = it;
            nfds = std::max(nfds, it + 1);
            FD_SET(fd, &pc_loop_fd_set);
        }
        timeval timeout = {refreshdelay, 0};
        if (select(nfds, &pc_loop_fd_set, 0, 0, &timeout) != -1) {
            if (FD_ISSET(self_pipe.first, &pc_loop_fd_set)) {
                return false;
            }
        }
    } else {
        // If select() not possible, pause to prevent 100%
        usleep(1000);
    }
    return true;
}

void Nethogs::clean_up() {
    // close file descriptors
    for (int &it: pc_loop_fd_list) {
        ::close(it);
    }

    procclean();
//    if ((!tracemode))
//        exit_ui();
}

Nethogs::Nethogs() {
    process_init();
    devices = get_devices(0, NULL, all);
    if (devices == NULL) {
        error_msg("Failed to get interfaces by network utilization devices."
                  "Switch to terminal more for more details");
        forceExit("No devices to monitor."
                  "loopback interfaces or devices that are not up/running", 0);
    }
    std::cout << "creating pipe\n";
    self_pipe = create_self_pipe();
    if (self_pipe.first == -1 || self_pipe.second == -1) {
        error_msg("\"Failed to init fd");
        forceExit("Error creating pipe file descriptors\n", 0);
    } else {
        // add the self-pipe to allow interrupting select()
        pc_loop_fd_list.push_back(self_pipe.first);
    }
    current_dev = devices;

    while (current_dev != NULL) {
        ++nb_devices;

        if (!getLocal(current_dev->name, tracemode)) {
            forceExit("getifaddrs failed while establishing local IP.", 0);
        }

        dp_handle *newhandle =
                dp_open_live(current_dev->name, BUFSIZ, promisc, 100, filter, errbuf);
        if (newhandle != NULL) {
            dp_addcb(newhandle, dp_packet_ip, process_ip);
            dp_addcb(newhandle, dp_packet_ip6, process_ip6);
            dp_addcb(newhandle, dp_packet_tcp, process_tcp);
            dp_addcb(newhandle, dp_packet_udp, process_udp);
            if (dp_setnonblock(newhandle, 1, errbuf) == -1) {
                fprintf(stderr, "Error putting libpcap in nonblocking mode\n");
            }
            handles.push_front(handle(newhandle, current_dev->name));

            if (pc_loop_use_select) {
                // some devices may not support pcap_get_selectable_fd
                int const fd = pcap_get_selectable_fd(newhandle->pcap_handle);
                if (fd != -1) {
                    pc_loop_fd_list.push_back(fd);
                } else {
                    pc_loop_use_select = false;
                    pc_loop_fd_list.clear();
                    fprintf(stderr, "failed to get selectable_fd for %s\n",
                            current_dev->name);
                }
            }
        } else {
            fprintf(stderr, "Error opening handler for device %s\n",
                    current_dev->name);
            ++nb_failed_devices;
        }

        current_dev = current_dev->next;
    }

    if (nb_devices == nb_failed_devices) {
        if (geteuid() != 0)
            fprintf(stderr,
                    "To run nethogs without being root, you need to enable "
                    "capabilities on the program (cap_net_admin, cap_net_raw, "
                    "cap_dac_read_search, cap_sys_ptrace). "
                    "See the documentation for details.\n");
        forceExit("Error opening pcap handlers for all devices.\n", 0);
    }



    userdata = (dpargs *) malloc(sizeof(struct dpargs));
}

void Nethogs::update() {
    refreshconninode();
    refreshcount++;

    if (viewMode == VIEWMODE_KBPS || viewMode == VIEWMODE_MBPS ||
        viewMode == VIEWMODE_GBPS) {
        remove_timed_out_processes();
    }

    ProcList *curproc = processes;
    int nproc = processes->size();

    /* initialize to null pointers */
    Line *lines[nproc];
    for (int i = 0; i < nproc; i++)
        lines[i] = NULL;

    int n = 0;

    while (curproc != NULL) {
        // walk though its connections, summing up their data, and
        // throwing away connections that haven't received a package
        // in the last CONNTIMEOUT seconds.4
        assert(curproc->getVal() != NULL);
        assert(nproc == processes->size());

        float value_sent = 0, value_recv = 0;

        if (viewMode == VIEWMODE_KBPS) {
            curproc->getVal()->getkbps(&value_recv, &value_sent);
        } else if (viewMode == VIEWMODE_MBPS) {
            curproc->getVal()->getmbps(&value_recv, &value_sent);
        } else if (viewMode == VIEWMODE_GBPS) {
            curproc->getVal()->getgbps(&value_recv, &value_sent);
        } else if (viewMode == VIEWMODE_TOTAL_KB) {
            curproc->getVal()->gettotalkb(&value_recv, &value_sent);
        } else if (viewMode == VIEWMODE_TOTAL_MB) {
            curproc->getVal()->gettotalmb(&value_recv, &value_sent);
        } else if (viewMode == VIEWMODE_TOTAL_B) {
            curproc->getVal()->gettotalb(&value_recv, &value_sent);
        } else {
            forceExit("Invalid viewMode: %d", reinterpret_cast<const char *>(viewMode));
        }
        uid_t uid = curproc->getVal()->getUid();
        assert(curproc->getVal()->pid >= 0);
        assert(n < nproc);

        lines[n] = new Line(curproc->getVal()->name, curproc->getVal()->cmdline,
                            value_recv, value_sent, curproc->getVal()->pid, uid,
                            curproc->getVal()->devicename);
        curproc = curproc->next;
        n++;
    }

    /* sort the accumulated lines */
    qsort(lines, nproc, sizeof(Line *), Line::GreatestFirst);

    //trigger the update of the table
    QVector<Line*> linesVector(lines,lines+n);
    emit updateNetUtilSignal(linesVector, nproc);
    if (refreshlimit != 0 && refreshcount >= refreshlimit)
        quit_cb(0);
}

void Nethogs::process_loop() {
    while (true) {
        bool packets_read = false;

        for (auto current_handle = handles.begin(); current_handle != handles.end(); current_handle++) {
            userdata->device = current_handle->devicename;
            userdata->sa_family = AF_UNSPEC;
            int retval = dp_dispatch(current_handle->content, -1, (u_char *) userdata,
                                     sizeof(struct dpargs));
            if (retval == -1)
                std::cerr << "Error dispatching for device "
                          << current_handle->devicename << ": "
                          << dp_geterr(current_handle->content) << std::endl;
            else if (retval < 0)
                std::cerr << "Error dispatching for device "
                          << current_handle->devicename << ": " << retval << std::endl;
            else if (retval != 0)
                packets_read = true;
        }

        time_t const now = ::time(NULL);
        if (last_refresh_time + refreshdelay <= now) {
            last_refresh_time = now;
            update();
            ++refresh_count;
            if ((!DEBUG) && (!tracemode)) {
                // handle user input
                //ui_tick();
            }
            if ((garbage_collection_period > 0) &&
                (refresh_count % garbage_collection_period == 0)) {
                garbage_collect_processes();
            }
        }

        // if not packets, do a select() until next packet
        if (!packets_read)
            if (!wait_for_next_trigger())
                // Shutdown requested - exit the loop
                break;
    }
    clean_up();
}

void Nethogs::start() {
    std::cout << "Starting network utilization\n";
    std::thread netProcess(&Nethogs::process_loop, this);
    netProcess.detach();
}

void MainWindow::initNetUtil() {

    statsModel =  new QStandardItemModel(0,5,this);
    statsModel->setHorizontalHeaderLabels(QStringList() << "PID"  <<"User" << "Program"
                                                        << "Device" << "Sent" << "Received" );

    ui->netTabel->setModel(statsModel);
    ui->netStats->resizeRowsToContents();
    ui->netTabel->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->netTabel->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->netTabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->netTabel->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->netTabel->setSelectionMode(QAbstractItemView::SingleSelection);

    //table  to show the total network stats
    totalModel = new QStandardItemModel(0, 3, this);
    totalModel->setHorizontalHeaderLabels(QStringList() << " \u2211 Sent : "
                                                        << "\u2211 Received : " << "\u2211 Processes : ");
    ui->netStats->setFixedHeight(25);
    ui->netStats->setModel(totalModel);
    ui->netStats->resizeRowsToContents();
    ui->netStats->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->netStats->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->netStats->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->netStats->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::updateNetUtil(QVector<Line *> data, int n) {
    for(int i = 0; i < n; i++){
        Line *line = data[i];
        if (i < statsModel->rowCount()) {
            // Update existing and if pid == 0 skip
            if(line->m_pid == 0) continue;
            statsModel->item(i, 0)->setText(QString::fromStdString(std::to_string(line->m_pid)));
            statsModel->item(i, 1)->setText(QString::fromStdString(std::to_string(line->m_uid)));
            statsModel->item(i, 2)->setText(QString::fromStdString(line->m_name));
            statsModel->item(i, 3)->setText(QString::fromStdString(line->devicename));
            statsModel->item(i, 4)->setText(QString::number(line->sent_value,'f',2)+ unit);
            statsModel->item(i, 5)->setText(QString::number(line->recv_value,'f',2)+ unit);
        } else {
            // Add new row if it doesn't exist and check for pid != 0
            if(line->m_pid == 0) continue;
            QList<QStandardItem *> items;
            items.append(new QStandardItem(QString::fromStdString(std::to_string(line->m_pid))));
            items.append(new QStandardItem(QString::fromStdString(std::to_string(line->m_uid))));
            items.append(new QStandardItem(QString::fromStdString(line->m_name)));
            items.append(new QStandardItem(QString::fromStdString(line->devicename)));
            items.append(new QStandardItem(QString::number(line->sent_value,'f',2) + unit));
            items.append(new QStandardItem(QString::number(line->recv_value,'f',2) +unit ));
            statsModel->appendRow(items);
        }
    }
    float total_sent =0;
    float totalReceive=0;
    float sendSpeed = 0;
    float receiveSpeed = 0;

    for(int i = 0; i < statsModel->rowCount(); i++){
        QStandardItem* sent = statsModel->item(i, 4);
        QStandardItem* recv = statsModel->item(i, 5);
        total_sent += (sent->text().split(unit)).first().toFloat();
        totalReceive += (recv->text().split(unit)).first().toFloat();
    }

    auto adjustCols = [&](int coeff){
        for(int i =0;i<statsModel->rowCount();i++){
            QStandardItem *sent = statsModel->item(i, 4);
            QStandardItem *recv = statsModel->item(i, 5);
            sent->setText(QString::number(sent->text().toFloat()/coeff,'f',2) + unit);
            recv->setText(QString::number(recv->text().toFloat()/coeff,'f',2) + unit);
        }
    };

    if(total_sent > 2048 || totalReceive > 2048){
        unit = " MBps";
        adjustCols(1024);
        sendSpeed = total_sent/1024;
        receiveSpeed = totalReceive/1024;
    }else if(total_sent > (2*1024*1024) || totalReceive > (2*1024*1024)){
        unit = " GBps";
        adjustCols(1024*1024);
        sendSpeed = total_sent/(1024*1024);
        receiveSpeed = totalReceive/(1024*1024);
    }else if(total_sent < 2048 || totalReceive < 2048){
        unit = " KBps";
        sendSpeed = total_sent;
        receiveSpeed = totalReceive;
    }

    QString proc= QString::fromStdString("\u2211 Processes : " + std::to_string(statsModel->rowCount()));
    QString sentInfo= QString::fromStdString("\u2211 Sent : " + QString::number(sendSpeed,'f',2).toStdString()  + unit.toStdString());
    QString recvInfo= QString::fromStdString( "\u2211 Received : " + QString::number(receiveSpeed,'f',2).toStdString() + unit.toStdString());
    totalModel->setHorizontalHeaderLabels(QStringList() << sentInfo << recvInfo << proc);
}
