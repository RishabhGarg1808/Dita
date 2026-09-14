#include <QApplication>
#include <QMessageBox>
#include <QLabel>
#include <QWidget>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include "unistd.h"
#include "mainwindow.h"

// Returns true if this process is allowed to open a raw packet socket,
// i.e. if it can capture traffic. This covers root, users that hold
// CAP_NET_RAW (e.g. via file capabilities set with setcap), and members
// of the 'pcap' group - so the app no longer requires root.
bool can_capture() {
    // libpcap/PcapPlusPlus capture uses AF_PACKET SOCK_RAW sockets; creating
    // one requires CAP_NET_RAW on every kernel.
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd >= 0) {
        ::close(fd);
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // Check the permission *before* constructing the window, so that a user
    // without capture rights gets a helpful message instead of capture errors.
    if (!can_capture()) {
        QMessageBox dialog;
        dialog.setWindowTitle("Permission warning");
        dialog.setIcon(QMessageBox::Warning);
        dialog.setText("Dita does not have permission to capture packets.");
        dialog.setInformativeText(
            "You can grant the permission without using root:\n\n"
            "  1. File capabilities (one time, needs sudo once):\n"
            "     sudo setcap cap_net_raw,cap_net_admin,cap_dac_read_search,\\\n"
            "         cap_sys_ptrace+ep <path-to-dita-binary>\n\n"
            "  2. pcap group (one time, re-login afterwards):\n"
            "     sudo usermod -aG pcap $USER\n\n"
            "Or keep running with sudo. Continue anyway?");
        dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        dialog.setButtonText(QMessageBox::Yes, "Continue anyway");
        dialog.setButtonText(QMessageBox::No, "Quit");
        if (dialog.exec() != QMessageBox::Yes) {
            return 1;
        }
    }
    MainWindow window;
    window.show();
    return QApplication::exec();
}
